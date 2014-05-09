/**
 * Name: sha256.h
 * Author: martyj (GitHub)
 * Description: Process SHA-256 for 32bit
 **/

#ifndef _SHA256_H
#define _SHA256_H

#ifndef uint8
	#define uint8 unsigned char
#endif

#ifndef uint32
	#if defined(__i386__)
		#define uint32 unsigned long int
	#elif defined(_WIN32) || defined(_WIN64)
		typedef unsigned __int32 uint32;
	#else
		#define uint32 unsigned int
	#endif
#endif

typedef struct sha256_32byte_context
{
	uint32 state[8];
}
sha256_32byte_context;

void sha256_32byte_starts(sha256_32byte_context *ctx);
void sha256_32byte_update(sha256_32byte_context *ctx, uint8 *input);
void sha256_32byte_finish(sha256_32byte_context *ctx, uint8 digest[32]);
void sha256_32byte(uint8* str, uint8 output[32]);

char charToHex(char val);
void byteToHex(char bytes[2], uint8 byte);
void byte_array_to_str(uint8* bytes, char* str, int len);

#endif /* sha2.h */
