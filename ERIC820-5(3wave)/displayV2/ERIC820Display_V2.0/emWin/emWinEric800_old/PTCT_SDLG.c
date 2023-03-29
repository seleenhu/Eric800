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
#include "InitSystem.h"
#include "NumPadDLG.h"
#include "param.h"
extern const unsigned char _aclogo2_s[17237UL + 1];
WM_HWIN PC_hWin;
static uint8_t SetType;   // 0:设定PT   1:设定CT 
uint16_t PT_Value;
uint16_t CT_Value;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x00)
#define ID_BUTTON_0     (GUI_ID_USER + 0x01)
#define ID_BUTTON_1     (GUI_ID_USER + 0x02)
#define ID_BUTTON_2     (GUI_ID_USER + 0x03)
#define ID_BUTTON_3     (GUI_ID_USER + 0x04)
#define ID_BUTTON_4     (GUI_ID_USER + 0x05)
#define ID_BUTTON_5     (GUI_ID_USER + 0x06)
#define ID_BUTTON_6     (GUI_ID_USER + 0x07)
#define ID_BUTTON_7     (GUI_ID_USER + 0x08)
#define ID_BUTTON_8     (GUI_ID_USER + 0x09)
#define ID_BUTTON_9     (GUI_ID_USER + 0x0A)
#define ID_BUTTON_10     (GUI_ID_USER + 0x0B)
#define ID_BUTTON_11     (GUI_ID_USER + 0x0C)
#define ID_BUTTON_12     (GUI_ID_USER + 0x0D)
#define ID_BUTTON_13     (GUI_ID_USER + 0x0E)
#ifdef EnableSyntonySet
#define ID_BUTTON_14     (GUI_ID_USER + 0x0F)
#endif
#define ID_EDIT_0     (GUI_ID_USER + 0x14)
#define ID_EDIT_1     (GUI_ID_USER + 0x15)

#define WM_SetValue 		WM_USER + 0x01 /* ????? */ // -------------- (2)



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
  { FRAMEWIN_CreateIndirect, "PTCT_S", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "RealMonitor", ID_BUTTON_0, 150, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SystemSet", ID_BUTTON_1, 280, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "AlarmList", ID_BUTTON_2, 410, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "PowerQuality", ID_BUTTON_3, 540, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_4, 670, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Temp&Humdity Set", ID_BUTTON_5, 0, 65, 150, 39, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Communication Set", ID_BUTTON_6, 0, 105, 150, 39, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cupboard Set", ID_BUTTON_7, 0, 145, 150, 39, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "PT&CT Set", ID_BUTTON_8, 0, 185, 150, 39, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Leakage Current Set", ID_BUTTON_9, 0, 225, 150, 39, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Voltag Limit Set", ID_BUTTON_10, 0, 265, 150, 39, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "OutPut Set", ID_BUTTON_11, 0, 305, 150, 39, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Calibration", ID_BUTTON_12, 0, 345, 150, 39, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Device set", ID_BUTTON_13, 0, 385, 150, 39, 0, 0x0, 0 },
	#ifdef EnableSyntonySet
	{ BUTTON_CreateIndirect, "Syntony set", ID_BUTTON_14, 0, 425, 150, 39, 0, 0x0, 0 },
	#endif
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 370, 119, 80, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_1, 370, 200, 80, 30, 0, 0x64, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
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
				GUI_SetColor(0x00959595);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x00959595);
				GUI_SetColor(GUI_WHITE);
			}
			/*
			GUI_SetFont(&GUI_Font20_ASCII);
			GUI_DispStringInRect("RealMonitor", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
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
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}
		/*
			GUI_SetFont(&GUI_Font20_ASCII);
			GUI_DispStringInRect("SystemSet", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
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

			/*
			GUI_SetFont(&GUI_Font20_ASCII);
			GUI_DispStringInRect("AlarmList", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
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

			/*
			GUI_SetFont(&GUI_Font20_ASCII);
			GUI_DispStringInRect("PowerQuality", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font32);
			GUI_DispStringInRect("电能质量", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

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
				GUI_SetColor(0x00959595);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x00959595);
				GUI_SetColor(GUI_WHITE);
			}

			/*
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringInRect("Temp&Humdity Set", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("温湿度设置", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

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
				GUI_SetColor(0x00959595);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x00959595);
				GUI_SetColor(GUI_WHITE);
			}

			/*
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringInRect("Communication Set", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("通讯设置", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cbButton7(WM_MESSAGE * pMsg)
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
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringInRect("Cupboard Set", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("柜体设置", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cbButton8(WM_MESSAGE * pMsg)
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
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringInRect("PT&CT Set", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("PT&CT设置", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cbButton9(WM_MESSAGE * pMsg)
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
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringInRect("Leakage Current Set", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font24);
			//GUI_DispStringInRect("漏电流设置", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
			GUI_DispStringInRect("零序电流", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cbButton10(WM_MESSAGE * pMsg)
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
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringInRect("Voltage Limit Set", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("电压门限设置", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cbButton11(WM_MESSAGE * pMsg)
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
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringInRect("Voltage Limit Set", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("传动实验", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cbButton12(WM_MESSAGE * pMsg)
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
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringInRect("Voltage Limit Set", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("校准设置", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cbButton13(WM_MESSAGE * pMsg)
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
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringInRect("Voltage Limit Set", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("通用设置", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
			break;

		default:
			BUTTON_Callback(pMsg);
	}
}
#ifdef EnableSyntonySet
static void _cbButton14(WM_MESSAGE * pMsg)
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
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringInRect("Voltage Limit Set", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			*/
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("谐振设置", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
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
  int     NCode;
  int     Id;
	char Buffer[10];
	char s[10];
	
  GUI_MEMDEV_Handle hMemJPEG;
  GUI_JPEG_INFO JpegInfo;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
		
		case WM_SetValue:
		switch(SetType)
		{
			case 0:
				Setting.RatioStruct.RatioPT=PT_Value;
				//g_tParam.RatioPT=PT_Value;
				break;
			case 1:
				Setting.RatioStruct.RatioCT=CT_Value;
				//g_tParam.RatioCT=CT_Value;
				break;
			default:
				break;
		}
			//SaveParam();
		SystemSet.ParamSaveFlag = FlagSystemState;
			break;		

  case WM_PAINT:
      //========bmp logo
    GUI_JPEG_GetInfo(_aclogo2_s, sizeof(_aclogo2_s) ,&JpegInfo);

    hMemJPEG = GUI_MEMDEV_CreateEx(2,2,JpegInfo.XSize,JpegInfo.YSize,GUI_MEMDEV_NOTRANS);
    //hMemJPEG = GUI_MEMDEV_CreateFixed(0,0,JpegInfo.XSize,JpegInfo.YSize,GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_16,GUICC_M565 );
    GUI_MEMDEV_Select(hMemJPEG);
    GUI_JPEG_Draw(_aclogo2_s,sizeof(_aclogo2_s),0,0);
    GUI_MEMDEV_Select(0);

    GUI_MEMDEV_WriteAt(hMemJPEG,2,2);
	
		GUI_MEMDEV_Delete(hMemJPEG);//


    GUI_SetColor(GUI_GREEN);
    //GUI_SetFont(GUI_FONT_20B_ASCII);
		GUI_SetFont(&XBF_Font24);
    GUI_DispStringAt("PT变比设置",200,120);

    GUI_DispStringAt("CT变比设置",200,200);





      break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'PTCT_S'
    //
		SetType=0;
		SetIndex=3;//
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleVis(hItem, 0);
    FRAMEWIN_SetClientColor(hItem,0x00333333);
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
		EDIT_SetFont(hItem, GUI_FONT_24B_ASCII);
		sprintf(s,"%d",Setting.RatioStruct.RatioPT);
    EDIT_SetText(hItem, s);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
		EDIT_SetFont(hItem, GUI_FONT_24B_ASCII);
		sprintf(s,"%d",Setting.RatioStruct.RatioCT);
    EDIT_SetText(hItem, s);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);


        //增加回调函数改变BUTTON的皮肤。
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

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton4);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton5);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_6);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton6);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_7);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton7);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_8);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton8);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_9);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton9);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_10);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton10);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_11);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton11);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_12);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton12);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_13);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton13);
		#ifdef EnableSyntonySet
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_14);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbButton14);
		#endif

		if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}

    // USER START (Optionally insert additional code for further widget initialization)
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
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
        GUI_EndDialog(pMsg->hWin,0);
        CreateMain();//切换到主画面
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
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
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
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
        GUI_EndDialog(pMsg->hWin,0);
        CreateOverVoltage_A();
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
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
        GUI_EndDialog(pMsg->hWin,0);
        CreatePowerQuality();
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
    case ID_BUTTON_4: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
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
    case ID_BUTTON_5: // Notifications sent by 'Temp&Humdity Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				#ifdef StepOverNum
				 if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
         GUI_EndDialog(pMsg->hWin,0);
         CreateTempHumdity_S();//切换到系统设置画面
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
    case ID_BUTTON_6: // Notifications sent by 'Communication Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
        GUI_EndDialog(pMsg->hWin,0);
        CreateCommunication_s();//
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
    case ID_BUTTON_7: // Notifications sent by 'Cupboard Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
        GUI_EndDialog(pMsg->hWin,0);
        CreateCupboard_S();//
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
    case ID_BUTTON_8: // Notifications sent by 'PT&CT Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
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
    case ID_BUTTON_9: // Notifications sent by 'Leakage Current Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
        GUI_EndDialog(pMsg->hWin,0);
        CreateLeakageCurrent_S();
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
    case ID_BUTTON_10: // Notifications sent by 'Voltag Limit Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
        GUI_EndDialog(pMsg->hWin,0);
        CreateVoltageLimit_S();
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
		case ID_BUTTON_11: // Notifications sent by 'Voltag Limit Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
        GUI_EndDialog(pMsg->hWin,0);
        
				CreateOutPutTest_S();
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
		case ID_BUTTON_12: // Notifications sent by 'Voltag Limit Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
        GUI_EndDialog(pMsg->hWin,0);
        
				Calibration_S();
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
		case ID_BUTTON_13: // Notifications sent by 'Voltag Limit Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				#ifdef StepOverNum
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				#endif
        GUI_EndDialog(pMsg->hWin,0);
        
				CreateDevice_S();
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
		#ifdef EnableSyntonySet	
		case ID_BUTTON_14: // Notifications sent by 'Voltag Limit Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				GUI_EndDialog(pMsg->hWin,0);
        
				CreateSyntony();
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
    case ID_EDIT_0: // Notifications sent by 'Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				SetType=0;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
			  EDIT_GetText(hItem,Buffer,10);
				PT_Value=atof(Buffer);//读初始值
			
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
			  EDIT_GetText(hItem,Buffer,10);
				PT_Value=atof(Buffer);//
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_1: // Notifications sent by 'Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				if(NumPadOpen==1)
				{
					GUI_EndDialog(NM_hWin,0);
					NumPadOpen=0;
				}
				SetType=1;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
			  EDIT_GetText(hItem,Buffer,10);
				CT_Value=atof(Buffer);//读初始值
			
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
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
			  EDIT_GetText(hItem,Buffer,10);
				CT_Value=atof(Buffer);
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
*       CreatePTCT_S
*/
WM_HWIN CreatePTCT_S(void);
WM_HWIN CreatePTCT_S(void) {
  //WM_HWIN hWin;

  PC_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return PC_hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
