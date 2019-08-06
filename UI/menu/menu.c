#include "menu.h"
#include "stdio.h"
#include "stdbool.h"

void someFunc();

#define MAKE_ITEM(Name, Child, Next, Text, Value, Function) \
		extern MenuItem Child; \
		extern MenuItem Next;   \
		MenuItem Name = {(void*)&Child,(void*)&Next, { Text },Value, Function}

Value valueBool;
Value valueInt;
Value valueFloat;
Value valueEnum;


MenuItem   Null_Menu = {NULL, NULL, {0x00}};

MenuItem rootItem;

MAKE_ITEM(rootItem,item1,Null_Menu,"root",NULL,NULL);
	MAKE_ITEM(item1,item11,item2,"������� 1",&valueInt,NULL);
		MAKE_ITEM(item11,Null_Menu,item12,"������� 11",&valueBool,NULL);
		MAKE_ITEM(item12,Null_Menu,item13,"������� 12 �������",&valueInt,NULL);
		MAKE_ITEM(item13,Null_Menu,item14,"������� 13",&valueFloat,NULL);
		MAKE_ITEM(item14,Null_Menu,item15,"������� 14",&valueEnum,NULL);
		MAKE_ITEM(item15,Null_Menu,Null_Menu,"�����",&valueEnum,someFunc);
	MAKE_ITEM(item2,item21,item3,"������� 2 �������",NULL,NULL);
		MAKE_ITEM(item21,item211,Null_Menu,"��������� 1",NULL,NULL);
			MAKE_ITEM(item211,item2111,Null_Menu,"��������� 2",NULL,NULL);
				MAKE_ITEM(item2111,item21111,Null_Menu,"��������� 3",NULL,NULL);
					MAKE_ITEM(item21111,rootItem,Null_Menu,"��������� 4",NULL,NULL);
	MAKE_ITEM(item3,Null_Menu,item4,"������� 3",NULL,NULL);
	MAKE_ITEM(item4,Null_Menu,Null_Menu,"������� 4",NULL,NULL);



Value value;
char * unitsBool []={"���.","����."};
char * unitsInt[]={"�"};
char * unitsFloat[]={"�"};
char * unitsEnum []={"����1","����2","����3","����4","����5","����6"};

extern bool testBool;
extern int testInt;
extern float testFloat;
extern uint8_t testEnum;

void someFunc(){
	testInt=0;
}

void Menu_init(){

	Value_initAsBool(&valueBool,&testBool,unitsBool);
	Value_initAsInt(&valueInt,&testInt,unitsInt,0,50);
	Value_initAsFloat(&valueFloat,&testFloat,unitsFloat,0,50,2);
	Value_initAsEnum(&valueEnum,&testEnum,unitsEnum,0,5);
}


