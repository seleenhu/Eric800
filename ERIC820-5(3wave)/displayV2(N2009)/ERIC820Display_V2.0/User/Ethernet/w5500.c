
#include "bsp.h"
//#include <stdio.h>
//#include <string.h>
//#include "w5500.h"
//#include "w5500_config.h"

#ifdef __DEF_IINCHIP_PPP__
#include "md5.h"
#endif

static uint8 I_STATUS[MAX_SOCK_NUM];
static uint16 SSIZE[MAX_SOCK_NUM]; /**< Max Tx buffer size by each channel */
static uint16 RSIZE[MAX_SOCK_NUM]; /**< Max Rx buffer size by each channel */

uint8 getISR(uint8 s)
{
    return I_STATUS[s];
}
void putISR(uint8 s, uint8 val)
{
    I_STATUS[s] = val;
}

uint16 getIINCHIP_RxMAX(uint8 s)
{
    return RSIZE[s];
}
uint16 getIINCHIP_TxMAX(uint8 s)
{
    return SSIZE[s];
}
void IINCHIP_CSoff(void)
{
    WIZ_CS(LOW);
}
void IINCHIP_CSon(void)
{
    WIZ_CS(HIGH);
}
#if 0
uint8  IINCHIP_SpiSendData(uint8 dat)
{
	//bsp_spiWrite0(dat);
	//return 1;
    return(SPI2_SendByte(dat));
}
#else
void  IINCHIP_SpiWriteData(uint8 dat)
{
	W5500_spiWrite(dat);	
}
uint8  IINCHIP_SpiReadData(void)
{
	return(W5500_spiRead());
}
#endif
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{

	IINCHIP_CSoff();                              		// CS=0, SPI start
    IINCHIP_SpiWriteData( (addrbsb & 0x00FF0000)>>16);	// Address byte 1
    IINCHIP_SpiWriteData( (addrbsb & 0x0000FF00)>> 8);	// Address byte 2
    IINCHIP_SpiWriteData( (addrbsb & 0x000000F8) + 4);   // Data write command and Write data length 1
    IINCHIP_SpiWriteData(data);                    		// Data write (write 1byte data)
    IINCHIP_CSon();

}

uint8 IINCHIP_READ(uint32 addrbsb)
{

	 uint8 data = 0;
    //IINCHIP_ISR_DISABLE();                        	// Interrupt Service Routine Disable
    IINCHIP_CSoff();                              		// CS=0, SPI start
    IINCHIP_SpiWriteData( (addrbsb & 0x00FF0000)>>16);	// Address byte 1
    IINCHIP_SpiWriteData( (addrbsb & 0x0000FF00)>> 8);	// Address byte 2
    IINCHIP_SpiWriteData( (addrbsb & 0x000000F8))    ;	// Data read command and Read data length 1
    data = IINCHIP_SpiReadData();             		// Data read (read 1byte data)
    IINCHIP_CSon();                               		// CS=1,  SPI end
    //IINCHIP_ISR_ENABLE();                         	// Interrupt Service Routine Enable
    return data;

}

uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len)
{

	uint16 idx = 0;
    //if(len == 0) NetDBG("Unexpected2 length 0\r\n");

    //IINCHIP_ISR_DISABLE();
    IINCHIP_CSoff();                              		// CS=0, SPI start
    IINCHIP_SpiWriteData( (addrbsb & 0x00FF0000)>>16);	// Address byte 1
    IINCHIP_SpiWriteData( (addrbsb & 0x0000FF00)>> 8);	// Address byte 2
    IINCHIP_SpiWriteData( (addrbsb & 0x000000F8) + 4);   // Data write command and Write data length 1
    for(idx = 0; idx < len; idx++)                		// Write data in loop
    {
        IINCHIP_SpiWriteData(buf[idx]);
    }
    IINCHIP_CSon();                               		// CS=1, SPI end
    //IINCHIP_ISR_ENABLE();                         	// Interrupt Service Routine Enable

    return len;

}

uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{

	uint16 idx = 0;
    //if(len == 0)
    //{
    //    NetDBG("Unexpected2 length 0\r\n");
    //}

    //IINCHIP_ISR_DISABLE();
    //SPI MODE I/F
    IINCHIP_CSoff();                                  	// CS=0, SPI start
    IINCHIP_SpiWriteData( (addrbsb & 0x00FF0000)>>16);	// Address byte 1
    IINCHIP_SpiWriteData( (addrbsb & 0x0000FF00)>> 8);	// Address byte 2
    IINCHIP_SpiWriteData( (addrbsb & 0x000000F8));    	// Data write command and Write data length 1
    for(idx = 0; idx < len; idx++)                    	// Write data in loop
    {
        buf[idx] = IINCHIP_SpiReadData();
    }
    IINCHIP_CSon();                                   	// CS=1, SPI end
    //IINCHIP_ISR_ENABLE();                             // Interrupt Service Routine Enable

    return len;

}


/**
@brief  This function is for resetting of the iinchip. Initializes the iinchip to work in whether DIRECT or INDIRECT mode
*/
void iinchip_init(void)
{
    setMR( MR_RST );
#ifdef __DEF_IINCHIP_DBG__
    NetDBG("MR value is %02x \r\n",IINCHIP_READ_COMMON(MR));
#endif
}

/**
@brief  This function set the transmit & receive buffer size as per the channels is used
Note for TMSR and RMSR bits are as follows\n
bit 1-0 : memory size of channel #0 \n
bit 3-2 : memory size of channel #1 \n
bit 5-4 : memory size of channel #2 \n
bit 7-6 : memory size of channel #3 \n
bit 9-8 : memory size of channel #4 \n
bit 11-10 : memory size of channel #5 \n
bit 12-12 : memory size of channel #6 \n
bit 15-14 : memory size of channel #7 \n
W5500的Tx, Rx的最大寄存器宽度是16K Bytes,\n
In the range of 16KBytes, the memory size could be allocated dynamically by each channel.\n
Be attentive to sum of memory size shouldn't exceed 8Kbytes\n
and to data transmission and receiption from non-allocated channel may cause some problems.\n
If the 16KBytes memory is already  assigned to centain channel, \n
other 3 channels couldn't be used, for there's no available memory.\n
If two 4KBytes memory are assigned to two each channels, \n
other 2 channels couldn't be used, for there's no available memory.\n
*/
void sysinit( uint8 * tx_size, uint8 * rx_size  )
{
    int16 i;
    int16 ssum,rsum;
#ifdef __DEF_IINCHIP_DBG__
    NetDBG("sysinit()\r\n");
#endif
    ssum = 0;
    rsum = 0;

    for (i = 0 ; i < MAX_SOCK_NUM; i++)       				// Set the size, masking and base address of Tx & Rx memory by each channel
    {
        IINCHIP_WRITE( (Sn_TXMEM_SIZE(i)), tx_size[i]);		// MCU记录每个Socket发送缓存的大小到数组tx_size[]对应的元素值
        IINCHIP_WRITE( (Sn_RXMEM_SIZE(i)), rx_size[i]);		// MCU记录每个Socket接收缓存的大小到数组rx_size[]对应的元素值

#ifdef __DEF_IINCHIP_DBG__
        NetDBG("tx_size[%d]: %d, Sn_TXMEM_SIZE = %d\r\n",i, tx_size[i], IINCHIP_READ(Sn_TXMEM_SIZE(i)));
        NetDBG("rx_size[%d]: %d, Sn_RXMEM_SIZE = %d\r\n",i, rx_size[i], IINCHIP_READ(Sn_RXMEM_SIZE(i)));
#endif
        SSIZE[i] = (int16)(0);
        RSIZE[i] = (int16)(0);


        if (ssum <= 16384)
        {
            switch( tx_size[i] )
            {
            case 1:
                SSIZE[i] = (int16)(1024);
                break;
            case 2:
                SSIZE[i] = (int16)(2048);
                break;
            case 4:
                SSIZE[i] = (int16)(4096);
                break;
            case 8:
                SSIZE[i] = (int16)(8192);
                break;
            case 16:
                SSIZE[i] = (int16)(16384);
                break;
            default :
                RSIZE[i] = (int16)(2048);
                break;
            }
        }

        if (rsum <= 16384)
        {
            switch( rx_size[i] )
            {
            case 1:
                RSIZE[i] = (int16)(1024);
                break;
            case 2:
                RSIZE[i] = (int16)(2048);
                break;
            case 4:
                RSIZE[i] = (int16)(4096);
                break;
            case 8:
                RSIZE[i] = (int16)(8192);
                break;
            case 16:
                RSIZE[i] = (int16)(16384);
                break;
            default :
                RSIZE[i] = (int16)(2048);
                break;
            }
        }
        ssum += SSIZE[i];
        rsum += RSIZE[i];

    }
}

// added

/*.
*/
void setGAR(
    uint8 * addr  /**< a pointer to a 4 -byte array responsible to set the Gateway IP address. */
)
{
    wiz_write_buf(GAR0, addr, 4);
}
void getGWIP(uint8 * addr)
{
    wiz_read_buf(GAR0, addr, 4);
}

/**
@brief  It sets up SubnetMask address
*/
void setSUBR(uint8 * addr)
{
    wiz_write_buf(SUBR0, addr, 4);
}
/**
@brief  This function sets up MAC address.
*/
void setSHAR(
    uint8 * addr  /**< a pointer to a 6 -byte array responsible to set the MAC address. */
)
{
    wiz_write_buf(SHAR0, addr, 6);
}

/**
@brief  This function sets up Source IP address.
*/
void setSIPR(
    uint8 * addr  /**< a pointer to a 4 -byte array responsible to set the Source IP address. */
)
{
    wiz_write_buf(SIPR0, addr, 4);
}

/**
@brief  W5500心跳检测程序，设置Socket在线时间寄存器Sn_KPALVTR，单位为5s
*/
void setkeepalive(SOCKET s)
{
    IINCHIP_WRITE(Sn_KPALVTR(s),0x02);
}

/**
@brief  This function sets up Source IP address.
*/
void getGAR(uint8 * addr)
{
    wiz_read_buf(GAR0, addr, 4);
}
void getSUBR(uint8 * addr)
{
    wiz_read_buf(SUBR0, addr, 4);
}
void getSHAR(uint8 * addr)
{
    wiz_read_buf(SHAR0, addr, 6);
}
void getSIPR(uint8 * addr)
{
    wiz_read_buf(SIPR0, addr, 4);
}

void setMR(uint8 val)
{
    IINCHIP_WRITE(MR,val);
}

/**
@brief  This function gets Interrupt register in common register.
 */
uint8 getIR( void )
{
    return IINCHIP_READ(IR);
}

/**
@brief  This function sets up Retransmission time.

If there is no response from the peer or delay in response then retransmission
will be there as per RTR (Retry Time-value Register)setting
*/
void setRTR(uint16 timeout)
{
    IINCHIP_WRITE(RTR0,(uint8)((timeout & 0xff00) >> 8));
    IINCHIP_WRITE(RTR1,(uint8)(timeout & 0x00ff));
}

/**
@brief  This function set the number of Retransmission.

If there is no response from the peer or delay in response then recorded time
as per RTR & RCR register seeting then time out will occur.
*/
void setRCR(uint8 retry)
{
    IINCHIP_WRITE(WIZ_RCR,retry);
}

/**
@brief  This function set the interrupt mask Enable/Disable appropriate Interrupt. ('1' : interrupt enable)

If any bit in IMR is set as '0' then there is not interrupt signal though the bit is
set in IR register.
*/
void clearIR(uint8 mask)
{
    IINCHIP_WRITE(IR, ~mask | getIR() ); // must be setted 0x10.
}

/**
@brief  This sets the maximum segment size of TCP in Active Mode), while in Passive Mode this is set by peer
*/
void setSn_MSS(SOCKET s, uint16 Sn_MSSR)
{
    IINCHIP_WRITE( Sn_MSSR0(s), (uint8)((Sn_MSSR & 0xff00) >> 8));
    IINCHIP_WRITE( Sn_MSSR1(s), (uint8)(Sn_MSSR & 0x00ff));
}

void setSn_TTL(SOCKET s, uint8 ttl)
{
    IINCHIP_WRITE( Sn_TTL(s) , ttl);
}



/**
@brief  get socket interrupt status

These below functions are used to read the Interrupt & Soket Status register
*/
uint8 getSn_IR(SOCKET s)
{
    return IINCHIP_READ(Sn_IR(s));
}


/**
@brief   get socket status
*/
uint8 getSn_SR(SOCKET s)
{
    return IINCHIP_READ(Sn_SR(s)); // MCU读Sn_SR对应地址里面的数值并返回
}


/**
@brief  get socket TX free buf size

This gives free buffer size of transmit buffer. This is the data size that user can transmit.
User shuold check this value first and control the size of transmitting data
*/
uint16 getSn_TX_FSR(SOCKET s)
{
    uint16 val=0,val1=0;
    do
    {
        val1 = IINCHIP_READ(Sn_TX_FSR0(s));
        val1 = (val1 << 8) + IINCHIP_READ(Sn_TX_FSR1(s));
        if (val1 != 0)
        {
            val = IINCHIP_READ(Sn_TX_FSR0(s));
            val = (val << 8) + IINCHIP_READ(Sn_TX_FSR1(s));
        }
    }
    while (val != val1);
    return val;
}


/**
@brief   get socket RX recv buf size

This gives size of received data in receive buffer.
*/
uint16 getSn_RX_RSR(SOCKET s)		// 获取Sn_RX_RSR空闲接收缓存寄存器的值并返回,Sn_RX_RSR中保存的是接收缓存中已接收和保存的数据大小
{
    uint16 val=0,val1=0;
    do
    {
        val1 = IINCHIP_READ(Sn_RX_RSR0(s));
        val1 = (val1 << 8) + IINCHIP_READ(Sn_RX_RSR1(s));		// MCU读出Sn_RX_RSR的值赋给val1
        if(val1 != 0)											// 如果Sn_RX_RSR的值不为0，说明接收缓存中收到了数据
        {
            val = IINCHIP_READ(Sn_RX_RSR0(s));
            val = (val << 8) + IINCHIP_READ(Sn_RX_RSR1(s));		// MCU读出Sn_RX_RSR的值赋给val
        }
    }
    while (val != val1);										// 此时应该val=val1,表达式为假，跳出do while循环
    return val;													// 返回val
}


/**
@brief   This function is being called by send() and sendto() function also.

This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
register. User should read upper byte first and lower byte later to get proper value.
*/
void send_data_processing(SOCKET s, uint8 *data, uint16 len)		// MCU把数据发送给W5500的过程，W5500将数据写入buf，并更新数据的写指针的地址
{
    uint16 ptr =0;
    uint32 addrbsb =0;
    if(len == 0)
    {
        //NetDBG("CH: %d Unexpected1 length 0\r\n", s);
        return;
    }

// Sn_RX_WR是发送写指针寄存器，用来保存发送缓存中将要传输数据的首地址
    ptr = IINCHIP_READ( Sn_TX_WR0(s) );
    ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_TX_WR1(s));		// MCU读取Sn_RX_WR寄存器的值

    addrbsb = (uint32)(ptr<<8) + (s<<5) + 0x10;						// 将发送缓存中将要传输数据的首地址转成32位
    wiz_write_buf(addrbsb, data, len);								// W5500从该首地址开始写入数据，数据长度为len

    ptr += len;														// 首地址的值+len变为数据新的首地址
    IINCHIP_WRITE( Sn_TX_WR0(s) ,(uint8)((ptr & 0xff00) >> 8));
    IINCHIP_WRITE( Sn_TX_WR1(s),(uint8)(ptr & 0x00ff));				// 将新的首地址保存在Sn_RX_WR中
}


/**
@brief  This function is being called by recv() also.

This function read the Rx read pointer register
and after copy the data from receive buffer update the Rx write pointer register.
User should read upper byte first and lower byte later to get proper value.
*/
void recv_data_processing(SOCKET s, uint8 *data, uint16 len)
{
    uint16 ptr = 0;
    uint32 addrbsb = 0;

    if(len == 0)
    {
        //NetDBG("CH: %d Unexpected2 length 0\r\n", s);
        return;
    }
// Sn_RX_RD是发送读指针寄存器
    ptr = IINCHIP_READ( Sn_RX_RD0(s) );
    ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ( Sn_RX_RD1(s) );		// MCU读取Sn_RX_RD寄存器的值

    addrbsb = (uint32)(ptr<<8) + (s<<5) + 0x18;						// 将发送缓存中将要传输数据的首地址转成32位
    wiz_read_buf(addrbsb, data, len);								// W5500从该首地址开始读取数据，数据长度为len
    ptr += len;														// 首地址的值+len变为数据新的首地址

    IINCHIP_WRITE( Sn_RX_RD0(s), (uint8)((ptr & 0xff00) >> 8));
    IINCHIP_WRITE( Sn_RX_RD1(s), (uint8)(ptr & 0x00ff));			// 将新的首地址保存在Sn_RX_RD中
}

void setSn_IR(uint8 s, uint8 val)
{
    IINCHIP_WRITE(Sn_IR(s), val);
}




