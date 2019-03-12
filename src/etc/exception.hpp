#ifndef EXCEPTION
#define EXCEPTION

#include <iostream>

class Exception {
private:
    std::string msg;
public:
    Exception(std::string msg) {
        this->msg = msg;
    }

    void print() const {
        std::cout << this->msg << std::endl;
    }
};

#endif