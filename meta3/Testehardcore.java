class Testehardcore {

    // ========================================================================
    // 1. TESTES DE OVERFLOW E UNDERFLOW
    // ========================================================================
    public static void testBounds() {
        int a;
        double b;

        // Overflows de Inteiros (Passam o limite de 2147483647)
        a = 2147483648; 
        a = 999999999999999999999999999;

        // Overflows de Doubles (Passam o limite de ~1.79e308)
        b = 1.8e308;
        b = 999999999999999999999999999E+222;

        // Underflows de Doubles (Arredondam para 0.0 por falta de precisão)
        b = 0.5e-4965;
        b = 0.00000001e999999999999999999999999999999999999999999;
    }
// ========================================================================
    // 2. TESTES SemErrStatExpr: IF e WHILE
    // ========================================================================
    public static void testControlStructures() {
        int x;
        boolean y;

        // Erro na condição (int + boolean). O tipo fica 'undef'.
        // O 'if' NÃO deve reportar erro com 'undef', mas DEVE analisar o bloco!
        if (1 + true) {
            x = 2.5; // O compilador TEM de apanhar este erro (Assign double to int)
        } else {
            y = 10;  // O compilador TEM de apanhar este erro (Assign int to boolean)
        }

        // Erro na condição (double ^ double).
        while (2.0 ^ 3.0) {
            // Bloco continua a ser avaliado
            System.out.print(true + false); // Erro no operador +
        }
    }

    // ========================================================================
    // 3. TESTES SemErrStatExpr: PRINT, RETURN e ASSIGNMENTS
    // ========================================================================
    public static double testPrintAndReturn() {
        int z;
        
        // Print com expressão inválida. Erro no &&, mas o Print não deve berrar do 'undef'
        System.out.print( (z = 5) && 10 );

        // Return com expressão inválida. Erro no >>, return recebe 'undef' e deve calar-se.
        return 1.0 >> 2; 
    }

    // ========================================================================
    // 4. TESTES SemErrStatExpr: CASCATA E RESOLUÇÃO DE MÉTODOS
    // ========================================================================
    public static void testCascadingUndef() {
        int a;
        boolean b;
        double c;

        // Erro na atribuição (b = 5), gera 'boolean' ou 'undef'. 
        // O operador '+' tenta somar 10 com o resultado.
        a = 10 + (b = 5); 

        // Erro na soma (1 + true), gera 'undef'. 
        // O operador '*' vai tentar multiplicar 'undef' por 5.0.
        c = (1 + true) * 5.0; 
        
        // Chamada de método com argumento inválido.
        // O argumento é (1 + false) -> erro de operador, tipo 'undef'.
        // O compilador deve procurar dummy(undef) e falhar graciosamente.
        dummy(1 + false); 
    }

    // Método auxiliar para o teste de chamada
    public static void dummy(int arg) {
    }
}