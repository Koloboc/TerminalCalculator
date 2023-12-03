#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "element.h"
#include "utils.h"
#include "dic.h"

int is_liter(char ch){
	if((ch >= 'A') && (ch <= 'Z'))
		   return 1;
	else if((ch >= 'a') && (ch <= 'z'))
			return 1;
	return 0;
}

int is_digit(char ch){
	if((ch >= '0' && ch <= '9'))
			return 1;
	return 0;

}

void read_words(Element *root, char *str){
	size_t pos = 0;
	size_t len = strlen(str);
	char *name  = NULL;

	while(pos <= len){
		name = read_word(str, &pos, len);
		if(!name) name = read_number(str, &pos, len);
		if(!name) name = read_2mark(str, &pos, len);
		if(!name) name = read_1mark(str, &pos, len);
		if(name){
		   add_el_name(root, name);
		}else{
			pos++;
		}
	}
	return;
}

char *read_1mark(char *str, size_t *pos, size_t len){
	char *result = NULL;
	char tmp;

	if(*pos <= len){
		if	   (str[*pos] == '+') tmp = '+';
		else if(str[*pos] == '-') tmp = '-';
		else if(str[*pos] == '*') tmp = '*';
		else if(str[*pos] == '/') tmp = '/';
		else if(str[*pos] == '=') tmp = '=';
		else if(str[*pos] == '>') tmp = '>';
		else if(str[*pos] == '<') tmp = '<';
		else if(str[*pos] == '(') tmp = '(';
		else if(str[*pos] == ')') tmp = ')';
		else if(str[*pos] == '{') tmp = '{';
		else if(str[*pos] == '}') tmp = '}';
		if(tmp){
			result = (char*)malloc(sizeof(char) * 2);
			if(!result){
				printf("error alloc result for %c\n", tmp);
				return result; //  NULL
			}
			result[0] = tmp;
			result[1] = '\0';
		//printf("reading mark: %s\n", &tmp);
			*pos += 1;
		}
	}
	return result;
}

char *read_2mark(char *str, size_t *pos, size_t len){
	char *result = NULL;
	char *tmp = NULL;

	if(*pos <= len){
		if	   ((str[*pos] == '<') && (str[*pos + 1] == '=')) tmp = "<=";
		else if(((str[*pos] == '>') && (str[*pos + 1] == '='))) tmp = ">=";
		else if(((str[*pos] == '=') && (str[*pos + 1] == '='))) tmp = "==";
		else if(((str[*pos] == '<') && (str[*pos + 1] == '>'))) tmp = "<>";
		else if(((str[*pos] == '!') && (str[*pos + 1] == '='))) tmp = "!=";
		if(tmp){
			*pos += 2;
			result = (char*)malloc(sizeof(char) * 3);
			if(!result){
				printf("error alloc result for %s\n", tmp);
				return result; //  NULL
			}
			strncpy(result, tmp, 2);
			//result[2] = '\0';
		}
	}
	return result;
}

char *read_number(char *str, size_t *pos, size_t len){
	char *result = NULL;
	size_t start = *pos;
	int kt = 0;

	if(((*pos <= len) && is_digit(str[*pos])) || str[*pos] == '-'){
		while(((*pos <= len) && is_digit(str[*pos])) || ((str[*pos] == '.') && (kt < 1))){
			if(str[*pos] == '.') 
				kt++;
			(*pos)++;
		}
		result = (char*)malloc(sizeof(char) * ((*pos) - start) + 1);
		if(!result){
			printf("error alloc char result\n");
			return NULL;
		}
		strncpy(result, &str[start], (*pos) - start);
	}
	return result;
}

char *read_word(char *str, size_t *pos, size_t len){
	char *result = NULL;
	size_t start = *pos;

	if((*pos <= len) && is_liter(str[*pos])){
		while(((*pos <= len) && is_liter(str[*pos])) || is_digit(str[*pos]) || str[*pos] == '_'){
			(*pos)++;
		}
		result = (char*)malloc(sizeof(char) * (*pos - start) + 1);
		if(!result){
			printf("error alloc char result\n");
			return NULL;
		}
		strncpy(result, &str[start], *pos - start);
	}
	return result;
}

char *file_to_str(char *namefile){
	struct stat st;
	char *str = NULL;
	char *iter = NULL;
	char ch;

	if((stat(namefile, &st)) < 0){
		printf("%s file not exist\n", namefile);
		return 0;
	}
	off_t size = st.st_size;

	FILE *in = NULL;
    in = fopen("test", "r");
	if(!in)
		return NULL;

	str = (char*)malloc(sizeof(char) * size + 1);
	if(!str){
		printf("error alloc str\n");
		return NULL;
	}
	iter = str;
	while((ch = fgetc(in)) != EOF){
		if(ch != '\n'){
			*iter = ch;
			iter++;
		}
		//*iter = '\0';
	}
	fclose(in);
	return str;
}
