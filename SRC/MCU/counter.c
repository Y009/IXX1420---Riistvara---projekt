/*
 * counter.c
 *
 *  Created on: 31. okt 2016
 *      Author: Y009
 */

unsigned long int Counter = 0;

void counter_setCounter(int x){
	if (x!=1)
		Counter = x;		//on seda osa vaja v]i kogu aeg lihtsalt +1?
	else
		Counter += x;
}

unsigned long int counter_getCounter(void){
  return Counter;
}

void counter_checkFlag(){
	timer_diTAI();
	if(getFlag()){
		setCounter(1); 	// v]i teen siin lihtsalt Counter++;
		setFlag(0);		/* Clear timerA interrupt up flag */
	}
	timer_enTAI();
}
