{%
  #include "y.tab.h"
%}

identifier    [a-zA-Z_][a-zA-Z0-9_]*
op_asign      `+=' | `-=' | `*=' | `/=' | `%=' | `^=' | `&&=' | `||='
string        '"'identifier'"' | '''identifier'''
operation     identifier | identifier'!' | identifier'?'
var_name      '$'identifier | '@'identifier | identifier
symbol        ':'identifier

/* We don't need to include: */
/*   -> strings as arrays    */
/*   -> regexp               */

%%

{ identifier }  { return IDENTIFIER }
{ op_asign }    { return OP_ASIGN }
{ string }      { return STRING }
{ operation }   { return OPERATION }
{ var_name }    { return VAR_NAME }
{ symbol }      { return SYMBOL }

%%
