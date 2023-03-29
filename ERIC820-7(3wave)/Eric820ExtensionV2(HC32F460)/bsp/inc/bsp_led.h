#ifndef _BSPLED_H_
#define _BSPLED_H_

#define RedLedPort						(PortB)
#define RedLedPin			  			(Pin04)

#define GreenLedPort					(PortB)
#define GreenLedPin			  		(Pin03)

#define RedLedOn()            	PORT_ResetBits(RedLedPort, RedLedPin)
#define RedLedOff()           	PORT_SetBits(RedLedPort, RedLedPin)

#define GreenLedOn()            PORT_ResetBits(GreenLedPort, GreenLedPin)
#define GreenLedOff()           PORT_SetBits(GreenLedPort,GreenLedPin)

#define GreenLedToggle()				PORT_Toggle(GreenLedPort, GreenLedPin)
#define RedLedToggle()					PORT_Toggle(RedLedPort, RedLedPin)


void LedInit(void);

#endif





