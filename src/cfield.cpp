#include "cfield.h"

CField::CField(int m, int n) :
    CMatrix(m,n)
{
    //ctor
}

CField::~CField()
{
    //dtor
}

float CField::get(float i, float j)
{
    // get fractional parts
	float fi = i-(int)i;
	float fj = j-(int)j;
	// get the corner cell (a)
	int x = (int)i;
	int y = (int)j;
	int index00 = postionAsIndex(x,y);
	int index10 = postionAsIndex(x+1,y);
	int index01 = postionAsIndex(x,y+1);
	int index11 = postionAsIndex(x+1,y+1);

    // get fractions of the values from each cell
    float ia = (1.0f-fj)*(1.0f-fi) * _data[index00];
    float ib = (1.0f-fj)*(fi)      * _data[index10];
    float ic = (fj)     *(1.0f-fi) * _data[index01];
    float id = (fj)     *(fi)      * _data[index11];

    return ia+ib+ic+id;
}

void CField::set(float i, float j, float v)
{
    // get fractional parts
	float fi = i-(int)i;
	float fj = j-(int)j;

    // get fractions of the values for each target cell
    float ia = (1.0f-fj)*(1.0f-fi) * v;
    float ib = (1.0f-fj)*(fi)      * v;
    float ic = (fj)     *(1.0f-fi) * v;
    float id = (fj)     *(fi)      * v;

    // get the corner cell (a)
	int x = (int)i;
	int y = (int)j;

    int index00 = postionAsIndex(x,y);
	int index10 = postionAsIndex(x+1,y);
	int index01 = postionAsIndex(x,y+1);
	int index11 = postionAsIndex(x+1,y+1);

    // Set into each cell
    _data[index00] = ia;
    _data[index10] = ib;
    _data[index01] = ic;
    _data[index11] = id;
}

// Given a floating point position within the field, add to it
// with bilinear interpolation
void CField::add(float i, float j, float v)
{
    if (i<0 || j<0 || i>(float)_m-1.0001f || j>(float)_n-1.0001f)
		return;

    // get fractional parts
	float fi = i-(int)i;
	float fj = j-(int)j;

    // get fractions of the values for each target cell
    float ia = (1.0f-fj)*(1.0f-fi) * v;
    float ib = (1.0f-fj)*(fi)      * v;
    float ic = (fj)     *(1.0f-fi) * v;
    float id = (fj)     *(fi)      * v;

    // get the corner cell (a)
	int x = (int)i;
	int y = (int)j;

    int index00 = postionAsIndex(x,y);
	int index10 = postionAsIndex(x+1,y);
	int index01 = postionAsIndex(x,y+1);
	int index11 = postionAsIndex(x+1,y+1);

    // Set into each cell
    _data[index00] += ia;
    _data[index10] += ib;
    _data[index01] += ic;
    _data[index11] += id;
}

void CField::zeroEdge()
{
    for(int i=0; i<_m; i++)
    {
        _data[postionAsIndex(i,0)]      = 0.f;
        _data[postionAsIndex(i,_n-1)]   = 0.f;
    }

    for(int j=0; j<_n; j++)
    {
        _data[postionAsIndex(0,j)]      = 0.f;
        _data[postionAsIndex(_m-1,j)]    = 0.f;
    }
}


void CField::pushEdge()
{
    for(int i=0; i<_m; i++)
    {
        _data[postionAsIndex(i,1)]      = _data[postionAsIndex(i,0)];
        _data[postionAsIndex(i,_n-2)]   = _data[postionAsIndex(i,_n-1)];
    }

    for(int j=0; j<_n; j++)
    {
        _data[postionAsIndex(1,j)]      = _data[postionAsIndex(0,j)];
        _data[postionAsIndex(_m-2,j)]    = _data[postionAsIndex(_m-1,j)];
    }
}

void CField::copyEdgeFrom(CField& other)
{
    for(int i=0; i<_m; i++)
    {
        _data[postionAsIndex(i,0)]      = other._data[postionAsIndex(i,0)];
        _data[postionAsIndex(i,_n-1)]   = other._data[postionAsIndex(i,_n-1)];
    }

    for(int j=0; j<_n; j++)
    {
        _data[postionAsIndex(0,j)]      = other._data[postionAsIndex(0,j)];
        _data[postionAsIndex(_m-1,j)]   = other._data[postionAsIndex(_m-1,j)];
    }
}

void CField::edgeForce(float left, float right, float top, float bottom)
{   //FIXME check if side are correct
    for(int i=0; i<_m; i++)
    {
        _data[postionAsIndex(i,0)]      += top;
        _data[postionAsIndex(i,_n-1)]   += bottom;
    }

    for(int j=0; j<_n; j++)
    {
        _data[postionAsIndex(0,j)]      += left;
        _data[postionAsIndex(_m-1,j)]   += right;
    }
}

// Given a field p_from, and a field p_to, then add f*p_from to p_to
// can be used to apply a heat field to velocity
void CField::applyForce(float force, float dt)
{
    force *= dt;
    *this += force;
}

void CField::forceFrom(CField& from, float force, float dt)
{
    force *= dt;
    *this += from * force;
}

// Build the new pressure values in p1, then swap p0 and p1
//Cells adjacent to border cells have a different diffusion.  They still take in the four surrounding cells
//but since the border cells are not updated, the adjacent cells should emit less.
void CField::diffuse(CField& output, float scale, float dt)
{
    //TODO if you calc th ind correct this code get smaller
    // for example: if the left cell does not exist take the center cell
    // TODO asserts needed
    float a = dt * scale;
    float temp;
    int inner_m = _m-1;
    int inner_n = _n-1;
    //int ind;

    // top and bot edges
    for(int i=1; i<inner_m; i++)
    {
        temp = _data[postionAsIndex(i,0)] + a * (
                          _data[postionAsIndex(i-1,0)]
                        + _data[postionAsIndex(i+1,0)]
                        + _data[postionAsIndex(i,1)]
                        - 3.f * _data[postionAsIndex(i,0)]);
        output._data[postionAsIndex(i,0)] = temp;

        temp = _data[postionAsIndex(i,inner_m)] + a * (
                          _data[postionAsIndex(i-1,inner_m)]
                        + _data[postionAsIndex(i+1,inner_m)]
                        + _data[postionAsIndex(i,inner_m-1)]
                        - 3.f * _data[postionAsIndex(i,inner_m)]);
        output._data[postionAsIndex(i,inner_m)] = temp;
    }
    // left and right edges
    for(int j=1; j<inner_n; j++)
    {
        temp = _data[postionAsIndex(0,j)] + a * (
                          _data[postionAsIndex(0,j-1)]
                        + _data[postionAsIndex(0,j+1)]
                        + _data[postionAsIndex(1,j)]
                        - 3.f * _data[postionAsIndex(0,j)]);
        output._data[postionAsIndex(0,j)] = temp;

        temp = _data[postionAsIndex(inner_n,j)] + a * (
                          _data[postionAsIndex(inner_n,j-1)]
                        + _data[postionAsIndex(inner_n,j+1)]
                        + _data[postionAsIndex(inner_n-1,j)]
                        - 3.f * _data[postionAsIndex(inner_n,j)]);
        output._data[postionAsIndex(inner_n,j)] = temp;
    }

    // corners
    temp = _data[postionAsIndex(0,0)] + a * (
              _data[postionAsIndex(0,1)]
            + _data[postionAsIndex(1,0)]
            - 2.f * _data[postionAsIndex(0,0)]);
    output._data[postionAsIndex(0,0)] = temp;

    temp = _data[postionAsIndex(inner_m,0)] + a * (
              _data[postionAsIndex(inner_m,1)]
            + _data[postionAsIndex(inner_m-1,0)]
            - 2.f * _data[postionAsIndex(inner_m,0)]);
    output._data[postionAsIndex(inner_m,0)] = temp;

    temp = _data[postionAsIndex(0,inner_n)] + a * (
              _data[postionAsIndex(1,inner_n)]
            + _data[postionAsIndex(0,inner_n-1)]
            - 2.f * _data[postionAsIndex(0,inner_n)]);
    output._data[postionAsIndex(0,inner_n)] = temp;

    temp = _data[postionAsIndex(inner_m,inner_n)] + a * (
              _data[postionAsIndex(0,1)]
            + _data[postionAsIndex(1,0)]
            - 2.f * _data[postionAsIndex(inner_m,inner_n)]);
    output._data[postionAsIndex(inner_m,inner_n)] = temp;

    for(int i=1; i<inner_m; i++)
    {
        for(int j=1; j<inner_n; j++)
        {
            int ind = postionAsIndex(i,j);
            int indL = postionAsIndex(i-1,j);
            int indR = postionAsIndex(i+1,j);
            int indU = postionAsIndex(i,j+1);
            int indB = postionAsIndex(i,j-1);
            temp = _data[ind] + a * (
                      _data[indL]
                    + _data[indR]
                    + _data[indU]
                    + _data[indB]
                    - 4.f * _data[ind]);
            output._data[ind] = temp;
        }
    }
}

void CField::diffuseLR(CField& output, float scale, float dt)
{
    // TODO asserts needed
    float a = dt * scale;
    int inner_m = _m-1;
    int inner_n = _n-1;

    for(int i=0; i<inner_m; i++)
    {
        for(int j=0; j<inner_n; j++)
        {
            int ind = postionAsIndex(i,j);
            int indR = postionAsIndex(i+1,j);
            float m = _data[ind];
            float n = _data[indR];
            output._data[ind] = m + a * (n-m);
            output._data[indR] = n + a * (m-n);
        }
    }
}

void CField::diffuseGlobal(CField& output, float scale, float dt)
{
    // TODO asserts needed
    float a = dt * scale;
    float average = avg();
    for(int i=0; i<_size; i++)
    {
        output._data[i] = (1.0f-a) * _data[i] + a*average;
    }
}

// Decay a positive value towards zero via a quadratic function
void CField::decayQuadraticly(CField& output, float a, float b, float c, float dt)
{
    for(int i=0; i<_size; i++)
    {
        float v = _data[i];
        float v2 = v*v;
		v-= dt*(a*v2 + b*v + c);
		if (v <0.0f){ v= 0.0f; }
        output._data[i] = v;
    }
}
