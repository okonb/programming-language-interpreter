#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.hpp"

std::fstream str("./Makefile");

int main(){
    std::stringstream str2;
    str2 << "   233";
    std::cout << str2.str();
    return 0;
}