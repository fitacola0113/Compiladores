class AssignBlocks {
    public static int main(String[] args) {
        int a, b, c;
        // Associatividade à direita correta: a = (b = (c = 0))
        a = b = c = 0;
        
        // Em Juc Meta 2 isto tem de gerar árvore válida: Assign(Assign(a, b), 0)
        
        
        // Blocos vazios aninhados (A AST não deve imprimir nenhum nó Block para isto!)
        { { { ; ; } } }
        
        return 0;
    }
}