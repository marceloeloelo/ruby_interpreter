echo "bison -vd ruby_grammar.y"
bison -vd ruby_grammar.y
echo "gcc -c ruby_grammar.tab.c"
gcc -c ruby_grammar.tab.c
echo "flex ruby_lex_analyzer.l"
flex ruby_lex_analyzer.l
echo "gcc -c lex.yy.c"
gcc -c lex.yy.c
echo "gcc -o interpreter lex.yy.o ruby_grammar.tab.o -lm"
gcc -o interpreter lex.yy.o ruby_grammar.tab.o ast.c st.c errors.c -lm

