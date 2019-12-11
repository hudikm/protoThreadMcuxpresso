/*
 * LedFlasher.cpp
 *
 *  Created on: 11. 12. 2019
 *      Author: martin
 */

#include <LedFlasher.h>
#include "fsl_gpio.h"
#include "pin_mux.h"

bool LedFlasher::Run() {
	PT_BEGIN()
	;
	swtimer->startTimer(1000);
	while(1)  {
		GPIO_TogglePinsOutput(BOARD_INITPINS_LED_GREEN_GPIO, 1<<BOARD_INITPINS_LED_GREEN_PIN);
		PT_WAIT_UNTIL(swtimer->isExpired());
		swtimer->restartTimer();
	}

PT_END()
;
}

