#include <cassert>

#include <cstdlib>
#include <cmath>

#include "floatvec.hpp"


FloatVec  FloatVec::operator+ ( const FloatVec& righthandside )
{
  FloatVec result(*this);
  result += righthandside;
  return result;
}

FloatVec  FloatVec::operator+ ( const float c )
{
  FloatVec result(*this);
  result += c;
  return result;
}

FloatVec  FloatVec::operator- ( const FloatVec& righthandside )
{
  FloatVec result(*this);
  result -= righthandside;
  return result;
}

FloatVec  FloatVec::operator- ( const float c )
{
  FloatVec result(*this);
  result -= c;
  return result;
}

FloatVec  FloatVec::operator- ()
{
  FloatVec result(*this);
  result *= -1.;
  return result;
}

FloatVec  FloatVec::operator* ( const FloatVec& righthandside )
{
  FloatVec result(*this);
  result *= righthandside;
  return result;
}

FloatVec  FloatVec::operator* ( const float c )
{
  FloatVec result(*this);
  result *= c;
  return result;
}

FloatVec  FloatVec::operator/ ( const FloatVec& righthandside )
{
  FloatVec result(*this);
  result /= righthandside;
  return result;
}

FloatVec  FloatVec::operator/ ( const float c )
{
  FloatVec result(*this);
  result /= c;
  return result;
}
