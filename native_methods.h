// Ruby Object native methods like puts, get, etc
#ifndef NATIVE_METHODS_H
#define NATIVE_METHODS_H

#include "structures.h"
#include "ast.h"
#include "eval.h"
#include "utils.h"
#include <stdio.h>
#include <math.h>

#define PUTS "puts"
#define GETS "gets"

int is_native_method(struct method_call_node*);
void eval_native_method(struct method_call_node*);

#endif