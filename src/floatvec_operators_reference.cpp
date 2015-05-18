#include <cassert>

#include <cstdlib>
#include <cmath>

#include "floatvec.hpp"


FloatVec& FloatVec::operator+=( const FloatVec& righthandside )
{
    for(size_t i=0; i<_length; i++) { _data[i] += righthandside._data[i]; }

  return *this;
}

FloatVec& FloatVec::operator+=( const float c )
{
    for(size_t i=0; i<_length; i++) { _data[i] += c; }
  return *this;
}

FloatVec& FloatVec::operator-=( const FloatVec& righthandside )
{
  assert((_length == righthandside._length) && "operator-=: length does not match");
    for(size_t i=0; i<_length; i++) { _data[i] -= righthandside._data[i]; }
  return *this;
}

FloatVec& FloatVec::operator-=( const float c )
{
    for(size_t i=0; i<_length; i++) { _data[i] -= c; }
   return *this;
}

FloatVec& FloatVec::operator*=( const FloatVec& righthandside )
{
  assert((_length == righthandside._length) && "operator*=: length does not match");
    for(size_t i=0; i<_length; i++) { _data[i] *= righthandside._data[i]; }
  return *this;
}

FloatVec& FloatVec::operator*=( const float c )
{
    for(size_t i=0; i<_length; i++) { _data[i] *= c; }
  return *this;
}

FloatVec& FloatVec::operator/=( const FloatVec& righthandside )
{
  assert((_length == righthandside._length) && "operator+=: length does not match");

    for(size_t i=0; i<_length; i++) { _data[i] /= righthandside._data[i]; }
  return *this;
}

FloatVec& FloatVec::operator/=( const float c )
{
    for(size_t i=0; i<_length; i++) { _data[i] /= c; }
  return *this;
}
