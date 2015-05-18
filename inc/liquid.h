#ifndef LIQUID_H
#define LIQUID_H

#include <SDL.h>

#include "floatvec.hpp"

class Liquid
{
    public:
        /** Default constructor */
        Liquid();
        /** Default destructor */
        virtual ~Liquid();

        void simulation_step();
        void render(SDL_Texture* texture);

    protected:
    private:
        static const float visc = 1.512041288;  // uPa * s + K^-1/2
        static const float diff = 1.5e-5;       // m^2 * s^1
                     float dt   = 1.f;

        static const float f_esp = 1.19209289551e-7;
        static const float f_esp_2 = f_esp * f_esp;

        static const int K_MAX = 20;
        static const float block_size_m = 1;

        FloatVec density_0;
        FloatVec density_0_weight;
        FloatVec density_t0;
        FloatVec density_t1;

        FloatVec velocity_x_0;
        FloatVec velocity_y_0;
        FloatVec velocity_0_weight;

        FloatVec velocity_x_t0;
        FloatVec velocity_y_t0;

        FloatVec velocity_x_t1;
        FloatVec velocity_y_t1;

        void density_step();
        void velocity_step();

        void addSource(FloatVec& add, FloatVec& result);
        void setSource(FloatVec& add, FloatVec& mask, FloatVec& result);
};

#endif // LIQUID_H
