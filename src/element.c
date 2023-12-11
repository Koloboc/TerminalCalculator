#include <stdlib.h>
#include "element.h"
#include "dic.h"


void exec_el(Element *el){
	Element *arg1 = NULL;
	Element *arg2 = NULL;
	int precission = 10;

	if(el->dic->type == OPERAND) return;

	if(el->inner){
		arg1 = el->inner;
		exec_el(arg1);
		if(el->inner->next){
			arg2 = el->inner->next;
			exec_el(arg2);
		}
	}

	if(strcmp(el->value, "+") == 0){
		if(arg2 && arg1){
			double d1 = strtod(arg1->value, NULL);
			double d2 = strtod(arg2->value,NULL);
			double rez = d1 + d2;
#ifdef DEBUG
			printf("operation '+': d1=%f; d2=%f; rez=%f\n", d1, d2, rez);
#endif
			if(el->value) free(el->value);
			el->value = (char*)malloc(sizeof(char) * precission);
			sprintf(el->value, "%f", rez);
			free_el(arg1);
			free_el(arg2);
		} 
	}else if(strcmp(el->value, "-") == 0){
		if(arg2 && arg1){
			double d1 = strtod(arg1->value, NULL);
			double d2 = strtod(arg2->value,NULL);
			double rez = d1 - d2;
#ifdef DEBUG
			printf("operation '-': d1=%f; d2=%f; rez=%f\n", d1, d2, rez);
#endif
			if(el->value) free(el->value);
			el->value = (char*)malloc(sizeof(char) * precission);
			sprintf(el->value, "%f", rez);
			free_el(arg1);
			free_el(arg2);
		} 
	}else if(strcmp(el->value, "/") == 0){
		if(arg2 && arg1){
			double d1 = strtod(arg1->value, NULL);
			double d2 = strtod(arg2->value,NULL);
			if(!d2){
				printf("division on 0");
				return;
			}
			double rez = d1 / d2;
#ifdef DEBUG
			printf("operation '/': d1=%f; d2=%f; rez=%f\n", d1, d2, rez);
#endif
			if(el->value) free(el->value);
			el->value = (char*)malloc(sizeof(char) * precission);
			sprintf(el->value, "%f", rez);
			free_el(arg1);
			free_el(arg2);
		} 
	}else if(strcmp(el->value, "*") == 0){
		if(arg2 && arg1){
			double d1 = strtod(arg1->value, NULL);
			double d2 = strtod(arg2->value,NULL);
			double rez = d1 * d2;
#ifdef DEBUG
			printf("operation '*': d1=%f; d2=%f; rez=%f\n", d1, d2, rez);
#endif
			if(el->value) free(el->value);
			el->value = (char*)malloc(sizeof(char) * precission);
			sprintf(el->value, "%f", rez);
			free_el(arg1);
			free_el(arg2);
		} 
	}else if(strcmp(el->value, "(") == 0){
		if(el->inner){
			exec_el(el->inner);
			free(el->value);
			el->value = strdup(el->inner->value);
			free_el(el->inner); 
		}
	}
	el->inner = NULL;
}

void make_tree_ma(Element *root){
	Element *iter = root->inner;
	int pr = 3;

	while(pr--){
		while(iter){
			if(iter->inner)
				make_tree_ma(iter);
			if(iter->dic->type == ACTION && iter->dic->prior == pr){
				if(!iter->prev){
					printf("Error: miss operand before %s\n", iter->value);
					return;
				}
				if(!iter->next){
					printf("Error: miss operand avter %s\n", iter->value);
					return;
				}
				add_el(iter, iter->prev);
				if(iter->next && iter->next->inner)
					make_tree_ma(iter->next);
				add_el(iter, iter->next);
			}
			iter = iter->next;
		}
		iter = root->inner;
	}
}

// Построение структуры вложенности скобок и функций
void make_tree(Element *el, size_t code_open, size_t code_close){
	Element *container = el;
	Element *iter = el->inner;

	while(iter){
		Element *next = iter->next;
		if(container != el){
			if(iter->dic->code == code_close){
				container = container->parent;
				free_el(iter);
				iter = next;
				continue;
			}else{
				add_el(container, iter);
			}
		}
		if((iter->dic->code == code_open) || (iter->dic->type == IS_FOO))
			container = iter;
		iter = next;
	}
}

// Возыращает последний элемент во внутреннем списке root
// или NULL
Element *last_el(Element *root){
	Element *inn = root->inner;
	if(!inn)
		return NULL;
	while(inn->next)
		inn = inn->next;

	return inn;
}

// Отстыковывает элемент и делает егоa \"свободным\"
// Внутренние элементы (если есть) остаются без изменений
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

// Добавляет элемент в конец к родителю root
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

// Создает элемент нужного типа и со значением
void add_el_name(Element *dist, char *name, Types type){
	Element *newel = NULL;
	Dic *d = word_dic(name, type); // ищем в словаре, если нет, создается новый элемент словаря
	if(!d)
		printf("Error dic %s : %d\n", name, type);

	newel = (Element*)malloc(sizeof(Element));
	newel->dic = d;
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

