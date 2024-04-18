#define MD5_IMPLEMENTATION
#include "./md5.h"

int main(void) {
	unsigned char digest[16];
	md5("md5", digest);
	md5_print(digest);
}
