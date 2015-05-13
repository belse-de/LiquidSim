#ifndef FLUID_H
#define FLUID_H

#include <SDL.h>

#include "potentionalfield.hpp"
#include "vectorfield.h"
/*
class Fluid
{
    public:
        // Default constructor
        Fluid(int dim_x, int dim_y);
        // Default destructor
        virtual ~Fluid();
        // Copy constructor
        //\param other Object to copy from
        ///
        Fluid(const Fluid& other);

        void sim_step();
//        void add_source(Field<?>& next, Field<?>& curr);
        //void diffuse(Field& next, Field& curr, const float scale);
        //void advect(Field& next, Field& curr, VectorField direction);

    protected:
    private:
        int width;
        int height;
        PotentionalField density_curr;
        PotentionalField density_next;
        PotentionalField temperature_curr;
        PotentionalField temperature_next;
        VectorField      velocity_curr;
        VectorField      velocity_next;

        Field<bool>      sourceMask_density;
        PotentionalField sources_density;

        Field<bool>      sourceMask_velocity;
        PotentionalField sources_velocity;



        float dt = 0.25f;
        const float visc = 1.512041288; // uPa * s + K^-1/2
        const float diff = 1.5e-5; //m^2 * s^1
        //const float diff = 1.5e+0; //m^2 * s^1

        //pointer to a texture to which the sim is plotted
        SDL_Texture* m_render_texture = nullptr;
        // setting constants for rendering
        constexpr static const float m_spectrum_dens_min_kg_m3 =   0.f;
        constexpr static const float m_spectrum_dens_max_kg_m3 = 100.f;
        constexpr static const float m_spectrum_temp_min_degC  = -15.f;
        constexpr static const float m_spectrum_temp_max_degC  =  42.f;
        constexpr static const float m_spectrum_purple_max     = 1.f / 7.f;
        constexpr static const float m_spectrum_blue_max       = 2.f / 7.f;
        constexpr static const float m_spectrum_green_max      = 3.f / 7.f;
        constexpr static const float m_spectrum_yellow_max     = 4.f / 7.f;
        constexpr static const float m_spectrum_orange_max     = 5.f / 7.f;
        constexpr static const float m_spectrum_red_max        = 6.f / 7.f;
        constexpr static const float m_spectrum_change_rate    = 7.f;
};
*/
#endif // FLUID_H
