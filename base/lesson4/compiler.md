frontend -> middlend -> backend

frontend -> ast

ast -> middlend  -> ir (intermidiate representation)

ir -> backend -> assembler

frontend


Исходный код → [Лексер] → Токены → [Парсер] → AST → [Обход] → Результат


"2 + 3 * x"  →  [NUMBER(2), PLUS, NUMBER(3), STAR, IDENT("x")]


L = {aa, bb, cc}

L = {aaa,bbb, (ab, aaabbb)}
L = {a^n, b^n} 



a = 2 + 3 * 5;
b = a;

c = a*(b + 4);


Program → StatementList

StatementList → Statement | StatementList | ε

Statement → Assignment ";"

Assignment → IDENTIFIER "=" Expression

Expression → Expression "+" Term | Expression "-" Term | Term 

Term → Term "*" Factor | Factor

Factor → IDENTIFIER | NUMBER  | "(" Expression ")"


LL(1)


        (+)
       /   \
      2    (*)
           / \
          3   x


        (*)
       /   \
      x    (+)
           / \
          3   2