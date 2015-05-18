#include <cassert>

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cmath>

#include "floatvec.hpp"
#include "floatvec_ps_m256.hpp"


FloatVec_ps_m256& FloatVec_ps_m256::operator+=( const FloatVec_ps_m256& righthandside )
{
  assert((_length == righthandside._length) && "operator+=: length does not match");
  __m256* _data8 = (__m256*) _data;
  __m256* rhs_data8 = (__m256*) righthandside._data;
  for(size_t i=0; i<_iterationSteps; i++) { _data8[i] += rhs_data8[i]; }
  return *this;
}

FloatVec_ps_m256& FloatVec_ps_m256::operator+=( const float c )
{
    __m256* _data8 = (__m256*) _data;
    for(size_t i=0; i<_iterationSteps; i++) { _data8[i] += c; }
  return *this;
}

FloatVec_ps_m256& FloatVec_ps_m256::operator-=( const FloatVec_ps_m256& righthandside )
{
  assert((_length == righthandside._length) && "operator-=: length does not match");
  __m256* _data8 = (__m256*) _data;
  __m256* rhs_data8 = (__m256*) righthandside._data;
      for(size_t i=0; i<_iterationSteps; i++) { _data8[i] -= rhs_data8[i]; }
  return *this;
}

FloatVec_ps_m256& FloatVec_ps_m256::operator-=( const float c )
{
    __m256* _data8 = (__m256*) _data;
    for(size_t i=0; i<_iterationSteps; i++) { _data8[i] -= c; }
  return *this;
}

FloatVec_ps_m256& FloatVec_ps_m256::operator*=( const FloatVec_ps_m256& righthandside )
{
  assert((_length == righthandside._length) && "operator*=: length does not match");
  __m256* _data8 = (__m256*) _data;
  __m256* rhs_data8 = (__m256*) righthandside._data;
        for(size_t i=0; i<_iterationSteps; i++) { _data8[i] *= rhs_data8[i]; }
  return *this;
}

FloatVec_ps_m256& FloatVec_ps_m256::operator*=( const float c )
{
    __m256* _data8 = (__m256*) _data;
    for(size_t i=0; i<_iterationSteps; i++) { _data8[i] *= c; }
  return *this;
}

FloatVec_ps_m256& FloatVec_ps_m256::operator/=( const FloatVec_ps_m256& righthandside )
{
  assert((_length == righthandside._length) && "operator+=: length does not match");
  __m256* _data8 = (__m256*) _data;
  __m256* rhs_data8 = (__m256*) righthandside._data;
       for(size_t i=0; i<_iterationSteps; i++) { _data8[i] /= rhs_data8[i]; }

  return *this;
}

FloatVec_ps_m256& FloatVec_ps_m256::operator/=( const float c )
{
    __m256* _data8 = (__m256*) _data;
 for(size_t i=0; i<_iterationSteps; i++) { _data8[i] /= c; }
  return *this;
}
