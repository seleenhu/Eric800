
#ifndef _BSP_U9300C_H_
#define _BSP_U9300C_H_

uint8_t UIS891X_CPIN(void);
uint8_t UIS891X_CREG(void);
uint8_t UIS891X_CGATT(void);
uint8_t UIS891X_CIPMUX(uint8_t Cipmux);
uint8_t UIS891X_CIPHEAD(uint8_t CIPHEAD);
uint8_t UIS891X_CSTT(void);
uint8_t UIS891X_CIICR(void);
uint8_t UIS891X_CIFSR(void);
uint8_t UIS891X_CIPSTART(uint8_t *ipbuf,uint16_t severport);
uint8_t UIS891X_CIPSEND(uint8_t socketdata,uint8_t *buf, uint16_t buflen);
uint8_t UIS891X_TCPSEND(uint8_t socketdata,uint8_t *buf, uint16_t buflen);
uint8_t UIS891X_NormalRxDisplay(uint8_t socketdata,uint8_t *buf, uint16_t *length);
uint8_t UIS891X_NormalDisplay(uint8_t socketdata,uint8_t *buf,uint16_t *length);
uint8_t UIS891X_CIPACK(uint8_t socketdata,uint16_t len);


#endif


