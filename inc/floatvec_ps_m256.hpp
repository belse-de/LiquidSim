#ifndef FLOATVEC_PS_M256_H
#define FLOATVEC_PS_M256_H

#include <floatvec.hpp>


class FloatVec_ps_m256 : public FloatVec
{
    public:
        /** Default constructor */
        FloatVec_ps_m256(size_t length);
        /** Default destructor */
        virtual ~FloatVec_ps_m256();
        /** Copy constructor
         *  \param other Object to copy from
         */
        FloatVec_ps_m256(const FloatVec_ps_m256& other);

            void      setAll(const float c);
    void      zero();
    void      one();
    void      swap(FloatVec_ps_m256& other);
    float     sum();
    float     sqSum();
    float     avg();
    float     infNorm();
    //TODO: normal vector, /dot product
    //XXX: shift/rotate if needed

    float     operator()( const int pos );

    FloatVec_ps_m256& operator= ( const FloatVec_ps_m256& righthandside );
    bool      operator==( const FloatVec_ps_m256& righthandside ) const;
    bool      operator!=( const FloatVec_ps_m256& righthandside ) const;
    FloatVec_ps_m256& operator+=( const FloatVec_ps_m256& righthandside );
    FloatVec_ps_m256& operator+=( const float c );
    FloatVec_ps_m256& operator-=( const FloatVec_ps_m256& righthandside );
    FloatVec_ps_m256& operator-=( const float c );
    FloatVec_ps_m256& operator*=( const FloatVec_ps_m256& righthandside );
    FloatVec_ps_m256& operator*=( const float c );
    FloatVec_ps_m256& operator/=( const FloatVec_ps_m256& righthandside );
    FloatVec_ps_m256& operator/=( const float c );

    FloatVec  operator+ ( const FloatVec_ps_m256& righthandside );
    FloatVec  operator+ ( const float c );
    FloatVec  operator- ( const FloatVec_ps_m256& righthandside );
    FloatVec  operator- ( const float c );
    FloatVec  operator* ( const FloatVec_ps_m256& righthandside );
    FloatVec  operator* ( const float c );
    FloatVec  operator/ ( const FloatVec_ps_m256& righthandside );
    FloatVec  operator/ ( const float c );
    protected:
    private:
};

#endif // FLOATVEC_PS_M256_H
