#pragma once

#include <stddef.h>

struct RingBuf
{
  RingBuf() = delete;
  RingBuf(const RingBuf& other) = delete;
  RingBuf& operator=(const RingBuf& other) = delete;

  explicit RingBuf(size_t capacity);
  ~RingBuf();

  bool push(unsigned int value);
  bool pop(unsigned int& out);

  inline bool isEmpty() const {return m_count == 0; }
  inline bool isFull() const { return m_count == m_capacity; }
  inline size_t size() const { return m_count; }
  inline size_t capacity() const {return m_capacity; }

private:
  unsigned int* data;
  size_t m_front;
  size_t m_back;
  size_t m_capacity;
  size_t m_count;
};