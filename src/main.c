#include <string.h>
//#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "element.h"
#include "utils.h"
#include "dic.h"

extern Dic *dic;

int main(int argc, char** argv){
	char *prog_txt = NULL;

	if(argc < 2){
		printf("Usage: %s expresion\n", argv[0]);
		return 0;
	}

	if((prog_txt = file_to_str(argv[1])) == NULL)
		return EXIT_FAILURE;

	init_dic();

	//print_dic();
	Element *prog = (Element*)malloc(sizeof(Element));
	memset(prog, 0, sizeof(Element));
	read_words(prog, prog_txt);
	int cod_open = word_cod("(");
	int cod_close = word_cod(")");
	make_tree(prog, cod_open, cod_close);
	cod_open = word_cod("{");
	cod_close = word_cod("}");
	make_tree(prog, cod_open, cod_close);


	print_dic();
	print_prog(prog, 0);
	printf("%s\n", prog_txt);

	free_dic();
	free_prog(prog);

	free(prog_txt);
	return EXIT_SUCCESS;
}

