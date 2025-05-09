#pragma once

#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>

class MemoryArena {
 public:
  char* start = nullptr;
  // char* end = nullptr;
  char* buffer = nullptr;
  size_t totalSize = 0;
  size_t offset = 0;
  std::vector<std::function<void()>> destructors;

  explicit MemoryArena(std::size_t size) : totalSize(size), offset(0) {
    buffer = static_cast<char*>(std::malloc(size));
    if (!buffer) {
      throw std::bad_alloc();
    }
    start = buffer;
    // end = buffer + size;

    std::cout << buffer;
  }

  ~MemoryArena() {
    for (auto it = destructors.rbegin(); it != destructors.rend(); ++it) {
      (*it)();
    }
    std::free(buffer);
  }

  size_t alignOffset(size_t alignment) const {
    size_t currentPtr = reinterpret_cast<size_t>(start + offset);
    size_t padding = (alignment - (currentPtr % alignment)) % alignment;
    return offset + padding;
  }

  template <typename T, typename... Args>
  T* insertData(Args&&... args) {
    static_assert(!std::is_array_v<T>, "Arrays not supported");

    size_t alignedOffset = alignOffset(alignof(T));

    if (alignedOffset + sizeof(T) > totalSize) {
      throw std::bad_alloc();
    }

    T* object = new (start + alignedOffset) T(std::forward<Args>(args)...);
    offset = alignedOffset + sizeof(T);

    if constexpr (!std::is_trivially_destructible_v<T>) {
      destructors.emplace_back([object]() { object->~T(); });
    }

    return object;
  }

  template <typename T>
  void printMemoryRegion(const std::string& name, T* ptr) const {
    size_t objectSize = sizeof(T);
    size_t align = alignof(T);

    uintptr_t base = reinterpret_cast<uintptr_t>(start);
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    size_t relativeOffset = addr - base;

    std::cout << name << ": [" << static_cast<const void*>(ptr) << " - "
              << static_cast<const void*>(start + relativeOffset + objectSize)
              << "]"
              << " size=" << objectSize << " align=" << align << std::endl;
  }
};
