#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include <stdint.h>
#include <SDL.h>

void renderFloats2RGBA8888(SDL_Texture* texture, int renderWidth, int renderHeight, float* dens, float* temp);

void floats2RGBA8888s_switch(size_t size, float* ts, uint32_t* rgba8888s);
void floats2RGBA8888s_expf(size_t size, float* ts, uint32_t* rgba8888s);

uint32_t float2RGBA8888_switch(float t);
uint32_t float2RGBA8888_expf(float t);

#endif // RENDER_H_INCLUDED

