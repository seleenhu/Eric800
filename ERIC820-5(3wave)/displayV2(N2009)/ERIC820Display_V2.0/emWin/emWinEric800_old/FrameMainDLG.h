#ifndef SYSTEM_TDLG_H_INCLUDED
#define SYSTEM_TDLG_H_INCLUDED
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#include "GUI.h"

#if GUI_WINSUPPORT
  #include "WM.h"

  #include "CHECKBOX.h"
  #include "FRAMEWIN.h"
  #include "PROGBAR.h"
  #include "TEXT.h"
  #include "BUTTON.h"
  #include "SLIDER.h"
  #include "HEADER.h"
  #include "GRAPH.h"
  #include "ICONVIEW.h"
  #include "LISTVIEW.h"
  #include "TREEVIEW.h"
#else
  #include <stdlib.h>  // Definition of NULL
#endif

#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)
#define ID_RADIO_0      (GUI_ID_USER + 0x01)
#define ID_CHECKBOX_0   (GUI_ID_USER + 0x02)
#define ID_CHECKBOX_1   (GUI_ID_USER + 0x03)
#define ID_TEXT_0       (GUI_ID_USER + 0x04)
#define ID_GRAPH_0      (GUI_ID_USER + 0x05)


extern const GUI_WIDGET_CREATE_INFO _aDialogCreate_M[] = {
  { FRAMEWIN_CreateIndirect, "FrameMain", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { RADIO_CreateIndirect, "Radio", ID_RADIO_0, 650, 60, 100, 300, 0, 0x3c05, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox_Current", ID_CHECKBOX_0, 436, 30, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox_Voltage", ID_CHECKBOX_1, 519, 30, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 40, 30, 118, 20, 0, 0x64, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 40, 300, 500, 120, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

extern void _cbDialog_M(WM_MESSAGE * pMsg);

#endif // SYSTEM_TDLG_H_INCLUDED
