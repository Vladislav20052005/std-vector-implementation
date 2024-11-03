#include <cstddef>
#include <stdecept>
#include <memory>
#include <utility>

template <typename T> class Vector {
public:
  Vector() : size_(0), reserved_(8), data_(new T[8]) {}
  Vector(size_t size, const T &t = T()) : size_(0), reserved_(8) {
    data_ = new T[8];
    for (size_t i = 0; i < size; i++) {
      PushBack(t);
    }
  }
  template <typename Iterator>
  Vector(Iterator begin, Iterator end) : size_(0), reserved_(8) {
    data_ = new T[8];
    for (Iterator iter = begin; iter != end; ++iter) {
      PushBack(*iter);
    }
  }

  Vector(std::initializer_list<T> il) : size_(0), reserved_(8) {
    data_ = new T[8];
    for (T t : il) {
      PushBack(t);
    }
  }
  Vector(const Vector<T> &rhs) : size_(rhs.size_), reserved_(rhs.reserved_) {
    data_ = new T[reserved_];
    for (size_t i = 0; i < size_; i++) {
      data_[i] = rhs.data_[i];
    }
  }
  Vector(Vector &&rhs) : size_(rhs.size_), reserved_(rhs.reserved_) {
    Swap(rhs);
  }
  ~Vector() {
    delete[] data_;
    data_ = nullptr;
  }

  Vector<T> &operator=(const Vector<T> &rhs) {
    delete[] data_;
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

  void PushBack(T t) {
    if (size_ == reserved_) {
      Reallocate();
    }
    data_[size_] = t;
    size_++;
  }

  void PopBack() { size_--; }

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
      new_data[i] = data_[i];
    }
    delete[] data_;
    std::swap(new_data, data_);
    reserved_ *= 2;
  }

private:
  T *data_;
  size_t size_;
  size_t reserved_;
};