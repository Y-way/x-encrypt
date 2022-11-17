
#include "ZipUtils.h"

#include "zlib/zlib.h"
#include "Logging.h"
#include "Allocator.h"
namespace encrypt
{
#define BUFFER_INC_FACTOR (2)

    int ZipUtils::GZipUncompressWithHint(const byte* in, size_t inLength, size_t outLengthHint, byte** out, size_t* outLength)
    {
        X_ENCRYPT_ASSERT(in != NULL);
        X_ENCRYPT_ASSERT(inLength > 0);

        X_ENCRYPT_ASSERT(out != NULL);
        X_ENCRYPT_ASSERT(outLength != NULL);

        /* ret value */
        int err = Z_OK;

        size_t bufferSize = outLengthHint;
        *out = (byte*)XMEMORY_MALLOC(bufferSize);

        z_stream d_stream; /* decompression stream */
        d_stream.zalloc = (alloc_func)0;
        d_stream.zfree = (free_func)0;
        d_stream.opaque = (voidpf)0;

        d_stream.next_in = (z_const Bytef*)in;
        d_stream.avail_in = static_cast<uInt>(inLength);
        d_stream.next_out = *out;
        d_stream.avail_out = static_cast<uInt>(bufferSize);

        /* window size to hold 256k */
        if ((err = inflateInit2(&d_stream, 15 + 32)) != Z_OK)
        {
            return err;
        }

        while (true)
        {
            err = inflate(&d_stream, Z_NO_FLUSH);

            if (err == Z_STREAM_END)
            {
                break;
            }

            switch (err)
            {
            case Z_NEED_DICT:
                err = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(&d_stream);
                return err;
            }

            // not enough memory ?
            if (err != Z_STREAM_END)
            {
                *out = (byte*)XMEMORY_REALLOC(*out, bufferSize * BUFFER_INC_FACTOR);

                /* not enough memory, ouch */
                if (!*out)
                {
                    Logging::Write("ZipUtils: realloc failed");
                    inflateEnd(&d_stream);
                    return Z_MEM_ERROR;
                }

                d_stream.next_out = *out + bufferSize;
                d_stream.avail_out = static_cast<uInt>(bufferSize);
                bufferSize *= BUFFER_INC_FACTOR;
            }
        }
        *outLength = bufferSize - d_stream.avail_out;
        err = inflateEnd(&d_stream);
        return err;
    }

    bool ZipUtils::GZipUncompressWithHint(const byte* in, size_t inLength, byte** out, size_t* outSize, size_t outLengthHint)
    {
        X_ENCRYPT_ASSERT(outSize != NULL);
        int err = GZipUncompressWithHint(in, inLength, outLengthHint, out, outSize);

        if (err != Z_OK || *out == nullptr)
        {
            if (err == Z_MEM_ERROR)
            {
                Logging::Write("ZipUtils: Out of memory while decompressing map data!");
            }
            else if (err == Z_VERSION_ERROR)
            {
                Logging::Write("ZipUtils: Incompatible zlib version!");
            }
            else if (err == Z_DATA_ERROR)
            {
                Logging::Write("ZipUtils: Incorrect zlib compressed data!");
            }
            else
            {
                Logging::Write("ZipUtils: Unknown error while decompressing map data!");
            }

            if (*out != nullptr)
            {
                XMEMORY_FREE(*out);
                *out = nullptr;
            }
            *outSize = 0;
            return false;
        }
        return true;
    }

    bool ZipUtils::GZipUncompress(const byte* in, size_t inLength, byte** out, size_t* outSize)
    {
        // 256k for hint
        return Z_OK == GZipUncompressWithHint(in, inLength, 256 * 1024, out, outSize);
    }

    unsigned long ZipUtils::CompressMemoryBound(unsigned long inLength)
    {
        return compressBound(inLength);
    }

    /* Compress gzip data */
    int GZipCompress(Bytef* input, uLong inSize, Bytef* out, uLong* outSize)
    {
        X_ENCRYPT_ASSERT(out != NULL);
        X_ENCRYPT_ASSERT(outSize != NULL);

        int err = 0;
        const uInt max = (uInt)-1;
        uLong left;
        left = *outSize;
        *outSize = 0;

        z_stream c_stream;
        
        if (input && inSize > 0)
        {
            c_stream.zalloc = NULL;
            c_stream.zfree = NULL;
            c_stream.opaque = NULL;

            // hard to believe they don't have a macro for gzip encoding,"Add 16" is the best thing zlib can do:
            //"Add 16 to windowBits to write a simple gzip header and trailer around the compressed data instead of a zlib wrapper"

            err = deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
            if (err != Z_OK)
            {
                return err;
            }

            c_stream.next_in = input;
            c_stream.avail_in = inSize;
            c_stream.next_out = out;
            c_stream.avail_out = *outSize;
            while (c_stream.avail_in != 0 && c_stream.total_out < *outSize)
            {
                err = deflate(&c_stream, Z_NO_FLUSH);
                if (err != Z_OK)
                {
                    return err;
                }
            }

            if (c_stream.avail_in != 0)
            {
                return c_stream.avail_in;
            }

            for (;;) {
                err = deflate(&c_stream, Z_FINISH);
                if (err == Z_STREAM_END)
                    break;
                if (err != Z_OK)
                    return err;
            }
            err = deflateEnd(&c_stream);
            if (err != Z_OK)
            {
                return err;
            }

            if (outSize != nullptr)
            {
                *outSize = c_stream.total_out;
            }

            return err;
        }
        return -1;
    }

    bool ZipUtils::GZipCompress(const byte* in, size_t inLength, byte* out, unsigned long* outLength)
    {
        X_ENCRYPT_ASSERT(in != NULL);
        X_ENCRYPT_ASSERT(inLength > 0);

        X_ENCRYPT_ASSERT(out != NULL);
        X_ENCRYPT_ASSERT(outLength != NULL);

        z_stream stream;
        int err;
        const uInt max = (uInt)-1;
        uLong left;

        left = *outLength;
        *outLength = 0;

        stream.zalloc = (alloc_func)0;
        stream.zfree = (free_func)0;
        stream.opaque = (voidpf)0;

        //Initialize the state of zlib, if success return Z_OK.
        //deflateInit:zlib, deflateInit2:gzip
        //     err = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
        // hard to believe they don't have a macro for gzip encoding,"Add 16" is the best thing zlib can do:
        //"Add 16 to windowBits to write a simple gzip header and trailer around the compressed data instead of a zlib wrapper"
        err = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);
        if (err == Z_OK)
        {
            stream.next_out = out;
            stream.avail_out = 0;
            stream.next_in = (z_const Bytef*)in;
            stream.avail_in = 0;
            do {
                if (stream.avail_out == 0) {
                    stream.avail_out = left > (uLong)max ? max : (uInt)left;
                    left -= stream.avail_out;
                }
                if (stream.avail_in == 0) {
                    stream.avail_in = inLength > (uLong)max ? max : (uInt)inLength;
                    inLength -= stream.avail_in;
                }
                err = deflate(&stream, inLength ? Z_NO_FLUSH : Z_FINISH);
            } while (err == Z_OK);
            *outLength = stream.total_out;
            deflateEnd(&stream);
        }
        int ret = err == Z_STREAM_END ? Z_OK : err;
        if (ret != Z_OK)
        {
            if (ret == Z_MEM_ERROR)
            {
                Logging::Write("ZipUtils: Out of memory while compressing map data!");
            }
            else if (ret == Z_BUF_ERROR)
            {
                Logging::Write("ZipUtils: There was not enough room in the output buffer!");
            }
            *outLength = 0;
            return false;
        }
        return true;
    }
}