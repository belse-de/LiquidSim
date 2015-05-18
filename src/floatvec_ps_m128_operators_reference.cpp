#include <cassert>

#include <cstdlib>
#include <cmath>

#include "floatvec.hpp"
#include "floatvec_ps_m128.hpp"


FloatVec_ps_m128& FloatVec_ps_m128::operator+=( const FloatVec_ps_m128& righthandside )
{
  assert((_length == righthandside._length) && "operator+=: length does not match");
  __m128* _data4 = (__m128*) _data;
  __m128* rhs_data4 = (__m128*) righthandside._data;
    for(size_t i=0; i<_iterationSteps; i++) { _data4[i] += rhs_data4[i]; }
  return *this;
}

FloatVec_ps_m128& FloatVec_ps_m128::operator+=( const float c )
{
    __m128* _data4 = (__m128*) _data;
    for(size_t i=0; i<_iterationSteps; i++) { _data4[i] += c; }
  return *this;
}

FloatVec_ps_m128& FloatVec_ps_m128::operator-=( const FloatVec_ps_m128& righthandside )
{
  assert((_length == righthandside._length) && "operator-=: length does not match");
  __m128* _data4 = (__m128*) _data;
  __m128* rhs_data4 = (__m128*) righthandside._data;
    for(size_t i=0; i<_iterationSteps; i++) { _data4[i] -= rhs_data4[i]; }
  return *this;
}

FloatVec_ps_m128& FloatVec_ps_m128::operator-=( const float c )
{
    __m128* _data4 = (__m128*) _data;
    for(size_t i=0; i<_iterationSteps; i++) { _data4[i] -= c; }
  return *this;
}

FloatVec_ps_m128& FloatVec_ps_m128::operator*=( const FloatVec_ps_m128& righthandside )
{
  assert((_length == righthandside._length) && "operator*=: length does not match");
  __m128* _data4 = (__m128*) _data;
  __m128* rhs_data4 = (__m128*) righthandside._data;
    for(size_t i=0; i<_iterationSteps; i++) { _data4[i] *= rhs_data4[i]; }
  return *this;
}

FloatVec_ps_m128& FloatVec_ps_m128::operator*=( const float c )
{
    __m128* _data4 = (__m128*) _data;
    for(size_t i=0; i<_iterationSteps; i++) { _data4[i] *= c; }
  return *this;
}

FloatVec_ps_m128& FloatVec_ps_m128::operator/=( const FloatVec_ps_m128& righthandside )
{
  assert((_length == righthandside._length) && "operator+=: length does not match");
  __m128* _data4 = (__m128*) _data;
  __m128* rhs_data4 = (__m128*) righthandside._data;
    for(size_t i=0; i<_iterationSteps; i++) { _data4[i] /= rhs_data4[i]; }
  return *this;
}

FloatVec_ps_m128& FloatVec_ps_m128::operator/=( const float c )
{
    __m128* _data4 = (__m128*) _data;
    for(size_t i=0; i<_iterationSteps; i++) { _data4[i] /= c; }
  return *this;
}
