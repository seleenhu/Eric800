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
#define ID_WINDOW_0  (GUI_ID_USER + 0x00)

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
WM_HWIN VE_hWin;
uint8_t VetorOpen;
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "VetorDLG", ID_WINDOW_0, 2, 130, 796, 250, 0, 0x0, 0 },
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
  // USER START (Optionally insert additional variables)
  // USER END
	float Sort_D[4];
	float Scale;
	float temp;
	float tempX;
	float tempY;
  switch (pMsg->MsgId) {
  case WM_PAINT:
			GUI_SetPenSize(2);
			GUI_SetColor(GUI_YELLOW);
			GUI_SetFont(GUI_FONT_24B_ASCII);
			GUI_DispStringAt("Ua",60,5);
    	GUI_DrawLine(8,10,55,10);
	
			
			GUI_SetColor(GUI_GREEN);
			GUI_SetFont(GUI_FONT_24B_ASCII);
			GUI_DispStringAt("Ub",60,35);
			GUI_DrawLine(8,40,55,40);
	
			GUI_SetColor(GUI_RED);
			GUI_SetFont(GUI_FONT_24B_ASCII);
			GUI_DispStringAt("Uc",60,65);
			GUI_DrawLine(8,70,55,70);
  	
  		GUI_SetColor(GUI_WHITE);
    	GUI_SetPenSize(2);
    	GUI_DrawLine(400,2,400,200);
    	GUI_DrawLine(400,2,395,12);
    	GUI_DrawLine(400,2,405,12);
    	GUI_DrawLine(200,100,600,100);
    	GUI_DrawLine(590,95,600,100);
    	GUI_DrawLine(590,105,600,100);
	
			
	
			
			Sort_D[0]=ModuleComFaultbuf->RMSValue[0][1]*ModuleComFaultbuf->VabuleSin[0][1];
			Sort_D[1]=ModuleComFaultbuf->RMSValue[1][1]*ModuleComFaultbuf->VabuleSin[1][1];
			Sort_D[2]=ModuleComFaultbuf->RMSValue[2][1]*ModuleComFaultbuf->VabuleSin[2][1];
			Scale=sort(Sort_D,3)/80;
			//=======UA
			temp=ModuleComFaultbuf->RMSValue[0][1];
			tempY=temp*ModuleComFaultbuf->VabuleSin[0][1];
			tempY*=Scale;
			tempX=temp*ModuleComFaultbuf->VabuleCos[0][1];
			tempX*=Scale;
			GUI_SetColor(GUI_YELLOW);
    	GUI_SetPenSize(2);
			GUI_DrawLine(400,100,400+tempX,100+tempY);
			//========UB
			temp=ModuleComFaultbuf->RMSValue[1][1];
			tempY=temp*ModuleComFaultbuf->VabuleSin[1][1];
			tempY*=Scale;
			tempX=temp*ModuleComFaultbuf->VabuleCos[1][1];
			tempX*=Scale;
			GUI_SetColor(GUI_GREEN);
    	GUI_SetPenSize(2);
			GUI_DrawLine(400,100,400+tempX,100+tempY);
			//==========UC
			temp=ModuleComFaultbuf->RMSValue[2][1];
			tempY=temp*ModuleComFaultbuf->VabuleSin[2][1];
			tempY*=Scale;
			tempX=temp*ModuleComFaultbuf->VabuleCos[2][1];
			tempX*=Scale;
			GUI_SetColor(GUI_RED);
    	GUI_SetPenSize(2);
			GUI_DrawLine(400,100,400+tempX,100+tempY);
  	break;	
  case WM_INIT_DIALOG:
    //
    // Initialization of 'VetorDLG'
    //
    hItem = pMsg->hWin;
    
    WINDOW_SetBkColor(hItem,0x00333333);
    VetorOpen=1;
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
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
WM_HWIN CreateVetorDLG(void);
WM_HWIN CreateVetorDLG(void) {
 // WM_HWIN hWin;

  VE_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return VE_hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
