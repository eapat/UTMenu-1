#ifndef __MENU_H
#define __MENU_H
#include "value.h"

typedef struct{
	void* child;
	void* next;
	char* text;
	Value* value;
	void (*function)(void);
}MenuItem;

extern MenuItem Null_Menu;
extern MenuItem rootItem;

void Menu_init();

#endif
