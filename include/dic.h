#ifndef __DIC_H__
#define __DIC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double      acos(double);
double      asin(double);
double      atan(double);
double      ceil(double);
double      cos(double);
double      exp(double);
double      floor(double);
double      fmax(double, double);
double      fmin(double, double);
double      fmod(double, double);
double      log(double);
double      log10(double);
double      log2(double);
double      pow(double, double);
double      round(double);
double      sin(double);
double      sqrt(double);
double      tan(double);

typedef double(foonc_1_1)(double);
typedef double(foonc_1_2)(double, double);
#define MAX_PRIOR 2 // от 0 до 2

enum _types{
	UNDEF = 0,
	OPERAND,
	ACTION,
	OPEN_LEVEL,
	CLOSE_LEVEL,
	IS_FOO,
};
typedef enum _types Types;

typedef struct TPFoo{
	void *pfoo;
	int nargs;
}TPFoo;

typedef struct _dic{
	char *name;
	size_t code;
	Types type;
	int prior; // Приоритет от 0 до 3?
	TPFoo *pfoo;
	int need_grad;
	struct _dic *next;
}Dic;

Dic* word_dic(char *name, Types t);
size_t word_cod(char *name);
Types word_type(size_t cod);
Dic *add_word_dic(char *name, size_t, Types t, int prior, void *f);
void init_dic();
void free_dic();
void print_dic();

#endif
