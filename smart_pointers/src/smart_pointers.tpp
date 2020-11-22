#include "smart_pointers.h"

#include <iostream>

namespace task {

template <class T>
UniquePtr<T>::UniquePtr(UniquePtr&& uptr) {
  p = uptr.p;
  uptr.p = nullptr;
}
template <class T>
UniquePtr<T>::~UniquePtr() {
  delete p;
}

template <class T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& uptr) {
  p = uptr.p;
  uptr.p = nullptr;
  return *this;
}

template <class T>
T& UniquePtr<T>::operator*() {
  return *p;
}

template <class T>
T& UniquePtr<T>::operator*() const {
  return *p;
}
template <class T>
T* UniquePtr<T>::operator->() {
  return p;
}

template <class T>
T* UniquePtr<T>::operator->() const {
  return p;
}

template <class T>
T* UniquePtr<T>::get() const {
  return p;
}

template <class T>
T* UniquePtr<T>::get() {
  return p;
}

template <class T>
T* UniquePtr<T>::release() {
  T* ptr = p;
  p = nullptr;
  return ptr;
}

template <class T>
void UniquePtr<T>::reset(T* new_ptr) {
  T* tmp = p;
  p = new_ptr;
  delete tmp;
}

template <class T>
void UniquePtr<T>::swap(UniquePtr& uptr) {
  std::swap(p, uptr.p);
}

template <class T>
SharedPtr<T>::SharedPtr(T* ptr) {
  this->contr_block = new ControlBlock<T>(ptr, 1, 0);
}

template <class T>
SharedPtr<T>::SharedPtr(const SharedPtr& shptr) {
  this->contr_block = shptr.contr_block;
  this->contr_block->ref_count++;
}

template <class T>
SharedPtr<T>::SharedPtr(SharedPtr&& shptr) {
  this->contr_block = shptr.contr_block;
  shptr.contr_block = nullptr;
}

template <class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& wptr) {
  this->contr_block = wptr.contr_block;
  this->contr_block->ref_count++;
}

template <class T>
SharedPtr<T>::~SharedPtr() {
  if (contr_block) {
    contr_block->ref_count--;
    if (contr_block->ref_count == 0) {
      delete contr_block->p;
      contr_block->p = nullptr;
    }
    if (contr_block->ref_count == 0 && contr_block->weak_count == 0) {
      delete contr_block;
    }
  }
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& shptr) {
  if (&shptr == this) {
    return *this;
  }

  if (contr_block) {
    contr_block->ref_count--;
    if (contr_block->ref_count == 0) {
      delete contr_block->p;
      contr_block->p = nullptr;
    }
    if (contr_block->ref_count == 0 && contr_block->weak_count == 0) {
      delete contr_block;
    }
  }

  contr_block = shptr.contr_block;
  contr_block->ref_count++;
  return *this;
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& shptr) {
  if (&shptr == this) {
    return *this;
  }

  if (contr_block) {
    contr_block->ref_count--;
    if (contr_block->ref_count == 0) {
      delete contr_block->p;
      contr_block->p = nullptr;
    }
    if (contr_block->ref_count == 0 && contr_block->weak_count == 0) {
      delete contr_block;
    }
  }
  contr_block = shptr.contr_block;
  shptr.contr_block = nullptr;
  return *this;
}

template <class T>
T* SharedPtr<T>::get() {
  return contr_block->p;
}

template <class T>
T& SharedPtr<T>::operator*() {
  return *(contr_block->p);
}

template <class T>
T* SharedPtr<T>::operator->() {
  return contr_block->p;
}

template <class T>
size_t SharedPtr<T>::use_count() const {
  return contr_block->ref_count;
}

template <class T>
void SharedPtr<T>::reset(T* new_ptr) {
  if (contr_block != nullptr) {
    contr_block->ref_count--;
    if (contr_block->ref_count == 0) {
      delete contr_block->p;
      contr_block->p = nullptr;
    }
    if (contr_block->ref_count == 0 && contr_block->weak_count == 0) {
      delete contr_block;
    }
    contr_block = new ControlBlock<T>(new_ptr, 1, 0);
  }
}

template <class T>
void SharedPtr<T>::swap(SharedPtr& shptr) {
  std::swap(contr_block, shptr.contr_block);
}

template <class T>
WeakPtr<T>::WeakPtr(const WeakPtr& wptr) {
  contr_block = wptr.contr_block;
  contr_block->weak_count++;
}

template <class T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& shptr) {
  contr_block = shptr.contr_block;
  contr_block->weak_count++;
}

template <class T>
WeakPtr<T>::WeakPtr(WeakPtr&& wptr) {
  contr_block = wptr.contr_block;
  wptr.contr_block = nullptr;
}

template <class T>
WeakPtr<T>::~WeakPtr() {
  if (contr_block) {
    if (contr_block->ref_count + contr_block->weak_count == 0) {
      delete contr_block;
    } else {
      contr_block->weak_count--;
    }
  }
}

template <class T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& wptr) {
  if (&wptr == this) {
    return *this;
  }
  contr_block->weak_count--;
  if (contr_block->ref_count == 0 && contr_block->weak_count == 0) {
    delete contr_block;
  }
  contr_block = wptr.contr_block;
  contr_block->weak_count++;

  return *this;
}

template <class T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& wptr) {
  if (&wptr == this) {
    return *this;
  }
  contr_block = wptr.contr_block;
  wptr.contr_block = nullptr;

  return *this;
}

template <class T>
WeakPtr<T>& WeakPtr<T>::operator=(SharedPtr<T>&& shptr) {
  contr_block = shptr.contr_block;
  contr_block->weak_count++;
  return *this;
}

template <class T>
SharedPtr<T> WeakPtr<T>::lock() {
  return SharedPtr<T>(*this);
}

template <class T>
size_t WeakPtr<T>::use_count() {
  return contr_block ? contr_block->ref_count : 0;
}

template <class T>
bool WeakPtr<T>::expired() {
  return use_count() == 0 ? true : false;
}

template <class T>
void WeakPtr<T>::reset() {
  contr_block->weak_count--;
  if (contr_block->ref_count == 0 && contr_block->weak_count == 0) {
    delete contr_block;
  }
}

template <class T>
void WeakPtr<T>::swap(WeakPtr& wptr) {
  std::swap(contr_block, wptr.contr_block);
}

}  // namespace task