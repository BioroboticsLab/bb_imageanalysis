#ifndef TIMER_H_
#define	TIMER_H_

#include <iostream>
#include <ostream>
#include <time.h>

class Timer {
private:
	clock_t t1, t2;
public:

	/**
	 * starts the timer
	 */
	void startTimer();

	/**
	 * stops the timer and returns the time since the start
	 *
	 * @return time since start of timer
	 */
	double stopTimer();
};
#endif
