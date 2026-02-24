# Variables for the compiler and tools
LEX = lex
CC = cc
CFLAGS = -Wall

# Name of the final executable
TARGET = lexer

# The first rule is the default rule (run by typing 'make')
all: $(TARGET)

# Rule to link the object file and create the executable
$(TARGET): lex.yy.c
	$(CC) $(CFLAGS) lex.yy.c -o $(TARGET) -ll

# Rule to run Lex on the .l file
lex.yy.c: lexer.l
	$(LEX) lexer.l

# Rule to clean up generated files
clean:
	rm -f lex.yy.c $(TARGET)

# Phony targets ensure 'make' doesn't get confused if files named 'clean' or 'all' exist
.PHONY: all clean