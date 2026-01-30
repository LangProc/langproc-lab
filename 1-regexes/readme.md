Lab 1: Lexers and regular expressions
=====================================

This lab is about lexers and regular expressions. It is intended to give you enough working knowledge and experience to be able to design and implement the C lexer for your compiler.

Changelog
---------
- **26-Jan-2026#3:** For 267 cohort - Reworded `\` rule to only ignore the following character, instead of up to the next whitespace
- **26-Jan-2026#2:** For 267 cohort - Reworded `//` rule to not remove final newline of a line (as this can lead to awkward behaviours)
- **26-Jan-2026:** Added clarification that the "Number of comments" message does not start a new line if the preceding `\n` was deleted when a comment was removed.
- **23-Jan-2025:** Added clarification that comments inside attributes should not count towards the number of comments removed.


Specification
-------------

Write a tool using [Flex](https://www.cs.virginia.edu/~cr4bd/flex-manual/index.html) that reads a stream of ASCII characters, and processes it, character by character, by applying the rules below. In what follows, a _line_ is defined as any maximal sub-sequence of the stream whose last character is a `newline` and which does not contain any other `newline` characters. You may assume that the final character in the whole stream is a `newline`.

- The `//` sequence indicates the beginning of a _comment_. If `//` is encountered, remove it and all non-newline characters contained in the rest of the line.

- The `\` character indicates the beginning of an _escaped identifier_. If `\` is encountered, ignore the following character and move to the next next character in the stream, e.g. `\abc` -> ignore `a` and continue lexing from `b`

- The `(*` sequence indicates the beginning of an _attribute_. If `(*` is encountered, remove it, and then remove all characters up to and including the end of the attribute. The end of the attribute is the next occurrence of `*)` that is _not_ part of an escaped identifier and is _not_ part of a comment. The end of the attribute may either be on the same line or on a subsequent line. If there is no end of the attribute, then this rule does not apply.

- If any other character is encountered, ignore it and move to the next character in the stream.

Finally, the tool should add a line to the end of its output that says `Number of comments and attributes removed: n.` where `n` is the number of comments and attributes that have been removed.

_[**Edit 23-Jan-2025:** If a comment is nested inside an attribute, then it is automatically removed when the attribute is removed, and does not need removing explicitly; therefore, it doesn't count towards the number of comments removed. The [result](test/ref/09.stdout.txt) of [test 9](test/in/09.txt) clarifies this behaviour.]_

_[**Edit 26-Jan-2026:** If you remove a comment from the last line of the input, then the "Number of comments" message should appear on that line, not on a new line by itself.]_

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

You may find the [Flex manual](https://www.cs.virginia.edu/~cr4bd/flex-manual/index.html) helpful, particularly the [section about the syntax of patterns](https://www.cs.virginia.edu/~cr4bd/flex-manual/Patterns.html#Patterns).
