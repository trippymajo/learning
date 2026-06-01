#include "ringbuf.h"

RingBuf::RingBuf(size_t capacity)
  : data(new unsigned int[capacity]),
    m_front(0),
    m_back(0),
    m_count(0),
    m_capacity(capacity)
{
}

RingBuf::~RingBuf()
{
  delete[] data;
}

bool RingBuf::push(unsigned int value)
{
  if (this->isFull())
    return false;

  data[m_back] = value;
  m_back = (m_back + 1) % m_capacity;
  ++m_count;

  return true;
}

bool RingBuf::pop(unsigned int& out)
{
  if (this->isEmpty())
    return false;

  out = data[m_front];
  m_front = (m_front + 1) % m_capacity;
  --m_count;

  return true;
}