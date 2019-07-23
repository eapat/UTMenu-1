#include "menu.h"

#define MAKE_ITEM(Name, Child, Next, Text) \
		extern MenuItem Child; \
		extern MenuItem Next;   \
		MenuItem Name = {(void*)&Child,(void*)&Next, { Text }}


MenuItem   Null_Menu = {(void*)0, (void*)0, {0x00}};

MenuItem itemRoot;

MAKE_ITEM(rootItem,item1,Null_Menu,"root");
MAKE_ITEM(item1,Null_Menu,Null_Menu,"item1");
