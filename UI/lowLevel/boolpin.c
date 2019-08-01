#include "boolpin.h"

/*
 * Инициализация кнопки
 *	uint16_t debounce_ms-задержка для антидребезга в мс
 *	uint16_t delay_ms-задержка перед началом автоклика в мс
 *	uint16_t period_ms-периодичность автоклика в мс
 */
void Boolpin_init(Boolpin* boolpin,GPIO_TypeDef* port,uint16_t pin,uint16_t debounce_ms,uint16_t delay_ms,uint16_t period_ms)
{
	boolpin->port=port;
	boolpin->pin=pin;
	boolpin->debounce_ms=debounce_ms;
	boolpin->delay_ms=delay_ms;
	boolpin->defaultPeriod_ms=period_ms;
	boolpin->autoClick=(delay_ms!=0);
	boolpin->start=0;
	boolpin->state=false;
  
	boolpin->flag=0;
	boolpin->flagLong=0;
	boolpin->event=0;
	boolpin->currentPeriod_ms=boolpin->defaultPeriod_ms;
}

/*
 * Обновление состояния структуры
 *
 *	возвращает true, если возникло событие и сбрасывает его
 */
bool Boolpin_update(Boolpin* boolpin) {

    bool ps=HAL_GPIO_ReadPin(boolpin->port,boolpin->pin);
  
    if ((!boolpin->flag)&&(!ps)) {
    	boolpin->start = HAL_GetTick();
    	boolpin->flag = true;
    }
    else if ((boolpin->flag)&&(ps)) {
    	boolpin->flag = false;
    	boolpin->state = false;
    	boolpin->currentPeriod_ms=boolpin->defaultPeriod_ms;
    }
    
    else if ((boolpin->state==false) && (boolpin->flag)&&((HAL_GetTick()-boolpin->start)>boolpin->debounce_ms)) {
    	boolpin->event = true;
    	boolpin->state = true;
    }
    else if(boolpin->autoClick && boolpin->state && !boolpin->flagLong){
    	boolpin->flagLong=true;
    	boolpin->start=HAL_GetTick();
    }
    
    else if (boolpin->state && (boolpin->flagLong)&&((HAL_GetTick()-boolpin->start)>boolpin->delay_ms)) {
    	boolpin->event=true;
    	boolpin->start=boolpin->start+boolpin->currentPeriod_ms;

    	if(boolpin->currentPeriod_ms>100)
    	{
    		boolpin->currentPeriod_ms=(int)(boolpin->currentPeriod_ms*0.9);
    	}
    }
    

    if(boolpin->event){
    	boolpin->event=false;
        return true;
      }

      return boolpin->event;

}


