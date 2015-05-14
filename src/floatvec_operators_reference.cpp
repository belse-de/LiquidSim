#include <cassert>

#include <cstdlib>
#include <cmath>

#include "floatvec.hpp"


FloatVec& FloatVec::operator+=( const FloatVec& righthandside )
{
  assert((_length == righthandside._length) && "operator+=: length does not match");
  if(isDivisibleBy8)
  {
    for(size_t i=0; i<_length8; i++) { _data8[i] += righthandside._data8[i]; }
  }
  else if(isDivisibleBy4)
  {
    for(size_t i=0; i<_length4; i++) { _data4[i] += righthandside._data4[i]; }
  }
  else
  {
    for(size_t i=0; i<_length; i++) { _data[i] += righthandside._data[i]; }
  }
  return *this;
}

FloatVec& FloatVec::operator+=( const float c )
{
  if(isDivisibleBy8)
  {
    for(size_t i=0; i<_length8; i++) { _data8[i] += c; }
  }
  else if(isDivisibleBy4)
  {
    for(size_t i=0; i<_length4; i++) { _data4[i] += c; }
  }
  else
  {
    for(size_t i=0; i<_length; i++) { _data[i] += c; }
  }
  return *this;
}

FloatVec& FloatVec::operator-=( const FloatVec& righthandside )
{
  assert((_length == righthandside._length) && "operator-=: length does not match");
  if(isDivisibleBy8)
  {
    for(size_t i=0; i<_length8; i++) { _data8[i] -= righthandside._data8[i]; }
  }
  else if(isDivisibleBy4)
  {
    for(size_t i=0; i<_length4; i++) { _data4[i] -= righthandside._data4[i]; }
  }
  else
  {
    for(size_t i=0; i<_length; i++) { _data[i] -= righthandside._data[i]; }
  }
  return *this;
}

FloatVec& FloatVec::operator-=( const float c )
{
  if(isDivisibleBy8)
  {
    for(size_t i=0; i<_length8; i++) { _data8[i] -= c; }
  }
  else if(isDivisibleBy4)
  {
    for(size_t i=0; i<_length4; i++) { _data4[i] -= c; }
  }
  else
  {
    for(size_t i=0; i<_length; i++) { _data[i] -= c; }
  }
  return *this;
}

FloatVec& FloatVec::operator*=( const FloatVec& righthandside )
{
  assert((_length == righthandside._length) && "operator*=: length does not match");
  if(isDivisibleBy8)
  {
    for(size_t i=0; i<_length8; i++) { _data8[i] *= righthandside._data8[i]; }
  }
  else if(isDivisibleBy4)
  {
    for(size_t i=0; i<_length4; i++) { _data4[i] *= righthandside._data4[i]; }
  }
  else
  {
    for(size_t i=0; i<_length; i++) { _data[i] *= righthandside._data[i]; }
  }
  return *this;
}

FloatVec& FloatVec::operator*=( const float c )
{
  if(isDivisibleBy8)
  {
    for(size_t i=0; i<_length8; i++) { _data8[i] *= c; }
  }
  else if(isDivisibleBy4)
  {
    for(size_t i=0; i<_length4; i++) { _data4[i] *= c; }
  }
  else
  {
    for(size_t i=0; i<_length; i++) { _data[i] *= c; }
  }
  return *this;
}

FloatVec& FloatVec::operator/=( const FloatVec& righthandside )
{
  assert((_length == righthandside._length) && "operator+=: length does not match");
  if(isDivisibleBy8)
  {
    for(size_t i=0; i<_length8; i++) { _data8[i] /= righthandside._data8[i]; }
  }
  else if(isDivisibleBy4)
  {
    for(size_t i=0; i<_length4; i++) { _data4[i] /= righthandside._data4[i]; }
  }
  else
  {
    for(size_t i=0; i<_length; i++) { _data[i] /= righthandside._data[i]; }
  }
  return *this;
}

FloatVec& FloatVec::operator/=( const float c )
{
  if(isDivisibleBy8)
  {
    for(size_t i=0; i<_length8; i++) { _data8[i] /= c; }
  }
  else if(isDivisibleBy4)
  {
    for(size_t i=0; i<_length4; i++) { _data4[i] /= c; }
  }
  else
  {
    for(size_t i=0; i<_length; i++) { _data[i] /= c; }
  }
  return *this;
}
