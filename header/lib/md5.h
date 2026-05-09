/**
 * This is an OpenSSL-compatible implementation of the RSA Data Security, Inc.
 * MD5 Message-Digest Algorithm (RFC 1321).
 *
 * @link   http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 * @author Alexander Peslyak <solar@openwall.com>
 */
#pragma once

// type aliases
typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;

typedef struct {
   uint  lo, hi;
   uint  a, b, c, d;
   uchar buffer[64];
   uint  block[16];
} MD5Context;

extern void MD5_Init  (MD5Context* ctx);
extern void MD5_Update(MD5Context* ctx, const void* data, ulong size);
extern void MD5_Final (uchar* result, MD5Context* ctx);
