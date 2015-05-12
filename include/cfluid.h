#ifndef CFLUID_H
#define CFLUID_H

#include "cfield.h"

#include <SDL.h>

class CFluid
{
    public:
        /** Default constructor */
        CFluid(int m, int n);
        /** Default destructor */
        virtual ~CFluid();

        void    reset();
        void    edgeVelocities();
        void    acceleratByPressure(float force, float dt);
        void    VelocityFriction(float a, float b, float c, float dt);
        void    clampVelocity(float v_max);
        void    collide(float x,float y, float &x1, float &y1);
        void    reverseAdvection(CField& in, CField& out, float scale, float dt);
        void    reverseSignedAdvection(CField& in, CField& out, float scale, float dt);
        void    forwardAdvection(CField& in, CField& out, float scale, float dt);
        void    partialForwardAdvection(CField& in, CField& out, float scale, float partial, float dt);
        void    BFECCForwardAdvection(CField& in, CField& out, float scale, float dt);
        float   curl(int x, int y);
        void    vorticityConfinement(float scale, float dt);
        void    update(float dt);
        void    render(SDL_Surface* s);

        // "Physical" constants
        float _velocity_diffusion;
        int	_diffusion_iterations;

        // With no pressure diffusion, waves of pressure keep moving
        // But only if pressure is advected ahead of velocity
        float _pressure_diffusion;
        float _heat_diffusion;
        float _ink_diffusion;

        // friction applied to velocity.  As a simple fraction each time step
        // so 1.0f will give you no friction

        // Friction is very important to the realism of the simulation
        // changes to these coeffficinets will GREATLY affect the apperence of the simulation.
        // the high factor friction (a) wich is applied to the square of the velocity,
        // acts as a smooth limit to velocity, which causes streams to break up into turbulent flow
        // The c term will make things stop, which then shows up our border artefacts
        // but a high enough (>0.01) c term is needed to stop things just dissipating too fast
        // Friction is dt*(a*v^2 + b*v + c)
        float _velocity_friction_a;
        float _velocity_friction_b;
        float _velocity_friction_c;
        // Pressure accelleration.  Larger values (>10) are more realistic (like water)
        // values that are too large lead to chaotic waves
        float _vorticity;
        float _pressure_acc;
        float _ink_heat;
        float _heat_force;
        float _heat_friction_a;
        float _heat_friction_b;
        float _heat_friction_c;
        // high ink advection allows fast moving nice swirlys
        float _ink_advection;
        // seems nice if both velocity and pressure at same value, which makes sense
        float _velocity_advection;
        float _pressure_advection;
        float _heat_advection;
 //   protected:
 //   private:
        CField _pressure_old;
        CField _pressure_new;

        CField _ink_old;
        CField _ink_new;

        CField _heat_old;
        CField _heat_new;

        CField _velocity_x_0;
        CField _velocity_y_0;

        CField _velocity_x_1;
        CField _velocity_y_1;

        CField _velocity_x_2;
        CField _velocity_y_2;

        // temp accumulators of data for advection
        CField _sources;
        CField _fractions;
        CField _source_fractions; //FIXME size *= 4 -> why

        // BFECC needs its own buffer
        // can't reuse other, since they might be used by the advection step
        CField _BFECC;
        CField _balance;

        int _m; //size in x direction
        int _n; //size in y direction
        int _size; //the number of fields

        bool    _wrapping = false;
        char buf[512];
};

#endif // CFLUID_H
