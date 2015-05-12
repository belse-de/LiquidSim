#include "potentionalfield.hpp"

PotentionalField::PotentionalField(int dim_x, int dim_y) :
    Field<float>(dim_x, dim_y)
{
    //ctor
}

PotentionalField::~PotentionalField()
{
    //dtor
}

PotentionalField::PotentionalField(const PotentionalField& other) :
    Field<float>(other)
{

}

