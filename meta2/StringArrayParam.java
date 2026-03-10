class StringArrayParam {
    // Sintaticamente CORRETO em Juc (único parâmetro)
    public static void main(String[] args) {
        return;
    }
    
    // Sintaticamente ERRO em Juc (String[] não admite vírgulas depois segundo a gramática EBNF)
    public static void metodoErrado(String[] args, int a) {
        return;
    }
}
