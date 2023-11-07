#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack *head_op = NULL;
Stack *head_ac = NULL;

Stack *stage_one(char *str){
	char *tock = str;
	int lenline = strlen(tock);
	int mode = UNDEF_MODE;
	char *s_tocken = NULL;
	char *e_tocken = NULL;
	Stack *stage = NULL;
	Stack *tail = NULL;
	while(1){
		if(!stage && tail) stage = tail;
		switch(*tock){
			case '(':
			case ')':
			case '+':
			case '-':
			case '*':
			case '/':
				if((mode == MULT_CHAR || mode == MULT_DIGIT) && mode != UNDEF_MODE){
					tail = push_stage(tail, s_tocken, e_tocken);
					tail->type = OPERAND;
				}
				tail = push_stage(tail, tock, tock);
				tail->type = ACTION;
				s_tocken = NULL;
				e_tocken = NULL;
				mode = SINGL_CHAR;
				break;
			case ' ':
				if(mode == MULT_CHAR || mode == MULT_DIGIT ){
					tail = push_stage(tail, s_tocken, e_tocken);
					tail->type = OPERAND;
				}
				s_tocken = NULL;
				e_tocken = NULL;
				mode = UNDEF_MODE;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(mode == MULT_DIGIT)
					e_tocken = tock;

				if(!s_tocken) 
					s_tocken = tock;

				if(!e_tocken) 
					e_tocken = tock;

				mode = MULT_DIGIT;
				break;
			case '.':
				break;
			default:
				printf("error parsing expression symbol %c %ld\n", *tock, tock - str);
				break;
		}
		tock++;
		if(tock == str + lenline){
			if(mode == MULT_DIGIT && s_tocken && e_tocken){
				tail = push_stage(tail, s_tocken, e_tocken);
				tail->type = OPERAND;
			}
			break;
		}
	}
	return stage;
}

int main(int argc, char** argv){
	if(argc < 2){
		printf("Usage: %s expresion\n", argv[0]);
		return 0;
	}
	Stack *stage = stage_one(argv[1]);
	Stack *el = NULL;
	//while(stage){
		//el = pop(&stage);
		//if(el){
			//if(el->type == OPERAND){
					//push(&head_op, el);
			//}else if(el->type == ACTION){
					//push(&head_ac, el);
			//}else{
				//printf("undefined type: %d\n", el->type);
				//free(el);
			//}
		//}
	//}
	
	printf("operands:\n");
	print_stack(head_op);
	printf("actions:\n");
	print_stack(head_ac);
	printf("stage:\n");
	print_stack(stage);
	destroy_head(stage);
	destroy_head(head_op);
	destroy_head(head_ac);


	return 0;
}

