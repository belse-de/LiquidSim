#include <cassert>

#include <cstdlib>
#include <cstring>
#include <cmath>

#include "floatvec.hpp"

FloatVec::FloatVec(size_t length):
    _length(length), _length4(length/4), _length8(length/8),
    isDivisibleBy4((length%4)==0), isDivisibleBy8((length%8)==0)
{
  _data = new float[length]{0.f};
  _data8 = (__m256*)_data;
  _data4 = (__m128*)_data8;
}

FloatVec::~FloatVec()
{
  delete[](_data);
}

FloatVec::FloatVec(FloatVec& other):
    _length(other._length), _length4(other._length/4), _length8(other._length/8),
    isDivisibleBy4((other._length%4)==0), isDivisibleBy8((other._length%8)==0)
{
  _data = new float[_length]{0.f};
  _data8 = (__m256*)_data;
  _data4 = (__m128*)_data8;
  memcpy(_data, other._data, _length * sizeof(float));
}

void      FloatVec::setAll(const float c)
{
  if(isDivisibleBy8)
  {
    __m256 c256 = _mm256_set1_ps(c);
    for(size_t i=0; i<_length8; i++) { _data8[i] = c256; }
  }
  else if(isDivisibleBy4)
  {
    __m128 c128 = _mm_set1_ps(c);
    for(size_t i=0; i<_length4; i++) { _data4[i] += c128; }
  }
  else
  {
    for(size_t i=0; i<_length; i++) { _data[i] += c; }
  }
}

void      FloatVec::zero()
{
  setAll(0.f);
}

void      FloatVec::one()
{
  setAll(1.f);
}

void      FloatVec::swap(FloatVec& other)
{
  if (this == &other) return; // handle self assignment
  assert((_length == other._length) && "swap: length does not match");
  float* tmp = _data;
  _data8 = (__m256*)other._data;
  _data4 = (__m128*)other._data;
  _data = other._data;
  other._data = tmp;
  other._data4 = (__m128*)tmp;
  other._data8 = (__m256*)tmp;
}

float     FloatVec::sum()
{
  float sum_value = 0;
  if(isDivisibleBy8)
  {
    __m256 sum8 = _mm256_set1_ps(0.f);
    for(size_t i=0; i<_length8; i++) { sum8 += _data8[i]; }
    __m256 sum82 = _mm256_permute2f128_ps(sum8 , sum8 , 1);
    sum8 = _mm256_add_ps(sum8, sum82);
    sum8 = _mm256_hadd_ps(sum8, sum8);
    sum8 = _mm256_hadd_ps(sum8, sum8);
    sum_value = *((float*)&sum8);
  }
  else if(isDivisibleBy4)
  {
    __m128 sum4 = _mm_set1_ps(0.f);
    for(size_t i=0; i<_length4; i++) { sum4 += _data4[i]; }
    sum4 = _mm_hadd_ps(sum4,sum4);
    sum4 = _mm_hadd_ps(sum4,sum4);
    sum_value = *((float*)&sum4);
  }
  else
  {
    for(size_t i=0; i<_length; i++) { sum_value += _data[i]; }
  }
  return sum_value;
}

float     FloatVec::sqSum()
{
  float sum_value = 0;
  if(isDivisibleBy8)
  {
      __m256 sum8 = _mm256_set1_ps(0.f);
    for(size_t i=0; i<_length8; i++) { sum8 += _mm256_mul_ps(_data8[i], _data8[i]); }
    __m256 sum82 = _mm256_permute2f128_ps(sum8 , sum8 , 1);
    sum8 = _mm256_add_ps(sum8, sum82);
    sum8 = _mm256_hadd_ps(sum8, sum8);
    sum8 = _mm256_hadd_ps(sum8, sum8);
    sum_value = *((float*)&sum8);
  }
  else if(isDivisibleBy4)
  {
    __m128 sum4 = _mm_set1_ps(0.f);
    for(size_t i=0; i<_length4; i++) { sum4 += _mm_mul_ps(_data4[i], _data4[i]); }
    sum4 = _mm_hadd_ps(sum4,sum4);
    sum4 = _mm_hadd_ps(sum4,sum4);
    sum_value = *((float*)&sum4);
  }
  else
  {
    for(size_t i=0; i<_length; i++) { sum_value += _data[i] * _data[i]; }
  }
  return sum_value;
}

float     FloatVec::avg()
{
  return (sum() / _length);
}

float     FloatVec::infNorm()
{
  return sqrtf(sqSum());
}

float     FloatVec::operator()( const int pos )
{
  int pos_inBound = _length + (pos%_length);
  return _data[pos_inBound];
}

FloatVec& FloatVec::operator= ( const FloatVec& righthandside )
{
  if (this == &righthandside) return *this; // handle self assignment
  assert((_length == righthandside._length) && "operator=: length does not match");
  memcpy(_data, righthandside._data, _length * sizeof(float));
  return *this;
}

bool      FloatVec::operator==( const FloatVec& righthandside ) const
{
  assert((_length == righthandside._length) && "operator==: length does not match");
  return (memcmp(_data, righthandside._data, _length * sizeof(float)) == 0);
}

bool      FloatVec::operator!=( const FloatVec& righthandside ) const
{
  assert((_length == righthandside._length) && "operator!=: length does not match");
  return (memcmp(_data, righthandside._data, _length * sizeof(float)) != 0);
}



