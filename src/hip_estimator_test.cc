#include "hip_estimator.h"
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
}  // namespace

TEST(hip_estimator, random) {
  static const int kNumTrial = 100;
  static const size_t kNumInsertion = 100000;
  static const int kNumBucketBits = 10;

  double result_sum_se = 0.0;
  for (int trial = 0; trial < kNumTrial; ++trial) {
    hip_estimator::hip_estimator<uint64_t> hip(kNumBucketBits);

    std::unordered_set<uint64_t> us;
    while (us.size() < kNumInsertion) {
      uint64_t v = xorshift64();
      us.insert(v);
      hip.insert(v);
    }

    const double e = fabs(double(hip.count()) - kNumInsertion);
    result_sum_se += e * e;
  }

  const double result_mse = result_sum_se / kNumTrial;
  const double result_rmse = sqrt(result_mse);
  const double result_nrmse = result_rmse / kNumInsertion;

  const double theory_nrmse = sqrt(3.0 / (4.0 * (1 << kNumBucketBits)));
  std::cout << result_nrmse << " < " << theory_nrmse << std::endl;
  EXPECT_LT(result_nrmse, theory_nrmse);
}
