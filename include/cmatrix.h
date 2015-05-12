#ifndef CMATRIX_H
#define CMATRIX_H


class CMatrix
{
    public:
        /** Default constructor */
        CMatrix(int m, int n);
        /** Default destructor */
        virtual ~CMatrix();
        /** Copy constructor
         *  \param other Object to copy from
         */
        CMatrix(const CMatrix& other);
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        CMatrix& operator=(const CMatrix& other);

        bool			operator==  ( const CMatrix& M ) const;
        bool			operator!=  ( const CMatrix& M ) const;
        CMatrix&		operator+=	( const CMatrix& M );
        CMatrix&		operator+=	( const float c );
        CMatrix&		operator-=	( const CMatrix& M );
        CMatrix&		operator-=	( const float c );
        CMatrix&		operator*=	( const CMatrix& M );
        CMatrix&		operator*=	( const float c );
        CMatrix&		operator/=	( const CMatrix& M );
        CMatrix&		operator/=	( const float c );

        void    set(int x, int y, float c);
        float   get(int x, int y);
        void    set(float c);
        void    zero();
        void    one();

        void    swap(CMatrix& M);
        float   sum();
        float   avg();

        float* _data;
        int     postionAsIndex(int x, int y);

    protected:
        void    copyFrom( const CMatrix& M );
        void    copyTo( const CMatrix& M );
        //int     postionAsIndex(int x, int y);
        bool    checkPosition(int x, int y);
        bool    checkIndex(int i);

        const int _m;
        const int _n;
        const int _size;
    private:

};

CMatrix		operator+ ( const CMatrix& v1, const CMatrix& v2 );
CMatrix		operator- ( const CMatrix& v1, const CMatrix& v2 );
CMatrix		operator* ( const CMatrix& v1, const CMatrix& v2 );
CMatrix		operator* ( const CMatrix& v, const float s );
CMatrix		operator* ( const float s, const CMatrix& v );
CMatrix		operator/ ( const CMatrix& v, const float s );
CMatrix		operator- ( const CMatrix& v );

#endif // CMATRIX_H
