#ifndef __EDIT_WINDOW_H
#define __EDIT_WINDOW_H

#include <stdbool.h>
#include "canvas.h"
#include "layout.h"
#include "value.h"

typedef struct {
	Canvas* canvas;
	Font* font;
	Layout layout;
	bool inProgress;
	Value* val;
}EditWindow;

void EditWindow_init(EditWindow* this,Canvas* canvas,Font* font,Layout layout);
void EditWindow_start(EditWindow* this, Value* val);
void EditWindow_draw(EditWindow* this);
void EditWindow_inc(EditWindow* this);
void EditWindow_dec(EditWindow* this);
void EditWindow_enter(EditWindow* this);
void EditWindow_back(EditWindow* this);


#endif
