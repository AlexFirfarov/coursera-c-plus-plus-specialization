#pragma once

#include <cstdlib>
#include <algorithm>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector() : data(nullptr), size_(0), capacity_(0) {}

  explicit SimpleVector(size_t size) : data(new T[size]), size_(size), capacity_(size) {}

  ~SimpleVector() {
    delete[] data;
  }

  SimpleVector& operator=(const SimpleVector& other) {
    if (capacity_ < other.size_) {
      delete[] data;
      data = new T[other.capacity_];
      capacity_ = other.capacity_;
    }

    size_ = other.size_;
    std::copy(other.begin(), other.end(), begin());

    return *this;
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
    if (size_ == capacity_) {
      size_t new_capacity_ = capacity_ > 0 ? 2 * capacity_ : 1;
      T* new_data = new T[new_capacity_];
      std::copy(data, data + size_, new_data);
      delete[] data;

      data = new_data;
      capacity_ = new_capacity_;
    }

    data[size_++] = value;
  }

private:
  T* data;
  size_t size_;
  size_t capacity_;
};
