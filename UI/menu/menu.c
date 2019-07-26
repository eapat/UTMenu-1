#include "menu.h"
#include "stdio.h"

#define MAKE_ITEM(Name, Child, Next, Text) \
		extern MenuItem Child; \
		extern MenuItem Next;   \
		MenuItem Name = {(void*)&Child,(void*)&Next, { Text }}


MenuItem   Null_Menu = {NULL, NULL, {0x00}};

MenuItem rootItem;

MAKE_ITEM(rootItem,item1,Null_Menu,"root");
	MAKE_ITEM(item1,item11,item2,"item1");
		MAKE_ITEM(item11,item111,item12,"item11");
			MAKE_ITEM(item111,Null_Menu,Null_Menu,"item111");
		MAKE_ITEM(item12,item121,item13,"item12");
			MAKE_ITEM(item121,Null_Menu,item122,"item121");
			MAKE_ITEM(item122,Null_Menu,Null_Menu,"item123");
		MAKE_ITEM(item13,item131,Null_Menu,"item13");
			MAKE_ITEM(item131,Null_Menu,item132,"item131");
			MAKE_ITEM(item132,Null_Menu,item133,"item132");
			MAKE_ITEM(item133,Null_Menu,Null_Menu,"item133");

	MAKE_ITEM(item2,Null_Menu,item3,"item2");
	MAKE_ITEM(item3,Null_Menu,item4,"item3");
	MAKE_ITEM(item4,Null_Menu,item5,"item4");
	MAKE_ITEM(item5,Null_Menu,item6,"item5");
	MAKE_ITEM(item6,Null_Menu,item7,"item6");
	MAKE_ITEM(item7,Null_Menu,item8,"item7");
	MAKE_ITEM(item8,Null_Menu,item9,"item8");
	MAKE_ITEM(item9,Null_Menu,Null_Menu,"item9");
