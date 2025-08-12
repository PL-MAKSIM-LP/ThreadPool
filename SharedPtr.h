#pragma once

#include <cstddef>
#include <utility>
template <typename T>
class MySharedPtr {
  struct ControlBlock {
    ControlBlock(T* _data) : data(_data), count(1) {}
    T* data;
    size_t count;
  };

  ControlBlock* controlBlock = nullptr;

 public:
  MySharedPtr() {}
  MySharedPtr(T* data) {
    if (data) {
      controlBlock = new ControlBlock(data);
    }
  }

  MySharedPtr(const T& data) : controlBlock(new ControlBlock(new T(data))) {}
  MySharedPtr(T&& data)
      : controlBlock(new ControlBlock(new T(std::move(data)))) {}

  MySharedPtr(const MySharedPtr& other) {
    controlBlock = other.controlBlock;
    if (controlBlock) {
      ++controlBlock->count;
    }
  }

  MySharedPtr& operator=(const MySharedPtr& other) {
    if (&other != this) {
      release();
      controlBlock = other.controlBlock;
      if (controlBlock) {
        ++controlBlock->count;
      }
    }

    return *this;
  }

  MySharedPtr(MySharedPtr&& other) {
    controlBlock = other.controlBlock;
    other.controlBlock = nullptr;
  }

  MySharedPtr& operator=(MySharedPtr&& other) {
    if (&other != this) {
      release();
      controlBlock = other.controlBlock;
      other.controlBlock = nullptr;
    }
    return *this;
  }

  ~MySharedPtr() { release(); }

  void release() {
    if (controlBlock) {
      --controlBlock->count;
      if (controlBlock->count == 0) {
        delete controlBlock->data;
        delete controlBlock;
        controlBlock = nullptr;
      }
    }
  }
  T* get() const { return controlBlock ? controlBlock->data : nullptr; }

  T& operator*() const { return *get(); }

  T* operator->() const { return get(); }

  size_t use_count() const { return controlBlock ? controlBlock->count : 0; }

  operator bool() const { return get() != nullptr; }
};
