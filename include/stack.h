#ifndef __STACK_H__
#define __STACK_H__

typedef enum _type{
	UNDEF_TYPE = 0,
	OPERAND,
	PLUS,
	MINUS,
	DIVIDE,
	MULTIPLY,
	SQRT,
	BREACK_OPEN,
	BREACK_CLOSE
}Type;

typedef enum _mode{
	UNDEF_MODE = 0,
	SINGL_CHAR,
	SINGL_DIGIT,
	MULT_CHAR,
	MULT_DIGIT,
	END
}MODE;

typedef enum _prioritet{
	UNDEF_PRIOR = 0,
	P_PLUS = 1,
	P_MINUS = 1,
	P_DIVIDE = 2,
	P_MULTIPLY = 2,
	P_SQRT = 3,
	P_BREACK_CLOSE = 4,
	P_BREACK_OPEN = 5
}Prior;

typedef struct stack{
	char			*s_val;
	char			*e_val;
	double			val;
	Type			type;
	Prior			prior;
	struct stack	*next;
}Stack;
 
Stack *push_stage(Stack *stage, char *s_t, char *e_t);
void push(Stack **st, Stack *el);
Stack *pop(Stack **st);
void print_stack(Stack *st);
void destroy_head(Stack *st);

#endif
