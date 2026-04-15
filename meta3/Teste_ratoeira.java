class TesteRatoeiras {
    public static int x;
    
    // Conflito com variável global
    public static void x() {} 
    
    // Método normal (testar espaços normais)
    public static void f(int a, boolean b) {}
    
    // Conflito com método já existente
    public static void f(int c, boolean d) {} 
    
    // Parâmetro repetido
    public static void g(int a, double a) {}
}