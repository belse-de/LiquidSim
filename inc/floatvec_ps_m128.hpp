#ifndef FloatVec_ps_m128_PS_M128_H
#define FloatVec_ps_m128_PS_M128_H

#include <FloatVec_ps_m128.hpp>


class FloatVec_ps_m128 : public FloatVec
{
    public:
        /** Default constructor */
        FloatVec_ps_m128(size_t length);
        /** Default destructor */
        virtual ~FloatVec_ps_m128();
        /** Copy constructor
         *  \param other Object to copy from
         */
        FloatVec_ps_m128(const FloatVec_ps_m128& other);

            void      setAll(const float c);
    void      zero();
    void      one();
    void      swap(FloatVec_ps_m128& other);
    float     sum();
    float     sqSum();
    float     avg();
    float     infNorm();
    //TODO: normal vector, /dot product
    //XXX: shift/rotate if needed

    float     operator()( const int pos );

    FloatVec_ps_m128& operator= ( const FloatVec_ps_m128& righthandside );
    bool      operator==( const FloatVec_ps_m128& righthandside ) const;
    bool      operator!=( const FloatVec_ps_m128& righthandside ) const;
    FloatVec_ps_m128& operator+=( const FloatVec_ps_m128& righthandside );
    FloatVec_ps_m128& operator+=( const float c );
    FloatVec_ps_m128& operator-=( const FloatVec_ps_m128& righthandside );
    FloatVec_ps_m128& operator-=( const float c );
    FloatVec_ps_m128& operator*=( const FloatVec_ps_m128& righthandside );
    FloatVec_ps_m128& operator*=( const float c );
    FloatVec_ps_m128& operator/=( const FloatVec_ps_m128& righthandside );
    FloatVec_ps_m128& operator/=( const float c );

    FloatVec_ps_m128  operator+ ( const FloatVec_ps_m128& righthandside );
    FloatVec_ps_m128  operator+ ( const float c );
    FloatVec_ps_m128  operator- ( const FloatVec_ps_m128& righthandside );
    FloatVec_ps_m128  operator- ( const float c );
    FloatVec_ps_m128  operator* ( const FloatVec_ps_m128& righthandside );
    FloatVec_ps_m128  operator* ( const float c );
    FloatVec_ps_m128  operator/ ( const FloatVec_ps_m128& righthandside );
    FloatVec_ps_m128  operator/ ( const float c );

    protected:
    private:
};

#endif // FloatVec_ps_m128_PS_M128_H
