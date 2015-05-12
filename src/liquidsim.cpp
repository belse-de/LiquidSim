#include <iostream>
#include <cmath>
#include <cstring>
#include "liquidsim.h"

#include "renderer.hpp"

#include "sdlerror.hpp"
#define SWAP(x0,x) {float *tmp=x0;x0=x;x=tmp;}
#define IX(i,j) ((i)+(N+2)*(j))

//http://www.dgp.toronto.edu/~stam/reality/Research/pub.html
//http://autodeskresearch.com/pdf/talks/jos_gdc03.pdf
//http://www.dgp.toronto.edu/people/stam/reality/Research/pdf/GDC03.pdf
//http://www.dgp.utoronto.ca/~stam/reality/Talks/FluidsTalk/FluidsTalkNotes.pdf
//http://www.mat.ucsb.edu/~wakefield/594cm/assignment.htm
//http://www.gamasutra.com/view/feature/1549/practical_fluid_dynamics_part_1.php?print=1

/*int ConjugateGradient(const Matrix &A, Vector &x, const Vector &b,
   const Preconditioner &M, int &max_iter, Real &tol)
{*/

LiquidSim::LiquidSim()
{
    //ctor
}

void LiquidSim::setDiff(int mode)
{
    if( mode < 0 ) mode = 0;
    diffusion_state = mode % diffusion_state_max;
}

void LiquidSim::incDiff()
{
    if( diffusion_state < 0 ) diffusion_state = 0;
    diffusion_state = (diffusion_state + 1) % diffusion_state_max;
    std::cout << "diff. " << diffusion_state << std::endl;
    switch(diffusion_state)
    {
    case 0:
        std::cout << "diff. disabled" << std::endl;
        break;
    case 1:
        std::cout << "diff. unstable / simple" << std::endl;
        break;
    case 2:
        std::cout << "diff. stable / Gauss-Seidel relaxation" << std::endl;
        break;
    case 3:
        std::cout << "diff. stable / Conjugate Gradient (CG)" << std::endl;
        break;
    default:
        std::cout << "diff. ????? / ERROR" << std::endl;
        break;
    }
}

void LiquidSim::add_source(int N, float* __restrict__  x, float* __restrict__ s, float dt)
{
    const int x_size=(N+2)*(N+2);
    float* __restrict__ r_ = new float[x_size];

    for (int i=0; i<x_size; i++) r_[i] = dt * (s[i] + x[i]);

    memcpy(x, r_, x_size * sizeof(float));
    delete[] r_;
}
void LiquidSim::set_source(int N, float* x, float* s, float dt)
{
    int x_size=(N+2)*(N+2);
    for (int i=0; i<x_size; i++) {
            if(0. != s[i]) x[i] = dt*s[i];// bad loop form no conditional
    }
}

void LiquidSim::copy_values(int N, float* x, float* s, float dt)
{
    int x_size=(N+2)*(N+2);
    for (int i=0; i<x_size; i++) x[i] = dt * s[i];
}

void LiquidSim::calc_grad_xy(int N, float* u, float* v, float* x, float dt)
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            float grad_x = (x[IX(i-1,j)] - x[IX(i+1,j)]) / 2.;
            float grad_y = (x[IX(i,j-1)] - x[IX(i,j+1)]) / 2.;
            u[IX(i,j)] += dt * grad_x;
            v[IX(i,j)] += dt * grad_y;
        }
    }
}

void LiquidSim::diffuse(int N, int b, float* x, float* x0, float diff, float dt)
{
    //std::cout << "diffusion_state: " << diffusion_state << std::endl;
    static const float f_esp = 1.19209289551e-7;
    static const float f_esp_2 = f_esp * f_esp;
    const int _size = (N+1) * (N+1);
    const float tol     = _size * f_esp_2 * 1e7; //esp
    //const float tol_dif = _size * f_esp_2 * 1e7;
    const float tol_dif = _size * f_esp_2 * 1e8;

    int K_MAX = 20;
    float a  = dt*diff;// /(N*N);
    float a_ = (a>.5) ? 0.5 : a;

    float normb = 0.f;
    for(int i=0; i<_size; i++) { normb += x0[i] * x0[i]; }
    normb = sqrtf(normb);
    if(normb <= 0) normb = 1.f;

    // unstable simple
    float sum_old = 0.f;
    float sum = 0.f;

    //Gauss-Seidel relaxation.
    for(int k=0; k<K_MAX; k++)
    {
        sum = 0.f;
        for(int i=1; i<=N; i++)
        {
            for(int j=1; j<=N; j++)
            {
                if(k == 0)
                {
                    x[IX(i,j)] = x0[IX(i,j)] + a_*(x0[IX(i-1,j)]+x0[IX(i+1,j)]+x0[IX(i,j-1)]+x0[IX(i,j+1)]-4*x0[IX(i,j)] );
                } else
                {
                    x[IX(i,j)] = (x0[IX(i,j)] + a*(x[IX(i-1,j)]+x[IX(i+1,j)]+
                                               x[IX(i,j-1)]+x[IX(i,j+1)]))/(1+4*a);
                }

                float z = (1-4*a)*x[IX(i,j)] - a*(x[IX(i-1,j)]+x[IX(i+1,j)]+x[IX(i,j-1)]+x[IX(i,j+1)]);
                float error = z - x0[IX(i,j)];
                sum += error * error;
            }
        }
        set_bnd(N, b, x);

        if(sum <= tol) { K_MAX = k; }
        if( (sum_old-sum) <= tol_dif && k != 0 ) { K_MAX = k; }
        sum_old = sum;
    }
    std::cerr << b << ":" << K_MAX << ":" << sum  << std::endl;
}

void LiquidSim::advect(int N, int b, float* d, float* d0, float* u, float* v, float dt)
{
    int i, j, i0, j0, i1, j1;
    float x, y, s0, t0, s1, t1, dt0;
    dt0 = dt*N;
    for(i=1; i<=N; i++)
    {
        for(j=1; j<=N; j++)
        {
            x = i-dt0*u[IX(i,j)];
            y = j-dt0*v[IX(i,j)];
            if (x<0.5) x=0.5;
            if (x>N+0.5) x=N+ 0.5;
            i0=(int)x;
            i1=i0+1;
            if (y<0.5) y=0.5;
            if (y>N+0.5) y=N+ 0.5;
            j0=(int)y;
            j1=j0+1;
            s1 = x-i0;
            s0 = 1-s1;
            t1 = y-j0;
            t0 = 1-t1;
            d[IX(i,j)] = s0*(t0*d0[IX(i0,j0)]+t1*d0[IX(i0,j1)])+
                         s1*(t0*d0[IX(i1,j0)]+t1*d0[IX(i1,j1)]);
        }
    }
    set_bnd(N, b, d);
}

void LiquidSim::dens_step(int N, float* x, float* x0, float* u, float* v, float diff,
                            float dt)
{
    add_source(N, x, x0, dt);
    SWAP(x0, x);
    diffuse(N, 0, x, x0, diff, dt);
    SWAP(x0, x);
    advect(N, 0, x, x0, u, v, dt);
}

void LiquidSim::vel_step(int N, float* u, float* v, float* u0, float* v0,
                           float visc, float dt)
{
    add_source(N, u, u0, dt);
    add_source(N, v, v0, dt);
    SWAP(u0, u);
    diffuse(N, 1, u, u0, visc, dt);
    SWAP(v0, v);
    diffuse(N, 2, v, v0, visc, dt);
    project(N, u, v, u0, v0);
    SWAP(u0, u);
    SWAP(v0, v);
    advect(N, 1, u, u0, u0, v0, dt);
    advect(N, 2, v, v0, u0, v0, dt);
    project(N, u, v, u0, v0);
}

void LiquidSim::project(int N, float* u, float* v, float* p, float* div)
{
    int i, j, k;
    float h;
    h = 1.0/N;
    for(i=1; i<=N; i++)
    {
        for(j=1; j<=N; j++)
        {
            div[IX(i,j)] = -0.5*h*(u[IX(i+1,j)]-u[IX(i-1,j)]+
                                   v[IX(i,j+1)]-v[IX(i,j-1)]);
            p[IX(i,j)] = 0;
        }
    }
    set_bnd(N, 0, div);
    set_bnd(N, 0, p);
    for(k=0; k<20; k++)
    {
        for(i=1; i<=N; i++)
        {
            for(j=1; j<=N; j++)
            {
                p[IX(i,j)] = (div[IX(i,j)]+p[IX(i-1,j)]+p[IX(i+1,j)]+
                              p[IX(i,j-1)]+p[IX(i,j+1)])/4;
            }
        }
        set_bnd(N, 0, p);
    }
    for(i=1; i<=N; i++)
    {
        for(j=1; j<=N; j++)
        {
            u[IX(i,j)] -= 0.5*(p[IX(i+1,j)]-p[IX(i-1,j)])/h;
            v[IX(i,j)] -= 0.5*(p[IX(i,j+1)]-p[IX(i,j-1)])/h;
        }
    }
    set_bnd(N, 1, u);
    set_bnd(N, 2, v);
}

void LiquidSim::set_bnd(int N, int b, float* x)
{
    for(int i=1; i<=N; i++)
    {
        x[IX(0 ,i)]  = b==1 ? -x[IX(1,i)] : x[IX(1,i)];
        x[IX(N+1,i)] = b==1 ? -x[IX(N,i)] : x[IX(N,i)];
        x[IX(i,0)]   = b==2 ? -x[IX(i,1)] : x[IX(i,1)];
        x[IX(i,N+1)] = b==2 ? -x[IX(i,N)] : x[IX(i,N)];
    }
    x[IX(0 ,0)] = 0.5*(x[IX(1,0)]+x[IX(0 ,1)]);
    x[IX(0 ,N+1)] = 0.5*(x[IX(1,N+1)]+x[IX(0 ,N)]);
    x[IX(N+1,0)] = 0.5*(x[IX(N,0)]+x[IX(N+1,1)]);
    x[IX(N+1,N+1)] = 0.5*(x[IX(N,N+1)]+x[IX(N+1,N)]);
}

void LiquidSim::sim_step(int N, float* dens, float* dens_prev, float* u, float* v, float* u_prev, float* v_prev, float diff, float visc, float dt)
{
    vel_step(N, u, v, u_prev, v_prev, visc, dt);
    dens_step(N, dens, dens_prev, u, v, diff, dt);
}

// "map" the field into gfx mem
// burning trees are red
// trees are green
// "voids" are black;
SDL_Texture* LiquidSim::render(int N, float* f, SDL_Texture* texture)
{
    int M = N+2;
    int width = M;
    int height = M;

    renderFloats2RGBA8888(texture, width, height, nullptr, f);

    return texture;
}

