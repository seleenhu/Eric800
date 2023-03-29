
#include "bsp.h"


void LedInit(void)
{
		stc_port_init_t stcPortInit;
	
		PORT_DebugPortSetting(TDO_SWO,Disable);
		PORT_DebugPortSetting(TRST,Disable);

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enPinMode = Pin_Mode_Out;
		//stcPortInit.enExInt = Enable;
    stcPortInit.enPullUp = Enable;
    /* Initialize LED pin */
		PORT_Init(RedLedPort,RedLedPin,&stcPortInit);
		PORT_Init(GreenLedPort,GreenLedPin,&stcPortInit);
	
	
	
		RedLedOff();
	  GreenLedOff();
	
}




