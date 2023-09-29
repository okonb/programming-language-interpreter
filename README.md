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
*The language is currently not advanced or convenient enough to build anything non-trivial with it (no arrays, limited I/O etc.).*

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

A program consists of function declarations. Functions take in typed arguments and return values. By default, `main` is executed first.
```
fun add(a: const int, b: const int): int {
    return a + b;
}

fun main(): int {
    one: const int = 1;
    two: const int = 2;
    return add(one, two);
}
```
Non-const variable arguments are taken by reference which allows functions to modify its parameters.
```
fun add_one(number: int): void {
    number = number + 1;
}
fun main(): int {
    two: int = 1;
    add_one(two);
    return two;
}
``` 
Flow control is done through familiar, C-like constructs. Typing is strong, e.g. string concatenation operator `|` only accepts two strings, which necessitates use of conversion functions. 
```
fun flow(): void {
    num: const int = 4;
    if(num <= 2){
        return;
    }
    else if(num == 3 or num == 4){
        num = num + 1;
    }
    temp: int = num;
    while(temp > 0){
        print("Number is " | to_str_int(temp) | " \n");
        temp = temp - 1;
    }
}
```
The special ingredient is the `match` operation, which can be also an expression. Patterns can be composed of values, function identifiers or ad-hoc relation predicates.
```
fun is_even(n: const int): bool { return n % 2 == 0; }
fun example(num: const int): str {
    return match(num, to_str_int(num)){
        _,          "2" :   "two",
        is_even,    _   :   "even",
        < 0,        _   :   "negative",
        _,          _   :   "whatever"
    };
}
```
Program arguments may be accesed through special functions. File access is done through a simple API, only string operations are supported.
```
fun main(): int {
    args: cont int = arguments_number();
    if(args < 1){
        print("Provide filename as the first argument.\n");
        return -1;
    }

    to_print_file: file = open_file(argument(0));
    if(bad_file(to_print_file)){
        print("File path invalid.\n");
        return -1;
    }

    line: str = read_line(to_print_file);
    while(line != EOF_MARKER){
        print(line);
        line = read_line(to_print_file);
    }
    close_file(to_print_file);
    return 0;
}
```


A more detailed description may be found in the [`docs`](docs/README.md) catalog of the repository.

## Testing
Currently tests are performed by building and running the `tests` executable. GTest is used as a testing framework.

## Implementation details and caveats
A lexer must implement the ILexer interface which lets you perform a poor-man's dependency injection into the Parser object by reference to the interface. This makes it easy to swap or adapt lexers depending on needs. Lexer class implements a lazy-evaluating lexer and CommentFilterLexer skips comment tokens produced by Lexer.

The parser is broken into ParserBase and Parser for ease of testing. Implemented parser is a recursively descending one. It produces a list of function definition objects containing tree structures ready to be executed (we're skipping the formal AST).

The Interpreter is implemented as a visitor for elements of the program tree. For ease of implementation, most binary operators are defined as lambdas wrapped in `std::function`s inside maps in the OperatorResolver class and invoked through `std::visit` of type-erased `std::variant` variables. This makes execution a lot slower than it could be but greatly simplifies and reduces amount of required code. File handles inside programs are currently implemented in a very hacky way and demand a refactor. 

The standard `std::map` in maps which can be instantiated at compile time was substituted for a custom `light_map` - a constexpr capable, simple map type with linear search time complexity - perfectly suitable for small maps and offering greater optimization oppotrunities.

Error handling is done through exceptions. A good example of how to handle them is located in the interpreter main function.

When starting the project, I optimistically assumed that supporting national characters would be as easy as templating classes on character type and instantiating them as needed. Unfortunately C++ does not currently support compile time string literal conversion during template instantiation which makes it impossible to use without massive code duplication. I decided to leave the templates in just in case templated string literals appear in C++ within my lifetime.
## To dos
- exponent parsing in floating point literals
- adding an array type
- improving file handling
- argument parsing for the interpreter