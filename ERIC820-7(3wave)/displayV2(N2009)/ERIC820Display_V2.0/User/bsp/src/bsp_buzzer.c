
#include "bsp.h"
uint8_t BuzzerMode;
/*
---------------------------------------------------------------------
Name		BuzzerTask
Function	������������
Input		��
OutPut		��
Edit by seleen hu
Date		2019-04-08
---------------------------------------------------------------------
*/
void BuzzerTask(void)
{
	static uint8_t BuzzerTim;
	if(BuzzerPIN()){
		BuzzerTim++;
		if(0 == BuzzerMode){
			if(BuzzerTim>=10){
				BuzzerTim = 0;			
				BuzzerOFF();
			}
		}else if(1 == BuzzerMode){
			if(BuzzerTim>=80){
				BuzzerTim = 0;			
				BuzzerOFF();
				BuzzerMode = 0;
			}
		}
	}
}//BuzzerTask

