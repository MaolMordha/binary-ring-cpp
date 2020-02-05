#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <stdexcept>
#include <string>

class Exception: public std::exception {
    public:
        Exception(const std::string & message);
        Exception(std::string && message);

        const char * what() const noexcept override;
    
    private:
        std::string message_;
};

#endif /* EXCEPTION_HPP_ */
