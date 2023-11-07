#ifndef __STACK_H__
#define __STACK_H__

typedef enum _type{
	UNDEF_TYPE = 0,
	OPERAND,
	ACTION
}Type;

typedef enum _mode{
	UNDEF_MODE = 0,
	SINGL_CHAR,
	SINGL_DIGIT,
	MULT_CHAR,
	MULT_DIGIT,
	END
}MODE;

typedef struct stack{
	char *s_val;
	char *e_val;
	Type type;
	struct stack *next;
}Stack;
 
Stack *push_stage(Stack *stage, char *s_t, char *e_t);
void push(Stack **st, Stack *el);
Stack *pop(Stack **st);
void print_stack(Stack *st);
void destroy_head(Stack *st);

#endif
