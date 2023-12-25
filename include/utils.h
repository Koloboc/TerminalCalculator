#ifndef __UTILS_H__
#define __UTILS_H__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "element.h"

char *file_to_str(char *namefile);
void read_words(Element *root, char *str);
char *read_word(char *str, size_t *pos, size_t);
char *read_number(char *str, size_t *pos, size_t);
char *read_1mark(char *str, size_t *pos, size_t);
char *read_2mark(char *str, size_t *pos, size_t);
int is_liter(char ch);
int is_digit(char ch);
char *pr_v(char *sval);


#endif
