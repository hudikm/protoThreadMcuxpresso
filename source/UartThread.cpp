/*
 * UartThread.cpp
 *
 *  Created on: 11. 12. 2019
 *      Author: martin
 */

#include <UartThread.h>

bool UartThread::Run() {
	PT_BEGIN()
	;

	while (true) {
		// wait for sync byte
		PT_WAIT_UNTIL(isNewCharReady());
		ch = newChar;
		if (ch == 'a') {

			PT_WAIT_UNTIL(isNewCharReady());
			len = newChar - 0x30;
			// read n data bytes
			swtimer->startTimer(2000);
			for (i = 0; i < len; i++) {
				swtimer->restartTimer();
				PT_WAIT_UNTIL(isNewCharReady() || swtimer->isExpired());
				if(swtimer->isExpired()) break;
				data[i] = newChar;
			}
			data[i++] = '\0';
			PRINTF("\n\r%s",data);
		}
	}

PT_END()
;
}

bool UartThread::isNewCharReady() {
if (newCharFlag) {
	newCharFlag = false;
	return true;
} else
	return false;
}
