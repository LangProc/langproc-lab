Lab 1: Lexers and regular expressions
=====================================

This lab is about lexers and regular expressions. It is intended to give you enough working knowledge and experience to be able to design and implement the C lexer for your compiler.

Specification
-------------

Write a tool using Flex that reads a stream of ASCII characters, and processes it, character by character, by applying the rules below. In what follows, a _line_ is defined as any maximal sub-sequence of the stream whose last character is a `newline` and which does not contain any other `newline` characters. You may assume that the final character in the whole stream is a `newline`.

- The `//` sequence indicates the beginning of a _comment_. If `//` is encountered, remove it and the rest of the line.

- The `\` character indicates the beginning of an _escaped identifier_. If `\` is encountered, ignore it and move to the next space or `newline` in the stream.

- The `(*` sequence indicates the beginning of an _attribute_. If `(*` is encountered, remove it, and then remove all characters up to and including the end of the attribute. The end of the attribute is the next occurrence of `*)` that is _not_ part of an escaped identifier and is _not_ part of a comment. The end of the attribute may either be on the same line or on a subsequent line. If there is no end of the attribute, then this rule does not apply.

- If any other character is encountered, ignore it and move to the next character in the stream.

Finally, the tool should add a line to the end of its output that says `Number of comments and attributes removed: n.` where `n` is the number of comments and attributes that have been removed.

As an example, if the input stream looks like this:

```
module foo ()
  wire \hello;
  wire \//go away;
  //go away
  wire (* hello \*) world *) there;
endmodule

```

then the output should look like this:

```
module foo ()
  wire \hello;
  wire \//go away;
    wire  there;
endmodule
Number of comments and attributes removed: 2.
```

The program should be built by running the command `make nocomment`.

There is already a skeleton program setup, including:

- Flex source: [nocomment_lexer.flex](nocomment_lexer.flex)

- C++ driver program: [nocomment_main.cpp](nocomment_main.cpp)

- Makefile: [makefile](makefile)

The skeleton setup contains a number of suggestions where
things need to be changed and edited, but these are not exhaustive.

There is also a test-bench included, which is a partial set of test vectors for the program together with a script for running them. Passing these tests is equivalent to achieving 50% in the final assessment, with unseen tests covering the remaining 50%.

The components of the test are:

- [`test/in`](test/in): A set of input test files of increasing complexity.

- [`test/ref`](test/ref): The "golden" output for the given input files, which your program should match. There is one output for each input.

- [`test_lexer.sh`](test_lexer.sh): A script that runs the tests. It will build your program, then apply it to each input in turn to produce a file in `test/out`. It will then use [diff](https://en.wikipedia.org/wiki/Diff_utility) to work out whether the output matches the reference output. You can run this script via the command: `./test_lexer.sh`.
