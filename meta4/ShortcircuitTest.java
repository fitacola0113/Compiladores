class ShortcircuitTest {
    
    public static int sideEffects;

    // Esta função NUNCA deve ser executada se o short-circuit funcionar bem
    public static boolean badFunction() {
        sideEffects = sideEffects + 1;
        System.out.print("ERRO FATAL: Avaliou o lado direito!\n");
        return true;
    }

    public static void main(String[] args) {
        int i;
        boolean a;
        
        sideEffects = 0;
        i = 0;

        // --- TESTE 1: STACK OVERFLOW + SHORT-CIRCUIT BÁSICO ---
        // Vamos forçar o compilador a avaliar isto 10 mil vezes.
        // Se houver um alloca() perdido no LLVM IR, o programa estoira por falta de RAM.
        while (i < 10000) {
            
            // False && (Qualquer coisa) -> Nunca avalia a direita
            a = false && badFunction();
            
            // True || (Qualquer coisa) -> Nunca avalia a direita
            a = true || badFunction();
            
            i = i + 1;
        }

        // --- TESTE 2: SHORT-CIRCUIT ANINHADO (NESTED) ---
        // Testa se o array pré-alocado (logical_stack) lida bem com múltiplas profundidades
        a = (false && badFunction()) || (true || badFunction()) || (false && badFunction());

        // --- TESTE 3: ESTRUTURAS DE CONTROLO ---
        if (false && badFunction()) {
            System.out.print("Isto nao devia imprimir.\n");
        } else {
            a = true;
        }

        if (true || badFunction()) {
            a = true;
        }

        // --- VERIFICAÇÃO FINAL ---
        System.out.print("Efeitos colaterais disparados: ");
        System.out.print(sideEffects);
        System.out.print("\n");

        if (sideEffects == 0) {
            System.out.print("SUCESSO: O teu compilador Juc é imune a falhas de short-circuit!\n");
        }
    }
}