#include <cassert>

#include <cstdlib>
#include <cstring>
#include <cmath>

#include "floatvec.hpp"

FloatVec::FloatVec(size_t length):
    _data(new float[length]),
    _length(length), _iterationSteps(length)
{
  //_data = new float[length]{}; // + 7
  //uint32_t mask8 = ~0x1f; // mask for 32byte (8word) aliment
  //uint32_t mask4 = ~0x0f; // mask for 16byte (4word) aliment
  //raw_mem = (void*)((uint32_t)raw_mem & mask);
}

FloatVec::FloatVec():
    _data(nullptr),
    _length(0), _iterationSteps(0)
{
}

FloatVec::~FloatVec()
{
  delete[](_data);
}

FloatVec::FloatVec(FloatVec& other):
    _data(new float[other._length]),
    _length(other._length), _iterationSteps(other._length)
{
  memcpy(_data, other._data, _length * sizeof(float));
}

void      FloatVec::setAll(const float c)
{
    for(size_t i=0; i<_length; i++) { _data[i] += c; }
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
  other._data = tmp;
}

float     FloatVec::sum()
{
  float sum_value = 0;
    for(size_t i=0; i<_length; i++) { sum_value += _data[i]; }
  return sum_value;
}

float     FloatVec::sqSum()
{
  float sum_value = 0;
    for(size_t i=0; i<_length; i++) { sum_value += _data[i] * _data[i]; }
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
  int pos_inBound = _length + (pos %_length);
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



