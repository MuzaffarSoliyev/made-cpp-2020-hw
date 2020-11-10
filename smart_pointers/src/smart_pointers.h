#pragma once

namespace task {

template <class T>
class UniquePtr {
 public:
  T* p = nullptr;

  UniquePtr() {}
  explicit UniquePtr(T* ptr) : p(ptr) {}
  UniquePtr(UniquePtr&& uptr);
  UniquePtr(const UniquePtr& uptr) = delete;
  ~UniquePtr();
  UniquePtr& operator=(UniquePtr&& uptr);
  UniquePtr operator=(const UniquePtr& uptr) = delete;
  T* get() const;
  T* get();
  T& operator*() const;
  T& operator*();
  T* operator->() const;
  T* operator->();
  T* release();
  void reset(T* new_ptr = 0);
  void swap(UniquePtr& uptr);
};

template <class T>
struct ControlBlock {
  T* p;
  size_t ref_count;
  size_t weak_count;
  ControlBlock(T* ptr, size_t rc, size_t wc)
      : p(ptr), ref_count(rc), weak_count(wc) {}
};

template <class T>
class SharedPtr;

template <class T>
class WeakPtr;

template <class T>
class SharedPtr {
 public:
  ControlBlock<T>* contr_block;

  SharedPtr() : contr_block(nullptr) {}
  explicit SharedPtr(T* ptr);
  SharedPtr(const SharedPtr& shptr);
  SharedPtr(SharedPtr&& shptr);
  SharedPtr(const WeakPtr<T>& wptr);
  ~SharedPtr();
  SharedPtr<T>& operator=(const SharedPtr& shptr);
  SharedPtr<T>& operator=(SharedPtr&& shptr);

  T* get();
  T& operator*();
  T* operator->();

  size_t use_count() const;
  void reset(T* new_ptr = 0);
  void swap(SharedPtr& shptr);
};

template <class T>
class WeakPtr {
 public:
  ControlBlock<T>* contr_block;

  WeakPtr() : contr_block(nullptr) {}
  WeakPtr(const WeakPtr<T>& wptr);
  WeakPtr(const SharedPtr<T>& shptr);
  WeakPtr(WeakPtr&& wptr);
  ~WeakPtr();
  WeakPtr& operator=(const WeakPtr& wptr);
  WeakPtr& operator=(WeakPtr&& wptr);
  WeakPtr& operator=(SharedPtr<T>&& shptr);

  SharedPtr<T> lock();
  size_t use_count();
  bool expired();
  void reset();
  void swap(WeakPtr& wptr);
};

}  // namespace task

#include "smart_pointers.tpp"
