Lab 3: Code generation
======================

You will interpret a simple imperative AST, then generate equivalent code for a simple infinite-register ISA.

Input language
--------------

The input language provides a basic set of constructs, which support both statements and expressions. A simple example which prints the numbers from 10 down to 1 is:

    Seq [
      Assign : x [ 10 ]
      While [ LessThan [ 0 x ]
        Seq [
          Output [ x ]
          Assign : x [ Sub [ x 1 ] ]
        ]
      ]
    ]

The equivalent C would be:

    {
      x = 0;
      while(0 < x)
      {
        printf("%d\n",x);
        x = x - 1;
      }
    }

The language has the feature that variables do not need to be declared, so the variable `x` is declared when it is first assigned.

As a consequence of not requiring declarations, this language also only has a single global scope. So the variable `x` always refers to the same variable, no matter where it is used. This behaviour is chosen to make the problem simpler for the lab; in C you'll need to manage different scopes in some way.


Language Constructs
-------------------

Input/Output is built into the language, with four types of input-output modelled on the IO of a command-line process:

- Parameters : a vector of integers passed to the program, equivalent to command line parameters (`argv`).

- Input : a stream of integers, equivalent to reading integers from `std::cin`.

- Output : a stream of integers, equivalent to writing integers to `std::cout`.

- Return-value : a single integer return value, equivalent to the return-value of `main`, or the parameter passed to `exit`.

Input programs can contain the following constructs:

- Number: `decimalNumber`
  
  A number is a decimal number consisting of decimal digits and an optional sign (`-?[0-9]+`). When evaluated it returns the given number.

- Variable: `variable`

  A variable matches the regex `[a-z][a-z0-9]*`. It returns the value of the variable, which must already have been given a value elsewhere in the program. All variables appear at global scope, so a variable `x` will always refer to the same variable, no matter where it appears.
  
- Input: `Input`

  Reads an integer from the input stream of numbers and returns it. Note that it is not a variable because it is not lower-case.
  
- Param : `Param : I`

  Return the input parameter with index `I`.
  
- Output: `Output [ X ]`

  Evaluate `X`, then send the result to output. The return value is the value of `X`.
  
- LessThan : `LessThan [ A B ]`

  Evaluate `A` then `B`; return non-zero if `A < B`.

- Add : `Add [ A B ]`

  Evaluate `A` then `B`; return `A + B`.
  
- Sub : `Sub [ A B ]`

  Evaluate `A` then `B`; return `A - B`.
  
- Assign : `Assign : N [ V ]`

  Evaluate expression `V`, and assign it to variable `N`. The return value is the value of `V`.

- Seq : `Seq [ X Y ... ]`

  Executes `X`, then `Y`, and so on. Return value is that of the last part executed. There is always at least one operand, i.e. `Seq [ ]` is not a valid construct.
  
- If : `If [ C X Y ]`

  If evaluating `C` returns non-zero then execute and return `X`, otherwise execute and return `Y`.

- While : `While [ C X ]`
  
  As long as `C` evaluates to non-zero, execute `X`. Return value is zero (as `C` will be the last part evaluated).
  
Notice that the language has the property that _all_ constructs return a value, so everything can be considered to be an expression.  However, any expression can also have side-effects, i.e. it could modify a variable.
 
There are a number of input programs in `test/programs/*/in.code.txt`, which perform some simple calculations. If you mentally translate them to C, the meaning should be clear.

Data representation
-------------------

The language is designed to have an extremely lightweight and direct mapping to an AST, which allows for a very simple lexer and parser. The entire parser is contained in [src/ast_parse.cpp](src/ast_parse.cpp), which contains:

- `Tokenise`: turns an input character stream into a sequence of tokens, simply by splitting on whitespace, and
 
- `Parse`: a recursive-descent parser that implements a complete parser in ~40 lines of code.

This brevity is achieved by using a generic AST, which can represent any tree, but does not have classes for each node type. Instead, each tree node has:

- `type`: what type of node it is, or the name/value,

- `value `: an optional string representing a value at this node, and

- `branches`: a vector of zero or more sub-trees.

We'll use the following representation:

- Tree of type `xyz`, with no value or branches.
    ````
    xyz
    ````

- Tree of type `x` with value `y`, and no branches.
    ````
    x : y
    ````

- Tree of type `foo`, with no value and two sub-branches:
    ````
    foo [
        a : 5
        b : 7
    ]
    ````

- Tree of type `foo`, with a value `baz` and one sub-branch. The sub-branch has type `a` and has neither value nor sub-branches.
    ````
    foo : baz [
        a
    ]
    ````

This representation is similar to the approach taken in JSON or XML, where there is a general-purpose hierarchical data-structure, then meaning is imposed onto it later on. In this case, there are many ASTs which are syntactically correct, but do not match the grammar of the language constructs.  A disadvantage of this approach is that we don't discover that an AST is malformed at parse-time -- it only becomes apparent when we try to work with the tree.
  
  
Task 1: Interpretation
======================

There is a function called `Interpret` in [src/ast_interpret.cpp](src/ast_interpret.cpp) which provides the skeleton of an interpreter for the language. Complete the implementation, based on the semantics given earlier.

The test script `./test_interpreter.sh` applies the interpreter to a number of different input programs in `test/programs`, and checks that the outputs and results are correct. (By default, one of the test-cases passes already).


Task 2: Code generation
=======================

The file `src/vm.cpp` implements a simple virtual machine which uses a MIPS-like ISA, but with an infinite register file.

The supported assembly instructions are:

- `:label`: Establishes a label (jump target)

- `const dstReg immVal`: Loads the decimal immediate value into the destination register

- `input dstReg`: Read a value from input, and put it into the destination register.

- `param dstReg immVal`: Place the input parameter with index immVal into destination register.

- `output srcReg`: Write the value in source register into the output stream.

- `add dstReg srcRegA srcRegB`: Add the two source registers and write to destination.

- `sub dstReg srcRegA srcRegB`: Subtract the two source registers and write to destination.
        
- `lt dstReg srcRegA srcRegB`: If srcRegA < srcRegB, then dstReg=1, otherwise dstReg=0.

- `beq srcRegA srcRegB label`: If srcRegA == srcRegB, then jump to label.

- `bne srcRegA srcRegB label`: If srcRegA != srcRegB, then jump to label.

- `halt srcReg`: Halt the program and return value in srcReg.

(NB: A limitation of the current VM is that labels and register names cannot be larger than 63 characters.)

The VM can be compiled with
````
make bin/vm
````
and launched as follows:
````
bin/vm pathToAssembly Param0 Param1 ...
````
The input stream comes from `stdin`, and output goes to `stdout`.

An example program that copies input to output while adding 1 each time is:
````
const zero 0
const one 1
:top
input i
beq i zero bottom
add i i one
output i
beq one one top
:bottom
halt zero
````
The program stops copying when an input of 0 is encountered.

The program `bin/compiler` can be built using `make bin/compiler`, and is a compiler from the input language to the VM assembly language. It relies on a function called `Compile` in `src/ast_compile.cpp`, which is only partially complete. Complete the function so that it can compile all code constructs.

The test script `./test_compiler.sh` applies the compiler to the existing input programs in `test/programs` to get the assembly, then passes it through the virtual machine.  The interpreted program and the compiled program should provide the same output.


Submission
----------
Passing all given tests is worth 50% of the marks, with the remaining marks coming from unseen test-cases. As before: commit, test, push to GitHub, hash to Blackboard.
