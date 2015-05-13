#include <unistd.h>
#include <iostream>

#include "main.hpp"

#define DEFAULT_I 100
#define DEFAULT_W 256
#define DEFAULT_H 256
#define DEFAULT_S 1.f

//SDL needs this structure because it uses a macro to exchange
int main(int argC, char** argS)
{
#ifndef NDEBUG
    printf("Got %d arguments:\n", argC);
    for( int i = 0; i < argC; i++){ printf("    Argument %d :\n\t %s\n", i, argS[i]); }
#endif // NDEBUG

    uint32_t iterations, width, height;
    iterations = width = height = 0;
    float scale_m = 0;

    opterr = 1;
    int opt = 0;
    while ((opt = getopt(argC, argS, "i:w:h:s:?")) != -1)
    {
        switch(opt)
        {
        case 'i':
            iterations = atoi(optarg);
            break;
        case 'w':
            width = atoi(optarg);
            break;
        case 'h':
            height = atoi(optarg);
            break;
        case 's':
            scale_m = atoi(optarg);
            break;
        case '?':
            printf("-i iterations\n");
            printf("-w width\n");
            printf("-h height\n");
            printf("-s scale\n");
            exit(EXIT_SUCCESS);
        default:
            exit(EXIT_FAILURE);
        }
    }

    iterations = (iterations<=0) ? DEFAULT_I : iterations;
    width      = (width<=0)      ? DEFAULT_W : width;
    height     = (height<=0)     ? DEFAULT_H : height;
    scale_m    = (scale_m<=0)    ? DEFAULT_S : scale_m;

#ifndef NDEBUG
    printf("\n");
    printf("Starting simulation with %d iterations,\n", iterations);
    printf("    and an area of %dX%d elements.\n",      width, height);
    printf("    The used scale is %.6f\n\n",            scale_m);


    printf("+------------------------------+\n");
    printf("| starting sim                 |\n");
    printf("+------------------------------+\n");
    printf("\n");
#endif // NDEBUG

    try{
        SdlApp app(480, 480);
        app.draw();
        app.loop();
    } catch(SdlError& e){
        std::cerr << "SDL: " << newline << e.what() << std::endl;
        abort();
    } catch(SdlImgError& e){
        std::cerr << "IMG: " << newline << e.what() << std::endl;
        abort();
    }

    /*
    FluidSim sim(width, height, scale_m);
    sim.run(iterations);
    */

    exit(EXIT_SUCCESS);
}
