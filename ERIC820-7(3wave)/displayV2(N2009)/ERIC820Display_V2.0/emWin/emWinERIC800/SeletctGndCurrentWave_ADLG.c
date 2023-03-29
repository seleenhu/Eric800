﻿/*********************************************************************
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


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0  	(GUI_ID_USER + 0x00)
#define ID_GRAPH_0		(GUI_ID_USER + 0x01)	
#define ID_GRAPH_1		(GUI_ID_USER + 0x02)
#define ID_GRAPH_2		(GUI_ID_USER + 0x03)	
#define ID_TEXT_0			(GUI_ID_USER + 0x04)
#define ID_TEXT_1			(GUI_ID_USER + 0x05)
#define ID_TEXT_2			(GUI_ID_USER + 0x06)
#define ID_TEXT_3			(GUI_ID_USER + 0x07)
#define ID_TEXT_4			(GUI_ID_USER + 0x08)
#define ID_TEXT_5			(GUI_ID_USER + 0x09)

#define ID_TEXT_10		(GUI_ID_USER + 0x20)
#define ID_TEXT_20		(GUI_ID_USER + 0x21)
#define ID_TEXT_30		(GUI_ID_USER + 0x22)
#define ID_BUTTON_25	(GUI_ID_USER + 0x30)

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static GRAPH_DATA_Handle ahDataI0; // ------(1)
static GRAPH_DATA_Handle ahDataI1; // ------(2)
static GRAPH_DATA_Handle ahDataI2; // ------(3)

// USER START (Optionally insert additional static data)
WM_HWIN SelGndCurrentW_hWin;

// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "SelGndCurrentWDLG", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x0, 0 },
	{ GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 20, 30, 780, 120, 0, 0x0, 0 },
	{ GRAPH_CreateIndirect, "Graph", ID_GRAPH_1, 20, 180, 780, 120, 0, 0x0, 0 },
	{ GRAPH_CreateIndirect, "Graph", ID_GRAPH_2, 20, 330, 780, 120, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_0,  20, 00+2, 50, 25, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "", ID_TEXT_1,  70, 00+2, 110, 25, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "", ID_TEXT_2,  20, 150+2, 50, 25, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "", ID_TEXT_3,  70, 150+2, 110, 25, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "", ID_TEXT_4,  20, 300+2, 50, 25, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "", ID_TEXT_5,  70, 300+2, 110, 25, 0, 0x0, 0},
	
	{ TEXT_CreateIndirect, "", ID_TEXT_10,  220, 00+2, 150, 25, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "", ID_TEXT_20,  220, 150+2, 150, 25, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "", ID_TEXT_30,  220, 300+2, 150, 25, 0, 0x0, 0},
	
	{ BUTTON_CreateIndirect, "", ID_BUTTON_25, 720, 0, 80, 30, 0, 0x0, 0 },
	
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static float GetScale(void)
{
	uint8_t n;
	int16_t temp, Wave=0;
	float Scale;	
			
	for(n=0;n<32*3;n++){
	temp=	ModuleCombuf->FaultAllSelComBuf.Wave[0][3*32+n];
	temp=abs(temp);
  if(temp>Wave)
		Wave=temp;
	}		
 if(Wave==0)
		Wave=1;
 Scale=(Wave/ModuleCombuf->FaultAllSelComBuf.ratio[0])/50;
 return Scale;
}

static void _cbButton25(WM_MESSAGE * pMsg)
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
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("关闭", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  // USER START (Optionally insert additional variables)
  // USER END
	int     NCode;
  int     Id;
	float Sort_D[4];
	int i,n;
	float VoltageTemp[3];
	char s[128];
	int16_t Wave[3];
	static float Scale;
	
  switch (pMsg->MsgId) {
	case WM_DisplayWave:
			//pt=Setting.RatioStruct.RatioPT;
		// ReadSDAlarmVALenght();
		//hWinOld= WM_SelectWindow(WM_GetClientWindow(pMsg->hWin));	
		// if(AlarmGaCount<=0)
		
		if(SDGroundFault_sum<=0)
		 {
			  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
				TEXT_SetTextColor(hItem, GUI_GREEN); 
				TEXT_SetFont(hItem, &XBF_Font24);
				TEXT_SetText(hItem,"没有故障");
			  
			  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
			  TEXT_SetText(hItem,"");
			 
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
			  TEXT_SetText(hItem,"");
			 
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
			  TEXT_SetText(hItem,"");
			 
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
			  TEXT_SetText(hItem,"");
			 
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
			  TEXT_SetText(hItem,"");
			 
			 
		 }else{	 

		//=================I0
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
		TEXT_SetTextColor(hItem, GUI_RED); 
    //TEXT_SetFont(hItem, GUI_FONT_20B_ASCII);
		TEXT_SetFont(hItem, &XBF_Font24);
		
		if(0 == ModuleCombuf->FaultAllSelComBuf.CanId[0]){
			//	sprintf(s,"%d路",ModuleCombuf->FaultAllSelComBuf.FaultFlag[0]+1);
					sprintf(s,"无CAN");
		}else{
				sprintf(s,"CAN:");
		}
		TEXT_SetText(hItem,s);
		
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
		TEXT_SetTextColor(hItem, GUI_RED); 
		TEXT_SetFont(hItem, &XBF_Font24);
		
		if(0 == ModuleCombuf->FaultAllSelComBuf.CanId[0]){
				//sprintf(s,"零序接地");
				sprintf(s,"接入");
		}else{
				sprintf(s,"%d零序接地",ModuleCombuf->FaultAllSelComBuf.CanId[0]);
		}
		TEXT_SetText(hItem,s);
		
		//===============I1
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
		TEXT_SetTextColor(hItem, GUI_GREEN); 
    //TEXT_SetFont(hItem, GUI_FONT_20B_ASCII);
		TEXT_SetFont(hItem, &XBF_Font24);
		
		if(0 == ModuleCombuf->FaultAllSelComBuf.CanId[1]){
			//sprintf(s,"%d路",ModuleCombuf->FaultAllSelComBuf.FaultFlag[1]+1);
			sprintf(s,"无CAN");
		}else{
				sprintf(s,"CAN:");
		}
		TEXT_SetText(hItem,s);
		
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
		TEXT_SetTextColor(hItem, GUI_GREEN); 
		TEXT_SetFont(hItem, &XBF_Font24);
		
		if(0 == ModuleCombuf->FaultAllSelComBuf.CanId[1]){
				//sprintf(s,"零序接地");
				sprintf(s,"接入");
		}else{
				sprintf(s,"%d零序接地",ModuleCombuf->FaultAllSelComBuf.CanId[1]);
		}
		TEXT_SetText(hItem,s);
		
		//==============I2
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
		TEXT_SetTextColor(hItem, GUI_GREEN); 
    //TEXT_SetFont(hItem, GUI_FONT_20B_ASCII);
		TEXT_SetFont(hItem, &XBF_Font24);
		
		if(0 == ModuleCombuf->FaultAllSelComBuf.CanId[2]){
				//sprintf(s,"%d路",ModuleCombuf->FaultAllSelComBuf.FaultFlag[2]+1);
				sprintf(s,"无CAN");
		}else{
				sprintf(s,"CAN:");
		}
		TEXT_SetText(hItem,s);
		
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
		TEXT_SetTextColor(hItem, GUI_GREEN); 
		TEXT_SetFont(hItem, &XBF_Font24);
		
		if(0 == ModuleCombuf->FaultAllSelComBuf.CanId[2]){
				//sprintf(s,"零序接地");
			 sprintf(s,"接入");
		}else{
				sprintf(s,"%d零序接地",ModuleCombuf->FaultAllSelComBuf.CanId[2]);
		}
		TEXT_SetText(hItem,s);
		
		/*
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_20);	
		TEXT_SetFont(hItem,GUI_FONT_24B_ASCII);
		TEXT_SetTextColor(hItem, GUI_YELLOW);
		TEXT_SetText(hItem,"A");
*/	
		//===I0 RMS	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_10);
		TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
		VoltageTemp[0]=ModuleCombuf->FaultAllSelComBuf.RMSValue[0];
		Sort_D[0]=VoltageTemp[0]*SystemSet.SelRatioCTBuf[0];
		sprintf(s,"I0 =%6.2fA",Sort_D[0]);
		TEXT_SetTextColor(hItem, GUI_RED);
		TEXT_SetText(hItem,s);
		//===I1 RMS	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_20);
		TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
		VoltageTemp[1]=ModuleCombuf->FaultAllSelComBuf.RMSValue[1];
		Sort_D[1]=VoltageTemp[1]*SystemSet.SelRatioCTBuf[0];
		sprintf(s,"I1 =%6.2fA",Sort_D[1]);
		TEXT_SetTextColor(hItem, GUI_GREEN);
		TEXT_SetText(hItem,s);
		//===I2 RMS	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_30);
		TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
		VoltageTemp[2]=ModuleCombuf->FaultAllSelComBuf.RMSValue[2];
		Sort_D[2]=VoltageTemp[2]*SystemSet.SelRatioCTBuf[0];
		sprintf(s,"I2 =%6.2fA",Sort_D[2]);
		TEXT_SetTextColor(hItem, GUI_GREEN);
		TEXT_SetText(hItem,s);
		
		
		
		GRAPH_DATA_YT_Clear(ahDataI0);
		GRAPH_DATA_YT_Clear(ahDataI1);
		GRAPH_DATA_YT_Clear(ahDataI2);		
		//============I0 Graph	
		//Scale=VoltageTemp/(1.1/2);
		//Scale=VoltageTemp[0]/(10/1.42);
		Scale=GetScale();	 
		for(i=0;i<12;i++)//中心点在第10周期 前4后8
		{	
			for(n=0;n<64;n++){
				Wave[0] = ModuleCombuf->FaultAllSelComBuf.Wave[0][i*32+n/2];							
				GRAPH_DATA_YT_AddValue(ahDataI0,Wave[0]/(Scale*ModuleCombuf->FaultAllSelComBuf.ratio[0])+60);	
			}		 
		}
		
		//============I1 Graph	
		//Scale=VoltageTemp/(1.1/2);
		//Scale=VoltageTemp[1]/(30/2);
			 
		for(i=0;i<12;i++)//中心点在第10周期 前4后8
		{	
			for(n=0;n<64;n++){
				Wave[1] = ModuleCombuf->FaultAllSelComBuf.Wave[1][i*32+n/2];							
				GRAPH_DATA_YT_AddValue(ahDataI1,Wave[1]/(Scale*ModuleCombuf->FaultAllSelComBuf.ratio[1])+60);	
			}		 
		}
		
		
		//============I2 Graph	
		//Scale=VoltageTemp/(1.1/2);
		//Scale=VoltageTemp[2]/(30/2);
			 
		for(i=0;i<12;i++)//中心点在第10周期 前4后8
		{	
			for(n=0;n<64;n++){
				Wave[2] = ModuleCombuf->FaultAllSelComBuf.Wave[2][i*32+n/2];							
				GRAPH_DATA_YT_AddValue(ahDataI2,Wave[2]/(Scale*ModuleCombuf->FaultAllSelComBuf.ratio[2])+60);	
			}		 
		}
		
		
				
		
	}
		 
  break;		

		
  case WM_PAINT:
			//=======标Y轴刻度
		
			
		GUI_SetColor(GUI_YELLOW);
		GUI_SetFont(GUI_FONT_24B_ASCII);	 
		
		GUI_DispStringAt("A",2,32);
				
		GUI_DispStringAt("A",2,182);
				
		GUI_DispStringAt("A ",2,332);
			
		
	 
		//=======标X轴刻度
				GUI_DispStringAt("-80",20,450);
				GUI_DispStringAt("-40",276-128,450);
				GUI_DispStringAt("0",276,450);
				GUI_DispStringAt("40",276+128,450);
				GUI_DispStringAt("80",276+256,450);
				GUI_DispStringAt("120",276+384,450);
				GUI_DispStringAt("120",276+384,450);
				GUI_DispStringAt("ms",760,450);
  	break;	
  case WM_INIT_DIALOG:
    //
    // Initialization of 'VetorDLG'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem,0x00333333);
	
		//==== Initialization of 'Graph0'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
    GRAPH_SetBorder(hItem, 2, 0, 0, 0);
    GRAPH_SetColor(hItem,0x00333333,GRAPH_CI_BK);
    GRAPH_SetColor(hItem,0x000455ea,GRAPH_CI_BORDER);
   // GRAPH_SetColor(hItem,GUI_WHITE,GRAPH_CI_GRID);

    GRAPH_SetGridVis(hItem,1);
    GRAPH_SetGridDistX(hItem,20);//设X栅格 为20
    GRAPH_SetGridDistY(hItem,20);//设Y栅格

	/*创建数据对象 ********************** */
		ahDataI0 = GRAPH_DATA_YT_Create(GUI_YELLOW,780,0,0);
    /* 数据对象添加到图形控件*/
    GRAPH_AttachData(hItem,ahDataI0);
		GRAPH_DATA_YT_SetAlign(ahDataI0,GRAPH_ALIGN_LEFT);//向左对齐
		
		
		//==== Initialization of 'Graph1'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_1);
    GRAPH_SetBorder(hItem, 2, 0, 0, 0);
    GRAPH_SetColor(hItem,0x00333333,GRAPH_CI_BK);
    GRAPH_SetColor(hItem,0x000455ea,GRAPH_CI_BORDER);
   // GRAPH_SetColor(hItem,GUI_WHITE,GRAPH_CI_GRID);

    GRAPH_SetGridVis(hItem,1);
    GRAPH_SetGridDistX(hItem,20);//设X栅格 为20
    GRAPH_SetGridDistY(hItem,20);//设Y栅格
	

	/*创建数据对象 ********************** */
    ahDataI1 = GRAPH_DATA_YT_Create(GUI_YELLOW,780,0,0);
    /* 数据对象添加到图形控件*/
    GRAPH_AttachData(hItem,ahDataI1);
		GRAPH_DATA_YT_SetAlign(ahDataI1,GRAPH_ALIGN_LEFT);//向左对齐
		 
	//==== Initialization of 'Graph2'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_2);
    GRAPH_SetBorder(hItem, 2, 0, 0, 0);
    GRAPH_SetColor(hItem,0x00333333,GRAPH_CI_BK);
    GRAPH_SetColor(hItem,0x000455ea,GRAPH_CI_BORDER);
   // GRAPH_SetColor(hItem,GUI_WHITE,GRAPH_CI_GRID);

    GRAPH_SetGridVis(hItem,1);
    GRAPH_SetGridDistX(hItem,20);//设X栅格 为20
    GRAPH_SetGridDistY(hItem,20);//设Y栅格
	

	/*创建数据对象 ********************** */
    ahDataI2 = GRAPH_DATA_YT_Create(GUI_YELLOW,780,0,0);
    /* 数据对象添加到图形控件*/
    GRAPH_AttachData(hItem,ahDataI2);
		GRAPH_DATA_YT_SetAlign(ahDataI2,GRAPH_ALIGN_LEFT);//向左对齐
		
	

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_25);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton25);
		
	//==================读取接地故障
	StartReadSD_GASEL_Flag=1;
	//GA_WavePosition=0;
	//SystemAlarm->GroundAlarm.ResetFlag=0;
	ModuleCombuf->ResetFlag = 0;
		
    
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
		
	 case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
	 switch(Id) {
		 case ID_BUTTON_25:
				switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				GUI_EndDialog(pMsg->hWin,0);
        //CreateMain();
				CreateGround_A();
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
*       CreateVetorDLG
*/
WM_HWIN CreateSelGndCurrentWaveDLG(void);
WM_HWIN CreateSelGndCurrentWaveDLG(void) {
 // WM_HWIN hWin;

  SelGndCurrentW_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return SelGndCurrentW_hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
