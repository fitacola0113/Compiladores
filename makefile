# Variáveis para o compilador e ferramentas
LEX = lex
CC = gcc
CFLAGS = -Wall

# Nome do executável final exigido pelo enunciado 
TARGET = jucompiler

# O ficheiro de entrada deve chamar-se jucompiler.l 
# Nota: Alguns anos pedem .l, outros .1. Use o que o Mooshak exigir.
SOURCE = jucompiler.l

# Regra principal
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): lex.yy.c
	$(CC) $(CFLAGS) lex.yy.c -o $(TARGET)

# Regra para correr o Lex
lex.yy.c: $(SOURCE)
	$(LEX) $(SOURCE)

# Regra para limpar ficheiros gerados
clean:
	rm -f lex.yy.c $(TARGET)

# Submissão: cria o ficheiro .zip para a Meta 1 
zip:
	zip jucompiler.zip $(SOURCE)

.PHONY: all clean zip