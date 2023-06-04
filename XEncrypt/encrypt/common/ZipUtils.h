#pragma once

#include <string>
#include "config.h"
namespace encrypt
{
	class XENCRYPT_API ZipUtils
	{
	public:
		/**
		 * Inflates either zlib or gzip deflated memory. The inflated memory is expected to be freed by the caller.
		 * It will allocate 256k for the destination buffer. If it is not enough it will multiply the previous buffer size per 2, until there is enough memory.
		 * @return The length of the deflated buffer.
		 */
		static bool GZipUncompress(const byte* in, size_t inLength, byte** out, size_t* outSize);
		
		static unsigned long CompressMemoryBound(unsigned long inLength);
		/**
		 * Deflates either zlib or gzip deflated memory. The Deflated memory is expected to be freed by the caller.
		 * It will allocate 256k for the destination buffer. If it is not enough it will multiply the previous buffer size per 2, until there is enough memory.
		 * @return The length of the deflated buffer.
		 */
		static bool GZipCompress(const byte* in, size_t inLength, byte* out, unsigned long* outSize);
        /**
        * Compress from file source to file dest until EOF on source.
        * returns Z_OK on success
        * Z_MEM_ERROR if memory could not be allocated for processing
        * Z_STREAM_ERROR if an invalid compression level is supplied
        * Z_VERSION_ERROR if the version of zlib.h and the version of the library linked do not match, 
        * or Z_ERRNO if there is an error reading or writing the files. 
        */
        static int GZipCompress(const byte* in_str, size_t in_len, std::string& out_str, int level = -1);
        /**
        * Decompress from file source to file dest until stream ends or EOF.
        * returns Z_OK on success
        * Z_MEM_ERROR if memory could not be allocated for processing
        * Z_DATA_ERROR if the deflate data is invalid or incomplete
        * Z_VERSION_ERROR if the version of zlib.h and the version of the library linked do not match
        * or Z_ERRNO if there is an error reading or writing the files. 
        */
        static int ZipUtils::GZipDecompress(const byte* in_str, size_t in_len, std::string& out_str);
		/**
		* Inflates either zlib or gzip deflated memory. The inflated memory is expected to be freed by the caller.
		* @param outLengthHint It is assumed to be the needed room to allocate the inflated buffer.
		* @return The length of the deflated buffer.
		*/
		static bool GZipUncompressWithHint(const byte* in, size_t inLength, byte** out, size_t* outSize, size_t outLengthHint);

		/**
		 * Test the buffer is GZip format or not.
		 * @return True is GZip format. false is not.
		 */
		static bool IsGZipBuffer(const byte* buffer, size_t len)
		{
			return (len >= 2 && (buffer[0] == 0x1F && buffer[1] == 0x8B));
		}

	private:
		static int GZipUncompressWithHint(const byte* in, size_t inLength, size_t outLengthHint, byte** out, size_t* outLength);
	};
}

