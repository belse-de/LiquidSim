#include "liquid.h"

Liquid::Liquid(size_t width, size_t height) :
    _width(width), _height(height),
    density_0(width*height),density_0_weight(width*height),
    density_t0(width*height), density_t1(width*height),
    velocity_x_0(width*height), velocity_y_0(width*height),
    velocity_0_weight(width*height),
    velocity_x_t0(width*height), velocity_y_t0(width*height),
    velocity_x_t1(width*height), velocity_y_t1(width*height)
{
    //ctor
}

Liquid::~Liquid()
{
    //dtor
}
void Liquid::simulation_step()
{
    //add constant sources
    setSource(density_0, density_0_weight, density_t0);
    setSource(velocity_x_0, velocity_0_weight, velocity_x_t0);
    setSource(velocity_y_0, velocity_0_weight, velocity_y_t0);

    //add temporary sources
    //TODO:
    //addSource
    //
    //

    //simulate one step
    density_step();
    velocity_step();

    //swap t0 with t1 -> so new values become the current
    density_t0.swap(density_t1);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);

    //sim step finished
}

void Liquid::render(SDL_Texture* texture)
{

}

void Liquid::density_step()
{
    addSource(density_t0, density_t1);
    density_t0.swap(density_t1);

    diffuse(density_t0, density_t1, diff);
    density_t0.swap(density_t1);

    setBoundary(density_t0, density_t1, BoundaryMode::BOUNDARY_MODE_DENSITY);
    density_t0.swap(density_t1);

    advect(density_t0, density_t1, velocity_x_t0, velocity_y_t0);
    density_t0.swap(density_t1);

    setBoundary(density_t0, density_t1, BoundaryMode::BOUNDARY_MODE_DENSITY);
    density_t0.swap(density_t1);
}

void Liquid::velocity_step()
{
    addSource(velocity_x_t0, velocity_x_t1);
    addSource(velocity_y_t0, velocity_y_t1);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);

    diffuse(velocity_x_t0, velocity_x_t1, visc);
    diffuse(velocity_y_t0, velocity_y_t1, visc);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);

    setBoundary(velocity_x_t0, velocity_x_t1, BoundaryMode::BOUNDARY_MODE_VELOCITY_X);
    setBoundary(velocity_y_t0, velocity_y_t1, BoundaryMode::BOUNDARY_MODE_VELOCITY_Y);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);

    project(velocity_x_t0, velocity_y_t0, velocity_x_t1, velocity_y_t1);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);

    setBoundary(velocity_x_t0, velocity_x_t1, BoundaryMode::BOUNDARY_MODE_VELOCITY_X);
    setBoundary(velocity_y_t0, velocity_y_t1, BoundaryMode::BOUNDARY_MODE_VELOCITY_Y);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);

    advect(velocity_x_t0, velocity_x_t1, velocity_x_t0, velocity_y_t0);
    advect(velocity_y_t0, velocity_y_t1, velocity_x_t0, velocity_y_t0);

    project(velocity_x_t0, velocity_y_t0, velocity_x_t1, velocity_y_t1);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);

    setBoundary(velocity_x_t0, velocity_x_t1, BoundaryMode::BOUNDARY_MODE_VELOCITY_X);
    setBoundary(velocity_y_t0, velocity_y_t1, BoundaryMode::BOUNDARY_MODE_VELOCITY_Y);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);
}

void Liquid::addSource(FloatVec& add, FloatVec& result)
{
    result += add * dt;
}

void Liquid::setSource(FloatVec& add, FloatVec& mask, FloatVec& result)
{
    result = mask * add + (-mask+1.) * result;
}

void Liquid::diffuse(FloatVec& x_t0, FloatVec& x_t1, float diff)
{
    float c = diff * block_size_m * block_size_m;
    //float dt_ = (dt>c) ? (c) : (dt);
    float a  = dt * c;

    //Gauss-Seidel relaxation.
    for(int k=0; k<K_MAX; k++)
    {
        for(unsigned int i=1; i<_width-1; i++)
        {
            for(unsigned int j=1; j<_height-1; j++)
            {
                x_t1(i+j*_width) = (x_t0(i+j*_width)
                          + a*(
                               x_t1(i-1+j*_width)   + x_t1(i+1+j*_width)  +
                               x_t1(i+(j-1)*_width) + x_t1(i+(j+1)*_width)
                               )
                          ) / (1+4*a);
            }
        }
    }
}

void Liquid::setBoundary(FloatVec& d_t0, FloatVec& d_t1, BoundaryMode mode)
{
    d_t1 = d_t0;
}

void Liquid::advect(FloatVec& d_t0, FloatVec& d_t1, FloatVec& velocity_x, FloatVec& velocity_y)
{
    d_t1 = d_t0;
}

void Liquid::project(FloatVec& v_x_t0, FloatVec& v_y_t0, FloatVec& velocity_x, FloatVec& velocity_y)
{
    ;
}
