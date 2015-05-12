#include <SDL.h>

#include "sdlerror.hpp"

#include "main.hpp"

SdlError::SdlError():SdlError(""){}

SdlError::SdlError(const char* what_arg):SdlError(std::string{what_arg}){}

SdlError::SdlError(const std::string& what_arg):
    std::runtime_error( what_arg + newline + std::string{"SDL Error: "} + SDL_GetError()){}
