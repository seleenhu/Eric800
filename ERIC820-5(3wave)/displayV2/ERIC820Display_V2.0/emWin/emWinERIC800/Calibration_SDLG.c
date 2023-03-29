/*********************************************************************
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

extern const unsigned char _aclogo2_s[17237UL + 1];

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
#define ID_BUTTON_20  (GUI_ID_USER + 0x10)
#define ID_BUTTON_21  (GUI_ID_USER + 0x11)
#define ID_TEXT_1			(GUI_ID_USER + 0x12)

#define ID_TEXT_2			(GUI_ID_USER + 0x13)
#define ID_TEXT_3			(GUI_ID_USER + 0x14)
#define ID_TEXT_4			(GUI_ID_USER + 0x15)
#define ID_TEXT_5			(GUI_ID_USER + 0x16)

#define ID_TEXT_6			(GUI_ID_USER + 0x17)
#define ID_TEXT_7			(GUI_ID_USER + 0x18)
#define ID_TEXT_8			(GUI_ID_USER + 0x19)
#define ID_TEXT_9			(GUI_ID_USER + 0x1A)

#define ID_TEXT_10			(GUI_ID_USER + 0x20)
#define ID_TEXT_11			(GUI_ID_USER + 0x21)
#define ID_TEXT_12			(GUI_ID_USER + 0x22)
#define ID_TEXT_13			(GUI_ID_USER + 0x23)
#define ID_TEXT_14			(GUI_ID_USER + 0x24)
#define ID_TEXT_15			(GUI_ID_USER + 0x25)
#define ID_TEXT_16			(GUI_ID_USER + 0x26)
#define ID_TEXT_17			(GUI_ID_USER + 0x27)
#define ID_TEXT_18			(GUI_ID_USER + 0x28)
#define ID_TEXT_19			(GUI_ID_USER + 0x29)
#define ID_TEXT_20			(GUI_ID_USER + 0x2a)
#define ID_TEXT_21			(GUI_ID_USER + 0x2b)
#define ID_TEXT_22			(GUI_ID_USER + 0x2c)
#define ID_TEXT_23			(GUI_ID_USER + 0x2d)
#define ID_TEXT_24			(GUI_ID_USER + 0x2e)
#define ID_TEXT_25			(GUI_ID_USER + 0x2f)


#define WM_SetValue 		WM_USER + 0x01 /* 自定义消息 */ // -------------- (2)
//#define WM_DisplayReady WM_USER + 0x02


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
WM_HWIN CAB_hWin;
static uint8_t CalibrationReady; 
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "LeakageCurrent_S", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "RealMonitor", ID_BUTTON_0, 150, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SystemSet", ID_BUTTON_1, 280, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "AlarmList", ID_BUTTON_2, 410, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "PowerQuality", ID_BUTTON_3, 540, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_4, 670, 0, 129, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Temp&Humdity Set", ID_BUTTON_5, 0, 75, 150, 43, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Communication Set", ID_BUTTON_6, 0, 120, 150, 43, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cupboard Set", ID_BUTTON_7, 0, 165, 150, 43, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "PT&CT Set", ID_BUTTON_8, 0, 210, 150, 43, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Leakage Current Set", ID_BUTTON_9, 0, 255, 150, 43, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Voltag Limit Set", ID_BUTTON_10, 0, 300, 150, 43, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "OutPut Set", ID_BUTTON_11, 0, 345, 150, 43, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Calibration Set", ID_BUTTON_12, 0, 390, 150, 43, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Device Set", ID_BUTTON_13, 0, 435, 150, 43, 0, 0x0, 0 },
	#ifdef EnableSyntonySet
	{ BUTTON_CreateIndirect, "Syntony Set", ID_BUTTON_14, 0, 425, 150, 39, 0, 0x0, 0 },
	#endif
	{ BUTTON_CreateIndirect, "Start", ID_BUTTON_20, 200, 160, 125, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Calibration", ID_BUTTON_21, 200, 260, 125, 40, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_1, 380, 100, 400, 40, 0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "", ID_TEXT_2, 155, 360, 140, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_3, 315, 360, 140, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_4, 475, 360, 140, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_5, 635, 360, 145, 25, 0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "", ID_TEXT_6, 155, 400, 140, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_7, 315, 400, 140, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_8, 475, 400, 140, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_9, 635, 400, 140, 25, 0, 0x0, 0 },
	
	
	{ TEXT_CreateIndirect, "", ID_TEXT_10, 380, 150, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_11, 480, 150, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_12, 580, 150, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_13, 680, 150, 100, 25, 0, 0x0, 0 },
	
	
	{ TEXT_CreateIndirect, "", ID_TEXT_14, 380, 150+40, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_15, 480, 150+40, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_16, 580, 150+40, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_17, 680, 150+40, 100, 25, 0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "", ID_TEXT_18, 380, 150+40*2, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_19, 480, 150+40*2, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_20, 580, 150+40*2, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_21, 680, 150+40*2, 100, 25, 0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "", ID_TEXT_22, 380, 150+40*3, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_23, 480, 150+40*3, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_24, 580, 150+40*3, 100, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_25, 680, 150+40*3, 100, 25, 0, 0x0, 0 },
	
	
	
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
			GUI_SetFont(&XBF_Font24);
			GUI_DispStringInRect("参数设置", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
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
				GUI_SetColor(0x00959595);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x00959595);
				GUI_SetColor(GUI_WHITE);
			}
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

			
			GUI_SetFont(&XBF_Font24);			
			GUI_DispStringInRect("零序电流设置", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
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
				GUI_SetColor(0x000455ea);
				GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
				GUI_SetBkColor(0x000455ea);
				GUI_SetColor(GUI_WHITE);
			}			
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
	WM_HWIN hWinOld;
  int     NCode;
  int     Id;
	float f;
	char s[10];
  GUI_MEMDEV_Handle hMemJPEG;
  GUI_JPEG_INFO JpegInfo;
 

  switch (pMsg->MsgId) {
		#if 0
	case WM_DisplayReady:
		hWinOld= WM_SelectWindow(WM_GetClientWindow(pMsg->hWin));	
		GUI_SetColor(GUI_RED);
		GUI_SetTextMode(GUI_TM_TRANS);   
		GUI_SetFont(&XBF_Font32);  
		GUI_DispStringAt("准备OK,请确认是否需要校准", 380, 260);
		WM_SelectWindow(hWinOld);
		break;
		#endif
	case WM_TIMER:
		WM_RestartTimer(pMsg->Data.v,1000);
		if(ScreenCloseDisplay()){
			if(NumPadOpen==1)
			{
				GUI_EndDialog(NM_hWin,0);
				NumPadOpen=0;
			}
			GUI_EndDialog(pMsg->hWin,0);
			CreateMain();//切换到主画面
		}
		hWinOld= WM_SelectWindow(WM_GetClientWindow(pMsg->hWin));
		

	
		//======显示电压的有效值
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
		TEXT_SetTextColor(hItem, GUI_YELLOW);
		f = ModuleCombuf->RMS_buf[0];
		f*=Setting.RatioStruct.RatioPT;
		f=f/1000;
		sprintf(s,"Ua=%6.2fKV",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
		TEXT_SetTextColor(hItem, GUI_GREEN);		
		f = ModuleCombuf->RMS_buf[1];
		f*=Setting.RatioStruct.RatioPT;
		f=f/1000;
		sprintf(s,"Ub=%6.2fKV",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
		TEXT_SetTextColor(hItem, GUI_RED);		
		f = ModuleCombuf->RMS_buf[2];
		f*=Setting.RatioStruct.RatioPT;
		f=f/1000;
		sprintf(s,"Uc=%6.2fKV",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f = ModuleCombuf->RMS_buf[3];		
		sprintf(s,"3U0=%6.2fV",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
		TEXT_SetTextColor(hItem, GUI_YELLOW);		
		f = ModuleCombuf->RMS_buf[4];
		f*=Setting.RatioStruct.RatioCT;
		sprintf(s,"Ia=%6.2fA",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_7);
		TEXT_SetTextColor(hItem, GUI_GREEN);		
		f = ModuleCombuf->RMS_buf[5];
		f*=Setting.RatioStruct.RatioCT;
		sprintf(s,"Ib=%6.2fA",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_8);
		TEXT_SetTextColor(hItem, GUI_RED);		
		f = ModuleCombuf->RMS_buf[6];
		f*=Setting.RatioStruct.RatioCT;
		sprintf(s,"Ic=%6.2fA",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_9);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f = ModuleCombuf->RMS_buf[7];	
		sprintf(s,"3I0=%6.2fA",f);
		TEXT_SetFont(hItem, &XBF_Font24);
		TEXT_SetText(hItem,s);

		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_10);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[0];
		sprintf(s,"I1=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_11);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[1];
		sprintf(s,"I2=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_12);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[2];
		sprintf(s,"I3=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_13);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[3];
		sprintf(s,"I4=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_14);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[4];
		sprintf(s,"I5=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_15);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[5];
		sprintf(s,"I6=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_16);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[6];
		sprintf(s,"I7=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_17);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[7];
		sprintf(s,"I8=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_18);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[8];
		sprintf(s,"I9=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_19);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[9];
		sprintf(s,"I10=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_20);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[10];
		sprintf(s,"I11=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_21);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[11];
		sprintf(s,"I12=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_22);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[12];
		sprintf(s,"I13=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_23);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[13];
		sprintf(s,"I14=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_24);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[14];
		sprintf(s,"I15=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_25);
		TEXT_SetTextColor(hItem, GUI_WHITE);		
		f =Membuf->RMSValue[15];
		sprintf(s,"I16=%4.2fA",f);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem,s);
		
		WM_SelectWindow(hWinOld);	
		
		
		
		break;
		
  case WM_PAINT:
    //========bmp logo
    GUI_JPEG_GetInfo(_aclogo2_s, sizeof(_aclogo2_s) ,&JpegInfo);

    hMemJPEG = GUI_MEMDEV_CreateEx(2,2,JpegInfo.XSize,JpegInfo.YSize,GUI_MEMDEV_NOTRANS);    
    GUI_MEMDEV_Select(hMemJPEG);
    GUI_JPEG_Draw(_aclogo2_s,sizeof(_aclogo2_s),0,0);
    GUI_MEMDEV_Select(0);

    GUI_MEMDEV_WriteAt(hMemJPEG,2,2);
	
	GUI_MEMDEV_Delete(hMemJPEG);//

		
	GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_TRANS);
    
	GUI_SetFont(&XBF_Font24);
    
	GUI_DispStringAt("校准电压电流 ", 200, 104);
    
	GUI_DispStringAt("电压 100V,电流 5A ", 380, 60);
    
		
    break;

  case WM_INIT_DIALOG:
    //
    // Initialization of 'LeakageCurrent_S'
    //
	ScreenClrTim();	
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleVis(hItem, 0);
    FRAMEWIN_SetClientColor(hItem,0x00333333);
    //
		CalibrationReady=0;
	
		hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_20);
		BUTTON_SetFont(hItem,&XBF_Font24);
		BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,0x000455ea);
		BUTTON_SetText(hItem,"准备");
		
		hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_21);
		BUTTON_SetFont(hItem,&XBF_Font24);
		BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,0x000455ea);
		BUTTON_SetText(hItem,"校准");
    
    // USER START (Optionally insert additional code for further widget initialization)
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
				GUI_EndDialog(pMsg->hWin,0);
        CreatePTCT_S();//
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
		case ID_BUTTON_13: // Notifications sent by 'Device Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
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
		case ID_BUTTON_14: // Notifications sent by 'Device Set'
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
    case ID_BUTTON_20: // Notifications sent by 'Voltag Limit Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				CalibrationReady=1;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
				TEXT_SetTextColor(hItem, GUI_RED);
		
				TEXT_SetFont(hItem, &XBF_Font32);
				TEXT_SetText(hItem,"准备OK,确认是否校验");
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				
				
				//WM_SendMessageNoPara(WM_GetClientWindow(CAB_hWin), WM_DisplayReady);
				
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;		
			
		case ID_BUTTON_21: // Notifications sent by 'Voltag Limit Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				if(CalibrationReady==1)
			 { 
				 
				//ADCVerification();
				ModuleComSet10Fault(0x00,0x01,0x00);
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
				TEXT_SetTextColor(hItem, GUI_GREEN);
		
				TEXT_SetFont(hItem, &XBF_Font32);
				TEXT_SetText(hItem,"校验OK"); 
			 }
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
*       CreateLeakageCurrent_S
*/
WM_HWIN Calibration_S(void);
WM_HWIN Calibration_S(void) {
   //WM_HWIN hWin;
WM_HTIMER hTimer;
	
  CAB_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	hTimer =WM_CreateTimer(WM_GetClientWindow(CAB_hWin),0,1000,0);
	(void)hTimer;
  return CAB_hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
