#ifndef CFIELD_H
#define CFIELD_H

#include "CMatrix.h"


class CField : public CMatrix
{
    public:
        /** Default constructor */
        CField(int m, int n);
        /** Default destructor */
        virtual ~CField();

        float   get(float i, float j);
        float   get(int x, int y){ return CMatrix::get(x,y); }
        void    set(float i, float j, float v);
        void    add(float i, float j, float v);

        void    zeroEdge();
        void    pushEdge();
        void    copyEdgeFrom(CField& field);
        void    edgeForce(float left, float right, float top, float bottom);

        void    applyForce(float force, float dt);
        void    forceFrom(CField& from, float force, float dt);
        void    diffuse(CField& output, float scale, float dt);
        void    diffuseLR(CField& output, float scale, float dt);
        void    diffuseGlobal(CField& output, float scale, float dt);
        void    decayQuadraticly(CField& output, float a, float b, float c, float dt);
    protected:
    private:
};

#endif // CFIELD_H
