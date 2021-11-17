#include <cstdint>
#include <algorithm>

using namespace std;

// Реализуйте SimpleVector в этом файле
// и отправьте его на проверку

template <typename T>
class SimpleVector {
public:
  SimpleVector() : data(nullptr), size_(0), capacity_(0) {}

  explicit SimpleVector(size_t size) : data(new T[size]), size_(size), capacity_(size) {}

  ~SimpleVector() {
    delete[] data;
  }

  T& operator[](size_t index) {
    return data[index];
  }

  T* begin() {
    return data;
  }

  const T* begin() const {
    return data;
  }

  T* end() {
    return data + size_;
  }

  const T* end() const {
    return data + size_;
  }

  size_t Size() const {
    return size_;
  }

  size_t Capacity() const {
    return capacity_;
  }

  void PushBack(const T& value) {
    ExpandIfNeeded();
    data[size_++] = move(value);
  }

  void PushBack(T&& value) {
    ExpandIfNeeded();
    data[size_++] = move(value);
  }

  // При необходимости перегрузите
  // существующие публичные методы

private:
  void ExpandIfNeeded() {
    if (size_ == capacity_) {
      size_t new_capacity_ = capacity_ > 0 ? 2 * capacity_ : 1;
      T* new_data = new T[new_capacity_];
      std::move(data, data + size_, new_data);
      delete[] data;

      data = new_data;
      capacity_ = new_capacity_;
    }
  }

  T* data;
  size_t size_;
  size_t capacity_;
};
