#ifndef __B64_H__
#define __B64_H__

#include "common.h"

char * base64_decode(const char *src, size_t len,
			      size_t *out_len);

void
b64_encode(unsigned char *dst, const unsigned char *src, size_t siz);

#endif
