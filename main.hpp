#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

// c headers
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

// intrinsics / cpu includes
#include <x86intrin.h>
#include <cblas.h>
// SDL includes / fol multi media
#include <SDL.h>
//XXX: NO NEEDED, YET.
//#include <SDL_image.h>
//#include <SDL_ttf.h>
//#include <SDL_mixer.h>
//#include <SDL_net.h>

// std defines for truncating/clamping values
//#define min(x,y) ((x)<(y))?(x):(y)
//#define max(x,y) ((x)>(y))?(x):(y)
#define minmax(x,y,z) min(max((x),(y)),(z))

//local headers
#include "sdl.hpp"
#include "sdlerror.hpp"
#include "sdlimgerror.hpp"
#include "sdltexture.hpp"

// global constant expressions
constexpr static const auto newline_win = "\r\n";
constexpr static const auto newline_mac = "\r";
constexpr static const auto newline_unx = "\n";
constexpr static const auto newline = newline_win;
#endif // MAIN_HPP_INCLUDED
