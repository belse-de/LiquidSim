#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "renderer.hpp"

#define min(x,y) ((x)<(y))?(x):(y)
#define max(x,y) ((x)>(y))?(x):(y)
#define minmax(x,y,z) min(max((x),(y)),(z))

// setting constants
static const float dens_min      = 0;
static const float dens_max      = 40;
static const float dens_scale    = (dens_max - dens_min);

static const float temp_min      = 0;
static const float temp_max      = 40;
static const float temp_scale    = (temp_max - temp_min);

static const float wave_min      = 350;
static const float wave_max      = 720;
static const float wave_scale    = (wave_max - wave_min);

static const float purple_max    = 1. / 7.;
static const float blue_max      = 2. / 7.;
static const float green_max     = 3. / 7.;
static const float yellow_max    = 4. / 7.;
static const float orange_max    = 5. / 7.;
static const float red_max       = 6. / 7.;
static const float change_rate   = 7.;

void renderFloats2RGBA8888(SDL_Texture* texture, int renderWidth, int renderHeight, float* dens, float* temp)
{
    uint32_t raw_pixels_size = renderWidth * renderHeight;
	uint32_t raw_pixels[raw_pixels_size];

    if(!dens && temp)
    {
        for(uint32_t i=0; i<raw_pixels_size; i++)
        {
            uint8_t a8 = 255;
            raw_pixels[i] = a8 + float2RGBA8888_switch( temp[i] );
        }
    }else if(dens && !temp)
    {
        for(uint32_t i=0; i<raw_pixels_size; i++)
        {
            float a_scale = (dens[i] - dens_min) / dens_scale;
            a_scale = minmax(0.f, a_scale, 1.f);
            uint8_t a8 = a_scale * 255.;
            raw_pixels[i] = a8 + 0xFFFFFF00;
        }
    }else if(dens && temp)
    {
        for(uint32_t i=0; i<raw_pixels_size; i++)
        {
            float a_scale = (dens[i] - dens_min) / dens_scale;
            a_scale = minmax(0.f, a_scale, 1.f);
            uint8_t a8 = a_scale * 255.;
            raw_pixels[i] = a8 + float2RGBA8888_switch( temp[i] );
        }
    }

    SDL_UpdateTexture(texture, NULL, raw_pixels, renderWidth * sizeof(uint32_t));
}

void floats2RGBA8888s_switch(size_t size, float* fs, uint32_t* rgba8888s)
{
    for(uint32_t i=0; i<size; i++){ rgba8888s[i] =  float2RGBA8888_switch( fs[i] ) + 0xff; }
}

void floats2RGBA8888s_expf(size_t size, float* fs, uint32_t* rgba8888s)
{
    for(uint32_t i=0; i<size; i++){ rgba8888s[i] = float2RGBA8888_expf( fs[i] ) + 0xff; }
}

uint32_t float2RGBA8888_switch(float t)
{
    //scale t
    float t_scaled = ((t - temp_min)/ temp_scale);
    int t_switch = (int)(t_scaled * 7);

    float xr,yg,zb;
    float relative_s_f;
    switch(t_switch)
    {
    case 0:
        xr = t_scaled * change_rate * 0.5;
        yg = 0;
        zb = t_scaled * change_rate;
    break;
    case 1:
        xr = .5 - ((t_scaled-purple_max) * change_rate) * 0.5;
        yg = 0;
        zb = 1;
    break;
    case 2:
        relative_s_f = (t_scaled-blue_max) ;
        xr = 0;
        yg = (relative_s_f * change_rate);
        zb = 1. - (relative_s_f * change_rate);
    break;
    case 3:
        relative_s_f = (t_scaled-green_max) ;
        xr = (relative_s_f * change_rate);
        yg = 1;
        zb = 0;
    break;
    case 4:
    case 5:
        relative_s_f = (t_scaled-yellow_max) ;
        xr = 1;
        yg = 1. - (relative_s_f * change_rate * 0.5);
        zb = 0;
    break;
    case 6:
        relative_s_f = (t_scaled-red_max) ;
        xr = 1;
        yg = zb = (relative_s_f * change_rate);
    break;
    default:
        xr = yg = zb = 1;
    break;
    }

    // truncate to given min max values
    xr = min(xr,1.f);
    yg = min(yg,1.f);
    zb = min(zb,1.f);

    //convert scale to byte
    uint8_t r = (uint8_t)(255 * xr);
    uint8_t g = (uint8_t)(255 * yg);
    uint8_t b = (uint8_t)(255 * zb);
    uint8_t a = 0x00;

    //RGBA8888: does not depend on endianes, because we use it as number
    // and the compiler converts numbers correctly
    uint32_t rgba8888 = (r << 24) | (g << 16) | (b << 8) | (a);
    return rgba8888;
}

uint32_t float2RGBA8888_expf(float t)
{
    // setting constants
    float t_scaled = ((t - temp_min)/ temp_scale);

    float xr,yg,zb;
    float wave     = t_scaled * wave_scale + wave_min;

    // Fit_1931: Inputs: Wavelength in nanometers
    float xt1 = (wave-442.0f)* 0.0374f; //((wave<442.0f)?0.0624f:0.0374f);
    float xt2 = (wave-599.8f)* 0.0264f; //((wave<599.8f)?0.0264f:0.0323f);
    float xt3 = (wave-501.1f)* 0.0382f; //((wave<501.1f)?0.0490f:0.0382f);
    xr   = 0.362f*expf(-0.5f*xt1*xt1)
                + 1.056f*expf(-0.5f*xt2*xt2)
                - 0.065f*expf(-0.5f*xt3*xt3);

    float yt1 = (wave-568.8f)* 0.0213f; //((wave<568.8f)?0.0213f:0.0247f);
    float yt2 = (wave-530.9f)* 0.0322f; //((wave<530.9f)?0.0613f:0.0322f);
    yg   =0.821f*expf(-0.5f*yt1*yt1)
                + 0.286f*expf(-0.5f*yt2*yt2);

    float zt1 = (wave-437.0f)* 0.0278f; //((wave<437.0f)?0.0845f:0.0278f);
    float zt2 = (wave-459.0f)* 0.0385f; //((wave<459.0f)?0.0385f:0.0725f);
    zb   = 1.217f*expf(-0.5f*zt1*zt1)
                + 0.681f*expf(-0.5f*zt2*zt2);

    // scale to given min max values
    xr = min(xr,1.f);
    yg = min(yg,1.f);
    zb = min(zb,1.f);

    //convert scale to byte
    uint8_t r = (uint8_t)(255 * xr);
    uint8_t g = (uint8_t)(255 * yg);
    uint8_t b = (uint8_t)(255 * zb);
    uint8_t a = 0x00;

    //RGBA8888: does not depend on endianes, because we use it as number
    // and the compiler converts numbers correctly
    uint32_t rgba8888 = (r << 24) | (g << 16) | (b << 8) | (a);
    return rgba8888;
}

