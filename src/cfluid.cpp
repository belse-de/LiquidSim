#include "cfluid.h"
#include "cmatrix.h"
#include <cmath>
#include <cstdio>
#include <iostream>

CFluid::CFluid(int m, int n):
    _pressure_old(m,n), _pressure_new(m,n),
    _ink_old(m,n), _ink_new(m,n),
    _heat_old(m,n), _heat_new(m,n),
    _velocity_x_0(m,n), _velocity_y_0(m,n),
    _velocity_x_1(m,n), _velocity_y_1(m,n),
    _velocity_x_2(m,n), _velocity_y_2(m,n),
    _sources(m,n), _fractions(m,n), _source_fractions(m,n),
    _BFECC(m,n), _balance(m,n),
    _m(m), _n(n), _size(m*n)
{
    //ctor
}

CFluid::~CFluid()
{
    //dtor
}

void CFluid::reset()
{
    // should be 1.0f, lower is more like gas,
    //higher less compressible
    _pressure_old.one();
    _pressure_new.zero();
    // even spread of initial ink
    _ink_old.zero();
    _heat_old.zero();
    _heat_new.zero();
    _velocity_x_0.zero();
    _velocity_y_0.zero();
    _velocity_x_1.zero();
    _velocity_y_1.zero();
    _velocity_x_2.zero();
    _velocity_y_2.zero();
}

// any velocities that are facing outwards at the edge cells, face them inwards
void CFluid::edgeVelocities()
{
    float temp;
    for(int i=0; i<_m; i++)
    {
        temp = _velocity_y_0.get(i,0);
        if(0.f > temp){ _velocity_y_0.set(i,0, -temp); }
        temp = _velocity_y_0.get(i,_n-1);
        if(0.f < temp){ _velocity_y_0.set(i,_n-1, -temp); }
    }

    for(int j=0; j<_n; j++)
    {
        temp = _velocity_x_0.get(0,j);
        if(0.f > temp){ _velocity_y_0.set(0,j, -temp); }
        temp = _velocity_x_0.get(_m-1,j);
        if(0.f < temp){ _velocity_y_0.set(_m-1,j, -temp); }
    }
}

// OLD - cells on either side of a cell affect the middle cell.
// The force of pressure affects velocity
// THIS DOES NOT GIVE VELOCITY TO THE EDGES, LEADING TO STUCK CELLS
// (stuck cells only if velocity is advected before pressure)
// NEW - treat cells pairwise, which allows us to handle edge cells
// updates ALL cells
void CFluid::acceleratByPressure(float force, float dt)
{
    // Pressure differential between points
    // to get an accelleration force.
    float a = dt * force ;

    // First copy the values
    _velocity_x_1 = _velocity_x_0;
    _velocity_y_1 = _velocity_y_0;

    for(int i=0; i<_m; i++)
    {
        for(int j=0; j<_n; j++)
        {
            int index  = _velocity_x_0.postionAsIndex(i,  j);
            int indexR = (i+1<_m) ? _velocity_x_0.postionAsIndex(i+1,j) : _velocity_x_0.postionAsIndex(0,j);
            int indexU = (j+1<_n) ? _velocity_x_0.postionAsIndex(i,  j+1) : _velocity_x_0.postionAsIndex(i,  0);

            float force_x = _pressure_old._data[index] - _pressure_old._data[indexR];
            float force_y = _pressure_old._data[index] - _pressure_old._data[indexU];

            // forces act in same direction  on both cells
            _velocity_x_1._data[index]     +=  a * force_x;
            _velocity_x_1._data[indexR]    +=  a * force_x;  // B-A

            _velocity_y_1._data[index]     +=  a * force_y;
            _velocity_y_1._data[indexU]    +=  a * force_y;
        }
    }

    _velocity_x_1.swap(_velocity_x_0);
    _velocity_y_1.swap(_velocity_y_0);
}

// The force of pressure affects velocity
void CFluid::VelocityFriction(float a, float b, float c, float dt)
{
    // NOTE: We include the border cells in global friction
    for(int i=0; i<_m; i++)
    {
        for(int j=0; j<_n; j++)
        {
            int index  = _velocity_x_0.postionAsIndex(i,  j);
            float x = _velocity_x_0._data[index];
            float y = _velocity_y_0._data[index];
            float len2 = x*x + y*y;
            float len = sqrtf(len2);


            // FIXME: should never happen
            //float sign = 1.0f;
            if (len <0.0f)
            {
                len = -len;
                //sign = -1.0f;
            }

            // Scale by pressure
            float scale = len - dt*(a*len2 + b*len +c);
            if (scale<0.0f) scale = 0.0f;

            _velocity_x_0._data[index] = x * scale / len;
            _velocity_y_0._data[index] = y * scale / len;
        }
    }
}

// Clamp velocity to a maximum magnitude
void CFluid::clampVelocity(float v_max)
{
    float v_max2 = v_max*v_max;
    for(int i=0; i<_m; i++)
    {
        for(int j=0; j<_n; j++)
        {
            int index  = _velocity_x_0.postionAsIndex(i,  j);
            float x = _velocity_x_0._data[index];
            float y = _velocity_y_0._data[index];
            float len2 = x*x + y*y;

            if(len2 > v_max2){
                float len = sqrtf(len2);

                _velocity_x_0._data[index] = x * v_max / len;
                _velocity_y_0._data[index] = y * v_max / len;
            }
        }
    }
}

inline void CFluid::collide(float x,float y, float &x1, float &y1)
{
    float bound_left   = _m-1.0001f;
	float bound_bottom = _n-1.0001f;

	if(_wrapping)
    {
        if (x1<0) x1 = bound_left + x1;
        if (x1>bound_left) x1 -= bound_left;
        if (y1<0) y1 += bound_bottom;
        if (y1>bound_bottom) y1 -= bound_bottom;
    }
    else
    {
        // proper reflection
        if (x1<0) x1 = -x1;
        else if (x1>bound_left) x1=bound_left - (x1 - bound_left);
        if (y1<0) y1 = -y1;
        else if (y1>bound_bottom) y1=bound_bottom - (y1 - bound_bottom);
    }

    if (x1<0) x1 = 0.f;
    else if (x1>bound_left) x1=bound_left;
    if (y1<0) y1 = 0.f;
    else if (y1>bound_bottom) y1=bound_bottom;
}

// Move a scalar along the velocity field
// we are taking pressure from four cells (one of which might be the target cell
// and adding it to the target cell
// hence we need to subtract the appropiate amounts from each cell
//
// Cells are like
//
//    A----B
//    |    |
//    |    |
//    C----D
//
// (Should be square)
// so we work out the bilinear fraction at each of a,b,c,d
// NOTE: Using p1 here, to ensure consistency
void CFluid::reverseAdvection(CField& in, CField& out, float scale, float dt)
{
    // negate advection scale, since it's reverse advection
    float a = -dt * scale;

    // First copy the scalar values over, since we are adding/subtracting in values, not moving things
    out = in;

    // we need to zero out the fractions
    _fractions.zero();

    for(int i=0; i<_m; i++)
    {
        for(int j=0; j<_n; j++)
        {
            float x = _velocity_x_0.get(i,j);
            float y = _velocity_y_0.get(i,j);
            float len2 = x*x+y*y;

            if(0 < len2)
            {
                float x1 = i + x * a;
                float y1 = j + y * a;
                collide(i,j, x1,y1);

                // get fractional parts
                float fx = x1-(int)x1;
                float fy = y1-(int)y1;

/*
By adding the source value into the destination, we handle the problem of multiple destinations
but by subtracting it from the source we gloss ove the problem of multiple sources.
Suppose multiple destinations have the same (partial) source cells, then what happens is the first dest that
is processed will get all of that source cell (or all of the fraction it needs).  Subsequent dest
cells will get a reduced fraction.  In extreme cases this will lead to holes forming based on
the update order.

Solution:  Maintain an array for dest cells, and source cells.
For dest cells, store the four source cells and the four fractions
For source cells, store the number of dest cells that source from here, and the total fraction
E.G.  Dest cells A, B, C all source from cell D (and explicit others XYZ, which we don't need to store)
So, dest cells store A->D(0.1)XYZ..., B->D(0.5)XYZ.... C->D(0.7)XYZ...
Source Cell D is updated with A, B then C
Update A:   Dests = 1, Tot = 0.1
Update B:   Dests = 2, Tot = 0.6
Update C:   Dests = 3, Tot = 1.3

How much should go to each of A, B and C? They are asking for a total of 1.3, so should they get it all, or
should they just get 0.4333 in total?
Ad Hoc answer:
if total <=1 then they get what they ask for
if total >1 then is is divided between them proportionally.
If there were two at 1.0, they would get 0.5 each
If there were two at 0.5, they would get 0.5 each
If there were two at 0.1, they would get 0.1 each
If there were one at 0.6 and one at 0.8, they would get 0.6/1.4 and 0.8/1.4  (0.429 and 0.571) each

So in our example, total is 1.3,
A gets 0.1/1.3, B gets 0.6/1.3 C gets 0.7/1.3, all totalling 1.0

SO we need additional arrays
int mp_sources[size]
int mp_source_fractions[size*4]
float mp_fraction[size]
*/
                // Just calculaitng fractions for now
				float ia = (1.0f-fy)*(1.0f-fx);
				float ib = (1.0f-fy)*(fx)     ;
				float ic = (fy)     *(1.0f-fx);
				float id = (fy)     *(fx)     ;

				// Storing the source information for this dest cell (cell)
				// that's cell1, implying cell1+1, cell1+m_w and cell1+1+m_w
				// and the fractions, unless they are all zero
				// which is quite possible if advecting something sparse
				// but since we optimize for a more likely case, this test wastes time
				int index = _velocity_x_0.postionAsIndex(i,j);
				int index1 = _velocity_x_0.postionAsIndex((int)x1,(int)y1);
				_sources._data[index] = index1;

				//FIXME: _source_fractions hast 4 time the size
				_source_fractions._data[index*4+0] = ia;
                _source_fractions._data[index*4+1] = ib;
                _source_fractions._data[index*4+2] = ic;
                _source_fractions._data[index*4+3] = id;

                // Accumullting the fractions for the four sources
                _fractions._data[index1]		+= ia;
                _fractions._data[index1+1]	    += ib;
                _fractions._data[index1+_m]	+= ic;
                _fractions._data[index1+_m+1] += id;
            }
            else
            {
                // cell has zero velocity, advecting from itself, flag that, and opimize it out later
				_sources.set(i,j, -1);
            }
        }
    }

    // With nice advection, each cell has four sources, and four source fractions
    for(int i=0; i<_size; i++)
    {
        int source = _sources._data[i];
        if( -1 != source)
        {
            // Get the four fractional amounts we earlier interpolated
			float ia = _source_fractions._data[i*4+0];  // Using "cell", not "cell1", as it's an array of four values
			float ib = _source_fractions._data[i*4+1];  // and not the grid of four
			float ic = _source_fractions._data[i*4+2];
			float id = _source_fractions._data[i*4+3];
			// get the TOTAL fraction requested from each source cell
			float fa = _fractions._data[source];
			float fb = _fractions._data[source+1];
			float fc = _fractions._data[source+_m];
			float fd = _fractions._data[source+1+_m];
			// if less then 1.0 in total, we can have all we want

            // THis is a patch, since is fa is zero then ia will be zero
			// The idea is to ensure ALL of the cells a,b,c,d go somewhere.
			// But it may lead to artifacts.
			if (fa<1.0f) fa = 1.0f;
			if (fb<1.0f) fb = 1.0f;
			if (fc<1.0f) fc = 1.0f;
			if (fd<1.0f) fd = 1.0f;

			// scale the amount we are transferring
			ia /= fa;
			ib /= fb;
			ic /= fc;
			id /= fd;

			// Give the fraction of the original source, do not alter the original
			// So we are taking fractions from p_in, but not altering those values
			// as they are used again by later cells
			// if the field were mass conserving, then we could simply move the value
			// but if we try that we lose mass
			out._data[i] += ia * in._data[source] + ib * in._data[source+1] + ic * in._data[source+_m] + id * in._data[source+1+_m];
			// The accounting happens here, the values we added to the dest in p1 are subtracted from the soruces in p1
			out._data[source]       -= ia * in._data[source];
			out._data[source+1]     -= ib * in._data[source+1];
			out._data[source+_m]    -= ic * in._data[source+_m];
			out._data[source+_m+1]  -= id * in._data[source+_m+1];
        }
    }
}

// Signed advection is mass conserving, but allows signed quantities
// so could be used for velocity, since it's faster.
void CFluid::reverseSignedAdvection(CField& in, CField& out, float scale, float dt)
{
    // negate advection scale, since it's reverse advection
    float a = -dt * scale;
    // First copy the scalar values over, since we are adding/subtracting in values, not moving things
    out = in;

    for(int i=0; i<_m; i++)
    {
        for(int j=0; j<_n; j++)
        {
            float x = _velocity_x_0.get(i,j);
            float y = _velocity_y_0.get(i,j);
            float len2 = x*x+y*y;

            if(0 < len2)
            {
                float x1 = i + x * a;
                float y1 = j + y * a;
                collide(i,j, x1,y1);
				int source = _velocity_x_0.postionAsIndex((int)x1,(int)y1);

                // get fractional parts
                float fx = x1-(int)x1;
                float fy = y1-(int)y1;

                // Get amounts from (in) source cells
                float ia = (1.0f-fy)*(1.0f-fx) * in._data[source];
				float ib = (1.0f-fy)*(fx)      * in._data[source+1];
				float ic = (fy)     *(1.0f-fx) * in._data[source+_m];
				float id = (fy)     *(fx)      * in._data[source+_m+1];

				// add to (out) dest cell
				out._data[i] += ia + ib + ic + id ;
                // and subtract from (out) dest cells
                out._data[source]       -= ia;
                out._data[source+1]     -= ib;
                out._data[source+_m]    -= ic;
                out._data[source+_m+1]  -= id;
            }
        }
    }
}

// Move scalar along the velocity field
// Forward advection moves the value at a point forward along the vector from the same point
// and dissipates it between four points as needed
void CFluid::forwardAdvection(CField& in, CField& out, float scale, float dt)
{
    // Pressure differential between points
	// to get an accelleration force.
    float a = dt * scale;
    // First copy the values
    out = in;
    if (scale == 0.0f) return;

    for(int i=0; i<_m; i++)
    {
        for(int j=0; j<_n; j++)
        {
            float x = _velocity_x_0.get(i,j);
            float y = _velocity_y_0.get(i,j);
            float len2 = x*x+y*y;

            if(0 < len2)
            {
                float x1 = i + x * a;
                float y1 = j + y * a;
                collide(i,j, x1,y1);

				int source = _velocity_x_0.postionAsIndex((int)x1,(int)y1);
				int index = _velocity_x_0.postionAsIndex(i,j);

                // get fractional parts
                float fx = x1-(int)x1;
                float fy = y1-(int)y1;
// we are taking pressure from four cells (one of which might be the target cell
// and adding it to the target cell
// hence we need to subtract the appropiate amounts from each cell
//
// Cells are like
//
//    A----B
//    |    |
//    |    |
//    C----D
//
// (Should be square)
// so we work out the bilinear fraction at each of a,b,c,d
                float in_f = in._data[index];

                float ia = (1.0f-fy)*(1.0f-fx) * in_f;
				float ib = (1.0f-fy)*(fx)      * in_f;
				float ic = (fy)     *(1.0f-fx) * in_f;
				float id = (fy)     *(fx)      * in_f;

				// Subtract them from the source cell
				out._data[index] -= (ia+ib+ic+id);
				// Then add them to the four destination cells
				if(source < 0 || source >= _size)
                    std::cerr << "source out of bounds: " << source << "\n"
                        << _size << "\n"
                        << i << " " << j << "\n"
                        << x << " " << y << "\n"
                        << x1 << " " << y1 << "\n"
                        << std::endl;

				out._data[source]		+= ia;
				out._data[source+1]		+= ib;
				out._data[source+_m]	+= ic;
				out._data[source+_m+1]	+= id;
            }
        }
    }
}

// Move part of a scalar along the velocity field
// Intended to spread pressure along velocity lines
void CFluid::partialForwardAdvection(CField& in, CField& out, float scale, float partial, float dt)
{
    //FIXME: should be merged with ForwardAdvection
    //      only on line differs
    // Pressure differential between points
	// to get an accelleration force.
    float a = dt * scale;
    // First copy the values
    out = in;
    if (scale == 0.0f) return;

    for(int i=0; i<_m; i++)
    {
        for(int j=0; j<_n; j++)
        {
            float x = _velocity_x_0.get(i,j);
            float y = _velocity_y_0.get(i,j);
            float len2 = x*x+y*y;

            if(0 < len2)
            {
                float x1 = i + x * a;
                float y1 = j + y * a;
                collide(i,j, x1,y1);
				int source = _velocity_x_0.postionAsIndex((int)x1,(int)y1);
				int index = _velocity_x_0.postionAsIndex(i,j);

                // get fractional parts
                float fx = x1-(int)x1;
                float fy = y1-(int)y1;

                float in_f = in._data[index] * partial;

                float ia = (1.0f-fy)*(1.0f-fx) * in_f;
				float ib = (1.0f-fy)*(fx)      * in_f;
				float ic = (fy)     *(1.0f-fx) * in_f;
				float id = (fy)     *(fx)      * in_f;

				// Subtract them from the source cell
				out._data[index] -= (ia+ib+ic+id);
				// Then add them to the four destination cells
				out._data[source]		+= ia;
				out._data[source+1]		+= ib;
				out._data[source+_m]	+= ic;
				out._data[source+_m+1]	+= id;
            }
        }
    }
}

void CFluid::BFECCForwardAdvection(CField& in, CField& out, float scale, float dt)
{
    // BFECC Algorithm from Flowfixer, Kim, et al.
    // mp_bfecc = Advect( v,p_in)
    // p_out = Advect(-v,mp_bfecc)
    // mp_bfecc = p_in + (p_in-p_out)/2  (Performed in
    // Return Advect(mp_bfecc,p_out)

    // DOES NOT HELP WITH THE WAY I'M DOING THIGNS

    forwardAdvection(in,out,scale,dt);            // Forwards
    forwardAdvection(out,_BFECC,-scale,dt);    	  // then backwards should give us the original
    _BFECC -= in;                                 // Subtract it gives us the error
    _BFECC *= 0.5f;                                // half the error
    _BFECC += in;                   			  // add to original
    forwardAdvection(_BFECC,out,scale,dt);		  // and advect that
}

/**
 * Calculate the curl at position (i, j) in the fluid grid.
 * Physically this represents the vortex strength at the
 * cell. Computed as follows: w = (del x U) where U is the
 * velocity vector at (i, j).
 *
 **/
 float CFluid::curl(int x, int y)
{
    // difference in XV of cells above and below
    // positive number is a counter-clockwise rotation
    float x_curl = (_velocity_x_0.get(x, y + 1) -_velocity_x_0.get(x, y - 1)) * 0.5f;

    // difference in YV of cells left and right
    // positive number is a counter-clockwise rotation
    float y_curl = (_velocity_y_0.get(x + 1, y) - _velocity_y_0.get(x - 1, y)) * 0.5f;

    return x_curl - y_curl;
}

void CFluid::vorticityConfinement(float scale, float dt)
{
    _pressure_new.zero();
    _velocity_x_1.zero();
    _velocity_y_1.zero();

    for(int i=1; i<_m; i++)
    {
        for(int j=1; j<_n; j++)
        {
            _pressure_new.set(i,j, fabs(curl(i,j)));
        }
    }

    for(int i=2; i<_m-1; i++)
    {
        for(int j=2; j<_n-1; j++)
        {
            int index = _pressure_new.postionAsIndex(i,j);
            // get curl gradient across this cell, left right
            float lr_curl = (_pressure_new._data[index+1] - _pressure_new._data[index-1]) * 0.5f;
            // and up down
            float ud_curl = (_pressure_new._data[index+_m] - _pressure_new._data[index-_m]) * 0.5f;

            // Normalize the derivitive curl vector
            float length = (float) sqrtf(lr_curl * lr_curl + ud_curl * ud_curl) + 0.000001f;
            lr_curl /= length;
            ud_curl /= length;

            // get the magnitude of the curl
            float v = curl(i, j);
            // (lr,ud) would be perpendicular, so (-ud,lr) is tangential?
            _velocity_x_1._data[index] = -ud_curl *  v;
            _velocity_y_1._data[index] =  lr_curl *  v;
        }
    }
    _velocity_x_1.forceFrom(_velocity_x_0,scale,dt);
    _velocity_y_1.forceFrom(_velocity_y_0,scale,dt);
}

// Update the fluid with a time step dt
void CFluid::update(float dt)
{
    if(_velocity_diffusion != 0.f)
    {
        float scale = _velocity_diffusion/(float)_diffusion_iterations;
        for(int i=0; i<_diffusion_iterations; i++)
        {
            _velocity_x_0.diffuse(_velocity_x_1, scale,dt);
            _velocity_x_0.swap(_velocity_x_1);
            _velocity_y_0.diffuse(_velocity_y_1, scale,dt);
            _velocity_y_0.swap(_velocity_y_1);
        }
    }

    if(_pressure_diffusion != 0.f)
    {
        float scale = _pressure_diffusion/(float)_diffusion_iterations;
        for(int i=0; i<_diffusion_iterations; i++)
        {
            _pressure_old.diffuse(_pressure_new,scale,dt);
            _pressure_old.swap(_pressure_new);
        }
    }

    if(_heat_diffusion != 0.f)
    {
        float scale = _heat_diffusion/(float)_diffusion_iterations;
        for(int i=0; i<_diffusion_iterations; i++)
        {
            _heat_old.diffuse(_heat_new,scale,dt);
            _heat_old.swap(_heat_new);
        }
    }

    if(_ink_diffusion != 0.f)
    {
        float scale = _ink_diffusion/(float)_diffusion_iterations;
        for(int i=0; i<_diffusion_iterations; i++)
        {
            _ink_old.diffuse(_ink_new,scale,dt);
            _ink_old.swap(_ink_new);
        }
    }

    if(_ink_heat != 0.f)
    {
        // TO make smoke rise under its own steam, we make the ink apply an
		// upwards force on the velocity field
		// heat froce from the ink
        _ink_old.forceFrom(_velocity_y_0, _ink_heat,dt);
    }

    if(_heat_force != 0.f)
    {
        // TO make smoke rise under its own steam, we make the ink apply an
		// upwards force on the velocity field
		// heat froce from the ink
		_heat_old.forceFrom(_velocity_y_0,_heat_force,dt);
		// Cooling effect, otherwise things just explode
		if (_heat_friction_a!=0 || _heat_friction_b!=0 || _heat_friction_c != 0)
        {
            _heat_old.decayQuadraticly(_heat_old, _heat_friction_a, _heat_friction_b, _heat_friction_c, dt);
        }
    }

    // Vorticity confinement is a cosmetic patch that accellerates the velocity
    // field in a direction tangential to the curve defined by the surrounding points
    if(_vorticity != 0.f)
    {
        vorticityConfinement(_vorticity,dt);
    }

    // No major diff if we swap the order of advancing pressure acc and friction forces
    if(_pressure_acc != 0.f)
    {
        acceleratByPressure(_pressure_acc,dt);
    }

    if(_velocity_friction_a != 0.f || _velocity_friction_b != 0.f || _velocity_friction_c != 0.f)
    {
        VelocityFriction(_velocity_friction_a,_velocity_friction_b,_velocity_friction_c, dt);
    }

    // Clamping Veclocity prevents problems when too much energy is introduced into the system
    // it's not strictly necessary, and in fact leads to problems as it gives you a leading edge wave of
    // constant velocity, which cannot dissipate
    float before_p = 0.f;
    float tot_p = 0.f;
    float v_tot = 0.0f;
    float v_max = 0.0f;
    float v_max2 = v_max*v_max;
    for(int i=0; i<_m; i++)
    {
        for(int j=0; j<_n; j++)
        {
            int index = _pressure_new.postionAsIndex(i,j);
            before_p += _pressure_old._data[index];
            float x = _velocity_x_0._data[index];
            float y = _velocity_y_0._data[index];
            float v_len2 = x*x + y*y;

            if(v_len2 > v_max2)
            {
                v_max2 = v_len2;
            }
            v_tot += sqrtf(v_len2);
        }
    }

// Advection step.
// Forward advection works well in general, but does not handle the dissipation
// of single cells of pressure (or lines of cells).
// Reverse advection does not handle self-advection of velocity without diffusion
// Both?  Beautiful!!!

// Problems seems to be getting aswirly velocity field
// and the pressure accumulating at the edges
// and the artefacts of "ripples".  Or are they natural?
// Maybe try setting up a smoke source simulation, as that looks nice.

    // The advection scale is needed for when we change grid sizes
	// smaller grids mean large cells, so scale should be smaller
	float advection_scale = _m / 100.f;
	float inc_adv_scale = _ink_advection * advection_scale;
	_ink_new.one();
	forwardAdvection(_ink_new, _balance, inc_adv_scale, dt);

	// Advect the ink - ink is one fluid suspended in another, like smoke in air
    forwardAdvection(_ink_old, _ink_new, inc_adv_scale, dt);
    _ink_old.swap(_ink_new);
    forwardAdvection(_ink_old, _ink_new, inc_adv_scale, dt);
    _ink_old.swap(_ink_new);

	// Only advect the heat if it is applying a force
	if(_heat_force != 0)
    {
        float scale = _heat_advection * advection_scale;
        forwardAdvection(_heat_old, _heat_new, scale, dt);
        _heat_old.swap(_heat_new);
        forwardAdvection(_heat_old, _heat_new, scale, dt);
        _heat_old.swap(_heat_new);
    }

// Advection order makes very significant differences
// If presure is advected first, it leads to self-maintaining waves
// and ripple artifacts
// So the "velocity first" seems preferable as it naturally dissipates the waves
// By advecting the velocity first we advect the pressure using the next frames velocity
//
// Self advect the velocity via three buffers (if reverse advecting)
// buffers are 0,1 and 2
// our current velocity is in 0
// we advec t 0 to 1 using 0
// we the  advect 1 to 2 using 0 again
// then swap 2 into 0 so it becomes the new current velocity
    float vel_scale = _velocity_advection * advection_scale;
    forwardAdvection(_velocity_x_0, _velocity_x_1, vel_scale, dt);
    forwardAdvection(_velocity_y_0, _velocity_y_1, vel_scale, dt);

    // Advect from 1 into 2, then swap 0 and 2
    // We can use signed reverse advection as quantities can be negative
    reverseSignedAdvection(_velocity_x_1, _velocity_x_2, vel_scale, dt);
    reverseSignedAdvection(_velocity_y_1, _velocity_y_2, vel_scale, dt);
    _velocity_x_2.swap(_velocity_x_0);
    _velocity_y_2.swap(_velocity_y_0);

    // handle velocities at the edge, confining them to within the cells.
    // Not needed with correct edge sampling and pressure, as edge pressure will turn the vel
    // But since we have several
    edgeVelocities();

    // Advect the pressure, representing the compressible fluid (like the air)
    float press_scale = _pressure_advection * advection_scale;
    forwardAdvection(_pressure_old, _pressure_new, press_scale, dt);
    _pressure_old.swap(_pressure_new);
    forwardAdvection(_pressure_old, _pressure_new, press_scale, dt);
    _pressure_old.swap(_pressure_new);

    snprintf(buf, 512, "dt = %2.4f, P = %4.2f, P' = %4.2f, P'-P = %2.2f,  v_tot = %.2f, v_avg = %.3f, v_max = %.3f",
		dt, before_p, tot_p, before_p-tot_p, v_tot, v_tot/(_m*_n), v_max);
}

void CFluid::render(SDL_Surface* s)
{
    static const float temp_min = 0.;
    static const float temp_max = 42.;
    static const float temp_mid = (temp_min + temp_max) / 2.;

    uint32_t *pixels = (uint32_t *)s->pixels;
    SDL_PixelFormat *f = s->format;

    for(int n = 0; n < _n; n++){
        for(int m = 0; m < _m; m++){
            //int i = n * M + m;
            int x = _pressure_old.postionAsIndex(m,n);
            int xPic = _pressure_old.postionAsIndex(m,_m-n);
            float temp_ratio = (_pressure_old._data[x]-temp_min) / temp_mid;

            float b_f = (1. - temp_ratio);
            b_f = (0 > b_f) ? 0. : b_f;
            b_f = (1 < b_f) ? 1. : b_f;
            int b = 255. * b_f;
            float r_f = (temp_ratio - 1.);
            r_f = (0 > r_f) ? 0. : r_f;
            r_f = (1 < r_f) ? 1. : r_f;

            int r = 255. * r_f;
            int g = 255 - b - r;
            //if( 0 < x[i]) std::cout << "val: " << x[i] << "\n rat: " << temp_ratio << "\n red: " << r_f << "\n blu: " << b_f << std::endl;

            uint32_t color = SDL_MapRGBA(f, r,g,b,255);
            pixels[xPic] = color;
        }
    }
}
