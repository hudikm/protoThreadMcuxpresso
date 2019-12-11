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
#include "vector.h"
#include "numeric.h"
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

void print(const etl::ivector<int>& v) {
	for (int i : v) {
		PRINTF("%d \n\r", i);
	}
}

extern "C" void UART0_IRQHandler() {
	newCharFlag = true;
	newChar = GETCHAR();
	PUTCHAR(newChar);
}

SwTimer swtimer1(&getRunTimeInMs);
UartThread uartThread;
LedFlasher ledFlasher;
int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	// Setup Systick
	SysTick_Config(SYSTICK_TICKS);
	EnableIRQ(SysTick_IRQn);

	PRINTF("Hello World\n");

	etl::vector<int, 10> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	print(v1);

	/* Enable RX interrupt. */
	LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullInterruptEnable);
	EnableIRQ(UART0_IRQn);

	/* Force the counter to be placed into memory. */
	volatile static int i = 0;
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
