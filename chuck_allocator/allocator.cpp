#include <iostream>
#include <vector>

struct Block {
  size_t start;
  size_t size;
  Block(size_t start, size_t size) : start(start), size(size) {}
};

template <typename T>
struct Chunk {
  Chunk(size_t size) : size(size), free_size(size) {
    data = new char[size * sizeof(char)];
    next = nullptr;
  }

  ~Chunk() { delete[] data; }

  T* allocate(size_t size) {
    if (blocks.size() == 0) {
      Block new_block(0, size);
      blocks.push_back(new_block);
      return (T*)((char*)(data));
    } else if (blocks.back().start + size <= this->size) {
      Block new_block(blocks.back().start + blocks.back().size + 1, size);
      blocks.push_back(new_block);
      return (T*)((char*)(data) + blocks.back().start + blocks.back().size + 1);
    } else {
      return nullptr;
    }
  }

 public:
  T* data;
  size_t size;
  size_t free_size;
  Chunk<T>* next;
  std::vector<Block> blocks;
};

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

  Allocator(size_t chunk_size)
      : chunks(nullptr), ref_count(0), chunk_size(chunk_size) {}

  Allocator(const Allocator<T>& another) {
    chunks = another.chunks;
    ref_count = another.ref_count + 1;
    chunk_size = another.chunk_size;
  }

  ~Allocator() {
    if (ref_count > 0) {
      while (chunks) {
        Chunk<T> tmp = chunks;
        chunks = chunks->next;
        delete tmp;
      }
      ref_count = 0;
    }
  }

  T* allocate(const size_t size) {
    size_t mem_size = size * sizeof(T);
    if (mem_size > this->chunk_size) {
      throw std::bad_alloc();
    } else {
      if (chunks) {
        Chunk<T>* cur = this->chunks;
        Chunk<T>* prev = nullptr;
        if (cur != nullptr) {
          while (cur != nullptr) {
            T* tmp = cur->allocate(size);
            if (tmp != nullptr) {
              return tmp;
            }
            prev = cur;
            cur = cur->next;
          }
        } else {
          prev->next = new Chunk<T>(this->chunk_size);
          T* tmp = prev->next->allocate(size);
          return tmp;
        }

      } else {
        this->chunks = new Chunk<T>(this->chunk_size);
        T* tmp = this->chunks->allocate(size);
        return tmp;
      }
    }
  }

  Allocator<T>& operator=(const Allocator& another) {
    if (*this == another) {
      return *this;
    } else {
      chunks = another.chunks;
      ref_count = another.ref_count + 1;
      chunk_size = another.chunk_size;
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
  size_t ref_count = 0;
  Chunk<T>* chunks;
  size_t chunk_size;
};
