/* Файл реализует функции стэка из MenuItem* */
#include "stack.h"

void Stack_push(Stack* stack,MenuItem* menuItem){
	if(stack->pos<STACK_MAX_SIZE-1)
		stack->data[stack->pos++]=menuItem;
}

MenuItem* Stack_top(Stack* stack){
	if(stack->pos>0)
		return stack->data[stack->pos-1];
	else
		return &Null_Menu;
}

void Stack_pop(Stack* stack){
	if(stack->pos>0)
		stack->pos--;
}

void Stack_clear(Stack* stack){
	stack->pos=1;
}

