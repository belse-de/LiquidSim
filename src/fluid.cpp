#include "fluid.hpp"

/*
Fluid::Fluid(int dim_x, int dim_y):
    width(dim_x), height(dim_y),
    density_curr(dim_x,dim_y),
    density_next(dim_x,dim_y),
    temperature_curr(dim_x,dim_y),
    temperature_next(dim_x,dim_y),
    velocity_curr(dim_x,dim_y),
    velocity_next(dim_x,dim_y),
    sourceMask_density(dim_x,dim_y),
    sources_density(dim_x,dim_y),
    sourceMask_velocity(dim_x,dim_y),
    sources_velocity(dim_x,dim_y)
{
    //ctor
}


Fluid::~Fluid()
{
    //dtor
}

Fluid::Fluid(const Fluid& other):
    width(other.width), height(other.height),
    density_curr(other.density_curr),
    density_next(other.density_next),
    temperature_curr(other.temperature_curr),
    temperature_next(other.temperature_next),
    velocity_curr(other.velocity_curr),
    velocity_next(other.velocity_next),
    sourceMask_density(other.sourceMask_density),
    sources_density(other.sources_density),
    sourceMask_velocity(other.sourceMask_velocity),
    sources_velocity(other.sources_velocity),
    dt(other.dt)
{
    //copy ctor
}

void Fluid::sim_step()
{
    //set const / endless sources
    set_source(velocity_curr, sources_velocity, sourceMask_velocity);
    set_source(density_curr, sources_density, sourceMask_density);

    //vel_step
    add_source(velocity_next, velocity_curr);
    velocity_next.swap(velocity_curr);
    diffuse(velocity_next, velocity_curr, visc);
    //TODO: project
    velocity_next.swap(velocity_curr);
    advect(velocity_next, velocity_curr, velocity_curr);
    //TODO: project

    //dens_step
    add_source(density_next, density_curr);
    density_next.swap(density_curr);
    diffuse(density_next, density_curr, diff);
    density_next.swap(density_curr);
    advect(density_next, density_curr, velocity_curr);

    // change next to current
    velocity_next.swap(velocity_curr);
    density_next.swap(density_curr);
}

void Fluid::add_source(Field& next, Field& curr)
{
    next += dt*curr;
}

void Fluid::set_source(Field& next, Field& source, Field<bool>& mask)
{
    for(int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
        {
            next.element(i,j) = mask.element(i,j) ? source.element(i,j) : next.element(i,j);
        }
    }
}

void Fluid::diffuse(Field& next, Field& curr, const float scale)
{

}

void Fluid::advect(Field& next, Field& curr, VectorField direction)
{
    for(int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
        {
            Vector2 v(i,j);
            v -= direction.element(i,j);
            next.element(i,j) = dt * curr.element(v.x, v,y);
        }
    }
}
*/
