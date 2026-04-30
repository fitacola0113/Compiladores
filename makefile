LEX = flex
YACC = yacc
CC = gcc
CFLAGS = -Wall -Wno-unused-function
 
TARGET = jucompiler
 
all: $(TARGET)
 
$(TARGET): y.tab.c lex.yy.c semantics.c codegen.c
	$(CC) $(CFLAGS) y.tab.c lex.yy.c semantics.c codegen.c -o $(TARGET)
 
y.tab.c y.tab.h: jucompiler.y
	$(YACC) -d jucompiler.y
 
lex.yy.c: jucompiler.l y.tab.h
	$(LEX) jucompiler.l
 
clean:
	rm -f lex.yy.c y.tab.c y.tab.h $(TARGET) jucompiler.zip
 
zip:
	zip jucompiler.zip jucompiler.l jucompiler.y semantics.c semantics.h ast.h codegen.c codegen.h
 
.PHONY: all clean zip
 
