﻿/*
*******************************************************************
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
WM_HWIN PW_hWin;
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_LISTVIEW_0  (GUI_ID_USER + 0x01)


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
  { FRAMEWIN_CreateIndirect, "Protector_W", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 0, 0, 800, 443, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
	HEADER_Handle hHeader;
  int     NCode;
  int     Id;
	uint8_t i,n;
	uint32_t temp32;
	uint8_t temp8;
	float temp_f;
	char s[10];
	char s1[10];
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Protector_W'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 30);
    FRAMEWIN_SetText(hItem, "保护器状态");
		FRAMEWIN_SetTextColor(hItem,GUI_WHITE);
    FRAMEWIN_SetFont(hItem, &XBF_Font24);
		
	  FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    //
    // Initialization of 'Listview'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
		LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL,GUI_BLACK);
	  hHeader=LISTVIEW_GetHeader(hItem);
		HEADER_SetFont(hHeader,&XBF_Font24);
    LISTVIEW_AddColumn(hItem, 80, "ID", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 50, "柜号" ,GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 80, "类型",GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 80, "等级",GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 100, "参考电压", GUI_TA_HCENTER | GUI_TA_VCENTER);
    //LISTVIEW_AddRow(hItem, NULL);
    LISTVIEW_SetGridVis(hItem, 1);
    LISTVIEW_AddColumn(hItem, 100, "动作电压", GUI_TA_HCENTER | GUI_TA_VCENTER);
  
    LISTVIEW_AddColumn(hItem, 80, "状态",GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 300,"处理意见",GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_SetFont(hItem, &XBF_Font16);
    LISTVIEW_SetRowHeight(hItem, 20);
    LISTVIEW_SetHeaderHeight(hItem, 25);
		
		for(i=0;i<20;i++)
		{
		LISTVIEW_AddRow(hItem, NULL);
		LISTVIEW_SetFont(hItem,&XBF_Font16);
		temp32=Cupboard_P[i][11].Id&0x00ffffff;
		sprintf(s,"%04d",temp32);
		LISTVIEW_SetItemText(hItem, 0,i,s);//显示ID
		sprintf(s,"%02d",i+1);
		LISTVIEW_SetItemText(hItem, 1,i,s);//
//============================================			
			
		temp8=Setting.ProtectorType[i];
		switch(temp8)
		{
			case 0:
				LISTVIEW_SetItemText(hItem, 2,i,"电站型");//
				break;
			case 1:
				LISTVIEW_SetItemText(hItem, 2,i,"电机型");//
				break;
			case 2:
				LISTVIEW_SetItemText(hItem, 2,i,"电容型");//
				break;
			
			default:
				LISTVIEW_SetItemText(hItem, 2,i,"---");//
				break;
		}			
//============================================			
		temp32=Setting.VoltageSet.MaxRatedValue;
		temp_f=temp32/1000;
		sprintf(s,"%4.2fKV",temp_f);	
		LISTVIEW_SetItemText(hItem, 3,i,s);//电压等级
		
		
		
//===============================参考电压
	 switch(temp32)
	 {
		 case 6000:
			 switch(temp8)
			 {
				 case 0:
					 sprintf(s,"20.4KV");
				 break;
				 case 1:
					 sprintf(s,"15KV");
				 break;
				 case 2:
					 sprintf(s,"20.4KV");
				 break;
				 default:
					 sprintf(s,"---");
				 break;	 
			 }
			 break;
		 case 10000:
			 switch(temp8)
			 {
				 case 0:
					 sprintf(s,"35KV");
				 break;
				 case 1:
					 sprintf(s,"25KV");
				 break;
				 case 2:
					 sprintf(s,"34.5KV");
				 break;
				 default:
					 sprintf(s,"---");
				 break;	 
			 }
			 break;
		 case 35000:
			 switch(temp8)
			 {
				 case 0:
					 sprintf(s,"105.4KV");
				 break;
				 case 1:
					 sprintf(s,"---");
				 break;
				 case 2:
					 sprintf(s,"105KV");
				 break;
				 default:
					 sprintf(s,"---");
				 break;	 
			 }
			 break;
		 default:
			 sprintf(s,"---");
			 break;
	 }
		
		LISTVIEW_SetItemText(hItem, 4,i,s);//参考电压
	 
		
	 for(n=0;n<MaxActionSave;n++)
	 {
		 temp32=ActionProtectionSave[n].Protector.ProtectorAction.Id;
		 if(temp32==0)
		 {
			 sprintf(s,"---");
			 sprintf(s1,"---");
			 break;
		 }
		 if(temp32==Cupboard_P[i][11].Id)
		 {
			 sprintf(s,"动作");
			 
			 temp8=SDSuddentAlarmData->FaultStyle;
			 switch(temp8)
			 {
				 case 0: //A相
					 sprintf(s1,"A %6.2f",SDSuddentAlarmData->RMSValue[0][1]);
					 break;
				 case 1://B相
					 sprintf(s1,"B %6.2f",SDSuddentAlarmData->RMSValue[1][1]);
					 break;
				 case 2://C相
					 sprintf(s1,"C %6.2f",SDSuddentAlarmData->RMSValue[2][1]);
					 break;
				 default:
					 sprintf(s1,"---");
					 break;
			 }
			 
			 
			 break;
		 }
		 if(n==MaxActionSave)
			sprintf(s,"---");
			sprintf(s1,"---");
	 }
	 
		LISTVIEW_SetItemText(hItem, 5,i,s1);//动作电压
	  LISTVIEW_SetItemText(hItem, 6,i,s);//动作		
		
		if(i%2==0)
		{	
		LISTVIEW_SetItemBkColor(hItem,0,i,LISTVIEW_CI_UNSEL,0x00fdf7ea);
		LISTVIEW_SetItemBkColor(hItem,1,i,LISTVIEW_CI_UNSEL,0x00fdf7ea);
		LISTVIEW_SetItemBkColor(hItem,2,i,LISTVIEW_CI_UNSEL,0x00fdf7ea);	
		LISTVIEW_SetItemBkColor(hItem,3,i,LISTVIEW_CI_UNSEL,0x00fdf7ea);
		LISTVIEW_SetItemBkColor(hItem,4,i,LISTVIEW_CI_UNSEL,0x00fdf7ea);
		LISTVIEW_SetItemBkColor(hItem,5,i,LISTVIEW_CI_UNSEL,0x00fdf7ea);
		LISTVIEW_SetItemBkColor(hItem,6,i,LISTVIEW_CI_UNSEL,0x00fdf7ea);
		LISTVIEW_SetItemBkColor(hItem,7,i,LISTVIEW_CI_UNSEL,0x00fdf7ea);		
		}
		else
		{
		LISTVIEW_SetItemBkColor(hItem,0,i,LISTVIEW_CI_UNSEL,0x00f1f1fd);
		LISTVIEW_SetItemBkColor(hItem,1,i,LISTVIEW_CI_UNSEL,0x00f1f1fd);
		LISTVIEW_SetItemBkColor(hItem,2,i,LISTVIEW_CI_UNSEL,0x00f1f1fd);		
		LISTVIEW_SetItemBkColor(hItem,3,i,LISTVIEW_CI_UNSEL,0x00f1f1fd);
		LISTVIEW_SetItemBkColor(hItem,4,i,LISTVIEW_CI_UNSEL,0x00f1f1fd);
		LISTVIEW_SetItemBkColor(hItem,5,i,LISTVIEW_CI_UNSEL,0x00f1f1fd);
		LISTVIEW_SetItemBkColor(hItem,6,i,LISTVIEW_CI_UNSEL,0x00f1f1fd);
		LISTVIEW_SetItemBkColor(hItem,7,i,LISTVIEW_CI_UNSEL,0x00f1f1fd);				
		}	
	 
	 
				
		}
		
		
		
		
		
		
		
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_LISTVIEW_0: // Notifications sent by 'Listview'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
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
*       CreateProtector_W
*/
WM_HWIN CreateProtector_W(void);
WM_HWIN CreateProtector_W(void) {
  //WM_HWIN PW_hWin;

  PW_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return PW_hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
