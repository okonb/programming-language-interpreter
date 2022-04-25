#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.hpp"

std::fstream str("./Makefile");

int main(){
    std::stringstream str2;
    str2 << "test";
    Lexer lex(str);
    Lexer lex2(str2);
    return 0;
}