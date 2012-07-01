// Ruby Object native methods like puts, get, etc
#ifndef NATIVE_METHODS_H
#define NATIVE_METHODS_H

#include "structures.h"
#include "ast.h"
#include "eval.h"
#include "utils.h"
#include "errors.h"
#include <stdio.h>
#include <math.h>

#define PUTS          "puts"
#define GETS          "gets"
#define LENGTH        "length"
#define EACH_ITERATOR "each"
#define RESPOND_TO    "respond_to?"

int is_native_method(struct ast*);
struct ast* eval_native_method(struct ast*);
struct ast* eval_instance_native_method(struct ast*);

#endif
