#include <cassert>
#include <iostream>
#include "cmatrix.h"

CMatrix::CMatrix(int m, int n) :
    _m(m), _n(n), _size(m*n)
{
    //ctor
    _data = new float[_size]{0.f};
}

CMatrix::~CMatrix()
{
    //dtor
    delete[](_data);
}

CMatrix::CMatrix(const CMatrix& other) :
    _m(other._m), _n(other._n), _size(other._size)
{
    //copy ctor
    _data = new float[_size]{};
    //May be memcopy is faster???
    copyFrom(other);
}

CMatrix& CMatrix::operator=(const CMatrix& other)
{
    if (this == &other) return *this; // handle self assignment
    //assignment operator
    assert(_m == other._m && "On assignment the shape should be equal.");
    assert(_n == other._n && "On assignment the shape should be equal.");
    copyFrom(other);
    return *this;
}

bool CMatrix::operator==(const CMatrix& M) const
{
    assert(_m == M._m && "On comparison the shape should be equal.");
    assert(_n == M._n && "On comparison the shape should be equal."),
    assert(_size == M._size && "On comparison the shape should be equal.");
    for(int i=0; i<_size; i++){ if(M._data[i] == _data[i]) { return false; }}
    return true;
}

inline bool CMatrix::operator!=(const CMatrix& M) const { return !(*this == M); }

CMatrix& CMatrix::operator+=(const CMatrix& M)
{
    assert(_m == M._m && "On addition the shape should be equal.");
    assert(_n == M._n && "On addition the shape should be equal.");
    assert(_size == M._size && "On addition the shape should be equal.");
    for(int i=0; i<_size; i++){ _data[i] += M._data[i]; }
    return *this;
}

CMatrix& CMatrix::operator+=(const float c)
{
    for(int i=0; i<_size; i++){ _data[i] += c; }
    return *this;
}

CMatrix& CMatrix::operator-=(const CMatrix& M)
{
    assert(_m == M._m && "On subtraction the shape should be equal.");
    assert(_n == M._n && "On subtraction the shape should be equal.");
    assert(_size == M._size && "On subtraction the shape should be equal.");
    for(int i=0; i<_size; i++){ _data[i] -= M._data[i]; }
    return *this;
}

CMatrix& CMatrix::operator-=(const float c)
{
    for(int i=0; i<_size; i++){ _data[i] -= c; }
    return *this;
}

CMatrix& CMatrix::operator*=(const CMatrix& M)
{
    assert(_m == M._m && "On element vise multiplication the shape should be equal.");
    assert(_n == M._n && "On element vise multiplication the shape should be equal.");
    assert(_size == M._size && "On element vise multiplication the shape should be equal.");
    for(int i=0; i<_size; i++){ _data[i] *= M._data[i]; }
    return *this;
}

CMatrix& CMatrix::operator*=(const float c)
{
    for(int i=0; i<_size; i++){ _data[i] *= c; }
    return *this;
}

CMatrix& CMatrix::operator/=(const CMatrix& M)
{
    assert(_m == M._m && "On element vise devision the shape should be equal.");
    assert(_n == M._n && "On element vise devision the shape should be equal.");
    assert(_size == M._size && "On element vise devision the shape should be equal.");
    for(int i=0; i<_size; i++){ _data[i] *= M._data[i]; }
    return *this;
}

CMatrix& CMatrix::operator/=(const float c)
{
    for(int i=0; i<_size; i++){ _data[i] /= c; }
    return *this;
}

void CMatrix::set(int x, int y, float c)
{
    if(checkPosition(x,y)){
        int index = postionAsIndex(x,y);
        if(checkIndex(index)){
            _data[index] = c;
        }
    }
    std::cerr << "index out of bounds" << std::endl;
    //FIXME: Throw Index out of Bounds
}

float CMatrix::get(int x, int y)
{
    if(checkPosition(x,y)){
        int index = postionAsIndex(x,y);
        if(checkIndex(index)){
            return _data[index];
        }
    }
    //FIXME: Throw Index out of Bounds
    return -1;
}

void CMatrix::set(float c)
{
    for(int i=0; i<_size; i++){ _data[i] = c; }
}

void CMatrix::zero()
{
    for(int i=0; i<_size; i++){ _data[i] = 0.f; }
}

void CMatrix::one()
{
    for(int i=0; i<_size; i++){ _data[i] = 1.f; }
}


void CMatrix::copyFrom(const CMatrix& M)
{
    assert(_size == M._size && "Sizes should match if you copy one matrix to another");
    for(int i=0; i<_size; i++){ _data[i] = M._data[i]; }
    //memcpy(p_out,p_in,size*sizeof(float));
}

void CMatrix::copyTo(const CMatrix& M)
{
    assert(_size == M._size && "Sizes should match if you copy one matrix to another");
    for(int i=0; i<_size; i++){ M._data[i] = _data[i]; }
    //memcpy(p_out,p_in,size*sizeof(float));
}


int  CMatrix::postionAsIndex(int x, int y)   { return(y * _m + x); }

inline bool CMatrix::checkPosition(int x, int y)    { return (x>=0 && x<_m && y>=0 && y<_n); }

inline bool CMatrix::checkIndex(int i)              { return (i>=0 && i<_size); }

void CMatrix::swap(CMatrix& M)
{
    assert(_m == M._m && "On swap the shape should be equal.");
    assert(_n == M._n && "On swap the shape should be equal.");
    assert(_size == M._size && "On swap the shape should be equal.");
    float *temp = _data;
    _data = M._data;
    M._data = temp;
}

inline float CMatrix::sum()
{
    float sum = 0.f;
    for(int i=0; i<_size; i++)
    {
        sum += _data[i];
    }
    return sum;
}

float CMatrix::avg()
{
    return (sum() / _size);
}

CMatrix operator+(const CMatrix& v1, const CMatrix& v2)
{
    CMatrix result(v1);
    result += v2;
    return result;
}

CMatrix operator-(const CMatrix& v1, const CMatrix& v2)
{
    CMatrix result(v1);
    result -= v2;
    return result;
}

CMatrix operator*(const CMatrix& v1, const CMatrix& v2)
{
    CMatrix result(v1);
    result *= v2;
    return result;
}

CMatrix operator*(const CMatrix& v, const float s)
{
    CMatrix result(v);
    result *= s;
    return result;
}

CMatrix operator*(const float s, const CMatrix& v)
{
    CMatrix result(v);
    result *= s;
    return result;
}

CMatrix operator/(const CMatrix& v, const float s)
{
    CMatrix result(v);
    result /= s;
    return result;
}

CMatrix operator-(const CMatrix& v)
{
    CMatrix result(v);
    result *= -1.f;
    return result;
}
