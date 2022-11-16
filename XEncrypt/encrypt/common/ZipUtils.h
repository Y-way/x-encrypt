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
		static bool UncompressMemory(const byte* in, size_t inLength, byte** out, size_t* outSize);
		
		static unsigned long CompressMemoryBound(unsigned long inLength);
		/**
		 * Deflates either zlib or gzip deflated memory. The Deflated memory is expected to be freed by the caller.
		 * It will allocate 256k for the destination buffer. If it is not enough it will multiply the previous buffer size per 2, until there is enough memory.
		 * @return The length of the deflated buffer.
		 */
		static bool CompressMemory(const byte* in, size_t inLength, byte* out, unsigned long* outSize);

		/**
		* Inflates either zlib or gzip deflated memory. The inflated memory is expected to be freed by the caller.
		* @param outLengthHint It is assumed to be the needed room to allocate the inflated buffer.
		* @return The length of the deflated buffer.
		*/
		static bool UncompressMemoryWithHint(const byte* in, size_t inLength, byte** out, size_t* outSize, size_t outLengthHint);

		/**
		 * Test the buffer is GZip format or not.
		 * @return True is GZip format. false is not.
		 */
		static bool IsGZipBuffer(const byte* buffer, size_t len)
		{
			return (len >= 2 && (buffer[0] == 0x1F && buffer[1] == 0x8B));
		}

	private:
		static int UncompressMemoryWithHint(const byte* in, size_t inLength, size_t outLengthHint, byte** out, size_t* outLength);
	};
}

