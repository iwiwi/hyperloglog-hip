#include "dense_array.h"
#include "gtest/gtest.h"
#include <vector>
using namespace std;
using namespace hyperloglog_hip;
using testing::Types;

namespace {
// xorshift64* random generator [Vigna, 2014]
uint64_t x = 123456789LL;
uint64_t xorshift64() {
  x ^= x >> 12;
  x ^= x << 25;
  x ^= x >> 27;
  return x * 2685821657736338717LL;
}

template<int NumRegisterBits, typename Value = uint8_t>
class dense_array_tester {
 public:
  typedef dense_array<NumRegisterBits, Value> dense_array_type;
  typedef typename dense_array_type::value_type value_type;

  dense_array_tester(size_t size) : size_(size), da_(size), v_(size) {}

  void check_all() {
    for (size_t i = 0; i < size_; ++i) {
      ASSERT_NO_FATAL_FAILURE(check(i));
    }
  }

  void check(size_t i) {
    ASSERT_EQ(da_.get(i), v_[i]) << i;
  }

  void set(size_t i, value_type x) {
    x &= register_mask;
    v_[i] = x;
    da_.set(i, x);
  }

  void set_random(size_t i) {
    value_type x = xorshift64() & register_mask;
    set(i, x);
  }

 private:
  static constexpr size_t num_register_bits = NumRegisterBits;
  static constexpr uint64_t register_mask = uint64_t(-1) >> (64 - num_register_bits);

  size_t size_;
  dense_array_type da_;
  vector<value_type> v_;
};

typedef Types<
    dense_array_tester<1>, dense_array_tester<2>, dense_array_tester<3>,
    dense_array_tester<4>, dense_array_tester<5>, dense_array_tester<7>,
    dense_array_tester<1, uint16_t>, dense_array_tester<15, uint16_t>,
    dense_array_tester<1, uint32_t>, dense_array_tester<31, uint32_t>,
    dense_array_tester<1, uint64_t>, dense_array_tester<63, uint64_t>,
    dense_array_tester<8, uint8_t>, dense_array_tester<16, uint16_t>,
    dense_array_tester<32, uint32_t>, dense_array_tester<64, uint64_t>
> TesterTypes;
}

template<typename T>
class dense_array_test : public testing::Test {};
TYPED_TEST_CASE(dense_array_test, TesterTypes);

TYPED_TEST(dense_array_test, zero) {
  TypeParam t(0);
  ASSERT_NO_FATAL_FAILURE(t.check_all());
}

TYPED_TEST(dense_array_test, trivial3) {
  TypeParam t(3);
  ASSERT_NO_FATAL_FAILURE(t.check_all());
  t.set(0, 10);
  t.set(1, 20);
  t.set(2, 30);
  ASSERT_NO_FATAL_FAILURE(t.check_all());
}

TYPED_TEST(dense_array_test, random_size) {
  static const int kNumTrial = 10;
  static const size_t kMaxSize = 10000;

  for (int trial = 0; trial < kNumTrial; ++trial) {
    size_t size = xorshift64() % kMaxSize;

    TypeParam t(size);
    for (size_t i = 0; i < size; ++i) {
      t.set_random(xorshift64() % size);
    }
    ASSERT_NO_FATAL_FAILURE(t.check_all());
  }
}

TYPED_TEST(dense_array_test, power2_size) {
  static const int kNumTrial = 10;
  static const int kMaxPower = 12;

  for (int p = 0; p <= kMaxPower; ++p) {
    for (int trial = 0; trial < kNumTrial; ++trial) {
      size_t size = size_t(1) << p;

      TypeParam t(size);
      for (size_t i = 0; i < size; ++i) {
        t.set_random(xorshift64() % size);
      }
      ASSERT_NO_FATAL_FAILURE(t.check_all());
    }
  }
}
