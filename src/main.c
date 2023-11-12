#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "main.h"

Stack *head_op = NULL;
Stack *head_ac = NULL;

void mode_singlchar(Stack **stage, Stack **tail, char *cur_tok, char **s_t, char **e_t, int *mod, Type *type){
	if(*mod == MULT_CHAR || *mod == MULT_DIGIT){
		*tail = push_stage(*tail, *s_t, *e_t);
		if(!(*stage) && *tail) 
			*stage = *tail;

		(*tail)->type = OPERAND;
		if(!sscanf(*s_t, "%lf", &((*tail)->val)))
			perror("sscan:");
	}
	*tail = push_stage(*tail, cur_tok, cur_tok);
	if(!(*stage) && *tail) 
		*stage = *tail;

	Prior prior = UNDEF_PRIOR;
	//switch ((*tail)->type){
	switch (*type){
		case PLUS: prior = P_PLUS;break;
		case MINUS: prior = P_MINUS;break;
		case DIVIDE: prior = P_DIVIDE;break;
		case MULTIPLY: prior = P_MULTIPLY;break;
		case SQRT: prior = P_SQRT;break;
		case BREACK_OPEN: prior = P_BREACK_OPEN;break;
		case BREACK_CLOSE: prior = P_BREACK_CLOSE;break;
		case UNDEF_TYPE: prior = UNDEF_PRIOR;break;
		case OPERAND: prior = UNDEF_PRIOR;break;
	}
	(*tail)->prior = prior;
	(*tail)->type = *type;
	*s_t = NULL;
	*e_t = NULL;
	*mod = SINGL_CHAR;
}

Stack *stage_one(char *str){
	char *tock = str;
	int lenline = strlen(tock);
	int mode = UNDEF_MODE;
	char *s_tocken = NULL;
	char *e_tocken = NULL;
	Stack *stage = NULL;
	Stack *tail = NULL;
	while(1){
		Type type = UNDEF_TYPE;
		switch(*tock){
			case '(':
				type = BREACK_OPEN;
				mode_singlchar(&stage, &tail, tock, &s_tocken, &e_tocken, &mode, &type);
				break;
			case ')':
				type = BREACK_CLOSE;
				mode_singlchar(&stage, &tail, tock, &s_tocken, &e_tocken, &mode, &type);
				break;
			case '+':
				type = PLUS;
				mode_singlchar(&stage, &tail, tock, &s_tocken, &e_tocken, &mode, &type);
				break;
			case '-':
				type = MINUS;
				mode_singlchar(&stage, &tail, tock, &s_tocken, &e_tocken, &mode, &type);
				break;
			case '*':
				type = MULTIPLY;
				mode_singlchar(&stage, &tail, tock, &s_tocken, &e_tocken, &mode, &type);
				break;
			case '/':
				type = DIVIDE;
				mode_singlchar(&stage, &tail, tock, &s_tocken, &e_tocken, &mode, &type);
				break;
			case ' ':
				if(mode == MULT_CHAR || mode == MULT_DIGIT ){
					tail = push_stage(tail, s_tocken, e_tocken);
					if(!stage && tail) 
						stage = tail;
					tail->type = OPERAND;
					if(!sscanf(s_tocken, "%lf", &(tail->val)))
						perror("sscan:");
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
				if(mode == MULT_DIGIT)
					e_tocken = tock;
				mode = MULT_DIGIT;
				break;
			default:
				printf("error parsing expression symbol %c %ld\n", *tock, tock - str);
				break;
		}
		tock++;
		if(tock == str + lenline){
			if(mode == MULT_DIGIT && s_tocken && e_tocken){
				tail = push_stage(tail, s_tocken, e_tocken);
				if(!stage && tail) 
					stage = tail;
				tail->type = OPERAND;
				if(!sscanf(s_tocken, "%lf", &(tail->val)))
					perror("sscan:");
			}
			break;
		}
	}
	return stage;
}

double calculate(){
	double res;

	Stack *op2 = NULL;
	Stack *op1 = NULL;
	Stack *ac = NULL;

	ac = pop(&head_ac);

	switch (ac->type){
		case PLUS:
			op2 = pop(&head_op);
			op1 = pop(&head_op);
			if(op1 && op2)
				res = op1->val + op2->val;
			else
				printf("error calculate PLUS\n");
			break;
		case MINUS:
			op2 = pop(&head_op);
			op1 = pop(&head_op);
			if(op1 && op2)
				res = op1->val - op2->val;
			else
				printf("error calculate MINUS\n");
			break;
		case DIVIDE:
			op2 = pop(&head_op);
			op1 = pop(&head_op);
			if(op1 && op2)
				res = op1->val / op2->val;
			else
				printf("error calculate DIVIDE\n");
			break;
		case MULTIPLY:
			op2 = pop(&head_op);
			op1 = pop(&head_op);
			if(op1 && op2)
				res = op2->val * op1->val;
			else
				printf("error calculate MULTIPLY\n");
			break;
		case SQRT:
			op2 = pop(&head_op);
			if(op2)
				res = op2->val;
			else
				printf("error calculate SQRT\n");
			break;
		case BREACK_OPEN:
			op2 = pop(&head_op);
			res = op2->val;
			break;
		case BREACK_CLOSE:
			res = calculate();
			break;
		default:
			break;
	}

	if(op2){
		op2->val = res;
		push(&head_op, op2);
	}
	free(op1);
	free(ac);
	return res;
}

int parse(Stack *el){
	if(el->type == UNDEF_TYPE){
		printf("undefined type: %d\n", el->type);
		free(el);
		return 0;
	}
	if(el->type == OPERAND){
		push(&head_op, el);
		return 0;
	}

	// ACTIONS
	//

	if(!head_ac || el->prior > head_ac->prior){
		push(&head_ac, el);
		return 0;
	}
	calculate();
	//free(el);
	//el = NULL;
	return 1;
}

int main(int argc, char** argv){
	if(argc < 2){
		printf("Usage: %s expresion\n", argv[0]);
		return 0;
	}
	Stack *stage = stage_one(argv[1]);
	Stack *el = NULL;
	while(stage){
		el = pop(&stage);
		if(parse(el))
			push(&stage, el);
	}
	while(head_ac){
		calculate();
	}

	if(head_op)
		printf("%lf\n", head_op->val);

	/*
	printf("operands:\n");
	print_stack(head_op);
	printf("actions:\n");
	print_stack(head_ac);
	printf("stage:\n");
	print_stack(stage);
*/
	destroy_head(stage);
	destroy_head(head_op);
	destroy_head(head_ac);
	return EXIT_SUCCESS;
}

