class a{
    
    // Variável global para testar a secção "Essential" (field declarations)
    public static int globalVar;

    public static void main(String[] args) {
        int len;
        int firstArg;
        int unaryTest;

        // --- TESTE 1: Unary Plus/Minus, Global Vars e Assignments (Essential) ---
        unaryTest = +10;
        unaryTest = -unaryTest;
        globalVar = unaryTest; // globalVar fica a -10
        
        System.out.print("Teste Unario (deve ser -10): ");
        System.out.print(globalVar);
        System.out.print("\n");

        // --- TESTE 2: Tamanho dos Argumentos da Linha de Comandos (Essential) ---
        len = args.length;
        System.out.print("Numero de argumentos passados: ");
        System.out.print(len);
        System.out.print("\n");

        if (len > 0) {
            // --- TESTE 3: Leitura de Argumentos ---
            firstArg = Integer.parseInt(args[0]);
            System.out.print("O primeiro argumento e: ");
            System.out.print(firstArg);
            System.out.print("\n");

            // --- TESTE 4: Forward Declarations & Method Calls (Methods) ---
            // Chamamos 'avaliaArgumento' AQUI, mas ele só é declarado mais abaixo!
            avaliaArgumento(firstArg); 
        } else {
            System.out.print("Para testar em pleno, corre o executavel passando um argumento!\n");
            System.out.print("Exemplo: lli programa.ll 42\n");
        }
    }

    // Método declarado DEPOIS de ser chamado pelo main (Testa Forward Declarations)
    public static void avaliaArgumento(int val) {
        System.out.print("O triplo do argumento (calculado via forward declaration) e: ");
        // Mais uma chamada antecipada
        System.out.print(calculaTriplo(val));
        System.out.print("\n");
    }

    // Método final da cadeia
    public static int calculaTriplo(int n) {
        return n * 3;
    }
}