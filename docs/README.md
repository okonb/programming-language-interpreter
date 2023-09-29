## Language description
A program comprises of function definitions, one of which is executed at startup (`main` by default).

### Types
Types may be declared `const` which means their value may not change.
- `int` - 64-bit signed integer type
- `float` - IEEE 754 double-precision type
- `str` - text string
- `file` - file descriptor
- `bool` - boolean
- `void` - type used to mark functions as not value returning

### Operators
Supported operators for given types are:
- numeric:
  - arithmetic `+ - * /`
  - comparative `< > <= >= == !=`
- integer:
  - modulo `%`
- string:
  - concatenation `|`
  - comparison `== !=`
- boolean:
  - logical `not`, `and`, `or`

### Variables
Variables are statically and strongly typed and must have value at all times. The first assignment to the variable must take place on its declaration.
```
variable_name: type = value;
```
A variable is visible within a function call context and only inside the code block it was declared in. Variables declared in inner blocks shadow over ones declared in outer ones.

The assignment `var1 = var2;` means copying contents of `var2` and assigning it to `var1`.

### Functions
Functions are defined as such:
```
fun name(parameter1: type, parameter2: type): return_type {
    # do stuff...
}
```
Arguments are passed as references (if possible). If a local copy is needed it may be obtained through copy on assignment.

### Control flow
Constructs analogous to C ones:
- while loop `while(condition) {instructions;}`
- if instructions:
  - `if(condition){instructions;}`
  - `if(condition){instructions;} else{other_instructions;}`
  - `if(condition){instructions;} else if(condition2){other_instructions;}`
  - `if(condition){instructions;} else if(condition2){other_instructions;} else...` etc.
- `match` instruction/expression (described below)

Conditions must evaluate to boolean values.

### Match
```
match(expression1, expression2, ...){
    pattern1:   expression,
    pattern2:   expression,
    (...)
    _:          default_expression
}
```
Patterns are composed of special expressions and have as many elements as there are match arguments. A pattern element may be an expression, a single-argument function identifier or a single-sided comparison expression. An element is evaluated into a value and the relation of its and metch argument's types determines the acceptance criterium:
- element value type same as match argument type -> accepted if values are equal
- element value is boolean and argument type is different -> accepted if element value is true
- different combination of types -> no match
  
Underscore `_` indicates that we don't care about a specific match argument in a pattern.

Match statement is also an expression. If no pattern matches arguments when assigning match expression to a variable an error occurs.

### API
Program argument handling:
- `arguments_number(): int` - return number of arguments given to the program
- `argument(index: int): str` - returns an argument as a string

Conversion functions:
- `to_str_int(num: const int): str`
- `to_str_float(num: const float): str`
- `to_int_float(num: const float): int`
- `to_float_int(num: const int): float`

Input/output:
- `print(string: const str): void` - print `string` to the output stream
- `open_file(filename: const str): file`
- `close_file(file_descriptor: file): void`
- `bad_file(file_descriptor: file): bool` - returns true if file is opened correctly and can be read from
- `read_line(file_descriptor: file): str` - reads a line from file


### Formal syntax definition
Formal specification of the language in EBNF/regex can be found in the [`language_specification`](language_specification/) catalog.
