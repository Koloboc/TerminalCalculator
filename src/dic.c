#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dic.h"

Dic *dic = NULL;
size_t count_dic = 0;
int max_prior = 2;
// Находит элемент в словаре
// Если не найден, то создает новый (типа foo или operand
Dic* word_dic(char *name, Types type){
	Dic *d = dic;
	while(d){
		if(strcmp(d->name, name) == 0)
			return d;
		d = d->next;
	}
	if(type == IS_FOO || type == OPERAND)
		return add_word_dic(name, 0, type, 0, NULL);
	return NULL;
}

// Ищет по коду
// Возвращает Тип элемента
Types word_type(size_t cod){
	Dic *cur = dic;
	while(cur){
		if(cur->type == cod)
			return cur->type;
		cur = cur->next;
	}
	return 0;
}

// Ищет элемент по имени
// Возвращает код
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

// Создает и присоединяет в начало списка элемент
Dic *add_word_dic(char *name, size_t cod, Types t, int prior, void *f){
	TPFoo *tpfoo = NULL;

	Dic *newdic = (Dic*)malloc(sizeof(Dic));
	if(cod)
		newdic->code = cod;
	else
		newdic->code = ++count_dic;

	newdic->name = strdup(name);
	newdic->type = t;
	newdic->prior = prior;
	newdic->next = NULL;
	newdic->pfoo = NULL;
	newdic->next = dic;
	dic = newdic;
	if(f){
		tpfoo = (TPFoo*)malloc(sizeof(TPFoo));
		if((strcmp(name, "fmax") == 0) || 
		   (strcmp(name, "fmin") == 0)	||
		   (strcmp(name, "fmod") == 0)	||
		   (strcmp(name, "pow") == 0)){
			tpfoo->nargs = 2;
		}else{
			tpfoo->nargs = 1;
		}
		tpfoo->pfoo = f;
		newdic->pfoo = tpfoo;
	}
	return newdic;
}

// Создает базовые элементы
void init_dic(){
	add_word_dic("*", 0, ACTION, 1, NULL);
	add_word_dic("/", 0, ACTION, 1, NULL);
	add_word_dic("+", 0, ACTION, 0, NULL);
	add_word_dic("-", 0, ACTION, 0, NULL);
	add_word_dic("(", 0, OPEN_LEVEL, 2, NULL);
	add_word_dic(")", 0, CLOSE_LEVEL, 2, NULL);

	add_word_dic("acos", 0, IS_FOO, 2, &acos);
	add_word_dic("asin", 0, IS_FOO, 2, &asin);
	add_word_dic("atan", 0, IS_FOO, 2, &atan);
	add_word_dic("ceil", 0, IS_FOO, 2, &ceil);
	add_word_dic("cos", 0, IS_FOO, 2, &cos);
	add_word_dic("exp", 0, IS_FOO, 2, &exp);
	add_word_dic("floor", 0, IS_FOO, 2, &floor);
	add_word_dic("fmax", 0, IS_FOO, 2, &fmax);
	add_word_dic("fmin", 0, IS_FOO, 2, &fmin);
	add_word_dic("fmod", 0, IS_FOO, 2, &fmod);
	add_word_dic("log", 0, IS_FOO, 2, &log);
	add_word_dic("log10", 0, IS_FOO, 2, &log10);
	add_word_dic("log2", 0, IS_FOO, 2, &log2);
	add_word_dic("pow", 0, IS_FOO, 2, &pow);
	add_word_dic("round", 0, IS_FOO, 2, &round);
	add_word_dic("sin", 0, IS_FOO, 2, &sin);
	add_word_dic("sqrt", 0, IS_FOO, 2, &sqrt);
	add_word_dic("tan", 0, IS_FOO, 2, &tan);
}

void free_dic(){
	Dic *cur = dic;
	Dic *next = NULL;

	while(cur){
		free(cur->name);
		if(cur->pfoo)
			free(cur->pfoo);
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

