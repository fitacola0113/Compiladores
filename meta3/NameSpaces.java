class TestNamespaces {
    public static int a;       
    
    public static void a() {}  

    public static int b(int b) { 
        return b;
    }

    public static void main(String[] args) {
        int main; 
        main = 1;
        a = 2;
        a(); 
    }
}