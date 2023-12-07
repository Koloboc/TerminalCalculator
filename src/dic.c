#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dic.h"

Dic *dic = NULL;
size_t count_dic = 0;

Dic* word_dic(char *name, Types type){
	Dic *d = dic;
	while(d){
		if(strcmp(d->name, name) == 0)
			return d;
		d = d->next;
	}
	if(type == IS_FOO || type == OPERAND)
		return add_word_dic(name, 0, type);
	return NULL;
}

Types word_type(size_t cod){
	Dic *cur = dic;
	while(cur){
		if(cur->type == cod)
			return cur->type;
		cur = cur->next;
	}
	return 0;
}

size_t word_cod(char *name){
	Dic *curdic = dic;

	while(curdic){
		if(strcmp(curdic->name, name) == 0){
			return curdic->code;
		}
		curdic = curdic->next;
	}
	return 0;
}

Dic *add_word_dic(char *name, size_t cod, Types t){
	Dic *newdic = (Dic*)malloc(sizeof(Dic));
	if(cod)
		newdic->code = cod;
	else
		newdic->code = ++count_dic;

	newdic->name = strdup(name);
	newdic->type = t;
	newdic->next = NULL;
	newdic->next = dic;
	dic = newdic;
	return newdic;
}

void init_dic(){
	add_word_dic("+", 0, ACTION);
	add_word_dic("-", 0, ACTION);
	add_word_dic("*", 0, ACTION);
	add_word_dic("/", 0, ACTION);
	//add_word_dic("=", 0, ACTION);
	//add_word_dic("!=", 0, COMPARISON);
	//add_word_dic("<>", 0, COMPARISON);
	//add_word_dic("<", 0, COMPARISON);
	//add_word_dic(">", 0, COMPARISON);
	//add_word_dic("<=", 0, COMPARISON);
	//add_word_dic(">=", 0, COMPARISON);
	add_word_dic("(", 0, OPEN_LEVEL);
	add_word_dic(")", 0, CLOSE_LEVEL);
	add_word_dic("{", 0, OPEN_LEVEL);
	add_word_dic("}", 0, CLOSE_LEVEL);
}

void free_dic(){
	Dic *cur = dic;
	Dic *next = NULL;

	while(cur){
		free(cur->name);
		next = cur->next;;
		free(cur);
		cur = next;
	}
}

void print_dic(){
	Dic *pd = dic;
	while(pd){
		printf("name: %s \tcode:%ld \ttype: %d\n", pd->name, pd->code, pd->type);
		pd = pd->next;
	}
}

