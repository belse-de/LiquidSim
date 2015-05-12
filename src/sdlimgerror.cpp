#include <SDL_image.h>

#include "sdlimgerror.hpp"

#include "main.hpp"

SdlImgError::SdlImgError():SdlImgError(""){}
SdlImgError::SdlImgError(const char* what_arg):SdlImgError(std::string(what_arg)){}
SdlImgError::SdlImgError(const std::string& what_arg):
    std::runtime_error( what_arg + newline + std::string{"SDL_Image Error: "} + IMG_GetError()){}

