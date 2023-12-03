#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <stdio.h>
#include <stdlib.h>

struct _el{
	size_t code;
	char *value;

	struct _el *parent;
	struct _el *next;
	struct _el *prev;
	struct _el *inner;
};
typedef struct _el Element;

void disconnect(Element *el);
Element *last_el(Element *el);
void add_el(Element *dist, Element *el);
void add_el_name(Element *dist, char *name);
void make_tree(Element *root, int code_open, int code_close);
void free_prog(Element *el);
void free_el(Element *el);
void print_prog(Element *prog, int blanks);

#endif
