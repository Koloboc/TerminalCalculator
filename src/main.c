#include <string.h>
//#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "element.h"
#include "utils.h"
#include "dic.h"

//#define DEBUG 0
extern Dic *dic;
int main(int argc, char** argv){
	char *prog_txt = NULL;

	if(argc > 2){
		if(strcmp(argv[1], "-e") == 0){
			prog_txt = argv[2];
		}else{
			printf("%s expresion\n", argv[0]);
			return  EXIT_SUCCESS;
		}
	}else if(argc < 2){
		printf("Usage: %s <file>\n", argv[0]);
		printf("       %s -e \"expression\"\n", argv[0]);
		return  EXIT_SUCCESS;
	}

	if(!prog_txt)
		if((prog_txt = file_to_str(argv[1])) == NULL)
			return EXIT_FAILURE;

	init_dic();
	/*
#ifdef DEBUG
	print_dic();
#endif
*/
	Element *prog = (Element*)malloc(sizeof(Element));
	memset(prog, 0, sizeof(Element));
	read_words(prog, prog_txt);
	make_tree(prog, word_cod("("), word_cod(")"));
	make_tree_ma(prog);
#ifdef DEBUG
	print_prog(prog, 0);
#endif
	exec_el(prog->inner);

	printf("%s =\n%s\n", prog_txt, prog->inner->value);

	free_dic();
	free_prog(prog);

	if(argc < 2)
		free(prog_txt);
	return EXIT_SUCCESS;
}

