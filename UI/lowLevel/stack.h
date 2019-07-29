#ifndef __STACK_H
#define __STACK_H
#include "menu.h"

#define STACK_MAX_SIZE 20

typedef struct{
	MenuItem*data[STACK_MAX_SIZE];
	int pos;
}Stack;

void Stack_push(Stack* stack,MenuItem* menuItem);
void Stack_pop(Stack* stack);
MenuItem* Stack_top(Stack* stack);

#endif
