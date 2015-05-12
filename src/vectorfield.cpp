#include "vectorfield.h"

VectorField::VectorField(int dim_x, int dim_y) :
    Field<Vector2>(dim_x, dim_y)
{
    //ctor
}

VectorField::~VectorField()
{
    //dtor
}

/** Copy constructor
 *  \param other Object to copy from
 */
VectorField::VectorField(const VectorField& other) :
    Field<Vector2>(other)
{

}

