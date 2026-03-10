class Invocations {
    public static int foo(int a) { return a; }

    public static void main(String[] args) {
        // ParseArgs indexado pelo tamanho do próprio args
        System.out.print(Integer.parseInt(args[args.length]));
        
        // ParseArgs indexado por uma chamada de função que leva expressões
        System.out.print(Integer.parseInt(args[foo(1 + 2) * 3]));
        
        // Print de uma string literal escapada
        System.out.print("\"\\t\\n\\r\\f\\\\\"");
    }
}
