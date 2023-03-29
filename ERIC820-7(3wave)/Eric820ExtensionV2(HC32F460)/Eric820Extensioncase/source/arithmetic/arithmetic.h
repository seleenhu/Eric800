#ifndef _ARITHMETIC_H_
#define _ARITHMETIC_H_

extern uint8_t VerifyFlag;
extern uint8_t StartCalRmsFlag;

void CalRMS(uint16_t bt);
void CalVerify(uint16_t bt);
void SelCalRMS(void);

#endif
/*----------------------------end---------------------------*/

