#ifndef FLOATVEC_H_INCLUDED
#define FLOATVEC_H_INCLUDED

// intrinsics / cpu includes
#include <x86intrin.h>

class FloatVec
{
  public:
    FloatVec(size_t length);
    virtual ~FloatVec();
    FloatVec(FloatVec& other);

    void      setAll(const float c);
    void      zero();
    void      one();
    void      swap(FloatVec& other);
    float     sum();
    float     sqSum();
    float     avg();
    float     infNorm();
    //TODO: normal vector, /dot product
    //XXX: shift/rotate if needed

    float     operator()( const int pos );

    FloatVec& operator= ( const FloatVec& righthandside );
    bool      operator==( const FloatVec& righthandside ) const;
    bool      operator!=( const FloatVec& righthandside ) const;
    FloatVec& operator+=( const FloatVec& righthandside );
    FloatVec& operator+=( const float c );
    FloatVec& operator-=( const FloatVec& righthandside );
    FloatVec& operator-=( const float c );
    FloatVec& operator*=( const FloatVec& righthandside );
    FloatVec& operator*=( const float c );
    FloatVec& operator/=( const FloatVec& righthandside );
    FloatVec& operator/=( const float c );

    FloatVec  operator+ ( const FloatVec& righthandside );
    FloatVec  operator+ ( const float c );
    FloatVec  operator- ( const FloatVec& righthandside );
    FloatVec  operator- ( const float c );
    FloatVec  operator* ( const FloatVec& righthandside );
    FloatVec  operator* ( const float c );
    FloatVec  operator/ ( const FloatVec& righthandside );
    FloatVec  operator/ ( const float c );

    //XXX: only when needed
    //FloatVec  operator ~(); //negation
    //FloatVec  operator<<( const unsigned int rotateLeft );
    //FloatVec  operator>>( const unsigned int rotateLeft );

  protected:


  private:
    float*  _data;
    __m128* _data4;
    __m256* _data8;

    size_t  _length;
    size_t  _length4;
    size_t  _length8;

    bool    isDivisibleBy4;
    bool    isDivisibleBy8;
};

#endif // FLOATVEC_H_INCLUDED