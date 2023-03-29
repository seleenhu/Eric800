﻿/*
*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.36                          *
*        Compiled Aug 31 2016, 10:53:09                              *
*        (c) 2016 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "Emwin_eric800.h"
#include "InitSystem.h"
extern const unsigned char _aclogo2_s[17237UL + 1];
//#define ADCFifo_SDRAM_SIZE 10*1024*2


static GRAPH_DATA_Handle ahDataA; // ------(1)
static GRAPH_DATA_Handle ahDataB; // ------(2)
static GRAPH_DATA_Handle ahDataC; // ------(3)
static GRAPH_DATA_Handle ahDataU0; // ------(4)

static GRAPH_DATA_Handle ahDataIA; // ------(1)
static GRAPH_DATA_Handle ahDataIB; // ------(2)
static GRAPH_DATA_Handle ahDataIC; // ------(3)
static GRAPH_DATA_Handle ahDataI0; // ------(3)



/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0      (GUI_ID_USER + 0x00)
#define ID_BUTTON_0      (GUI_ID_USER + 0x01)
#define ID_BUTTON_1      (GUI_ID_USER + 0x02)
#define ID_BUTTON_2      (GUI_ID_USER + 0x03)
#define ID_BUTTON_3      (GUI_ID_USER + 0x04)
#define ID_BUTTON_4      (GUI_ID_USER + 0x05)
#define ID_GRAPH_0      (GUI_ID_USER + 0x1A)
#define ID_GRAPH_1      (GUI_ID_USER + 0x1B)
#define ID_BUTTON_5      (GUI_ID_USER + 0x1C)
#define ID_BUTTON_6      (GUI_ID_USER + 0x1D)

#define ID_TEXT_0 		(GUI_ID_USER + 0x20)
#define ID_TEXT_1 		(GUI_ID_USER + 0x21)
#define ID_TEXT_2 		(GUI_ID_USER + 0x22)
#define ID_TEXT_3 		(GUI_ID_USER + 0x23)

#define ID_TEXT_6 		(GUI_ID_USER + 0x24)
#define ID_TEXT_7 		(GUI_ID_USER + 0x25)
#define ID_TEXT_8 		(GUI_ID_USER + 0x26)
#define ID_TEXT_9 		(GUI_ID_USER + 0x27)
#define ID_TEXT_14 		(GUI_ID_USER + 0x28)
#define ID_TEXT_15 		(GUI_ID_USER + 0x29)







// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Main_Graph", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "RealMonitor", ID_BUTTON_0, 150, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SystemSet", ID_BUTTON_1, 280, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "AlarmList", ID_BUTTON_2, 410, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "PowerQuality", ID_BUTTON_3, 540, 0, 129, 45, 0, 0x0, 0 },
  //{ BUTTON_CreateIndirect, "Button", ID_BUTTON_4, 670, 0, 129, 45, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 20, 116, 780, 150, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_1, 20, 306, 780, 150, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_5, 500, 50, 100, 40, 0, 0x0, 0 },
	
	
	{ TEXT_CreateIndirect, "Ua=       KV", ID_TEXT_0, 35, 90, 147, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Ub=       KV", ID_TEXT_1, 220, 90, 147, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Uc=       KV", ID_TEXT_2, 420, 90, 147, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "3U0=      KV", ID_TEXT_3, 630, 90, 147, 25, 0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "Ia=       A", ID_TEXT_6, 35, 280, 147, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Ib=       A", ID_TEXT_7, 220, 280, 147, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Ic=       A", ID_TEXT_8, 420, 280, 147, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "3I0=      A", ID_TEXT_9, 630, 280, 147, 25, 0, 0x0, 0 },
	//=====displaytime
	{ TEXT_CreateIndirect, "             ", ID_TEXT_14, 680, 6, 120, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "             ", ID_TEXT_15, 680, 26, 120, 20, 0, 0x0, 0 },
	
  //{ BUTTON_CreateIndirect, "", ID_BUTTON_6, 650, 70, 30, 30, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static void Clear_Graph(void)
{
	GRAPH_DATA_YT_Clear(ahDataA);
	GRAPH_DATA_YT_Clear(ahDataB);
	GRAPH_DATA_YT_Clear(ahDataC);
	GRAPH_DATA_YT_Clear(ahDataU0);
	
	GRAPH_DATA_YT_Clear(ahDataIA);
	GRAPH_DATA_YT_Clear(ahDataIB);
	GRAPH_DATA_YT_Clear(ahDataIC);
	GRAPH_DATA_YT_Clear(ahDataI0);
}


static void _cbButton0(WM_MESSAGE * pMsg)
{
	WM_HWIN  hWin;
	GUI_RECT Rect;
	hWin  = pMsg->hWin;

	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin))
			{
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}
			else
			{
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}
			GUI_SetFont(&XBF_Font32);
			GUI_DispStringInRect("实时监控", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cbButton1(WM_MESSAGE * pMsg)
{
	WM_HWIN  hWin;
	GUI_RECT Rect;
	hWin  = pMsg->hWin;

	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin))
			{
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}
			else
			{
				GUI_SetColor(0x00959595);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x00959595);
				GUI_SetColor(GUI_WHITE);
			}			
			GUI_SetFont(&XBF_Font32);
			GUI_DispStringInRect("系统设置", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cbButton2(WM_MESSAGE * pMsg)
{
	WM_HWIN  hWin;
	GUI_RECT Rect;
	hWin  = pMsg->hWin;

	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin))
			{
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}
			else
			{
				GUI_SetColor(0x00959595);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x00959595);
				GUI_SetColor(GUI_WHITE);
			}			
			GUI_SetFont(&XBF_Font32);
			GUI_DispStringInRect("告警查询", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cbButton3(WM_MESSAGE * pMsg)
{
	WM_HWIN  hWin;
	GUI_RECT Rect;
	hWin  = pMsg->hWin;

	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin))
			{
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}
			else
			{
				GUI_SetColor(0x00959595);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x00959595);
				GUI_SetColor(GUI_WHITE);
			}			
			GUI_SetFont(&XBF_Font32);
			GUI_DispStringInRect("电能质量", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}
#if 0
static void _cbButton4(WM_MESSAGE * pMsg)
{
	WM_HWIN  hWin;
	GUI_RECT Rect;
	hWin  = pMsg->hWin;

	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin))
			{
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}
			else
			{
				GUI_SetColor(0x00959595);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x00959595);
				GUI_SetColor(GUI_WHITE);
			}
			/*
			GUI_SetFont(&GUI_Font20_ASCII);
			GUI_DispStringInRect("About us", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font32);
			GUI_DispStringInRect("关于我们", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

#endif
static void _cbButton5(WM_MESSAGE * pMsg)
{
	WM_HWIN  hWin;
	GUI_RECT Rect;
	hWin  = pMsg->hWin;

	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin))
			{
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}
			else
			{
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}			
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("实时监控", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}
#if 0
static void _cbButton6(WM_MESSAGE * pMsg)
{
	WM_HWIN  hWin;
	GUI_RECT Rect;
	hWin  = pMsg->hWin;

	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin))
			{
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}
			else
			{
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}
			/*
			GUI_SetFont(&GUI_Font20_ASCII);
			GUI_DispStringInRect("About us", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			//GUI_SetFont(&XBF_Font32);
			//GUI_DispStringInRect("????", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

#endif
// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
	//GUI_HMEM hMem;
  int     NCode;
  int     Id;
	int 		i,n;
	float f;
	char s[20];
	uint16_t Graph_Pt;
	//uint16_t pt;
	//uint16_t ct;
	float UaValue,UbValue,UcValue;//,U0Value;
	
	float URMS[4];
	float	IRMS[4];
	float Uscale;
	float Iscale;
	
	 
	GUI_MEMDEV_Handle hMemJPEG;
  GUI_JPEG_INFO JpegInfo;
//	GRAPH_SCALE_Handle hScale;
  //FRAMEWIN_SKINFLEX_PROPS FRAMEWIN_Props;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
	
	case WM_PAINT:
		
		//hMem=GUI_ALLOC_AllocZero(1024*500);	
		GUI_JPEG_GetInfo(_aclogo2_s, sizeof(_aclogo2_s) ,&JpegInfo);

		hMemJPEG = GUI_MEMDEV_CreateEx(2,2,JpegInfo.XSize,JpegInfo.YSize,GUI_MEMDEV_NOTRANS);
		//hMemJPEG = GUI_MEMDEV_CreateFixed(0,0,JpegInfo.XSize,JpegInfo.YSize,GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_16,GUICC_M565 );
		GUI_MEMDEV_Select(hMemJPEG);
		GUI_JPEG_Draw(_aclogo2_s,sizeof(_aclogo2_s),0,0);
		GUI_MEMDEV_Select(0);

		GUI_MEMDEV_WriteAt(hMemJPEG,2,2);
		GUI_MEMDEV_Delete(hMemJPEG);//	
	
		//=======标Y轴刻度
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(GUI_FONT_24B_ASCII);
		GUI_DispStringAt("KV",2,90);
		GUI_SetFont(GUI_FONT_16B_ASCII);
		//====time backcolor
		GUI_SetColor(0x00959595);
		GUI_FillRect(670, 0, 800, 45);
		
		
		
	 
		//=======标X轴刻度
	
		
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(GUI_FONT_24B_ASCII);
		GUI_DispStringAt("A",2,280);
		GUI_SetFont(GUI_FONT_16B_ASCII);
		#if 0
		GUI_DispStringAt("500",0,306);
		GUI_DispStringAt("0",0,381);
		GUI_DispStringAt("-500",0,456);
		#endif	
		break;
	case WM_TIMER:
		if(ScreenCloseDisplay()){
			ModuleComSet10Fault(0x01,0x00,0x00);       
			Clear_Graph();
			GUI_EndDialog(pMsg->hWin,0);
			CreateMain();
		}
	
	
		WM_RestartTimer(pMsg->Data.v,1000);
	
	
		 //==========display time
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_14);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetTextColor(hItem, GUI_WHITE);
		SystemTime=SysGetTime();    
		sprintf(s,"%02d:%02d:%02d",SystemTime.hour,SystemTime.minute,SystemTime.second);
		TEXT_SetText(hItem,s);
 
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_15);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetTextColor(hItem, GUI_WHITE);
		SystemTime=SysGetTime();
		sprintf(s,"20%02d-%02d-%02d",SystemTime.year,SystemTime.month,SystemTime.day);
		TEXT_SetText(hItem,s);
	
	//=========显示电压有效值
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
		TEXT_SetTextColor(hItem, GUI_YELLOW);
		//f=ADCFifo->RMS_buf[0];
		f = ModuleCombuf->RMS_buf[0];
		f*=Setting.RatioStruct.RatioPT;
		f=f/1000;
		f=(int32_t)(f*100)/100.0;
		sprintf(s,"Ua=%6.2fKV",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
		TEXT_SetTextColor(hItem, GUI_GREEN);
		//f=ADCFifo->RMS_buf[1];
		f = ModuleCombuf->RMS_buf[1];
		f*=Setting.RatioStruct.RatioPT;
		f=f/1000;
		f=(int32_t)(f*100)/100.0;
		sprintf(s,"Ub=%6.2fKV",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
		TEXT_SetTextColor(hItem, GUI_RED);
		//f=ADCFifo->RMS_buf[2];
		f = ModuleCombuf->RMS_buf[2];
		f*=Setting.RatioStruct.RatioPT;
		f=f/1000;
		f=(int32_t)(f*100)/100.0;
		sprintf(s,"Uc=%6.2fKV",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f = ModuleCombuf->RMS_buf[3];		
		f=(int32_t)(f*100)/100.0;
		sprintf(s,"3U0=%6.2fV",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);		
		
		
		URMS[0]=ModuleCombuf->RMS_buf[0];
		URMS[1]=ModuleCombuf->RMS_buf[1];
		URMS[2]=ModuleCombuf->RMS_buf[2];
		URMS[3]=ModuleCombuf->RMS_buf[3];
		
		Uscale=sort(URMS,4)/(60/1.42);
		
	//==============================
	
	//======显业电流有效值
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
		TEXT_SetTextColor(hItem, GUI_YELLOW);
		//f=ADCFifo->RMS_buf[4];
		f=ModuleCombuf->RMS_buf[4];
		f*=Setting.RatioStruct.RatioCT;
		f=(int32_t)(f*100)/100.0;
		sprintf(s,"Ia=%6.2fA",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_7);
		TEXT_SetTextColor(hItem, GUI_GREEN);
		//f=ADCFifo->RMS_buf[5];
		f=ModuleCombuf->RMS_buf[5];
		f*=Setting.RatioStruct.RatioCT;
		f=(int32_t)(f*100)/100.0;
		sprintf(s,"Ib=%6.2fA",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_8);
		TEXT_SetTextColor(hItem, GUI_RED);
		//f=ADCFifo->RMS_buf[6];
		f=ModuleCombuf->RMS_buf[6];
		f*=Setting.RatioStruct.RatioCT;
		f=(int32_t)(f*100)/100.0;
		sprintf(s,"Ic=%6.2fA",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_9);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f=ModuleCombuf->RMS_buf[7];		
		f=(int32_t)(f*100)/100.0;
		sprintf(s,"3I0=%6.2fA",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
				
		IRMS[0]=ModuleCombuf->RMS_buf[4];
		IRMS[1]=ModuleCombuf->RMS_buf[5];
		IRMS[2]=ModuleCombuf->RMS_buf[6];
		IRMS[3]=ModuleCombuf->RMS_buf[7];
		
		Iscale=sort(IRMS,4)/(60/1.42);
		
	
	
		//pt=Setting.RatioStruct.RatioPT;
		//ct=Setting.RatioStruct.RatioCT;
		//================
		GRAPH_DATA_YT_Clear(ahDataA);
		GRAPH_DATA_YT_Clear(ahDataB);
		GRAPH_DATA_YT_Clear(ahDataC);
		GRAPH_DATA_YT_Clear(ahDataU0);
	
		GRAPH_DATA_YT_Clear(ahDataIA);
		GRAPH_DATA_YT_Clear(ahDataIB);
		GRAPH_DATA_YT_Clear(ahDataIC);
		GRAPH_DATA_YT_Clear(ahDataI0);
		
	
		//Graph_Pt=ADCFifo->pt;
		//Graph_Pt=Get_pointer(ADCFifo->pt,12);//取12个周期的数据
		Graph_Pt = 0;
		for(i=0;i<12;i++)
		{
			
			//for(n=0;n<256;n++)
			n=0;
			while(1)
			{
				
				//=====电压波形
			 if(Graph_Pt>=ADCFifo_SDRAM_SIZE/2)
			 Graph_Pt=Graph_Pt-ADCFifo_SDRAM_SIZE/2;
			 
			 if(Decompression_LED_STATE())
				UaValue=0;
			 else
			 {	 
			 //UaValue=*(int16_t *)(ADCFifo.UAFIFO+Graph_Pt);
			 //UaValue=*(int16_t *)(ADCFifo->ADCFIFO[0]+Graph_Pt);
				
			 UaValue=*(int16_t *)(ModuleCombuf->wavebuf[0]+Graph_Pt);
				#if 1 
			 //UaValue/=ADCFifo->fftratio[0];
			 UaValue/=ModuleCombuf->fftratio[0];
			 //UaValue=(UaValue/pt)/Uscale;	 
			 UaValue=UaValue/Uscale;
				#endif
			 }
			 GRAPH_DATA_YT_AddValue(ahDataA,UaValue+75);
			 
			 
			 if(Decompression_LED_STATE())
				UbValue=0;
			 else
			 {
			 //UbValue=*(int16_t *)(ADCFifo.UBFIFO+Graph_Pt);
			 //UbValue=*(int16_t *)(ADCFifo->ADCFIFO[1]+Graph_Pt);
			 UbValue=*(int16_t *)(ModuleCombuf->wavebuf[1]+Graph_Pt);
				#if 1 
			 //UbValue/=ADCFifo->fftratio[1];
			 UbValue/=ModuleCombuf->fftratio[1];
			 //UbValue=(UbValue/pt)/Uscale;
			 UbValue=UbValue/Uscale;
				 
				#endif
			 }
			 GRAPH_DATA_YT_AddValue(ahDataB,UbValue+75);
			 
			 if(Decompression_LED_STATE())
				UcValue=0;
			 else
			 {
			 //UcValue=*(int16_t *)(ADCFifo.UCFIFO+Graph_Pt);
			 //UcValue=*(int16_t *)(ADCFifo->ADCFIFO[2]+Graph_Pt);
			 UcValue=*(int16_t *)(ModuleCombuf->wavebuf[2]+Graph_Pt);
				#if 1 
			 //UcValue/=ADCFifo->fftratio[2];
			 UcValue/=ModuleCombuf->fftratio[2];
			 //UcValue=(UcValue/pt)/Uscale;
			 UcValue=UcValue/Uscale;
				#endif
			 }
			 GRAPH_DATA_YT_AddValue(ahDataC,UcValue+75);
			 #ifdef Display3U0Enable
			 if(Decompression_LED_STATE())
				U0Value=0;
			 else
			 {
			 //U0Value=*(int16_t *)(ADCFifo.U0FIFO+Graph_Pt);
			 U0Value=*(int16_t *)(ADCFifo->ADCFIFO[3]+Graph_Pt);
			#if 1	 
			 U0Value/=ADCFifo->fftratio[3];
			 //U0Value=(U0Value/pt)/Uscale;
			 U0Value=U0Value/Uscale;
			#endif	 
			 }
			 GRAPH_DATA_YT_AddValue(ahDataU0,U0Value+75);
		   #endif
			 //========电流波形
			 if(Decompression_LED_STATE())
				UaValue=0;
			 else
			 {
			 //UaValue=*(int16_t *)(ADCFifo->ADCFIFO[4]+Graph_Pt);
			 //UaValue/=ADCFifo->fftratio[4];
			
			 UaValue=*(int16_t *)(ModuleCombuf->wavebuf[4]+Graph_Pt);
			 UaValue/=ModuleCombuf->fftratio[4];
			 //UaValue=(UaValue/ct)/Iscale;
			 UaValue=UaValue/Iscale;
			 }
			 GRAPH_DATA_YT_AddValue(ahDataIA,UaValue+75);
			 if(Decompression_LED_STATE())
				UbValue=0;
			 else
			 {
			 //UbValue=*(int16_t *)(ADCFifo->ADCFIFO[5]+Graph_Pt);
			// UbValue/=ADCFifo->fftratio[5];
			 UbValue=*(int16_t *)(ModuleCombuf->wavebuf[5]+Graph_Pt);
			 UbValue/=ModuleCombuf->fftratio[5];
			 //UbValue=(UbValue/ct)/Iscale;
			 UbValue=UbValue/Iscale;
			 }
			 GRAPH_DATA_YT_AddValue(ahDataIB,UbValue+75);
			 if(Decompression_LED_STATE())
				UcValue=0;
			 else
			 {
			 //UcValue=*(int16_t *)(ADCFifo->ADCFIFO[6]+Graph_Pt);
			 //UcValue/=ADCFifo->fftratio[6];
			 UcValue=*(int16_t *)(ModuleCombuf->wavebuf[6]+Graph_Pt);
			 UcValue/=ModuleCombuf->fftratio[6];
			 //UcValue=(UcValue/ct)/Iscale;
			 UcValue=UcValue/Iscale;
			 }
			 GRAPH_DATA_YT_AddValue(ahDataIC,UcValue+75);
			 
			 #ifdef Display3I0Enable
			 if(Decompression_LED_STATE())
				U0Value=0;
			 else
			 {
			 U0Value=*(int16_t *)(ADCFifo->ADCFIFO[7]+Graph_Pt);
			 U0Value/=ADCFifo->fftratio[7];
			 //U0Value=(U0Value/ct)/Iscale;
			 U0Value=U0Value/Iscale;
			 }
			 GRAPH_DATA_YT_AddValue(ahDataI0,U0Value+75);
			 
			 #endif
			 Graph_Pt++;
			 //Graph_Pt+=4;
			 n+=4;
			 if(n>=256)
				 break;
			}
			//Graph_Pt+=i*256;
			
			
		}
		
		
	  //==============================
		
		//================显示电流波形
		#if 0
		Graph_Pt=ADCFifo.pt;
		Graph_Pt=Get_pointer(ADCFifo.pt,4);//取4个周期的数据
		for(i=0;i<4;i++)
		{
			
			for(n=0;n<256;n++)
			{	
			 if(Graph_Pt>=ADCFifo_SDRAM_SIZE/2)
				Graph_Pt=Graph_Pt-ADCFifo_SDRAM_SIZE/2;
			 UaValue=*(int16_t *)(ADCFifo.IAFIFO+Graph_Pt)*ADCFifo.I_rati0*20;
			 
			 GRAPH_DATA_YT_AddValue(ahDataIA,UaValue+80);
			 
			 UbValue=*(int16_t *)(ADCFifo.IBFIFO+Graph_Pt)*ADCFifo.I_rati0*20;
			 GRAPH_DATA_YT_AddValue(ahDataIB,UbValue+80);
			 UcValue=*(int16_t *)(ADCFifo.ICFIFO+Graph_Pt)*ADCFifo.I_rati0*20;
			 GRAPH_DATA_YT_AddValue(ahDataIC,UcValue+80);
			 Graph_Pt++;
			 //Graph_Pt+=4;
			}
			
		}
		
		#endif
			
			break;
		
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Main_Graph'
    //
	ScreenClrTim();
		ModuleComSet10Fault(0x02,0x00,0x00);
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleVis(hItem, 0);
    FRAMEWIN_SetClientColor(hItem,0x00333333);
    //
    // Initialization of 'Graph'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
    GRAPH_SetBorder(hItem, 15, 0, 0, 0);
    //
    // Initialization of 'Graph'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_1);
    GRAPH_SetBorder(hItem, 15, 0, 0, 0);
    // USER START (Optionally insert additional code for further widget initialization)
	//========按键换肤的回调函数
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton0);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton2);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton3);
		#if 0
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton4);
		#endif
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton5);
		#if 0
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_6);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton6);
		#endif
		
		//===========电压显示波形初始化
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
    //GRAPH_SetBorder(hItem, 20, 0,0, 0);
		GRAPH_SetBorder(hItem, 2, 0,0, 0);
    GRAPH_SetColor(hItem,0x00333333,GRAPH_CI_BK);
    GRAPH_SetColor(hItem,0x000455ea,GRAPH_CI_BORDER);
   // GRAPH_SetColor(hItem,GUI_WHITE,GRAPH_CI_GRID);

    GRAPH_SetGridVis(hItem,1);
    GRAPH_SetGridDistX(hItem,20);//设X栅格 为20
    GRAPH_SetGridDistY(hItem,20);//设Y栅格
		
		#ifdef UseDefaultScale
     hScale = GRAPH_SCALE_Create(135,GUI_TS_NORMAL,GRAPH_SCALE_CF_HORIZONTAL,64);//产生X轴坐标小工具
    //hItem = GRAPH_SCALE_Create(20,GUI_TS_NORMAL,GRAPH_SCALE_CF_VERTICAL,10);
    GRAPH_AttachScale(hItem,hScale);//添加X坐标刻度
   // GRAPH_SCALE_SetFactor(hScale,0.1);//比例因子
    GRAPH_SCALE_SetTextColor(hScale,GUI_WHITE);
		GRAPH_SCALE_SetFont(hScale,GUI_FONT_16B_1);

   // hScale = GRAPH_SCALE_Create(0,GUI_TS_NORMAL,GRAPH_SCALE_CF_VERTICAL,20);//产生Y轴坐标小工具
   // hScale = GRAPH_SCALE_Create(20,GUI_TA_CENTER,GRAPH_SCALE_CF_VERTICAL,20);//产生Y轴坐标小工具
    hScale = GRAPH_SCALE_Create(4,GUI_TS_NORMAL,GRAPH_SCALE_CF_VERTICAL,30);
    GRAPH_SCALE_SetOff(hScale,75);//产生负坐标
    GRAPH_AttachScale(hItem,hScale);//添加Y坐标刻度
    GRAPH_SCALE_SetFactor(hScale,0.15);//比例因子
    GRAPH_SCALE_SetTextColor(hScale,GUI_YELLOW);
		GRAPH_SCALE_SetFont(hScale,GUI_FONT_16B_1);
		#endif
		
		
		
		/*创建数据对象 ********************** */
     ahDataA = GRAPH_DATA_YT_Create(GUI_YELLOW,758,0,0);
     /* 数据对象添加到图形控件*/
     GRAPH_AttachData(hItem,ahDataA);
		 GRAPH_DATA_YT_SetAlign(ahDataA,GRAPH_ALIGN_LEFT);//向左对齐
		 
		 /*创建数据对象 ********************** */
     ahDataB = GRAPH_DATA_YT_Create(GUI_GREEN,758,0,0);
     /* 数据对象添加到图形控件*/
     GRAPH_AttachData(hItem,ahDataB);
		 GRAPH_DATA_YT_SetAlign(ahDataB,GRAPH_ALIGN_LEFT);//向左对齐
		 
		 /*创建数据对象 ********************** */
     ahDataC = GRAPH_DATA_YT_Create(GUI_RED,758,0,0);
     /* 数据对象添加到图形控件*/
     GRAPH_AttachData(hItem,ahDataC);
		 GRAPH_DATA_YT_SetAlign(ahDataC,GRAPH_ALIGN_LEFT);//向左对齐
		 
		 /*创建数据对象 ********************** */
     ahDataU0 = GRAPH_DATA_YT_Create(GUI_WHITE,758,0,0);
     /* 数据对象添加到图形控件*/
     GRAPH_AttachData(hItem,ahDataU0);
		 GRAPH_DATA_YT_SetAlign(ahDataU0,GRAPH_ALIGN_LEFT);//向左对齐
		 
		
		//===============电流显示波形初始化
		hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_1);
    //GRAPH_SetBorder(hItem, 20, 0,0, 0);
		GRAPH_SetBorder(hItem, 2, 0,0, 0);
    GRAPH_SetColor(hItem,0x00333333,GRAPH_CI_BK);
    GRAPH_SetColor(hItem,0x000455ea,GRAPH_CI_BORDER);
   // GRAPH_SetColor(hItem,GUI_WHITE,GRAPH_CI_GRID);

    GRAPH_SetGridVis(hItem,1);
    GRAPH_SetGridDistX(hItem,20);//设X栅格 为20
    GRAPH_SetGridDistY(hItem,20);//设Y栅格
		#ifdef UseDefaultScale
     hScale = GRAPH_SCALE_Create(135,GUI_TS_NORMAL,GRAPH_SCALE_CF_HORIZONTAL,64);//产生X轴坐标小工具
    //hItem = GRAPH_SCALE_Create(20,GUI_TS_NORMAL,GRAPH_SCALE_CF_VERTICAL,10);
    GRAPH_AttachScale(hItem,hScale);//添加X坐标刻度
    //GRAPH_SCALE_SetFactor(hScale,0.1);//比例因子
    GRAPH_SCALE_SetTextColor(hScale,GUI_WHITE);
		GRAPH_SCALE_SetFont(hScale,GUI_FONT_16B_1);
		

   // hScale = GRAPH_SCALE_Create(0,GUI_TS_NORMAL,GRAPH_SCALE_CF_VERTICAL,20);//产生Y轴坐标小工具
   // hScale = GRAPH_SCALE_Create(20,GUI_TA_CENTER,GRAPH_SCALE_CF_VERTICAL,20);//产生Y轴坐标小工具
    hScale = GRAPH_SCALE_Create(4,GUI_TS_NORMAL,GRAPH_SCALE_CF_VERTICAL,30);
    GRAPH_SCALE_SetOff(hScale,75);//产生负坐标
    GRAPH_AttachScale(hItem,hScale);//添加Y坐标刻度
    GRAPH_SCALE_SetFactor(hScale,0.15);//比例因子
    GRAPH_SCALE_SetTextColor(hScale,GUI_YELLOW);
		GRAPH_SCALE_SetFont(hScale,GUI_FONT_16B_1);
		#endif
		
		
		/*创建数据对象 ********************** */
     ahDataIA = GRAPH_DATA_YT_Create(GUI_YELLOW,758,0,0);
     /* 数据对象添加到图形控件*/
     GRAPH_AttachData(hItem,ahDataIA);
		 GRAPH_DATA_YT_SetAlign(ahDataIA,GRAPH_ALIGN_LEFT);//向左对齐
		 
		 /*创建数据对象 ********************** */
     ahDataIB = GRAPH_DATA_YT_Create(GUI_GREEN,758,0,0);
     /* 数据对象添加到图形控件*/
     GRAPH_AttachData(hItem,ahDataIB);
		 GRAPH_DATA_YT_SetAlign(ahDataIB,GRAPH_ALIGN_LEFT);//向左对齐
		 
		 /*创建数据对象 ********************** */
     ahDataIC = GRAPH_DATA_YT_Create(GUI_RED,758,0,0);
     /* 数据对象添加到图形控件*/
     GRAPH_AttachData(hItem,ahDataIC);
		 GRAPH_DATA_YT_SetAlign(ahDataIC,GRAPH_ALIGN_LEFT);//向左对齐
		
		  /*创建数据对象 ********************** */
     ahDataI0 = GRAPH_DATA_YT_Create(GUI_WHITE,758,0,0);
     /* 数据对象添加到图形控件*/
     GRAPH_AttachData(hItem,ahDataI0);
		 GRAPH_DATA_YT_SetAlign(ahDataI0,GRAPH_ALIGN_LEFT);//向左对齐
		
		
		

	
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'RealMonitor'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'SystemSet'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				ModuleComSet10Fault(0x01,0x00,0x00);
				Clear_Graph();
				GUI_EndDialog(pMsg->hWin,0);
        //CreateTempHumdity_S();//鍒囨崲鍒扮郴缁熻缃敾闈?
				Createpassword();
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'AlarmList'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				ModuleComSet10Fault(0x01,0x00,0x00);
				Clear_Graph();
				GUI_EndDialog(pMsg->hWin,0);
        CreateOverVoltage_A();//鍒囨崲鍒版姤璀︾敾闈?
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'PowerQuality'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				ModuleComSet10Fault(0x01,0x00,0x00);
				Clear_Graph();
				GUI_EndDialog(pMsg->hWin,0);
        CreatePowerQuality();//鍒囨崲鍒扮數婧愯川閲忕敾闈?
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
			#if 0
    case ID_BUTTON_4: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				ModuleComSet10Fault(0x01,0x00,0x00);
				Clear_Graph();
				GUI_EndDialog(pMsg->hWin,0);
        Createaboutus();
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
			#endif
    case ID_BUTTON_5: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				ModuleComSet10Fault(0x01,0x00,0x00);
        // USER START (Optionally insert code for reacting on notification message)
				Clear_Graph();
				GUI_EndDialog(pMsg->hWin,0);
        CreateMain();
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
			#if 0
    case ID_BUTTON_6: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
			#endif
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateMain_Graph
*/
WM_HWIN CreateMain_Graph(void);
WM_HWIN CreateMain_Graph(void) {
  WM_HWIN hWin;
	WM_HTIMER hTimer;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	hTimer =WM_CreateTimer(WM_GetClientWindow(hWin),0,10,0);
	(void)hTimer;
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
