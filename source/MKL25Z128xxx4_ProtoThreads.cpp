/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    MKL25Z128xxx4_ProtoThreads.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "etl/delegate.h"
#include "etl/numeric.h"
#include "fsl_lpsci.h"
#include "SwTimer.h"
#include "UartThread.h"
#include "LedFlasher.h"
#include "common.hpp"

#define SYSTICK_MS 10U
#define SYSTICK_TICKS CLOCK_GetFreq(kCLOCK_CoreSysClk)/(1000/(SYSTICK_MS))

volatile uint64_t glTicks = 0;
volatile bool newCharFlag = false;
volatile char newChar;

uint64_t getRunTimeInMs() {
	return glTicks * SYSTICK_MS;
}

extern "C" void SysTick_Handler() {
	++glTicks;
}

extern "C" void UART0_IRQHandler() {
	newCharFlag = true;
	if ((kLPSCI_RxDataRegFullFlag) & LPSCI_GetStatusFlags(UART0)) {
		newCharFlag = true;
		newChar = LPSCI_ReadByte(UART0);
		LPSCI_WriteByte(UART0, newChar); // Echo
	}
}

// printf retargeting
extern "C" int _write(int iFileHandle, char *pcBuffer, int iLength){
	LPSCI_WriteBlocking(UART0, (uint8_t*)pcBuffer, iLength);
	return iLength;
}

SwTimer swtimer1(SwTimer::callback_f::create<getRunTimeInMs>());
UartThread uartThread(SwTimer::callback_f::create<getRunTimeInMs>());
LedFlasher ledFlasher(swtimer1);
int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();

#if SDK_DEBUGCONSOLE
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	// Setup Systick
	SysTick_Config(SYSTICK_TICKS);
	EnableIRQ(SysTick_IRQn);

	/* Inicializacia UART0 (LPSCI)*/
	CLOCK_SetLpsci0Clock(0x1U); // Zapnutie hodin tu alebo cez GUI clock wizzard

	lpsci_config_t user_config;
	LPSCI_GetDefaultConfig(&user_config);
	user_config.baudRate_Bps = 19200U;
	user_config.enableRx = true;
	user_config.enableTx = true;
	LPSCI_Init(UART0, &user_config, CLOCK_GetPllFllSelClkFreq());
	LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullInterruptEnable);

	/* Enable RX interrupt. */
	LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullInterruptEnable);
	EnableIRQ(UART0_IRQn);

	printf("ProtoThreads demo\n\r");

	/* Enter an infinite loop, just incrementing a counter. */
	swtimer1.startTimer(1000);

	while (1) {
		uartThread.Run();
		ledFlasher.Run();
		/* 'Dummy' NOP to allow source level single stepping of
		 tight while() loop */
		__asm volatile ("nop");
	}
	return 0;
}
