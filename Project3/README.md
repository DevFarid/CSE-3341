– Farid Kamizi
- Will be submitting: 
    - memory.c and memory.h: This is how we manage the program's memory mangement. Declaring and Changing values of variables. This many also contains some runtime exception error handling.

    - scanner.c and scanner.h: This is how we read and tokenize things. We also use it for reading `.data` files of our programs via `IN()`.
    
    - parser.c and parser.h: This is how we build our tree from reading the tokens.
    
    - semantic.c and semantic.h: This is how we do semantic checking (Compiler errors.)

    - execute.c and execute.h: This is how we execute the program, reading the syntax tree as we execute.

    - printer.c and printer.h: Useful for debugging the syntax tree, but we are not utilizing anything from this file for this project.

- No special features or comments were added.

- The overall design of the interpreter is looking clean. We just now have to add custom function calls. The variable management is done through C, we keep it simple. `realloc` is called each time we declare something new, but the reallocation just readjusts our memory space, we still have to manually store values at desired memory spaces.

- Did not find any bugs IMO, I first started to connect the simple terminals and non-terminals, and then I began solving test cases one by one.
