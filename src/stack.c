#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "main.h"

Stack *push_stage(Stack *stage, char *s_t, char *e_t){
	Stack *st = (Stack*)malloc(sizeof(Stack));
	if(st){
		st->s_val = s_t;
		st->e_val = e_t;
		if(stage)
			stage->next = st;
		else
			st->next = NULL;
	}else{
		perror("malloc stack_ac: ");
		exit(EXIT_FAILURE);
	}
	return st;
}

void push(Stack **st, Stack *el){
	el->next = *st;
	*st = el;
	return;
}	

Stack *pop(Stack **st){
	Stack *prev = NULL;
	Stack *cur = *st;
	if(!cur){
	   return (Stack*)NULL;
	}

	while(cur->next){
		prev = cur;
		cur = cur->next;
	}

	if(prev)
		prev->next = NULL;
	else
		*st = NULL;

	return cur;
}

void print_stack(Stack *st){
	char ch = 's';
	int quant = 0;
	while(st){
		printf("%d - ", st->type);
		quant = st->e_val - st->s_val + 1;
		char format[20] = "%.%";
		sprintf(format + 2, "%d", quant);
		sprintf(format + strlen(format), "%c\n", ch);
		printf(format, st->s_val);
		st = st->next;
	}
}

void destroy_head(Stack *st){
	while(st){
		Stack *t = st;
		st = st->next;
		free(t);
	}
}
