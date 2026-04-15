to delete de exe file
    - make clean

to compile:
    - make

to create a zip (to put in mooooshak)
    - make zip

to run
    - ./jucompiler
    flag:
        - -l com todos os avisos
        - -e1 apenas erros

como testar com os case test:
- bash test.sh ./jucompiler              // tirando o '-'

Login no moooooooooshak

    Login:            uc2023211936_uc2023208585
    Password:        H6dxhy


# Meta 3 - Analisador Semântico

## Estruturas e Gestão de Símbolos

//DONE Definir structs symbol e symbol_table em semantics.h
//DONE Implementar create_symbol_table
//DONE Implementar insert_symbol
//DONE Implementar print_symbol_tables
//DONE Implementar search_symbol_local (declarada no .h mas sem implementação)
//DONE Implementar search_symbol_global (declarada no .h mas sem implementação)
//DONE Implementar free_symbol_tables (declarada no .h mas sem implementação)

## Construção das Tabelas de Símbolos (Travessia da AST)

//DONE Implementar check_program em semantics.c (declarada no .h mas sem corpo)
//DONE Percorrer o nó Program e criar a tabela global "Class <Nome> Symbol Table"
//DONE Inserir variáveis globais (FieldDecl) na tabela global com tipo correto
//DONE Inserir métodos (MethodDecl) na tabela global com assinatura "(ParamTypes) ReturnType"
//DONE Para cada método, criar tabela local "Method <nome>(<ParamTypes>) Symbol Table"
//DONE Inserir símbolo "return" com o tipo de retorno do método em cada tabela local
//DONE Inserir parâmetros formais na tabela local com flag is_param = true
//DONE Inserir variáveis locais (VarDecl) na tabela local com is_param = false
//DONE Garantir que símbolos e tabelas são apresentados por ordem de declaração no fonte
//DONE Detetar e reportar "Symbol <token> already defined" em caso de redeclaração

## Verificação de Tipos e Anotação da AST

//DONE Anotar nós de expressão com o tipo inferido em n->anotated_type
//DONE Anotar literais: Natural -> int, Decimal -> double, BoolLit -> boolean
//DONE Anotar Identifier com o tipo encontrado na tabela de símbolos (ou undef se não encontrado)
//DONE Anotar operadores aritméticos (Add, Sub, Mul, Div, Mod): int op int -> int, double op double -> double, int op double / double op int -> double
//DONE Anotar operadores relacionais (Eq, Ne, Lt, Gt, Le, Ge): resultado sempre boolean
//DONE Anotar operadores lógicos (And, Or, Not): operandos boolean, resultado boolean
//DONE Anotar Xor, Lshift, Rshift: operandos int, resultado int
//DONE Anotar Minus e Plus unários: preservam o tipo do operando (int ou double)
//DONE Anotar Length: resultado int
//DONE Anotar ParseArgs: resultado int
//DONE Anotar Call com o tipo de retorno do método correspondente
//DONE Anotar Assign com o tipo da variável do lado esquerdo
//DONE Propagar tipo undef quando um operando tem tipo undef (sem gerar novos erros em cascata desnecessários)

## Deteção de Erros Semânticos

//DONE "Symbol <token> already defined" - redeclaração de variável ou método no mesmo âmbito
//TODO "Symbol _ is reserved" - uso do identificador reservado "_"
//TODO "Cannot find symbol <token>" - referência a identificador não declarado ou método sem correspondência compatível
//TODO "Operator <token> cannot be applied to type <type>" - operador unário aplicado a tipo incompatível
//TODO "Operator <token> cannot be applied to types <type>, <type>" - operador binário com tipos incompatíveis
//TODO "Incompatible type <type> in return statement" - tipo da expressão de retorno incompatível com o declarado (incluindo "void" quando return sem expressão mas método tem tipo de retorno)
//TODO "Incompatible type <type> in if statement" - condição do if não é boolean
//TODO "Incompatible type <type> in while statement" - condição do while não é boolean
//TODO "Incompatible type <type> in print statement" - tipo passado ao print não suportado
//TODO "Number <token> out of bounds" - literal inteiro fora do intervalo int (ex: 2147483648)
//TODO "Reference to method <token> is ambiguous" - mais do que um método compatível encontrado
//TODO Garantir que erros são impressos antes das tabelas e da AST anotada
//TODO Atribuir tipo undef a símbolos desconhecidos e a resultados de operações inválidas

## Impressão

//DONE Atualizar print_tree para imprimir "- <type>" depois do nome do nó quando anotated_type não é NULL (apenas para nós de expressão)
//DONE Verificar formato da tabela de símbolos: separador entre nome e tipo deve ser um único \t quando não há param_types (o enunciado mostra "Name\t\tType" para variáveis - confirmar alinhamento com os exemplos do repositório)
//DONE Imprimir uma linha em branco entre tabelas consecutivas e entre as tabelas e a AST anotada

## Opções da Linha de Comandos

//DONE Adicionar opção -s ao main: análise semântica + impressão das tabelas + AST anotada (sem gerar código)
//DONE Adicionar opção -e3 ao main: análise semântica, reportar só erros, terminar

## Compatibilidade com Metas Anteriores

//DONE Opção -l: análise lexical com tokens
//DONE Opção -e1: análise lexical só erros
//DONE Opção -t: análise sintática + AST (sem semântica)
//DONE Opção -e2: análise sintática só erros
//DONE Confirmar que sem opções o programa executa análise semântica completa (sem imprimir tabelas nem AST)

## Memória

//DONE Implementar free_symbol_tables e libertar toda a memória alocada para as tabelas de símbolos antes de terminar