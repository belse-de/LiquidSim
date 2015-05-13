#ifndef VECTORFIELD_H
#define VECTORFIELD_H

#include <field.hpp>

#include "vector2.h"

class VectorField : public Field<Vector2>
{
    public:
        /** Default constructor */
        VectorField(int dim_x, int dim_y);
        /** Default destructor */
        virtual ~VectorField();
        /** Copy constructor
         *  \param other Object to copy from
         */
        VectorField(const VectorField& other);
    protected:
    private:
};

#endif // VECTORFIELD_H
