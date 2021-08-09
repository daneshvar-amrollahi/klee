/**
 * @author ntauth
 * @brief  RSA key generation harness for leakage analysis
 * @note   Taints are set in the OpenSSL source code (rsa_gen.c, bn_prime.c),
 *         that's where the pubkey is generated
 */

#include <cassert>
#include <cmath>
#include <memory>

extern "C" {
#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
}

#include <klee/klee.h>

#define ASSERT assert

using std::unique_ptr;

using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;

/**
 * @from: http://en.wikipedia.org/wiki/Hamming_weight#Efficient_implementation
 */
static inline uint64_t popcount64(uint64_t x)
{
  uint64_t m1 = 0x5555555555555555ll;
  uint64_t m2 = 0x3333333333333333ll;
  uint64_t m4 = 0x0F0F0F0F0F0F0F0Fll;
  uint64_t h01 = 0x0101010101010101ll;

  x -= (x >> 1) & m1;
  x = (x & m2) + ((x >> 2) & m2);
  x = (x + (x >> 4)) & m4;

  return (x * h01) >> 56;
}

int main(int argc, char *argv[])
{
  constexpr size_t LO_BOUND = 3, HI_BOUND = 5;

  int rc, bits_exp;
  unsigned int bits;

  RSA_ptr rsa(RSA_new(), ::RSA_free);
  BN_ptr bn(BN_new(), ::BN_free);

  rc = BN_set_word(bn.get(), RSA_F4);
  ASSERT(rc == 1);

  // Compute symbolic range for input size
  bits_exp = klee_range(LO_BOUND, HI_BOUND + 1, "bits_exp");

  for (int i = LO_BOUND; i <= HI_BOUND; i++) {
    if (bits_exp == i)
      bits_exp = i;
  }

  bits = 1 << bits_exp;

  // Generate RSA key of length `bits`
  rc = RSA_generate_key_ex(rsa.get(), bits, bn.get(), NULL);
  ASSERT(rc == 1);

  return 0;
}