/*
 * counter.c
 *
 *  Created on: 31. okt 2016
 *      Author: Y009
 */

unsigned long int Counter = 0;

void setCounter(int x){
	if (x!=1)
		Counter = x;		//on seda osa vaja v]i kogu aeg lihtsalt +1?
	else
		Counter += x;
}

unsigned long int getCounter(void){
  return Counter;
}

void checkFlag(){
	diTAI();
	if(getFlag()){
		setCounter(1); 	// v]i teen siin lihtsalt Counter++;
		setFlag(0);		/* Clear timerA interrupt up flag */
	}
	enTAI();
}

//kuidas disable, enable peaks siin olema?
