#ifndef POTENTIONALFIELD_H
#define POTENTIONALFIELD_H

#include <field.hpp>


class PotentionalField : public Field<float>
{
    public:
        /** Default constructor */
        PotentionalField(int dim_x, int dim_y);
        /** Default destructor */
        virtual ~PotentionalField();
        /** Copy constructor
         *  \param other Object to copy from
         */
        PotentionalField(const PotentionalField& other);
    protected:
    private:
};

#endif // POTENTIONALFIELD_H
