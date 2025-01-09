#ifndef nocomment_hpp
#define nocomment_hpp

#include <string>

// The types of tokens that our lexer will identify.
enum TokenType 
{
  Eof,          // end of file
  Other
};

// Some tokens can be associated with a value.
union TokenValue
{
  char character;
};

// We will invoke our lexer by repeatedly calling yylex(). That
// function will return a TokenType (which is an int).
extern int yylex();

// Each time our lexer identifies a token, it stores its value
// (if relevant for the token type) into yylval.
extern TokenValue yylval;


#endif
