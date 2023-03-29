#ifndef EMWIN_ERIC800_H_INCLUDED
#define  EMWIN_ERIC800_H_INCLUDED
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#include "GUI.h"
#include "font.h"	
#include "bsp.h"	
#include "InitSystem.h"	
#include "LCDConf_Lin_Template.h"

#include "ff.h"	
//#define EnableSyntonySet 
	
#define MaxCupboardNum 20
#define MaxCupboardDetail 12 	
	
	
#define OverTempValue 125
#define OverHumiValue 110	
	
	
	
#define WM_DisplayWave 		WM_USER + 0x01 /* �Զ�����Ϣ */ // -------------- (2)	
#define WM_BACKON 		WM_USER + 0x02  /* �Զ�����Ϣ */ // -------------- (2)	
	

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


/*
************************************************************************
*						  FatFs
************************************************************************
*/
extern FRESULT result;
extern FIL file;
extern DIR DirInf;
extern UINT bw;
extern FATFS fs;

extern void _WriteByte2File(U8 Data, void * p); 




	
//=======�������� 433 ID �ṹ��
/*
typedef struct{
	int Bus_A;		//A ��ĸ��
	int Bus_B;		//B ��ĸ��
	int Bus_C;		//C ��ĸ��
	int TouchUp_A;	//A ���ϴ���
	int TouchUp_B;	//B ���ϴ���
	int TouchUp_C;	//C ���ϴ���
	int TouchDown_A; //A ���´���
	int TouchDown_B; //B ���´���	
	int TouchDown_C; //C ���´���
	int Cupboard_T;	 //�����¶�
	int Cupboard_H;	 //����ʪ��
	int LeakgaeCurrent_A;//A��©����
	int LeakgaeCurrent_B;//B��©����
	int LeakgaeCurrent_C;//A��©����
	int OverVoltage_T;//����ѹ�������¶�
	int OverVoltage_H;//����ѹ������ʪ��	
}CupboardId;
*/




typedef enum 
{
	A1=0,
	A2,
	A3,
	A4,
	A5,
	A6,
	A7,
	A8,
	A9,
	A10,
	A11,
	A12,
	A13,
	A14,
	A15,
	A16,
	A17,
	A18,
	A19,
	A20
}CupboardNo;





extern const char Ver[];



//extern AlarmTimStruct SystemTime;
extern int Cupboard_No;
//extern CupboardId CupboardIdAdr[20];
extern CupboardParameter Cupboard_P[20][12];
extern AlarmTimStruct SystemTime;

//extern WM_HWIN CreateSystem_T(void);
extern WM_HWIN CreateStart(void);
extern WM_HWIN CreateCalibration(void);
extern WM_HWIN CreateMain(void);
extern WM_HWIN CreateMain_messge(void);
extern WM_HWIN CreateTempHumdity_W(void);
extern WM_HWIN CreateTempHumdity_S(void);
extern WM_HWIN CreateCommunication_s(void);
extern WM_HWIN CreateCupboard_S(void);
extern WM_HWIN CreateLeakageCurrent_S(void);
extern WM_HWIN CreateVoltageLimit_S(void);
extern WM_HWIN CreatePTCT_S(void);
extern WM_HWIN CreateGround_A(void);
extern WM_HWIN CreateOverVoltage_A(void);
extern WM_HWIN CreateResonance_A(void);
extern WM_HWIN CreateTempHumidity_A(void);
extern WM_HWIN CreatePowerQuality(void);
extern WM_HWIN Createaboutus(void);
extern WM_HWIN CreateNumPad(void);
extern WM_HWIN CreateMain_Graph(void);
extern WM_HWIN CreateOutPutTest_S(void);
extern WM_HWIN Calibration_S(void);
extern WM_HWIN CreateShort_A(void);
extern WM_HWIN CreateDevice_S(void);
extern WM_HWIN Createpassword(void);
extern WM_HWIN CreateSyntony(void);
extern WM_HWIN CreatePTOpen_A(void);
extern WM_HWIN CreateUnderVoltage_A(void);
extern WM_HWIN CreateTempHumdity_W(void);
extern WM_HWIN CreateProtector_W(void);
extern WM_HWIN CreateWaveSelect(void);
extern WM_HWIN CreateGndWaveSelect(void);
extern WM_HWIN CreateVetorDLG(void);
extern WM_HWIN CreateSelGndCurrentWaveDLG(void);


extern WM_HWIN VA_hWin;
extern WM_HWIN GA_hWin;
extern WM_HWIN SA_hWin;
extern WM_HWIN SY_hWin;	
extern WM_HWIN TA_hWin;
extern WM_HWIN RA_hWin;
extern WM_HWIN UA_hWin;
extern WM_HWIN PW_hWin;
extern WM_HWIN SelGndCurrentW_hWin;


extern WM_HWIN NM_hWin;
extern WM_HWIN WS_hWin;
extern WM_HWIN VE_hWin;
extern WM_HWIN Main_Win;
extern WM_HWIN WSGND_hWin;

extern uint8_t NumPadOpen;
extern uint8_t  SelectWave;
extern uint8_t SelectWaveOpen;
extern uint8_t VetorOpen;
extern uint8_t  SelectGndWave;
extern uint8_t SelectGndWaveOpen;




#endif // SYSTEM_TDLG_H_INCLUDED

