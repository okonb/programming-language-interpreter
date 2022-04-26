#include <iostream>
#include <sstream>
#include "Lexer.hpp"

int main(){
    std::stringstream str;
    str << "   233";
    Lexer lex(str);
    lex.get_next_token();
    lex.get_next_token();
    lex.get_next_token();
    return 0;
}