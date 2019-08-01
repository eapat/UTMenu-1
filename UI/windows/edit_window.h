#ifndef __EDIT_WINDOW_H
#define __EDIT_WINDOW_H

#include <stdbool.h>
#include "canvas.h"
#include "value.h"

typedef struct {
	uint32_t prevTime;
	uint8_t shiftFlag;
	uint8_t shift;
}EWShiftString;

typedef struct {
	Font* font;
	Layout layout;
	enum Frame_style style;
}EWArea;


typedef struct {
	Canvas* canvas;
	Layout mainLayout;
	EWArea header;
	EWArea body;
	bool inProgress;
	Value* vlPt;
	Value vlCopy;
	char* headerText;
	EWShiftString shStr;
}EditWindow;

void EditWindow_init(EditWindow* this,Canvas* canvas,Font* headerFont, Font* bodyFont, Layout layout);
void EditWindow_start(EditWindow* this, Value* val, char* header);
void EditWindow_stop(EditWindow* this);
void EditWindow_draw(EditWindow* this, uint32_t currentTime);
void EditWindow_inc(EditWindow* this);
void EditWindow_dec(EditWindow* this);
void EditWindow_enter(EditWindow* this);
void EditWindow_back(EditWindow* this);
bool EditWindow_isRuning(EditWindow* this);

#endif
