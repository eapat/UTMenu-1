#ifndef __EDIT_WINDOW_H
#define __EDIT_WINDOW_H

#include <stdbool.h>
#include "canvas.h"
#include "value.h"

typedef struct {
	Canvas* canvas;
	Font* headerFont;
	Font* bodyFont;
	Layout mainLayout;
	Layout headerLayout;
	Layout bodyLayout;
	enum Frame_style headerStyle;
	enum Frame_style bodyStyle;
	bool inProgress;
	Value* vlPt;
	Value vlCopy;
	char* headerText;
}EditWindow;

void EditWindow_init(EditWindow* this,Canvas* canvas,Font* headerFont, Font* bodyFont, Layout layout);
void EditWindow_start(EditWindow* this, Value* val, char* header);
void EditWindow_stop(EditWindow* this);
void EditWindow_draw(EditWindow* this);
void EditWindow_inc(EditWindow* this);
void EditWindow_dec(EditWindow* this);
void EditWindow_enter(EditWindow* this);
void EditWindow_back(EditWindow* this);


#endif
