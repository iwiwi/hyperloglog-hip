#include <cstdint>
#include <memory>

namespace hip_estimator {
template<int NumRegisterBits>
class dense_array {
  // TODO
};

template<>
class dense_array<8> {
 public:
  dense_array(size_t size) : data_(new uint8_t[size]()) {}

  uint8_t get(size_t pos) const {
    return data_[pos];
  }

  void set(size_t pos, uint8_t val) {
    data_[pos] = val;
  }

 private:
  std::unique_ptr<uint8_t[]> data_;
};
}  // namespace hip_estimator
