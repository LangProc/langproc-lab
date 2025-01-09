CPPFLAGS += -W -Wall -g 

# This avoids error: 'fileno' was not declared in this scope
CPPFLAGS += -std=c++0x

# Avoid warnings about yyunput not being used
CPPFLAGS += -Wno-unused-function

all: nocomment

nocomment_lexer.yy.cpp: nocomment_lexer.flex
	flex -o nocomment_lexer.yy.cpp  nocomment_lexer.flex

nocomment: nocomment_lexer.yy.o nocomment_main.o
	g++ $(CPPFLAGS) -o nocomment nocomment_lexer.yy.o nocomment_main.o

clean:
	-rm -f nocomment nocomment.exe nocomment_main.o nocomment_lexer.yy.o
	-rm -f *.yy.cpp
	-rm -f test/out/*
