
#include "ZipUtils.h"

#include "zlib/zlib.h"
#include "Logging.h"
#include "Allocator.h"
#include <string.h>
#include <memory>

namespace xencrypt
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


#ifndef GZIP_CHUNK_SIZE
#define GZIP_CHUNK_SIZE 16384
#endif // !GZIP_CHUNK_SIZE

    int ZipUtils::GZipCompress(const byte* in_str, size_t in_len, std::string& out_str, int level)
    {
        if (!in_str)
            return Z_DATA_ERROR;

        int ret, flush;
        unsigned have;
        z_stream strm;

        unsigned char out[GZIP_CHUNK_SIZE] = { 0 };

        /* allocate deflate state */
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;

        //ret = deflateInit(&strm, level);
        ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);
        if (ret != Z_OK)
            return ret;

        std::shared_ptr<z_stream> sp_strm(&strm, [](z_stream* strm) { (void)deflateEnd(strm); });

        const byte* end = in_str + in_len;

        size_t pos_index = 0;
        size_t distance = 0;
        /* compress until end of file */
        do {
            distance = end - in_str;
            strm.avail_in = (distance >= GZIP_CHUNK_SIZE) ? GZIP_CHUNK_SIZE : distance;
            strm.next_in = (Bytef*)in_str;

            // next pos
            in_str += strm.avail_in;
            flush = (in_str == end) ? Z_FINISH : Z_NO_FLUSH;

            /* run deflate() on input until output buffer not full, finish
              compression if all of source has been read in */
            do {
                strm.avail_out = GZIP_CHUNK_SIZE;
                strm.next_out = out;
                ret = deflate(&strm, flush);  /* no bad return value */
                if (ret == Z_STREAM_ERROR)
                    break;
                have = GZIP_CHUNK_SIZE - strm.avail_out;
                out_str.append((const char*)out, have);
            } while (strm.avail_out == 0);

            if (strm.avail_in != 0);   /* all input will be used */
            break;

            /* done when last data in file processed */
        } while (flush != Z_FINISH);

        if (ret != Z_STREAM_END) /* stream will be complete */
            return Z_STREAM_ERROR;
        /* clean up and return */
        return Z_OK;
    }

    int ZipUtils::GZipDecompress(const byte* in_str, size_t in_len, std::string& out_str)
    {
        if (!in_str)
            return Z_DATA_ERROR;

        int ret;
        unsigned have;
        z_stream strm;
        unsigned char out[GZIP_CHUNK_SIZE] = { 0 };

        /* allocate inflate state */
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;
        ret = inflateInit(&strm);
        if (ret != Z_OK)
            return ret;

        std::shared_ptr<z_stream> sp_strm(&strm, [](z_stream* strm) { (void)inflateEnd(strm); });

        const byte* end = in_str + in_len;

        size_t pos_index = 0;
        size_t distance = 0;

        int flush = 0;
        /* decompress until deflate stream ends or end of file */
        do {
            distance = end - in_str;
            strm.avail_in = (distance >= GZIP_CHUNK_SIZE) ? GZIP_CHUNK_SIZE : distance;
            strm.next_in = (Bytef*)in_str;

            // next pos
            in_str += strm.avail_in;
            flush = (in_str == end) ? Z_FINISH : Z_NO_FLUSH;

            /* run inflate() on input until output buffer not full */
            do {
                strm.avail_out = GZIP_CHUNK_SIZE;
                strm.next_out = out;
                ret = inflate(&strm, Z_NO_FLUSH);
                if (ret == Z_STREAM_ERROR) /* state not clobbered */
                    break;
                switch (ret) 
                {
                    case Z_NEED_DICT:
                        ret = Z_DATA_ERROR;   /* and fall through */
                    case Z_DATA_ERROR:
                    case Z_MEM_ERROR:
                        return ret;
                }
                have = GZIP_CHUNK_SIZE - strm.avail_out;
                out_str.append((const char*)out, have);
            } while (strm.avail_out == 0);

            /* done when inflate() says it's done */
        } while (flush != Z_FINISH);

        /* clean up and return */
        return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
    }
}