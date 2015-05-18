#include <cassert>

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cmath>

#include "floatvec.hpp"
#include "floatvec_ps_m128.hpp"

FloatVec_ps_m128::FloatVec_ps_m128(size_t length)
{
    //ctor
    assert((length%4==0) && "FloatVec_ps_m128: length is not divable by 4!");
    _data = (float*)new __m128[length/4]{};
    uint32_t mask4 = 0x0f; // mask for 16byte (4word) alignment
    assert((((uint32_t)_data & mask4)==0) && "FloatVec_ps_m128: pointer is not 16byte (4word) aligned!");
    _length = length;
    _iterationSteps = length/4;
}

FloatVec_ps_m128::~FloatVec_ps_m128()
{
    //dtor
}

FloatVec_ps_m128::FloatVec_ps_m128(const FloatVec_ps_m128& other) : FloatVec()
{
    //copy ctor
    assert((other._length%4==0) && "FloatVec_ps_m128: length is not divable by 4!");
    _data = (float*)new __m128[other._length/4]{};
    uint32_t mask4 = 0x0f; // mask for 16byte (4word) alignment
    assert((((uint32_t)_data & mask4)==0) && "FloatVec_ps_m128: pointer is not 16byte (4word) aligned!");
    _length = other._length;
    _iterationSteps = other._length/4;

    memcpy(_data, other._data, _iterationSteps * sizeof(__m128));
}

void      FloatVec_ps_m128::setAll(const float c)
{
    __m128* _data4 = (__m128*) _data;
    __m128 c128 = _mm_set1_ps(c);
    for(size_t i=0; i<_iterationSteps; i++) { _data4[i] += c128; }
}

float     FloatVec_ps_m128::sum()
{
    __m128* _data4 = (__m128*) _data;
    float sum_value = 0;
    __m128 sum4 = _mm_set1_ps(0.f);
    for(size_t i=0; i<_iterationSteps; i++) { sum4 += _data4[i]; }
    sum4 = _mm_hadd_ps(sum4,sum4);
    sum4 = _mm_hadd_ps(sum4,sum4);
    sum_value = *((float*)&sum4);

  return sum_value;
}

float     FloatVec_ps_m128::sqSum()
{
    __m128* _data4 = (__m128*) _data;
  float sum_value = 0;
    __m128 sum4 = _mm_set1_ps(0.f);
    for(size_t i=0; i<_iterationSteps; i++) { sum4 += _mm_mul_ps(_data4[i], _data4[i]); }
    sum4 = _mm_hadd_ps(sum4,sum4);
    sum4 = _mm_hadd_ps(sum4,sum4);
    sum_value = *((float*)&sum4);
  return sum_value;
}
