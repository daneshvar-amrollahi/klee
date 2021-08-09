/**
 *  @author ntauth
 *  @brief  Harness for MD5 leakage analysis
 */

#include "md5.h"

#include <cassert>

#include <klee/klee.h>

int MD5_digest(unsigned char* in, size_t len, unsigned char out[MD5_DIGEST_LENGTH])
{
   unsigned int digest_length;

   EVP_MD_CTX* ctx = EVP_MD_CTX_new();
   EVP_DigestInit_ex(ctx, EVP_md5(), nullptr);
   
   if (!EVP_DigestUpdate(ctx, in, len)) {
     return -1;
   }

   if (!EVP_DigestFinal_ex(ctx, out, &digest_length)) {
      return -1;
   }

   EVP_MD_CTX_destroy(ctx);

   return digest_length;
}

int main(int argc, char** argv)
{
   // Allocate data
   unsigned char* data = new unsigned char[Md5DataSize];
   unsigned char digest[MD5_DIGEST_LENGTH];

   // Make symbolic data
   klee_make_symbolic(data, Md5DataSize, "data");

   // Taint data
   klee_set_taint(1, data, Md5DataSize);

   // Digest
   assert(MD5_digest(data, Md5DataSize, digest) > 0);

   return 0;
}