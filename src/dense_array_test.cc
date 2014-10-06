#include "dense_array.h"
#include "gtest/gtest.h"

TEST(dense_array, zero) {
  hip_estimator::dense_array<8> a(0);
}

TEST(dense_array, trivial) {
  hip_estimator::dense_array<8> a(3);
  a.set(0, 10);
  a.set(1, 20);
  a.set(2, 30);
  ASSERT_EQ(10, a.get(0));
  ASSERT_EQ(20, a.get(1));
  ASSERT_EQ(30, a.get(2));
}
