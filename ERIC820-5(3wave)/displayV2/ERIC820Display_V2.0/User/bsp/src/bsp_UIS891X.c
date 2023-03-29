#include "bsp.h"



//extern void  U4GSendTxbuf(u16 buflen);

/*
---------------------------------------------------------------------
Name		UIS891X_CPIN
Function	CPIN
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_CPIN(void)
{

	static uint8_t step,stepTim;;
	uint8_t com_len;
	uint8_t rt,flag;
	uint16_t pt;	
	
	uint8_t *buf;	
	buf = TX4Gbuff2;

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CPIN?\r\n");
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			
			stepTim++;
			if(stepTim<=20){
				flag = 0;
				com_len = sprintf((char *)buf,"AT+CPIN?\r");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				com_len = sprintf((char *)buf,"+CPIN: READY");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step = 0;
					stepTim = 0;
					rt = 0;
				}else{
					rt = 1;
				}				
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;	
}//UIS891X_CPIN

/*
---------------------------------------------------------------------
Name		UIS891X_CREG
Function	CPIN
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_CREG(void)
{

	static uint8_t step,stepTim;;
	uint8_t com_len;
	uint8_t rt,flag;
	uint16_t pt;
	static uint16_t bt_back;
	
	uint8_t *buf;	
	buf = TX4Gbuff2;

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CREG?\r\n");
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			
			stepTim++;
			if(stepTim<=20){
				flag = 0;
				com_len = sprintf((char *)buf,"AT+CREG?\r");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						bt_back = RX4G_bt;
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				com_len = sprintf((char *)buf,"OK");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					flag = 0;
					com_len = sprintf((char *)buf,"+CREG:");
					while(FIFO_length(bt_back,RX4G_bt,RX4Gbuff_len)>=com_len){
						if(0 == FIFO_bufcmp(RX4Gbuff,buf,bt_back,com_len,RX4Gbuff_len)){
							bt_back = FIFO_pointer(bt_back,com_len,RX4Gbuff_len);
							flag++;
							break;
						}else{
							bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);
						}
						
					}
					if(flag){
						flag = 0;
						while(bt_back != RX4G_bt){
							if(',' == RX4Gbuff[bt_back]){
								bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);
								flag++;
								break;
							}else{
								bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);
							}
							
						}
					}
					if(flag){
						flag = 0;
						while(bt_back != RX4G_bt){
							if(isdigit(RX4Gbuff[bt_back])){							
								flag++;
								break;
							}else{
								bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);
							}
							
						}
					}
					if(flag){
						flag = 0;
						if(('1' == RX4Gbuff[bt_back])||
						   ('5' == RX4Gbuff[bt_back])){
						   flag++;
						}
					}
					if(flag){
						rt = 0;
					}else{
						rt = 2;
					}
					step = 0;
					stepTim = 0;
				}else{
					rt = 1;
				}				
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;	
}//UIS891X_CREG

/*
---------------------------------------------------------------------
Name		UIS891X_CGATT
Function	CPIN
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_CGATT(void)
{

	static uint8_t step,stepTim;;
	uint8_t com_len;
	uint8_t rt,flag;
	uint16_t pt;	
	
	uint8_t *buf;	
	buf = TX4Gbuff2;

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CGATT=1\r\n");
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			
			stepTim++;
			if(stepTim<=20){
				flag = 0;
				com_len = sprintf((char *)buf,"AT+CGATT=1\r");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				com_len = sprintf((char *)buf,"OK");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step = 0;
					stepTim = 0;
					rt = 0;
				}else{
					rt = 1;
				}				
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;	
}//UIS891X_CGATT

/*
---------------------------------------------------------------------
Name		UIS891X_CIPMUX
Function	CPIN
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_CIPMUX(uint8_t Cipmux)
{

	static uint8_t step,stepTim;;
	uint8_t com_len;
	uint8_t rt,flag;
	uint16_t pt;	
	
	uint8_t *buf;	
	buf = TX4Gbuff2;

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CIPMUX=%d\r\n",Cipmux);
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			
			stepTim++;
			if(stepTim<=20){
				flag = 0;
				com_len = sprintf((char *)buf,"AT+CIPMUX=%d",Cipmux);				
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				com_len = sprintf((char *)buf,"OK");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step = 0;
					stepTim = 0;
					rt = 0;
				}else{
					rt = 1;
				}				
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;	
}//UIS891X_CIPMUX
/*
---------------------------------------------------------------------
Name		UIS891X_CIPHEAD
Function	CPIN
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_CIPHEAD(uint8_t CIPHEAD)
{

	static uint8_t step,stepTim;;
	uint8_t com_len;
	uint8_t rt,flag;
	uint16_t pt;	
	
	uint8_t *buf;	
	buf = TX4Gbuff2;

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CIPHEAD=%d\r\n",CIPHEAD);
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			
			stepTim++;
			if(stepTim<=20){
				flag = 0;
				com_len = sprintf((char *)buf,"AT+CIPHEAD=%d",CIPHEAD);				
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				com_len = sprintf((char *)buf,"OK");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step = 0;
					stepTim = 0;
					rt = 0;
				}else{
					rt = 1;
				}				
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;	
}//UIS891X_CIPHEAD
/*
---------------------------------------------------------------------
Name		UIS891X_CSTT
Function	CPIN
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_CSTT(void)
{

	static uint8_t step,stepTim;;
	uint8_t com_len;
	uint8_t rt,flag;
	uint16_t pt;	
	
	uint8_t *buf;	
	buf = TX4Gbuff2;

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CSTT=\"CMNET\",\"\",\"\"\r\n");
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			
			stepTim++;
			if(stepTim<=20){
				flag = 0;
				com_len = sprintf((char *)buf,"AT+CSTT=\"CMNET\",\"\",\"\"");				
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				com_len = sprintf((char *)buf,"OK");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step = 0;
					stepTim = 0;
					rt = 0;
				}else{
					rt = 1;
				}				
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;	
}//UIS891X_CSTT
/*
---------------------------------------------------------------------
Name		UIS891X_CIICR
Function	CPIN
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_CIICR(void)
{

	static uint8_t step,stepTim;;
	uint8_t com_len;
	uint8_t rt,flag;
	uint16_t pt;	
	
	uint8_t *buf;	
	buf = TX4Gbuff2;

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CIICR\r\n");
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			
			stepTim++;
			if(stepTim<=20){
				flag = 0;
				com_len = sprintf((char *)buf,"AT+CIICR");				
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				com_len = sprintf((char *)buf,"OK");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step = 0;
					stepTim = 0;
					rt = 0;
				}else{
					rt = 1;
				}				
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;	
}//UIS891X_CIICR
/*
---------------------------------------------------------------------
Name		UIS891X_CIICR
Function	CPIN
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_CIFSR(void)
{

	static uint8_t step,stepTim;;
	uint8_t i,com_len;
	uint8_t rt,flag;
	uint16_t pt;	
	static uint16_t bt_back;
	uint8_t *buf,ipbuf[4];	
	buf = TX4Gbuff2;

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CIFSR\r\n");
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			
			stepTim++;
			if(stepTim<=20){
				flag = 0;
				com_len = sprintf((char *)buf,"AT+CIFSR");				
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						bt_back = RX4G_bt;
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				com_len = sprintf((char *)buf,"OK");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					flag  = 0;
					com_len = sprintf((char *)buf,"IPV4:");
					while(FIFO_length(bt_back, RX4G_bt, RX4Gbuff_len)>=com_len){
						if(0 == FIFO_bufcmp(RX4Gbuff,buf,bt_back,com_len,RX4Gbuff_len)){
							bt_back = FIFO_pointer(bt_back,com_len,RX4Gbuff_len);
							flag = 1;
							break;
						}else{
							bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);
						}						
					}
					if(flag){						

						for(i=0;i<4;i++){
							flag = 0;
							while(bt_back != RX4G_bt){
								if(isdigit(RX4Gbuff[bt_back])){
									flag = 1;
									break;
								}else{
									bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);
								}
							}
							if(0 == flag){
								break;
							}
							ipbuf[i] = 0;
							flag = 0;
							while(bt_back != RX4G_bt){
								if(isdigit(RX4Gbuff[bt_back])){
									flag = 1;
									ipbuf[i] = ipbuf[i]*10 + (RX4Gbuff[bt_back] - '0');
									bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);									
								}else{
									break;									
								}
							}
							if(0 == flag){
								break;
							}						
						}
						if(i<4){
							rt = 2;
						}else{
							flag = 0;
							for(i=0;i<4;i++){
								if(ipbuf[i]){
									flag++;
								}
							}
							if(flag){
								rt = 0;
							}else{
								rt = 2;
							}
						}
					}else{
						rt = 2;
					}
					step = 0;
					stepTim = 0;
					
				}else{
					rt = 1;
				}				
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;	
}//UIS891X_CIFSR

/*
---------------------------------------------------------------------
Name		UIS891X_CIPSTART
Function	 CIPSTART
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_CIPSTART(uint8_t *ipbuf,uint16_t severport)
{
	
	static uint8_t step;
	static uint16_t stepTim;
	uint8_t com_len;
	uint8_t rt,flag;
	uint16_t pt;	
	
	uint8_t *buf;	
	buf = TX4Gbuff2;

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CIPSTART=\"TCP\",\"%d.%d.%d.%d\",%d\r\n",ipbuf[0],ipbuf[1],ipbuf[2],ipbuf[3],severport);
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:			
			stepTim++;
			if(stepTim<=20){
				flag = 0;
				com_len = sprintf((char *)buf,"AT+CIPSTART=\"TCP\",\"%d.%d.%d.%d\",%d",ipbuf[0],ipbuf[1],ipbuf[2],ipbuf[3],severport);
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:			
			stepTim++;
			if(stepTim<=20){
				flag = 0;
				com_len = sprintf((char *)buf,"OK");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;	
			
		case 3:
			stepTim++;
			if(stepTim<=3000){
				flag = 0;
				com_len = sprintf((char *)buf,"CONNECT OK");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step=0;
					stepTim = 0;
					rt = 0;
				}else{
					rt = 1;
				}
			}else{
				step = 0;
				rt = 2;
			}
			break;		
		
	}
	return rt;	
}//UIS891X_CIPSTART


/*
---------------------------------------------------------------------
Name		UIS891X_CIPSEND
Function	 命令TCPSEND
Input		No
OutPut	0 OK  >=1 Err
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_CIPSEND(uint8_t socketdata,uint8_t *buf, uint16_t buflen)
{
	static uint8_t step;
	static uint16_t stepTim;
	uint16_t com_len;
	uint8_t rt,flag;
	uint16_t pt;
	uint8_t *Nembuf;
	
	Nembuf = TX4Gbuff2;

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CIPSEND=%d,%d\r\n",socketdata,buflen);			
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			stepTim++;
			if(stepTim<=10){
				com_len = sprintf((char *)Nembuf,"AT+CIPSEND=%d,%d\r\n",socketdata,buflen);				
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,Nembuf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=100){
				flag = 0;
				pt = Buf4GRxSize;
				while(RX4G_bt != pt){
					if('>' == RX4Gbuff[RX4G_bt]){
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;					
				}				
			}else{
				step++;
				stepTim = 0;				
			}
			rt = 1;
			break;
		case 3:
			memcpy(TX4Gbuff,buf,buflen);
			U4GSendTxbuf(buflen);
			step++;
			rt = 1;			
			break;
		case 4:
			stepTim++;
			if(stepTim < 3000){
				flag = 0;
				com_len = sprintf((char *)Nembuf,"SEND OK");
				pt = Buf4GRxSize;				
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,Nembuf,RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
				}
				if(flag){
					step=0;	
					rt = 0;
				}else{
					rt = 1;	
				}
			}else{
				step = 0;
				rt = 2;
			}
			break;
		
	
	}
	
	return rt;
}//UIS891X_CIPSEND



/*
---------------------------------------------------------------------
Name		UIS891X_TCPSEND
Function	 命令TCPSEND
Input		No
OutPut	0 OK  >=1 Err
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_TCPSEND(uint8_t socketdata,uint8_t *buf, uint16_t buflen)
{
	static uint8_t step;
	static uint16_t stepTim;
	uint16_t com_len;
	uint8_t rt,flag;
	uint16_t pt;
	uint8_t *Nembuf;
	
	Nembuf = TX4Gbuff2;

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CIPSEND=%d\r\n",buflen);			
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			stepTim++;
			if(stepTim<=10){
				com_len = sprintf((char *)Nembuf,"AT+CIPSEND=%d",buflen);				
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,Nembuf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=100){
				flag = 0;
				pt = Buf4GRxSize;
				while(RX4G_bt != pt){
					if('>' == RX4Gbuff[RX4G_bt]){
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;					
				}				
			}else{
				step++;
				stepTim = 0;				
			}
			rt = 1;
			break;
		case 3:
			memcpy(TX4Gbuff,buf,buflen);
			U4GSendTxbuf(buflen);
			step++;
			rt = 1;			
			break;
		case 4:
			stepTim++;
			if(stepTim < 3000){
				flag = 0;
				com_len = sprintf((char *)Nembuf,"SEND OK");
				pt = Buf4GRxSize;				
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,Nembuf,RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
				}
				if(flag){
					step=0;	
					rt = 0;
				}else{
					rt = 1;	
				}
			}else{
				step = 0;
				rt = 2;
			}
			break;
		
	
	}
	
	return rt;
}//UIS891X_CIPSEND

/*
---------------------------------------------------------------------
Name		UIS891X_NormalRxDisplay
Function	 数据接收处理
Input		No
OutPut	No
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_NormalRxDisplay(uint8_t socketdata,uint8_t *buf, uint16_t *length)
{
	static uint8_t step;
	static uint16_t stepTim;
	uint8_t com_len;
	uint8_t rt,flag;
	uint16_t pt;
	static uint16_t bt_back;
	uint8_t *Nembuf;	
	Nembuf = TX4Gbuff2;

	

	switch(step){
		case 0:
			flag = 0;
			com_len = sprintf((char *)Nembuf,"+RECEIVE,%d,",socketdata);
			pt = Buf4GRxSize;
			while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
				if(0 == FIFO_bufcmp(RX4Gbuff,Nembuf,RX4G_bt,com_len,RX4Gbuff_len)){
					RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
					bt_back = RX4G_bt;
					flag = 1;
					break;
				}else{
					RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
				}
				
			}
			if(1 == flag){
				step++;					
				rt = 1;
				stepTim = 0;
			}else{	
				rt = 0;
			}			
			break;
		case 1:
			stepTim++;
			if(stepTim<=500){
				flag = 0;
				pt = Buf4GRxSize;
				while(RX4G_bt != pt){					
					if(':' == RX4Gbuff[RX4G_bt]){
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
					
				}
				if(flag){
					flag = 0;
					while(RX4G_bt != bt_back){					
						if(isdigit(RX4Gbuff[bt_back])){
							flag = 1;
							break;
						}else{
							bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);
						}
					}
					if(flag){
						flag = 0;
						*length = 0;
						while(RX4G_bt != bt_back){							
							if(isdigit(RX4Gbuff[bt_back])){
								*length = (*length) * 10 + (RX4Gbuff[bt_back] - '0');
								flag = 1;
								bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);
								
							}else{
								//bt_back = FIFO_pointer(RX4G_bt,bt_back,RX4Gbuff_len);
								break;
							}
						}
					}
					if((flag)&&(*length<1500)){					
						step++;	
						rt = 1;
					}else{
						rt = 4;
						step = 0;
					}
				}else{
					rt = 1;
				}
			}else{
				step = 0;
				stepTim = 0;
				rt = 2;
			}			
			break;		
		case 2:
			stepTim++;
			if(stepTim<=500){				
				pt = Buf4GRxSize;
				if(FIFO_length(RX4G_bt,pt,RX4Gbuff_len) >= *length){					
					for(com_len = 0; com_len < *length; com_len++){
						buf[com_len] = RX4Gbuff[RX4G_bt];
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					rt = 3;
					step = 0;
				}else{				
					rt = 1;
				}
			}else{
				step = 0;
				stepTim = 0;
				rt = 2;
			}
			break;
	}
	return rt;
}//UIS891X_NormalRxDisplay

/*
---------------------------------------------------------------------
Name		UIS891X_NormalDisplay
Function	 链接正常处理
Input		No
OutPut	No
Edit by fanzhong
Date		2020-09-03
---------------------------------------------------------------------
*/
uint8_t UIS891X_NormalDisplay(uint8_t socketdata,uint8_t *buf,uint16_t *length)
{
	static uint8_t step;
	static uint16_t stepTim;
	uint8_t com_len;
	uint8_t rt,flag;
	uint16_t pt,bt1;
	static uint16_t bt_back;
	uint8_t *Nembuf;	
	Nembuf = TX4Gbuff2;
	//Nembuf1 = Membuf->Net4Gbuf[2];
	uint8_t ComBuf[2]={0xff,0xbb};

	switch(step){
		case 0:
			stepTim++;
			if(stepTim < 1500){				
				flag = 0;
				//com_len = sprintf((char *)Nembuf,"+RECEIVE,%d,",socketdata);	
        com_len=2;				
				bt1 = RX4G_bt;
				pt = Buf4GRxSize;				
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,ComBuf,RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);
						bt_back = RX4G_bt;
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(1 == flag){
					step++;					
					rt = 1;
					stepTim = 0;
				}else{
					flag = 0;
					com_len = sprintf((char *)Nembuf,"%d,CLOSED",socketdata);
					while(FIFO_length(bt1,pt,RX4Gbuff_len)>=com_len){
						if(0 == FIFO_bufcmp(RX4Gbuff,Nembuf,bt1,com_len,RX4Gbuff_len)){
							//RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
							//bt_back = RX4G_bt;
							flag = 2;
							break;
						}else{
							bt1 = FIFO_pointer(bt1,1,RX4Gbuff_len);
						}
						
					}
				}
				if(2 == flag){
					stepTim = 0;
					rt = 2;
				}else{
					rt = 1;	
				}
			}else{
				stepTim = 0;
				rt = 3;
			}
			break;
			
		case 1:
			stepTim++;
			if(stepTim<=500){
				flag = 0;
				pt = Buf4GRxSize;
				bt1 = RX4G_bt;
				if(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=2){
					bt1=FIFO_pointer(bt1,1,RX4Gbuff_len);
					*length=RX4Gbuff[bt1];
					bt1=FIFO_pointer(bt1,1,RX4Gbuff_len);
					*length+=RX4Gbuff[bt1]<<8;
					flag=1;
					RX4G_bt=FIFO_pointer(RX4G_bt,3,RX4Gbuff_len);
				}
				else{
					rt=1;//等
				}
					
				if((flag)&&(*length<1500)){					
						step++;	
						rt = 1;
					}
				
				}
			else{
				stepTim = 0;
				rt = 3;
			}
				
			break;		
			
		case 2:
			stepTim++;
			if(stepTim<=500){				
				pt = Buf4GRxSize;
				if(FIFO_length(RX4G_bt,pt,RX4Gbuff_len) >= *length+2){		//加2位较验位
					RX4G_bt=FIFO_Backpointer(RX4G_bt,5,RX4Gbuff_len);	
					for(com_len = 0; com_len < *length+7; com_len++){
						buf[com_len] = RX4Gbuff[RX4G_bt];
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					*length+=7;
					rt = 0;
					step = 0;
				}else{				
					rt = 1;
				}
			}else{
				step = 0;
				stepTim = 0;
				rt = 3;
			}
			break;
	}
	if(rt>=2){
		*length = 0;
	}
	return rt;
}//UIS891X_NormalDisplay
/*
---------------------------------------------------------------------
Name		UIS891X_CIPACK
Function	 CIPSTART
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2020-08-25
---------------------------------------------------------------------
*/
uint8_t UIS891X_CIPACK(uint8_t socketdata,uint16_t len)
{
	
	static uint8_t step;
	static uint16_t stepTim,bt_back;
	uint8_t com_len;
	uint8_t i,rt,flag;
	uint16_t pt,ackbuf[3];	
	
	uint8_t *buf;	
	buf = TX4Gbuff2;

	switch(step){
		case 0:
			if(socketdata){
				com_len = sprintf((char *)TX4Gbuff,"AT+CIPACK=%d\r\n",socketdata);
			}else{
				com_len = sprintf((char *)TX4Gbuff,"AT+CIPACK\r\n");
			}
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:			
			stepTim++;
			if(stepTim<=20){
				flag = 0;
				if(socketdata){
					com_len = sprintf((char *)TX4Gbuff,"AT+CIPACK=%d",socketdata);
				}else{
					com_len = sprintf((char *)TX4Gbuff,"AT+CIPACK");
				}				
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						bt_back = RX4G_bt;
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=300){
				flag = 0;
				com_len = sprintf((char *)buf,"OK");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step=0;
					stepTim = 0;
					rt = 0;
					
					flag = 0;
					com_len = sprintf((char *)buf,"+CIPACK:");					
					while(FIFO_length(bt_back,RX4G_bt,RX4Gbuff_len)>=com_len){
						if(0 == FIFO_bufcmp(RX4Gbuff,buf,bt_back,com_len,RX4Gbuff_len)){
							bt_back = FIFO_pointer(bt_back,com_len,RX4Gbuff_len);
							flag = 1;
							break;
						}else{
							bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);
						}
						
					}
					if(flag){
						
						for(i=0;i<3;i++){
							flag = 0;
							while(bt_back!=RX4G_bt){
								if(isdigit(RX4Gbuff[bt_back])){
									flag = 1;
									break;
								}else{
									bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);
								}
							}
							if(0 == flag){
								break;
							}
							ackbuf[i] = 0;
							flag = 0;
							while(bt_back!=RX4G_bt){
								if(isdigit(RX4Gbuff[bt_back])){
									ackbuf[i] = ackbuf[i]*10 + (RX4Gbuff[bt_back]-'0');
									flag = 1;
									bt_back = FIFO_pointer(bt_back,1,RX4Gbuff_len);									
								}else{
									break;									
								}
							}
							if(0 == flag){
								break;
							}
						}
						if(flag){
							if((len == ackbuf[0])&&(len == ackbuf[1])){
								rt = 0;
							}else{
								rt = 3;
							}
						}else{
							rt = 3;
						}
					}
				}else{
					rt = 1;
				}
			}else{
				step = 0;
				rt = 2;
			}
			break;		
		
	}
	return rt;	
}//UIS891X_CIPACK





