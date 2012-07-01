// Ruby Object native methods like puts, get, etc
#ifndef NATIVE_METHODS_H
#define NATIVE_METHODS_H

#include "structures.h"
#include "ast.h"
#include "st.h"
#include "eval.h"
#include "utils.h"
#include "errors.h"
#include <stdio.h>
#include <math.h>

#define PUTS "puts"
#define GETS "gets"
#define NEW  "new"

int is_native_method(struct ast*);
struct ast* eval_native_method(struct ast*);

int is_class_native_method(struct method_call_node*);
struct ast* eval_class_native_method(struct method_call_node*);

#endif
