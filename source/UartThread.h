/*
 * UartThread.h
 *
 *  Created on: 11. 12. 2019
 *      Author: martin
 */

#ifndef UARTTHREAD_H_
#define UARTTHREAD_H_
#include <common.hpp>
#include "fsl_debug_console.h"
#include "Protothread.h"
#include "SwTimer.h"


class UartThread: public Protothread {
public:
	UartThread(SwTimer::callback_f func):swtimer(func){
	}
	virtual bool Run();


private:
	SwTimer swtimer;
	char ch;
	int len;
	int i;
	char data[100];
	bool isNewCharReady();
};

#endif /* UARTTHREAD_H_ */
