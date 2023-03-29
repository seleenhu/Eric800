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
#include<stdlib.h>
#include "Emwin_eric800.h"
#include "InitSystem.h"
#include "NumPadDLG.h"
#include "param.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_TEXT_0  (GUI_ID_USER + 0x01)
#define ID_EDIT_0  (GUI_ID_USER + 0x02)
#define ID_BUTTON_0  (GUI_ID_USER + 0x03)
#define ID_BUTTON_1  (GUI_ID_USER + 0x04)
#define ID_TEXT_1		(GUI_ID_USER + 0x05)


// USER START (Optionally insert additional defines)
// USER END
#define WM_SetValue 		WM_USER + 0x01 /* 自定义消息 */ // -------------- (2)
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
char getpasswords[5];
WM_HWIN PS_hWin;
// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  //{ FRAMEWIN_CreateIndirect, "password", ID_FRAMEWIN_0, 118, 57, 320, 240, 0, 0, 0 },
	{ FRAMEWIN_CreateIndirect, "password", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0, 0 },
  { TEXT_CreateIndirect, "passwords", ID_TEXT_0, 100+100, 100, 125, 40, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 250+100, 100, 100, 30, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Enter", ID_BUTTON_0, 100+100, 250, 80, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Cancel", ID_BUTTON_1, 200+100, 250, 80, 40, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_1, 100+100, 200, 150, 30, 0, 0x0, 0 },
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
	
  int     NCode;
  int     Id;
	//int 	pdata;
	int   data;
	
	//BUTTON_SKINFLEX_PROPS Bprops;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
	case WM_SetValue:
		#if 0
			data=1000*Setting.Passwords[0]+100*Setting.Passwords[1]+10*Setting.Passwords[2]+Setting.Passwords[3];
			if(getpasswords[4]==0x00&&data==atoi(getpasswords))
			{
				GUI_EndDialog(pMsg->hWin,0);
				CreateTempHumdity_S();
				
			}
			else //退出
			{
				
			}
			#endif
			break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'password'
    //
		SetIndex=7;
	 #if 0
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 30);
		FRAMEWIN_SetBarColor(hItem,0,0x000455ea);
		FRAMEWIN_SetBarColor(hItem,1,0x000455ea);
    FRAMEWIN_SetText(hItem, "");
	#endif
		hItem = pMsg->hWin;
    FRAMEWIN_SetTitleVis(hItem, 0);
    FRAMEWIN_SetClientColor(hItem,0x00333333);
	
    //
    // Initialization of 'passwords'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
		TEXT_SetTextColor(hItem,GUI_WHITE);
    TEXT_SetFont(hItem, &XBF_Font32);
		TEXT_SetText(hItem,"密码：");
	
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
    EDIT_SetText(hItem, "");
    EDIT_SetFont(hItem, GUI_FONT_24B_ASCII);
    //
    // Initialization of 'Retrun'
    //
		/*
		BUTTON_GetSkinFlexProps(&Bprops,BUTTON_SKINFLEX_PI_ENABLED);
		Bprops.Radius=2;
		Bprops.aColorUpper[0]=GUI_GREEN;
		Bprops.aColorLower[0]=GUI_GREEN;
		Bprops.aColorUpper[1]=GUI_GREEN;
		Bprops.aColorLower[1]=GUI_GREEN;
		BUTTON_SetSkinFlexProps(&Bprops,BUTTON_SKINFLEX_PI_ENABLED);
		*/
		
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetSkinClassic(hItem);
    BUTTON_SetFont(hItem, &XBF_Font24);
		BUTTON_SetText(hItem,"确定");
		BUTTON_SetBkColor(hItem,BUTTON_CI_UNPRESSED,GUI_GREEN);
		
		
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetSkinClassic(hItem);
    BUTTON_SetFont(hItem, &XBF_Font24);
		BUTTON_SetText(hItem,"取消");
		BUTTON_SetBkColor(hItem,BUTTON_CI_UNPRESSED,GUI_RED);
		
		
		getpasswords[0]=0x31;
		getpasswords[1]=0x31;
		getpasswords[2]=0x31;
		getpasswords[3]=0x31;
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_EDIT_0: // Notifications sent by 'Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				hItem=CreateNumPad();
				WM_SetStayOnTop(hItem,1);//将小键盘置顶
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				
        // USER END
				
        break;
        
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
				EDIT_GetText(hItem,getpasswords,10);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'Enter'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				#if 0
				data=1000*Setting.Passwords[0]+100*Setting.Passwords[1]+10*Setting.Passwords[2]+Setting.Passwords[3];
			  if(getpasswords[4]==0x00&&data==atoi(getpasswords))
				{
					GUI_EndDialog(pMsg->hWin,0);
					CreateTempHumdity_S();
				}	
				else
				{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
				TEXT_SetTextColor(hItem, GUI_RED);
		
				TEXT_SetFont(hItem, &XBF_Font32);
				TEXT_SetText(hItem,"密码错误");
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
				EDIT_SetText(hItem, "");	
				}
				#endif
				GUI_EndDialog(pMsg->hWin,0);
				CreateTempHumdity_S();
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
			
		case ID_BUTTON_1: // Notifications sent by 'Cancel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				GUI_EndDialog(pMsg->hWin,0);
				CreateMain();//切换到主画面
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
*       Createpassword
*/
WM_HWIN Createpassword(void);
WM_HWIN Createpassword(void) {
  //WM_HWIN hWin;

  PS_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return PS_hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/