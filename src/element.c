#include <stdlib.h>
#include "element.h"
#include "dic.h"

#undef DEBUG

// Операции с двумя аргументами (+-/*)
void op_dual(Element *el, char *op){
		if(el->inner && el->inner->next){
			double d1 = strtod(el->inner->value, NULL);
			double d2 = strtod(el->inner->next->value,NULL);
			double rez;
			switch (op[0]){
				case '+': rez = d1 + d2; break;
				case '-': rez = d1 - d2; break;
				case '*': rez = d1 * d2; break;
				case '/': rez = d1 / d2; break;
				default:
				  printf("uncnow operation '%c'", op[0]);
				  return;
			}
#ifdef DEBUG
			printf("%f '%c' %f = %f\n", d1, op[0], d2, rez);
#endif
			if(el->value) free(el->value);
			el->value = (char*)malloc(sizeof(char) * PRECISSION);
			sprintf(el->value, "%f", rez);
			free_el(el->inner->next); // Disconnect and free elem
			free_el(el->inner); // Disconnect and free elem
		}else{
			printf("missing arguments: '%c'\n", op[0]);
		}
		return;
}

void exec_brackets(Element *el){
	Element *in = el->inner;
	if(el->inner){
		if(el->parent->dic->type != IS_FOO){ // Если родитель это - функция
			free(el->value);
			el->value = strdup(el->inner->value);
			free_el(el->inner);
		}else{
			in = el->inner;
			Element *parent = el->parent;
			parent->inner = in;
			while(in){
				in->parent = parent;
				in = in->next;
			}
			free_el(el);
		}
	}
}

void exec_el(Element *el){
	Element *in = el->inner;

	if(el->dic->type == OPERAND)
		return;

	while(in){
		exec_el(in);
		in = in->next;
	}

	if(el->dic->type == ACTION){
		 // Вычесление operations (+ - / *)
		op_dual(el, el->value);
	}else if(strcmp(el->value, "(") == 0){
		// Вычесление скобок
		exec_brackets(el);
	}else if(el->dic->type == IS_FOO){
		// Вычесление Функций
		Dic *d = word_dic(el->value, IS_FOO); // Получаем соответсвующий елемент-dictonary (или создается новый элемент)
		double rez;
		if(d){
			if(d->pfoo){ // Если задан указатель на функцию
				if(d->pfoo->nargs == 2){ // Количество аргументов функции 2
					foonc_1_2* foo =  d->pfoo->pfoo;
					if(el->inner && el->inner->next){
					double d1 = strtod(el->inner->value, NULL);
					double d2 = strtod(el->inner->next->value, NULL);
					rez = (*foo)(d1, d2);
					free_el(el->inner->next);
					free_el(el->inner);
#ifdef DEBUG
					printf("operation '%s': d1=%f; d2=%f; rez=%f\n",el->value, d1, d2, rez);
#endif
					}
				}else{ // Количество аргументов функции 1
					foonc_1_1* foo =  d->pfoo->pfoo;
					double d1 = strtod(el->inner->value, NULL);
					rez = (*foo)(d1);
					free_el(el->inner);
#ifdef DEBUG
					printf("operation '%s': d1=%f; rez=%f\n",el->value, d1, rez);
#endif
				}
			}else{ // Нет указателя на функцию
				printf("Error: fuction %s not exist or not faund\n", el->value);

			}
			if(el->value) free(el->value);
			el->value = (char*)malloc(sizeof(char) * PRECISSION);
			sprintf(el->value, "%f", rez);
		}
	}
}

void make_tree_ma(Element *root){
	Element *iter = root->inner;
	int pr = MAX_PRIOR; // Приоритет выполнения начинаем с максимального ( функции, скобки,операции +-/*)

	while(pr--){
		while(iter){
			if(iter->inner)
				make_tree_ma(iter);
			if(iter->dic->type == ACTION && iter->dic->prior == pr){
				if(!iter->prev) return;
				if(!iter->next) return;

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

// Возвращает последний элемент во внутреннем списке root
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

