class DanglingWild {
    public static void main(String[] args) {
        boolean x, y;
        
        // O "else" deve prender-se sempre ao if(y), e o bloco exterior deve ter os nós corretos
        if (x) 
            if (y) 
                ; 
            else 
                System.out.print("y é falso");
                
        // Teste Lexical: O primeiro */ fecha o comentário, pelo que "a = 1;" tem de ser lido!
        /* comentário longo
           /* a tentar aninhar comentários (não existe em java)
        */ a = 1;
    }
}