bison -yd ruby_grammar.y
flex ruby_lex_analyzer.l
g++ y.tab.c lex.yy.c -lfl -o interpreter
