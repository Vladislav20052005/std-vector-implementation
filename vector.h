#include <cstddef>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#define MIN_RESERVED 8

size_t AlignPow2(size_t x) {
  size_t cnt = 0;
  for (size_t i = 0; i < sizeof(size_t) * 8; i++) {
    if ((1ULL << i) & x) {
      cnt++;
    }
  }
  if (cnt <= 1) {
    return x;
  }
  for (size_t i = sizeof(size_t) * 8 - 1; i >= 0; i--) {
    if ((1ULL << i) & x) {
      return (1ULL << (i + 1));
    }
  }
  return 0;
}

template <typename T> class Vector {
public:
  Vector() : size_(0), reserved_(MIN_RESERVED), data_(new T[MIN_RESERVED]) {}
  Vector(size_t size, T t = T()) : size_(size), reserved_(AlignPow2(size)) {
    data_ = new T[reserved_];
    for (size_t i = 0; i < size; i++) {
      data_[i] = t;
    }
  }
  template <typename Iterator,
            std::enable_if_t<!std::is_arithmetic<Iterator>::value> * = 0>
  Vector(Iterator begin, Iterator end) {
    size_t size = 0;
    for (Iterator iter = begin; iter != end; ++iter)
      size++;
    size_ = size;
    reserved_ = AlignPow2(size);
    data_ = new T[reserved_];
    size_t i = 0;
    for (Iterator iter = begin; iter != end; ++iter) {
      data_[i] = *iter;
      i++;
    }
  }

  Vector(std::initializer_list<T> il)
      : size_(il.size()), reserved_(AlignPow2(il.size())) {
    data_ = new T[reserved_];
    size_t i = 0;
    for (const T &t : il) {
      data_[i] = t;
      i++;
    }
  }
  Vector(const Vector<T> &rhs) : size_(rhs.size_), reserved_(rhs.reserved_) {
    data_ = new T[reserved_];
    for (size_t i = 0; i < size_; i++) {
      data_[i] = rhs.data_[i];
    }
  }
  Vector(Vector &&rhs)
      : size_(0), reserved_(MIN_RESERVED), data_(new T[MIN_RESERVED]) {
    Swap(rhs);
  }
  ~Vector() {
    if (data_ != nullptr) {
      delete[] data_;
      data_ = nullptr;
    }
  }

  Vector<T> &operator=(const Vector<T> &rhs) {
    this->~Vector();
    data_ = new T[rhs.reserved_];

    size_ = rhs.size_;
    reserved_ = rhs.reserved_;

    for (size_t i = 0; i < size_; i++) {
      data_[i] = rhs.data_[i];
    }
    return *this;
  }

  Vector<T> &operator=(Vector<T> &&rhs) {
    Swap(rhs);
    return *this;
  }

  T &operator[](size_t index) { return data_[index]; }

  const T &operator[](size_t index) const { return data_[index]; }

  T &At(size_t index) {
    if (index >= size_) {
      throw std::runtime_error("Incorrect index : " + std::to_string(index));
    }
    return data_[index];
  }

  const T &At(size_t index) const {
    if (index >= size_)
      throw std::runtime_error("Incorrect index : " + std::to_string(index));
    return data_[index];
  }

  void PushBack(T t = T()) {
    if (size_ == reserved_) {
      Reallocate();
    }
    data_[size_] = t;
    size_++;
  }

  T &&PopBack() {
    if (size_ == 0) {
      throw std::runtime_error("Trying to popback a vector with 0 size");
    }
    size_--;
    return std::move(data_[size_ + 1]);
  }

  void Swap(Vector<T> &rhs) {
    std::swap(data_, rhs.data_);
    std::swap(size_, rhs.size_);
    std::swap(reserved_, rhs.reserved_);
  }

  size_t Size() { return size_; }

  const T &Front() {
    if (size_ == 0)
      throw std::runtime_error("0 size");
    return *data_;
  }
  const T &Back() {
    if (size_ == 0)
      throw std::runtime_error("0 size");
    return *(data_ + size_ - 1);
  }

  T *begin() { return data_; }

  T *end() { return data_ + size_; }

private:
  void Reallocate() {
    T *new_data = new T[reserved_ * 2];
    for (size_t i = 0; i < reserved_; i++) {
      new_data[i] = std::move(data_[i]);
    }
    this->~Vector();
    std::swap(new_data, data_);
    reserved_ *= 2;
  }

private:
  T *data_;
  size_t size_;
  size_t reserved_;
};
