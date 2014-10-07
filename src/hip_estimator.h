#ifndef HIP_ESTIMATOR_HIP_ESTIMATOR_H_
#define HIP_ESTIMATOR_HIP_ESTIMATOR_H_

#include <algorithm>
#include <cstdint>
#include "dense_array.h"

namespace hip_estimator {
template<typename Key, typename Hash = std::hash<Key>, int NumRegisterBits = 5>
class hip_estimator {
 public:
  typedef Key key_type;
  typedef Hash hash_type;

  hip_estimator(size_t num_bucket_bits = 14)
    : num_bucket_bits_(num_bucket_bits), M_(1 << num_bucket_bits),
      c_(0), s_(1 << num_bucket_bits) {}

  void insert(const key_type &v) {
    static constexpr uint64_t num_register_bits = NumRegisterBits;
    static constexpr uint64_t register_limit = (uint64_t(1) << num_register_bits) - 1;

    const uint64_t h = hash_(v) * magic1 + magic2;
    const uint64_t h0 = h & ((uint64_t(1) << num_bucket_bits_) - 1);
    const uint64_t h1 = h >> num_bucket_bits_;

    const uint64_t b_old = M_.get(h0);
    const uint64_t b_new = h1 == 0 ? register_limit :
        std::min(register_limit, uint64_t(1 + __builtin_ctzl(h1)));

    if (b_new > b_old) {
      M_.set(h0, b_new);
      c_ += 1.0 / (s_ / (uint64_t(1) << num_bucket_bits_));
      s_ -= 1.0 / (uint64_t(1) << b_old);
      if (b_new < register_limit) {
        s_ += 1.0 / (uint64_t(1) << b_new);
      }
    }
  }

  size_t count() const {
    return round(c_);
  }

 private:
  static constexpr uint64_t magic1 = 1;  // TODO: large prime
  static constexpr uint64_t magic2 = 0;

  const size_t num_bucket_bits_;
  dense_array<NumRegisterBits> M_;
  double c_, s_;
  hash_type hash_;
};
}  // namespace hip_estimator

#endif  // HIP_ESTIMATOR_HIP_ESTIMATOR_H_
