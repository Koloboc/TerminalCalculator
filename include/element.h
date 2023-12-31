#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <stdio.h>
#include <stdlib.h>
#include "dic.h"

#define PRECISSION 10

struct _el{
	char *value;
	Dic *dic;

	struct _el *parent;
	struct _el *next;
	struct _el *prev;
	struct _el *inner;
};
typedef struct _el Element;


void disconnect(Element *el);
Element *last_el(Element *el);
void add_el(Element *dist, Element *el);
void add_el_name(Element *dist, char *name,  Types t);
void make_tree(Element *root, size_t code_open, size_t code_close);
void make_tree_ma(Element *root);

void exec_el(Element *el);
void op_dual(Element *el, char *op);
void exec_brackets(Element *el);

void free_prog(Element *el);
void free_el(Element *el);
void print_prog(Element *prog, int blanks);

#endif
