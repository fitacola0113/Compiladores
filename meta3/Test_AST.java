class TestAST {
    public static void main(String[] args) {
        int i;
        double d;
        boolean b;
        i = Integer.parseInt(args[0]);
        d = i + 2.5 * 3;
        b = (i < d) && !b;
        System.out.print(b);
    }
}