/**
 * @author ntauth
 * @brief  Harness for MD5 leakage analysis
 */

#pragma once

#include <stdlib.h>
#include <string.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/md5.h>

constexpr size_t Md5DataSize = 64;

int MD5_digest(unsigned char* in, size_t len, unsigned char out[MD5_DIGEST_LENGTH]);
