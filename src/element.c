#include "element.h"
#include "dic.h"

void make_tree(Element *el, int code_open, int code_close){
	Element *container = el;
	Element *iter = el->inner;

	while(iter){
		Element *next = iter->next;
		if(container != el){
			if(iter->code == code_close){
				container = container->parent;
				free_el(iter);
				continue;
			}else{
				add_el(container, iter);
			}
		}
		if(iter->code == code_open)
			container = iter;
		iter = next;
	}
}

Element *last_el(Element *root){
	Element *inn = root->inner;
	if(!inn)
		return NULL;
	while(inn->next)
		inn = inn->next;

	return inn;
}

void disconnect(Element *el){
	Element *pa = el->parent;
	if(pa)						  // Родитель существует
		if(pa->inner == el){		  // Элемент является первым у прлителя
			pa->inner = el->next; // Исключаем элемент и записываем следцющий
			el->parent = NULL;
		}

	if(el->next) el->next->prev = el->prev;
	if(el->prev) el->prev->next = el->next;
}

void add_el(Element *root, Element *el){
	disconnect(el);
	Element *last_dist = last_el(root);
	if(last_dist){
		last_dist->next = el;
		el->next = NULL;
		el->prev = last_dist;
		el->parent = root;
	}else{
		root->inner = el;
		el->parent = root;
		el->next = NULL;
		el->prev = NULL;
	}
}

void add_el_name(Element *dist, char *name){
	Element *newel = NULL;

	newel = (Element*)malloc(sizeof(Element));
	newel->code = word_cod(name);
	newel->value = name;
	newel->parent = NULL;
	newel->inner = NULL;
	newel->next = NULL;
	newel->prev = NULL;

	add_el(dist, newel);
	return;
}

void free_el(Element *el){
	disconnect(el);
	if(el->value)
		free(el->value);

	//if(el->parent->inner == el) el->parent->inner = NULL;
	//if(el->next) el->next->prev = el->prev;
	//if(el->prev) el->prev->next = el->next;
	free(el);
}

void free_prog(Element *root){
	Element *n = NULL;
	while(root){
		n = root->next;
		if(root->inner)
			free_prog(root->inner);
		//free_el(root);
		if(root->value)
			free(root->value);
		free(root);
		root = n;
	}
}

void print_prog(Element *root, int blanks){
	while(root){
		if(root->value){
			for(int i = 0; i < blanks; i++){
				putchar('\t');
			}
			printf("%s\n", root->value);
		}

		if(root->inner){
			print_prog(root->inner, blanks + 1);
		}
		root = root->next;
	}
}

