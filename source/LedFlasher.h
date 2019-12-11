/*
 * LedFlasher.h
 *
 *  Created on: 11. 12. 2019
 *      Author: martin
 */

#ifndef LEDFLASHER_H_
#define LEDFLASHER_H_
#include "Protothread.h"
#include "SwTimer.h"
#include "common.hpp"

class LedFlasher : public Protothread {
	public:
	LedFlasher(){
		swtimer = new SwTimer(&getRunTimeInMs);
	}
     virtual bool Run();
	private:
     SwTimer *swtimer;
};

#endif /* LEDFLASHER_H_ */
