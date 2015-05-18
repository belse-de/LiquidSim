#include "liquid.h"

Liquid::Liquid()
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

    setBoundary(density_t0, density_t1, BOUNDARY_MODE_DENSITY);
    density_t0.swap(density_t1);

    advect(density_t0, density_t1, velocity_x_t0, velocity_y_t0);
    density_t0.swap(density_t1);

    setBoundary(density_t0, density_t1, BOUNDARY_MODE_DENSITY);
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

    setBoundary(velocity_x_t0, velocity_x_t1, BOUNDARY_MODE_VELOCITY_X);
    setBoundary(velocity_y_t0, velocity_y_t1, BOUNDARY_MODE_VELOCITY_Y);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);

    project(velocity_x_t0, velocity_y_t0, velocity_x_t1, velocity_y_t1);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);

    setBoundary(velocity_x_t0, velocity_x_t1, BOUNDARY_MODE_VELOCITY_X);
    setBoundary(velocity_y_t0, velocity_y_t1, BOUNDARY_MODE_VELOCITY_Y);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);

    advect(velocity_x_t0, velocity_x_t1, velocity_x_t0, velocity_y_t0);
    advect(velocity_y_t0, velocity_y_t1, velocity_x_t0, velocity_y_t0);

    project(velocity_x_t0, velocity_y_t0, velocity_x_t1, velocity_y_t1);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);

    setBoundary(velocity_x_t0, velocity_x_t1, BOUNDARY_MODE_VELOCITY_X);
    setBoundary(velocity_y_t0, velocity_y_t1, BOUNDARY_MODE_VELOCITY_Y);
    velocity_x_t0.swap(velocity_x_t1);
    velocity_y_t0.swap(velocity_y_t1);
}

void Liquid::addSource(FloatVec& add, FloatVec& result)
{
    result += _dt * add;
}

void Liquid::setSource(FloatVec& add, FloatVec& mask, FloatVec& result)
{
    result = mask * add + (1-mask) * result);
}

void diffuse(FloatVec& velocity_x_t0, FloatVec& velocity_x_t1, float diff)
{
    float c = diff * block_size_m * block_size_m;
    float dt_ = (dt>c) ? (c) : (dt);
    float a  = dt_ * c;
}
