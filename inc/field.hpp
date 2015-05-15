#ifndef FIELD_H
#define FIELD_H

#include <cmath>
#include <cstring>

#define FAST 1

template <class T>
class Field
{
    public:
        /** Default constructor */
        Field(int dim_x, int dim_y);
        /** Default destructor */
        virtual ~Field();
        /** Copy constructor
         *  \param other Object to copy from
         */
        Field(const Field& other);
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Field& operator=(const Field& other);
        bool operator==(const Field<T>& f)      const;
        bool operator!=(const Field<T>& f)      const;
        Field<T>& operator+=(const Field<T>& f) const;
        Field<T>& operator+=(const double f)    const;
        Field<T>& operator+=(const float f)     const;
        Field<T>& operator-=(const Field<T>& f) const;
        Field<T>& operator-=(const double f)    const;
        Field<T>& operator-=(const float f)     const;
        Field<T>& operator*=(const Field<T>& f) const;
        Field<T>& operator*=(const double f)    const;
        Field<T>& operator*=(const float f)     const;
        Field<T>& operator/=(const Field<T>& f) const;
        Field<T>& operator/=(const double f)    const;
        Field<T>& operator/=(const float f)     const;

        Field<T>& swap(Field<T>& rhs);

        bool& isWrapping();
        T& element(int x, int y);
        T element(float x, float y);
        void element(float x, float y, T& input);

        //virtual void clamp(T& upper_bound, T& lower_bound) = 0;


        const int m_dimention_size; //!< Member variable "m_dimention_size"
        const int m_dimention_x;    //!< Member variable "m_dimention_x"
        const int m_dimention_y;    //!< Member variable "m_dimention_y"
        
        constexpr static const float visc = 1.512041288; // uPa * s + K^-1/2
        constexpr static const float diff = 1.5e-5; //m^2 * s^1

        constexpr static const float f_esp    = 1.19209289551e-7;
        constexpr static const float f_pi     = 3.14159265358979323846;
        constexpr static const float f_e      = 2.7182818284590452354;
        constexpr static const float f_sqrt2  = 1.41421356237309504880;
    protected:
        void boundIndecies(int& x, int& y);
    private:
        T* m_data;
        bool m_wrapping = false;
};

template <class T> Field<T>::Field(int dim_x, int dim_y) :
    m_dimention_size(dim_x*dim_y), m_dimention_x(dim_x), m_dimention_y(dim_y)
{
    //ctor
    m_data = new T[m_dimention_size]{};
}

template <class T> Field<T>::~Field()
{
    //dtor
    delete[](m_data);
    m_data = nullptr;
}

template <class T> Field<T>::Field(const Field<T>& other) :
    m_dimention_size(other.m_dimention_size),
    m_dimention_x(other.m_dimention_x),
    m_dimention_y(other.m_dimention_y)
{
    //copy ctor
    m_data = new T[other.m_dimention_size]{};
    memcpy(m_data,other.m_data, other.m_dimention_size*sizeof(T));
}

template <class T> Field<T>& Field<T>::operator=(const Field<T>& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    memcpy(m_data,rhs.m_data, m_dimention_size*sizeof(T));
    return *this;
}

template <class T> bool Field<T>::operator==(const Field<T>& f) const
{
    bool isEqual = true;
    for(int i=0; i<m_dimention_size; i++)
    {
        if(f.m_data[i] != m_data[i])
        {
            isEqual = false;
            #ifdef FAST
            break;
            #endif // FAST
        }
    }
    return isEqual;
}

template <class T> bool Field<T>::operator!=(const Field<T>& f) const
{
    return !(*this == f);
}

template <class T> Field<T>& Field<T>::operator+=(const Field<T>& f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] += f.m_data[i];
    }
    return *this;
}

template <class T> Field<T>& Field<T>::operator+=(const double f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] += f;
    }
    return *this;
}

template <class T> Field<T>& Field<T>::operator+=(const float f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] += f;
    }
    return *this;
}

template <class T> Field<T>& Field<T>::operator-=(const Field<T>& f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] -= f.m_data[i];
    }
    return *this;
}

template <class T> Field<T>& Field<T>::operator-=(const double f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] -= f;
    }
    return *this;
}

template <class T> Field<T>& Field<T>::operator-=(const float f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] -= f;
    }
    return *this;
}

template <class T> Field<T>& Field<T>::operator*=(const Field<T>& f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] *= f.m_data[i];
    }
    return *this;
}

template <class T> Field<T>& Field<T>::operator*=(const double f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] *= f;
    }
    return *this;
}

template <class T> Field<T>& Field<T>::operator*=(const float f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] *= f;
    }
    return *this;
}

template <class T> Field<T>& Field<T>::operator/=(const Field<T>& f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] /= f.m_data[i];
    }
    return *this;
}

template <class T> Field<T>& Field<T>::operator/=(const double f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] /= f;
    }
    return *this;
}

template <class T> Field<T>& Field<T>::swap(Field<T>& rhs)
{
    T *temp    = m_data;
    m_data     = rhs.m_data;
    rhs.m_data = temp;
}

template <class T> Field<T>& Field<T>::operator/=(const float f) const
{
    for(int i=0; i<m_dimention_size; i++)
    {
        m_data[i] /= f;
    }
    return *this;
}

template <class T> bool& Field<T>::isWrapping()
{
    return m_wrapping;
}

template <class T> T& Field<T>::element(int x, int y)
{
    boundIndecies(x,y);
    int index = x + y * m_dimention_x;
    return m_data[index];
}

template <class T> T Field<T>::element(float x, float y)
{
    int x_down = (int)floorf(x);
    int x_up   = (int)ceilf(x);
    int y_down = (int)floorf(y);
    int y_up   = (int)ceilf(y);

    float x_down_f = 1. - (x - x_down);
    float x_up_f   = 1. - (x_up - x);
    float y_down_f = 1. - (y - y_down);
    float y_up_f   = 1. - (y_up - y);

    boundIndecies(x_down,y_down);
    boundIndecies(x_up,y_up);

    T data00 = element(x_down, y_down);
    T data10 = element(x_up, y_down);
    T data01 = element(x_down, y_up);
    T data11 = element(x_up, y_up);

    T data = data00 * x_down_f * y_down_f
            + data10 * x_up_f * y_down_f
            + data01 * x_down_f * y_up_f
            + data11 * x_up_f * y_up_f;

    return data;
}

template <class T> void Field<T>::element(float x, float y, T& input)
{
    int x_down = (int)floorf(x);
    int x_up   = (int)ceilf(x);
    int y_down = (int)floorf(y);
    int y_up   = (int)ceilf(y);

    float x_down_f = 1. - (x - x_down);
    float x_up_f   = 1. - (x_up - x);
    float y_down_f = 1. - (y - y_down);
    float y_up_f   = 1. - (y_up - y);

    boundIndecies(x_down,y_down);
    boundIndecies(x_up,y_up);

    element(x_down, y_down) = input * x_down_f * y_down_f;
    element(x_up, y_down)   = input * x_up_f * y_down_f;
    element(x_down, y_up)   = input * x_down_f * y_up_f;
    element(x_up, y_up)     = input * x_up_f * y_up_f;
}

template <class T> void Field<T>::boundIndecies(int& x, int& y)
{
    if(m_wrapping)
    {
        // handling to large numbers
        x = x % m_dimention_x;
        y = y % m_dimention_y;
        //handling negativ indices
        x = (x < 0) ? m_dimention_x + x : x;
        y = (y < 0) ? m_dimention_y + y : y;

    }
    else
    {
        //handling too small indices
        x = (x < 0) ? 0 : x;
        y = (y < 0) ? 0 : y;
        //handling too big indices
        x = (x > m_dimention_x) ? m_dimention_x : x;
        y = (y > m_dimention_y) ? m_dimention_y : y;
    }
}

#endif // FIELD_H
