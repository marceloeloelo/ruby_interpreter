bison -vd ruby_grammar.y
flex ruby_lex_analyzer.l
gcc ruby_grammar.tab.c lex.yy.c -lfl -o interpreter
