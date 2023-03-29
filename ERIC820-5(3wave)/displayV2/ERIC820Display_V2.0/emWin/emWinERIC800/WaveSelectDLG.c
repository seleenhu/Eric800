﻿/*
**********************************************************************
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
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_BUTTON_0  (GUI_ID_USER + 0x01)
#define ID_BUTTON_1  (GUI_ID_USER + 0x02)
#define ID_BUTTON_2  (GUI_ID_USER + 0x03)
#define ID_BUTTON_3  (GUI_ID_USER + 0x04)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
WM_HWIN WS_hWin;
// USER START (Optionally insert additional static data)
uint8_t  SelectWave;
uint8_t SelectWaveOpen;

// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "WaveSelect", ID_FRAMEWIN_0, 430, 180, 130, 196, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "VoltageW", ID_BUTTON_0, 2, 2, 120, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3U0W", ID_BUTTON_1, 2, 49, 120, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CurrentW", ID_BUTTON_2, 2, 96, 120, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "VectorW", ID_BUTTON_3, 2, 143, 120, 45, 0, 0x0, 0 },
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
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'WaveSelect'
    //
	ScreenClrTim();
		SelectWaveOpen=1;
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleVis(hItem, 0);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetFont(hItem,&XBF_Font24);
		BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,0x000455ea);
		BUTTON_SetText(hItem,"电压波形");
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem,&XBF_Font24);
		BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,0x000455ea);
		BUTTON_SetText(hItem,"3U0波形");
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
		BUTTON_SetFont(hItem,&XBF_Font24);
		BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,0x000455ea);
		BUTTON_SetText(hItem,"电流波形");
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
		BUTTON_SetFont(hItem,&XBF_Font24);
		BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,0x000455ea);
		BUTTON_SetText(hItem,"矢量图");
		
    break;
  case WM_TIMER:
	WM_RestartTimer(pMsg->Data.v,1000);
	if(ScreenCloseDisplay()){
		SelectWaveOpen=0;
		GUI_EndDialog(pMsg->hWin,0); 
	}
	break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'VoltageW'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				
			  SelectWave=0;
				SelectWaveOpen=0;
				GUI_EndDialog(pMsg->hWin,0); 
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
    case ID_BUTTON_1: // Notifications sent by '3U0W'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				
			  SelectWave=1;
				SelectWaveOpen=0;
				GUI_EndDialog(pMsg->hWin,0); 
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
    case ID_BUTTON_2: // Notifications sent by 'CrrentW'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				
			  SelectWave=2;
				SelectWaveOpen=0;
				GUI_EndDialog(pMsg->hWin,0); 
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
    case ID_BUTTON_3: // Notifications sent by 'VectorW'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				
			  SelectWave=3;
				SelectWaveOpen=0;
				GUI_EndDialog(pMsg->hWin,0); 
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
*       CreateWaveSelect
*/
WM_HWIN CreateWaveSelect(void);
WM_HWIN CreateWaveSelect(void) {
 // WM_HWIN hWin;
  WM_HTIMER hTimer;
  WS_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  hTimer =WM_CreateTimer(WM_GetClientWindow(WS_hWin),0,1000,0);
  (void)hTimer;
  return WS_hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
