#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack *head_op;
Stack *head_ac;

void mode_singlchar(Stack **stage, Stack **tail, char *cur_tok, char **s_t, char **e_t, int *mod, Type *type);
Stack *stage_one(char *str);
double calculate();
int paese(Stack *el);

#endif
