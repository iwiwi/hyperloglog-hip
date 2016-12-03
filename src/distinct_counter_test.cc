#include "distinct_counter.h"
#include <unordered_set>
#include "gtest/gtest.h"
using namespace std;

namespace {
// xorshift64* random generator [Vigna, 2014]
uint64_t x = 123456789LL;
uint64_t xorshift64() {
  x ^= x >> 12; // a
  x ^= x << 25; // b
  x ^= x >> 27; // c
  return x * 2685821657736338717LL;
}

template<typename Key>
class distinct_counter_tester {
 public:
  typedef hyperloglog_hip::distinct_counter<Key, std::hash<Key>, 8> distinct_counter_type;
  typedef typename distinct_counter_type::key_type key_type;

  distinct_counter_tester(size_t num_bucket_bits) : hip_(num_bucket_bits) {}

  void insert(const key_type &v) {
    hip_.insert(v);
    us_.insert(v);
  }

  size_t count() {
    return us_.size();
  }

  size_t error() {
    size_t c1 = hip_.count(), c2 = us_.size();
    return c1 > c2 ? c1 - c2 : c2 - c1;
  }

 private:
  distinct_counter_type hip_;
  unordered_set<key_type> us_;
};
}  // namespace

TEST(distinct_counter, random) {
  static const int kNumTrial = 100;
  static const size_t kNumInsertion = 100000;
  static const int kNumBucketBits = 10;

  double result_sum_se = 0.0;
  for (int trial = 0; trial < kNumTrial; ++trial) {
    distinct_counter_tester<uint64_t> tester(kNumBucketBits);

    while (tester.count() < kNumInsertion) {
      tester.insert(xorshift64());
    }

    const double e = tester.error();
    result_sum_se += e * e;
  }

  const double result_mse = result_sum_se / kNumTrial;
  const double result_rmse = sqrt(result_mse);
  const double result_nrmse = result_rmse / kNumInsertion;

  const double theory_nrmse = sqrt(3.0 / (4.0 * (1 << kNumBucketBits)));
  std::cout << result_nrmse << " < " << theory_nrmse << std::endl;
  EXPECT_LT(result_nrmse, theory_nrmse);
}
