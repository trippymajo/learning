// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstring>
#include <chrono>
#include <vector>
#include <numeric>

// v1.  Lose
// mem_sru:  0.105924 sec
// memcpy:   0.0066561 sec
//void* mem_sru(void* dest, const void* src, size_t b)
//{
//  size_t i;
//  for (i = 0; i < b; ++i)
//    ((char*)dest)[i] = ((char*)src)[i];
//
//  return dest;
//}

// v2. Lose
// mem_sru:  0.0197293 sec
// memcpy:   0.0075251 sec
//void* mem_sru(void* dest, const void* src, size_t b)
//{
//  size_t i;
//  uint8_t* d = (uint8_t*)dest;
//  const uint8_t* s = (const uint8_t*)src;
//
//  while (b > 0)
//  {
//    if (b == 1)
//    {
//      *d++ = *s++;
//      --b;
//    }
//    else if (b != 0)
//    {
//      if (b >= 8) { *(uint64_t*)d = *(const uint64_t*)s; d += 8; s += 8; b -= 8;}
//      else if (b >= 4) { *(uint32_t*)d = *(const uint32_t*)s; d += 4; s += 4; b -= 4;}
//      else if (b >= 2) { *(uint16_t*)d = *(const uint16_t*)s; d += 2; s += 2; b -= 2;}
//    }
//
//  }
//
//  return dest;
//}

// v3. Lose
// mem_sru:  0.0088992 sec
// memcpy : 0.0065429 sec
//void* mem_sru(void* dest, const void* src, size_t b)
//{
//  size_t i;
//  uint8_t* d = (uint8_t*)dest;
//  const uint8_t* s = (const uint8_t*)src;
//
//  while (b > 0)
//  {
//    if (b >= 8) { *(uint64_t*)d = *(const uint64_t*)s; d += 8; s += 8; b -= 8; }
//    else if (b >= 4) { *(uint32_t*)d = *(const uint32_t*)s; d += 4; s += 4; b -= 4; }
//    else if (b >= 2) { *(uint16_t*)d = *(const uint16_t*)s; d += 2; s += 2; b -= 2; }
//    else { *d++ = *s++; --b; }
//  }
//
//  return dest;
//}

// v.Someone's Lose
//mem_sru:  0.026975 sec
//memcpy : 0.0066599 sec
void my_memcpy(void* dst, void* src, size_t my_size)
{
  auto dst64 = static_cast<int64_t*>(dst);
  auto src64 = static_cast<int64_t*>(src);

  int64_t total = my_size / sizeof(int64_t);
  int64_t offset = total * sizeof(int64_t);
  int64_t left = my_size - offset;


  for (int64_t i = 0; i < my_size / sizeof(int64_t); ++i)
  {
    dst64[i] = src64[i];
  }

  auto dst8 = static_cast<char*>(dst) - offset;
  auto src8 = static_cast<char*>(src) - offset;

  for (int64_t i = 0; i < left; ++i)
  {
    dst8[i] = src8[i];
  }
}

int main()
{
  constexpr size_t data_size = 1024 * 1024 * 100; // Mem for test
  std::vector<char> src(data_size, 44);
  std::vector<char> dst1(data_size, 0);
  std::vector<char> dst2(data_size, 0);

  volatile char sink = 0;

  auto start1 = std::chrono::high_resolution_clock::now();
  my_memcpy(dst1.data(), src.data(), data_size);
  auto end1 = std::chrono::high_resolution_clock::now();
  sink = dst1[0];
  auto duration1 = std::chrono::duration<double>(end1 - start1).count();

  auto start2 = std::chrono::high_resolution_clock::now();
  memcpy(dst2.data(), src.data(), data_size);
  auto end2 = std::chrono::high_resolution_clock::now();
  sink = dst2[0];
  auto duration2 = std::chrono::duration<double>(end2 - start2).count();

  std::cout << "mem_sru:  " << duration1 << " sec" << std::endl;
  std::cout << "memcpy:   " << duration2 << " sec" << std::endl;
  std::cout << "Result check: " << (dst1 == dst2 ? "OK" : "FAIL") << std::endl;

  return sink;
}
