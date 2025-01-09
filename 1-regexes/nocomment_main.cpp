#include "nocomment.hpp"

#include <iostream>
#include <cassert>

// This is the yylval variable that the nocomment.hpp header file refers to.
TokenValue yylval;

int main () { 

  while (1) {

    // Lex the next character in the input stream.
    TokenType type = (TokenType) yylex();
    
    if (type == Eof) {
      
      break;
      
    } else if (type == Other) {

      std::cout << yylval.character;

    } else {
      
      assert(0); // Error out!
      return 1;
      
    }
  }

  std::cout << "Number of comments and attributes removed: 0.\n";
    
  return 0;
}
