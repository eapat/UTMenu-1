#include "LCD12864.h"

//Процедура выдачи байта в индикатор
void LCDWriteByte(uint8_t data, uint8_t cd) {
	if (cd == 1) HAL_GPIO_WritePin(LCD_A0_PORT, LCD_A0_PIN, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_A0_PORT, LCD_A0_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_E1_PORT, LCD_E1_PIN, GPIO_PIN_SET);

	if ((data & 0x01) == 0x01) HAL_GPIO_WritePin(LCD_DB0_PORT, LCD_DB0_PIN, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_DB0_PORT, LCD_DB0_PIN, GPIO_PIN_RESET);

	if ((data & 0x02) == 0x02) HAL_GPIO_WritePin(LCD_DB1_PORT, LCD_DB1_PIN, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_DB1_PORT, LCD_DB1_PIN, GPIO_PIN_RESET);

	if ((data & 0x04) == 0x04) HAL_GPIO_WritePin(LCD_DB2_PORT, LCD_DB2_PIN, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_DB2_PORT, LCD_DB2_PIN, GPIO_PIN_RESET);

	if ((data & 0x08) == 0x08) HAL_GPIO_WritePin(LCD_DB3_PORT, LCD_DB3_PIN, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_DB3_PORT, LCD_DB3_PIN, GPIO_PIN_RESET);

	if ((data & 0x10) == 0x10) HAL_GPIO_WritePin(LCD_DB4_PORT, LCD_DB4_PIN, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_DB4_PORT, LCD_DB4_PIN, GPIO_PIN_RESET);

	if ((data & 0x20) == 0x20) HAL_GPIO_WritePin(LCD_DB5_PORT, LCD_DB5_PIN, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_DB5_PORT, LCD_DB5_PIN, GPIO_PIN_RESET);

	if ((data & 0x40) == 0x40) HAL_GPIO_WritePin(LCD_DB6_PORT, LCD_DB6_PIN, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_DB6_PORT, LCD_DB6_PIN, GPIO_PIN_RESET);

	if ((data & 0x80) == 0x80) HAL_GPIO_WritePin(LCD_DB7_PORT, LCD_DB7_PIN, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_DB7_PORT, LCD_DB7_PIN, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, GPIO_PIN_SET);


	for (int i = 0; i < 300; i++) asm("NOP");

	HAL_GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_E1_PORT, LCD_E1_PIN, GPIO_PIN_RESET);
}

void LCD_WriteData(uint8_t data) {
    LCDWriteByte(data, 1);
}

void LCD_WriteComand(uint8_t data) {
  LCDWriteByte(data, 0);
}

void LCD_init() {
	HAL_GPIO_WritePin(LCD_DB0_PORT,LCD_DB0_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DB1_PORT,LCD_DB1_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DB2_PORT,LCD_DB2_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DB3_PORT,LCD_DB3_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DB4_PORT,LCD_DB4_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DB5_PORT,LCD_DB5_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DB6_PORT,LCD_DB6_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DB7_PORT,LCD_DB7_PIN,GPIO_PIN_RESET);

	HAL_GPIO_WritePin(LCD_E1_PORT,LCD_E1_PIN,GPIO_PIN_RESET);	//CS
	HAL_GPIO_WritePin(LCD_RES_PORT,LCD_RES_PIN,GPIO_PIN_RESET);	//RES
	HAL_GPIO_WritePin(LCD_RW_PORT,LCD_RW_PIN,GPIO_PIN_RESET);	//RW
	HAL_GPIO_WritePin(LCD_A0_PORT,LCD_A0_PIN,GPIO_PIN_RESET);	//DC
	HAL_GPIO_WritePin(LCD_E_PORT,LCD_E_PIN,GPIO_PIN_RESET);		//E

	HAL_GPIO_WritePin(LCD_DISP_PORT,LCD_DISP_PIN,GPIO_PIN_SET);	//DISP

	HAL_GPIO_WritePin(LCD_RES_PORT, LCD_RES_PIN, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD_RES_PORT, LCD_RES_PIN, GPIO_PIN_SET);

	uint8_t cmd[] = {
		0xae,				// LCD off (reset)
		0x00,				// set low column address
		0x10,				// set high column address
		0x40,				// display start set
		0x21, 0, 127,		// Set Column Address 0..127
		0x22, 0, 7,			// Set Page Address 0..7
		0x2e,				// stop horizontal scroll
		0xb0,				// page address
		0x81, 0xF0,			// Set contrast control register
		0xa0,				// set segment re-map
		0xa4,				// normal display mode
		0xa6,				// set normal/inverse display
		0xa8, 				// Set multiplex ratio
		0x3f, 0xd3, 0x00,	// set display offset
		0xad, 0x8e,			// Set dc/dc on/off
		0xc0,				// Set com output scan direction
		0xd5, 0xf0,			// Set display clock divide ratio/oscillator/frequency
		0xd8, 0x05,			// Set area color mode on/off & low power display mode
		0xd9, 0xc2,			// Set pre-charge period
		0xda, 0x12,			// Set com pins hardware configuration
		0xdb, 0x08,			// Set vcom deselect level
		0xaf				// Set Display ON
	};

	for (int i = 0; i < sizeof(cmd); i++)
		LCD_WriteComand(cmd[i]);
}

void LCD_reset(void) {
	uint8_t cmd[] = {
		0x00,				// set low column address
		0x10,				// set high column address
		0x40,				// display start set
		0x21, 0, 127,		// Set Column Address 0..127
		0x22, 0, 7			// Set Page Address 0..7
	};
	for (int i = 0; i < sizeof(cmd); i++)
		LCD_WriteComand(cmd[i]);
}

void LCD_draw(Canvas* canvas) {
	if (canvas->bitmap != NULL) {
		uint8_t rows = canvas->height / 8;
		if (canvas->height % 8 > 0) rows++;
		for (uint8_t j = 0; j < rows; j++) {
			LCD_WriteComand(0xB0 + j);
			for (uint8_t i = 0; i < canvas->width; i++) {
				if ((i < canvas->width)&(j < 8)) {
					LCD_WriteData(canvas->bitmap[j*canvas->width + i]);
				}
			}
		}
		LCD_WriteComand(0xB0 + rows);
	}
}
