# Programming language interpreter
This project is an implementation of an interpreter for a simple, C-like syntax programming language with a pattern matching feature.
```
fun example(number: const int): str {
    return match(number){
        is_even : "even",           # function predicate
        < 200   : "less than 200",  # implicit comparison
        _       : "boring"          # default
    };
}
```

## Building
### Requirements
- C++20 compatible compiler (tested with g++13 and clang++15)
- Cmake 3.14+

In the repository root execute commands:
```
mkdir build
cd build
cmake ..
cmake --build .
```
Resulting executables `main` and `tests` may be found in the `build` directory.
## Running
Run interpreter with command:
```
./main /path/to/program/to/run -- <arguments for the program>
```

## Language overview
TODO

A detailed description may be found in the `docs` catalog of the repository.

## Testing
Currently tests are performed by building and running the `tests` executable. GTest is used as a testing framework.

## Implementation details and caveats
A lexer must implement the ILexer interface which lets you perform a poor-man's dependency injection into the Parser object by reference to the interface. This makes it easy to swap or adapt lexers depending on needs. Lexer class implements a lazy-evaluating lexer and CommentFilterLexer skips comment tokens produced by Lexer.

The parser is broken into ParserBase and Parser for ease of testing. Implemented parser is a recursively descending one. It produces a list of function definition objects containing tree structures ready to be executed (we're skipping the formal AST).

The Interpreter is implemented as a visitor for elements of the program tree. For ease of implementation, most binary operators are defined as lambdas wrapped in `std::function`s inside maps in the OperatorResolver class and invoked through `std::visit` of type-erased `std::variant` variables. This makes execution a lot slower than it could be but greatly simplifies and reduces amount of required code. File handles inside programs are currently implemented in a very hacky way and demand a refactor. 

Error handling is done through exceptions. A good example of how to handle them is located in the interpreter main function.

When starting the project, I optimistically assumed that supporting national characters would be as easy as templating classes on character type and instantiating them as needed. Unfortunately C++ does not currently support compile time string literal conversion during template instantiation which makes it impossible to use without massive code duplication. I decided to leave the templates in just in case templated string literals appear in C++ within my lifetime.
## To dos
- exponent parsing in floating point literals
- adding an array type
- improving file handling
- argument parsing for the interpreter