#include "menu.h"
#include "stdio.h"
#include "stdbool.h"

#define MAKE_ITEM(Name, Child, Next, Text, Value) \
		extern MenuItem Child; \
		extern MenuItem Next;   \
		MenuItem Name = {(void*)&Child,(void*)&Next, { Text },Value}

Value valueBool;
Value valueInt;
Value valueFloat;
Value valueEnum;


MenuItem   Null_Menu = {NULL, NULL, {0x00}};

MenuItem rootItem;

MAKE_ITEM(rootItem,item1,Null_Menu,"root",NULL);
	MAKE_ITEM(item1,item11,item2,"item1 long item very",NULL);
		MAKE_ITEM(item11,Null_Menu,item12,"item11 long item very",&valueBool);
		MAKE_ITEM(item12,Null_Menu,item13,"item12 long item very",&valueInt);
		MAKE_ITEM(item13,Null_Menu,item14,"item13 long item very",&valueFloat);
		MAKE_ITEM(item14,Null_Menu,Null_Menu,"item14 long item very",&valueEnum);

	MAKE_ITEM(item2,Null_Menu,item3,"какой то длинный текст",NULL);
	MAKE_ITEM(item3,Null_Menu,item4,"item3 long item very",NULL);
	MAKE_ITEM(item4,Null_Menu,item5,"item4 long item very",NULL);
	MAKE_ITEM(item5,Null_Menu,item6,"item5 long item very",NULL);
	MAKE_ITEM(item6,Null_Menu,item7,"item6 long item very",NULL);
	MAKE_ITEM(item7,Null_Menu,item8,"item7 long item very",NULL);
	MAKE_ITEM(item8,Null_Menu,item9,"item8 long item very",NULL);
	MAKE_ITEM(item9,Null_Menu,Null_Menu,"item9 long item very",NULL);

Value value;
char * unitsBool []={"Вкл.","Откл."};
char * unitsInt[]={"Ц"};
char * unitsFloat[]={"П"};
char * unitsEnum []={"Сост1.","Сост2","Сост3","Сост4","Сост5","Сост6"};

extern bool testBool;
extern int testInt;
extern float testFloat;
extern uint8_t testEnum;


void Menu_init(){
	Value_initAsBool(&valueBool,&testBool,unitsBool);
	Value_initAsInt(&valueInt,&testInt,unitsInt,0,50);
	Value_initAsFloat(&valueFloat,&testFloat,unitsFloat,0,50,2);
	Value_initAsEnum(&valueEnum,&testEnum,unitsEnum,0,5);


}
