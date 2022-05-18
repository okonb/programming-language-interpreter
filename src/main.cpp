#include <iostream>
#include <sstream>
#include "Parser.hpp"
#include "Lexer.hpp"
#include "CommentFilterLexer.hpp"



int main(){
    std::stringstream s;
    s.unsetf(std::ios::skipws);
    s << R"(fun fibonacci(n: const int): int {
        return match(n){
            0:  0,
            1:  1,
            _:  fibonacci(n - 1) + fibonacci(n - 2)
        };
    }

    fun fizzbuzz(n: const int): void {
        temp: int = 1;
        while(temp <= n){
            match(temp % 3, temp % 5){
                0, 0:   print("fizzbuzz"),
                0, _:   print("fizz"),
                _, 0:   print("buzz"),
                _, _:   print(temp)
            }
            temp = temp + 1;
        }
    }

    fun progressive_tax(salary: const float): float {   # not accurate
        tax_rate: float = match(salary){
            < 1000.0:   0.1,
            < 2500.0:   0.3,
            _:          0.5
        };
        return salary * tax_rate;
    }

    fun is_even(n: const int): int {
        return match(n % 2){
            0:  1,
            1:  0
        };
    }

    fun is_even2(n: const int): int {
        return match(n){
            is_even:    1,
            _:          0
        };
    }


    fun print_file(filename: const str): void {
        handle: file = open_file(filename);
        if(bad_file(handle)){
            return;
        }
        else{
            line: str = read_line(handle);
            while(line != EOF_MARKER){
                print(line);
                line = read_line(handle);
            }
            close_file(handle);
        }
    }

    fun main(): void{
        if(arguments_number() < 1){
            exit(1);
        }
        print_file(argument(1));

        number: int = 6;
        print(to_str(number) | ". Fibonacci number is " | to_str(fibonacci(number)));

        print("2 + 3 * 2 = " | to_str(2+3*2) | " == 8?");

        if(number < 12 and is_even(number) or not is_even(number)){
            print("Logical precedence works ?");
        }

        print("Tax for 1500zl = " | tax_rate(1500.0));
    }

    )";
    Lexer lex(s);
    CommentFilterLexer<char> filered_lex{lex};
    Parser parser(filered_lex);
    try{
        parser.parse().print_self(std::cout);
    }
    catch(UnexpectedTokenException<char> &e){
        std::cerr << e.what();
    }
    catch(SyntaxErrorException<char> &e){
        std::cerr << e.what();
    }
    catch(std::exception &e){
        std::cerr << e.what();
    }
    catch(...){
        std::cerr << "unknown exception\n";
    }


    return 0;
}