#include <iostream>

template <typename T>
class Allocator {
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;
  template <typename U>
  struct rebind {
    typedef Allocator<U> other;
  };
  using is_always_equal = std::true_type;
  const size_type CHUNK_SIZE = 1000;
  Allocator() {
    head = new Chunk(CHUNK_SIZE);
    ref_count = 1;
  }

  Allocator(const Allocator<T>& another) {
    head = another.head;
    ref_count = another.ref_count + 1;
  }

  ~Allocator() {
    if (ref_count > 0) {
      while (head->next) {
        Chunk* tmp = head;
        head = head->next;
        delete tmp;
      }
      ref_count = 1;
    }
  }

  T* allocate(const size_type size) {
    if (size > CHUNK_SIZE) {
      throw std::bad_alloc();
    } else {
      Chunk* tmp = head;
      if (tmp->get_free_size() > size) {
        return tmp->allocate(size);
      } else if (!(tmp->next)) {
        Chunk* new_chunk = new Chunk(CHUNK_SIZE);
        tmp->next = new_chunk;
        return tmp->allocate(size);
      } else {
        while (tmp->next) {
          if (tmp->get_free_size() > size) {
            return tmp->allocate(size);
          } else {
            tmp = tmp->next;
          }
        }
      }
    }
  }

  Allocator<T>& operator=(const Allocator& another) {
    if (*this == another) {
      return *this;
    } else {
      head = another.head;
      ref_count = another.ref_count + 1;
      return *this;
    }
  }

  void deallocate(T* p, std::size_t n) {}

  template <class... Args>
  void construct(T* p, Args&&... args) {
    new (p) T(args...);
  }

  void destroy(T* p) { p->~T(); }

 private:
  class Chunk {
    friend class Allocator;
    Chunk(size_t size) : free_size(size) {
      data = (pointer) new char[size * sizeof(char)];
      begin_block = data;
      next = nullptr;
    }

    ~Chunk() { delete[] data; }

    size_type get_free_size() { return free_size; }

    pointer allocate(size_t size) {
      pointer begin = begin_block;
      begin_block = begin_block + size * sizeof(T);
      free_size -= size;
      return begin;
    }

   public:
    pointer data;
    pointer begin_block;
    size_type free_size;
    Chunk* next;
  };

  size_type ref_count = 0;
  Chunk* head = nullptr;
};
