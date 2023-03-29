#ifndef __Font_H
#define __Font_H
/*
#include "GUI.h"
#include "DIALOG.h"
#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "GRAPH.h"
#include "MENU.h"
#include "MULTIPAGE.h"
#include "ICONVIEW.h"
#include "TREEVIEW.h"
*/
#include "ff.h"
#include "emwin_eric800.h"

/*
*********************************************************************************************************
*	                                  定义XBF格式字体
*********************************************************************************************************
*/
extern GUI_FONT     XBF_Font16;
extern GUI_FONT     XBF_Font24;
extern GUI_FONT     XBF_Font32;




/*
************************************************************************
*						  FatFs
************************************************************************
*/
/*
extern FRESULT result;
extern FIL file;
extern DIR DirInf;
extern UINT bw;
extern FATFS fs;
*/
//extern void _WriteByte2File(U8 Data, void * p); 
extern void LoadFontLib(void);
/*
************************************************************************
*						外部文件调用
************************************************************************
*/
extern void GUI_SetXBF(void); 

//extern void TOUCH_Calibration(void);

#endif
