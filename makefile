# Variáveis para o compilador e ferramentas
LEX = flex
YACC = yacc
CC = gcc
# O -Wno-unused-function serve para o GCC não chatear com avisos de funções do Lex/Yacc que não usamos
CFLAGS = -Wall -Wno-unused-function 

# Nome do executável final
TARGET = jucompiler

# Regra principal
all: $(TARGET)

# Compilar o executável final juntando o Yacc, o Lex e a Semântica
$(TARGET): y.tab.c lex.yy.c semantics.c
	$(CC) $(CFLAGS) y.tab.c lex.yy.c semantics.c -o $(TARGET)

# Correr o Yacc (gera y.tab.c e y.tab.h)
y.tab.c y.tab.h: jucompiler.y
	$(YACC) -d jucompiler.y

# Correr o Lex (gera lex.yy.c e depende do y.tab.h do Yacc)
lex.yy.c: jucompiler.l y.tab.h
	$(LEX) jucompiler.l

# Limpar todos os ficheiros gerados
clean:
	rm -f lex.yy.c y.tab.c y.tab.h $(TARGET) jucompiler.zip

# Submissão Meta 3: junta todos os ficheiros no zip
zip:
	zip jucompiler.zip jucompiler.l jucompiler.y semantics.c semantics.h ast.h

.PHONY: all clean zip