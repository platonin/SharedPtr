#ifndef SHARED
#define SHARED
#include <cstdint>
#include <utility>

template <typename T> class SharedPtr {
private:
  T *ptr_;
  int64_t *count_;

public:
  SharedPtr() {
    this->ptr_ = nullptr;
    count_ = nullptr;
  }

  explicit SharedPtr(T *ptr) {
    this->ptr_ = ptr;
    if (ptr != nullptr) {
      count_ = new int64_t(1);
    } else {
      count_ = nullptr;
    }
  }

  SharedPtr(const SharedPtr &ptr) {
    ptr_ = ptr.ptr_;
    count_ = ptr.count_;
    if (count_ != nullptr) {
      (*count_)++;
    }
  }

  SharedPtr &operator=(const SharedPtr &ptr) {
    if (ptr.ptr_ == ptr_ && ptr.count_ == count_) {
    //   if (count_ != nullptr) {
    //     (*count_)++;
    //   }
      return *this;
    }

    if (count_ != nullptr) {
      (*count_)--;
      if (*count_ == 0) {
        delete ptr_;
        delete count_;
        ptr_ = nullptr;
        count_ = nullptr;
      }
    }

    ptr_ = ptr.ptr_;
    count_ = ptr.count_;
    if (count_ != nullptr) {
      (*count_)++;
    }
    return *this;
  }

  SharedPtr(SharedPtr &&ptr) noexcept {
    ptr_ = std::move(ptr.ptr_);
    count_ = std::move(ptr.count_);
    ptr.ptr_ = nullptr;
    ptr.count_ = nullptr;
  }

  SharedPtr &operator=(SharedPtr &&ptr) noexcept {
    if (count_ != nullptr) {
      (*count_)--;
      if (*count_ == 0) {
        delete ptr_;
        delete count_;
        ptr_ = nullptr;
        count_ = nullptr;
      }
    }
    ptr_ = std::move(ptr.ptr_);
    count_ = std::move(ptr.count_);
    ptr.ptr_ = nullptr;
    ptr.count_ = nullptr;
    return *this;
  }

  ~SharedPtr() {
    if (count_ != nullptr) {
      (*count_)--;
      if (*count_ == 0) {
        delete ptr_;
        delete count_;
        ptr_ = nullptr;
        count_ = nullptr;
      }
    }
  }

  void Reset(T *ptr = nullptr) {
    if (count_ != nullptr) {
      (*count_)--;
      if (*count_ == 0) {
        delete ptr_;
        delete count_;
      }
    }
    this->ptr_ = ptr;
    count_ = new int64_t(1);
  }

  T *Get() const { return ptr_; }

  int64_t UseCount() const {
    if (count_ != nullptr && ptr_ != nullptr) {
      return *count_;
    }
    return static_cast<int64_t>(0);
  }

  void Swap(SharedPtr &new_ptr) {
    T *temp_ptr = new_ptr.ptr_;
    int64_t *temp_c = new_ptr.count_;
    new_ptr.ptr_ = this->ptr_;
    new_ptr.count_ = this->count_;
    this->ptr_ = temp_ptr;
    this->count_ = temp_c;
  }

  T &operator*() const { return *this->ptr_; }

  T *operator->() const { return this->ptr_; }

  explicit operator bool() const { return this->ptr_ != nullptr; }
};

#endif