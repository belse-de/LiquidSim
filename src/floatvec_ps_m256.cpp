#include <cassert>

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cmath>

#include "floatvec.hpp"
#include "floatvec_ps_m256.hpp"

FloatVec_ps_m256::FloatVec_ps_m256(size_t length)
{
    //ctor
    assert((length%8==0) && "FloatVec_ps_m256: length is not divable by 8!");
    _data = (float*)new __m256[length/8]{};
    uint32_t mask8 = 0x1f; // mask for 32byte (8word) alignment
    assert((((uint32_t)_data & mask8)==0) && "FloatVec_ps_m256: pointer is not 32byte (8word) aligned!");
    _length = length;
    _iterationSteps = length/8;
}

FloatVec_ps_m256::~FloatVec_ps_m256()
{
    //dtor
}

FloatVec_ps_m256::FloatVec_ps_m256(const FloatVec_ps_m256& other) : FloatVec()
{
    //copy ctor
    assert((other._length%8==0) && "FloatVec_ps_m256: length is not divable by 8!");
    _data = (float*)new __m256[other._length/8]{};
    uint32_t mask8 = 0x1f; // mask for 16byte (4word) alignment
    assert((((uint32_t)_data & mask8)==0) && "FloatVec_ps_m256: pointer is not 32byte (8word) aligned!");
    _length = other._length;
    _iterationSteps = other._length/8;

    memcpy(_data, other._data, _iterationSteps * sizeof(__m256));
}

void      FloatVec_ps_m256::setAll(const float c)
{
    __m256* _data8 = (__m256*) _data;
    __m256 c256 = _mm256_set1_ps(c);
    for(size_t i=0; i<_iterationSteps; i++) { _data8[i] = c256; }
}

float     FloatVec_ps_m256::sum()
{
    __m256* _data8 = (__m256*) _data;
    float sum_value = 0;

    __m256 sum8 = _mm256_set1_ps(0.f);
    for(size_t i=0; i<_iterationSteps; i++) { sum8 += _data8[i]; }
    __m256 sum82 = _mm256_permute2f128_ps(sum8 , sum8 , 1);
    sum8 = _mm256_add_ps(sum8, sum82);
    sum8 = _mm256_hadd_ps(sum8, sum8);
    sum8 = _mm256_hadd_ps(sum8, sum8);
    sum_value = *((float*)&sum8);

  return sum_value;
}

float     FloatVec_ps_m256::sqSum()
{
    __m256* _data8 = (__m256*) _data;
  float sum_value = 0;
      __m256 sum8 = _mm256_set1_ps(0.f);
    for(size_t i=0; i<_iterationSteps; i++) { sum8 += _mm256_mul_ps(_data8[i], _data8[i]); }
    __m256 sum82 = _mm256_permute2f128_ps(sum8 , sum8 , 1);
    sum8 = _mm256_add_ps(sum8, sum82);
    sum8 = _mm256_hadd_ps(sum8, sum8);
    sum8 = _mm256_hadd_ps(sum8, sum8);
    sum_value = *((float*)&sum8);
  return sum_value;
}
