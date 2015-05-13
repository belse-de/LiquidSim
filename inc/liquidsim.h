#ifndef LIQUIDSIM_H
#define LIQUIDSIM_H

#include <SDL.h>

class LiquidSim
{
    public:
        /** Default constructor */
        LiquidSim();
        void add_source(int N, float* __restrict__ x, float* __restrict__ s, float dt);
        void set_source(int N, float* x, float* s, float dt);
        void copy_values(int N, float* x, float* s, float dt);
        void calc_grad_xy(int N, float* u, float* v, float* x, float dt);
        void diffuse(int N, int b, float* x, float* x0, float diff, float dt);
        void advect(int N, int b, float* d, float* d0, float* u, float* v, float dt);
        void dens_step(int N, float* x, float* x0, float* u, float* v, float diff, float dt);
        void vel_step(int N, float* u, float* v, float* u0, float* v0, float visc, float dt);
        void project(int N, float* u, float* v, float* p, float* div);
        void set_bnd(int N, int b, float* x);
        void sim_step(int N, float* dens, float* dens_prev, float* u, float* v, float* u_prev, float* v_prev, float diff, float visc, float dt);

        void setDiff(int mode = 2);
        void incDiff();

        SDL_Texture* render(int N, float* f, SDL_Texture* texture);
    protected:
    private:
        int diffusion_state = 1;
        int diffusion_state_max = 5;
        //float visc;
        //float diff;
        //int M;
        //int N;
};

#endif // LIQUIDSIM_H
