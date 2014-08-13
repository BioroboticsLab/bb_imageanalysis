#include "Timer.h"

using namespace std;

void Timer::startTimer() {
	t1 = clock();
}

double Timer::stopTimer( ) {
	t2 = clock();
	return (double)(t2 - t1) / CLOCKS_PER_SEC;
	//return difftime(this->t2, this->t1);
	//cout << "time spent: " << runtime << "s or " << runtime/60 << "m" << endl;
}
