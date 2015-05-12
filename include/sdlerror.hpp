#ifndef SDLERROR_HPP
#define SDLERROR_HPP

#include <stdexcept>

class SdlError : public std::runtime_error
{
    public:
        /** Default constructor */
        SdlError();
        SdlError(const std::string& what_arg);
        SdlError(const char* what_arg);

        /** Default destructor */
        virtual ~SdlError() = default;
    protected:
    private:
};

#endif // SDLERROR_HPP
