#ifndef SDLIMGERROR_H
#define SDLIMGERROR_H

#include <stdexcept>


class SdlImgError : public std::runtime_error
{
    public:
        /** Default constructor */
        SdlImgError();
        SdlImgError(const std::string& what_arg);
        SdlImgError(const char* what_arg);

        /** Default destructor */
        virtual ~SdlImgError() = default;
    protected:
    private:
};

#endif // SDLIMGERROR_H
