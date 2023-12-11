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

	free(prog_txt);
	return EXIT_SUCCESS;
}

