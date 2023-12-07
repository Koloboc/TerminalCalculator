#ifndef __DIC_H__
#define __DIC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum _types{
	UNDEF = 0,
	OPERAND,
	ACTION,
	OPEN_LEVEL,
	CLOSE_LEVEL,
	IS_FOO,
};
typedef enum _types Types;

typedef struct _dic{
	char *name;
	size_t code;
	Types type;
	struct _dic *next;
}Dic;

Dic* word_dic(char *name, Types t);
size_t word_cod(char *name);
Types word_type(size_t cod);
Dic *add_word_dic(char *name, size_t, Types t);
void init_dic();
void free_dic();
void print_dic();

#endif
