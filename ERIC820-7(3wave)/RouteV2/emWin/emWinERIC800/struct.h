#ifndef struct_H_INCLUDED
#define struct_H_INCLUDED
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif


#include "bsp.h"


//��Ҫ���ö�ֵ����
#define   IDLEN                  64 
#define   SAMPLE_FREQUENCY       256  
#define   WAVE_NUM               12
#define   SAMPLE_FREQUENCY_1S    12800
#define   U_CHANNEL_NUM          4
#define   I_CHANNEL_NUM          3
//#define   THD_CHANNEL_NUM        49
#define   THD_CHANNEL_NUM        50
#define   POWER_FACTOR_NUM       4
#define   IP_ADDR_LENGTH         4
#define   CHANNEL_NUM            8

//const int MAXHARM =50;
//const int ADLEN=12800;
//const int MAXCH=8;




#define  SERVICE_IP      1      // ������IP ��ʶ
#define  CLIENT_IP       2      // �ͻ���IP ��ʶ
#define  RepondMod_On    1       //�ش���־
#define  RepondMod_Off   2       //���ش���־
#define  OVERTIME_5S     5      //��ʱʱ��ֵ
#define  OVERTIME_10S    10
#define  OVERTIME_15S    15
#define  OVERTIME_20S    20


//�������ò����궨��
#define   PT                   100    //����ȷ�� PT���
#define   CT                   100    // CT���
#define   PRIMARY_SIDE         1   //ԭ��
#define   SECONDARY_SIDE       2   //����
#define   TRIGON_CONNECTION    3//����������
#define   STAR_CONNECTION      4//��������
#define   OVER_U_VALUE         30000 //����ȷ��  ��ѹ����ֵ
#define   OVER_I_VALUE         900    //����ȷ�� ��������ֵ
#define   PEAK_VALUE_U         1000 //  ��ѹ��ֵ ����ѹ��ֵ
#define   PEAK_VALUE_I         1000 //  ������ֵ ��������ֵ
#define   OVERVOLTAGE          10000// ��ѹֵ �� 




//��ѹ��������������
#define      A_GROUND          1 
#define      B_GROUND          2
#define      C_GROUND          3
#define      AB_GROUND         4
#define      BC_GROUND         5
#define      AC_GROUND         6
#define      ABC_GROUND        7
#define      NO_GROUND         8





//ͨѶ������Ҫ��������
//����������־����ز���
#define TagCommandSVR_RESET     			0	//��λ�ͻ���
#define TagCommandSVR_SETTIME					11	//�����ն�ʱ��
#define TagCommandSVR_GETTIME					12	//��ȡ�ն�ʱ��

#define TagCommandSVR_SETDEVINFO			23 //�����������ò��� �����õ���DevInfo
#define TagCommandSVR_GETDEVINFO			24 //�õ����ò����� DevInfo
#define TagCommandSVR_SETCONFIGPARM  	35 //�������в���	ConfigParm
#define TagCommandSVR_GETCONFIGPARM  	36	//��ȡ���в���	ConfigParm
#define TagCommandSVR_GETLOGDATA			47 //��ȡ��־�ļ�
#define TagCommandSVR_GETLOGDATAANDDELETE			48 //��ȡ��־�ļ�����ɾ��
#define TagCommandSVR_GETRAWDATA      59				//��ȡ��������
#define TagCommandSVR_GETEARTHDATA    59       //��ȡ�ӵع��ϲ�������
#define TagCommandSVR_GETOVERVOLTAGEDATA      60       //��ȡ����ѹ��������������
#define TagCommandSVR_GETELECTIMEDATA      61       //��ȡ��ʱ�Ե�FFT�����ĵ�������ֵ���� 
#define TagCommandSVR_GETTESTDATA			81				//��ȡ���Ա���






//ʱ��ṹ�� ������ʱ���� ΢�� ֻ����֪��������ֵ��ʱ�� ����֪��΢��ʱ��ֵ
typedef struct
{
	uint16_t year;
	uint16_t month;
	uint16_t day;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
	uint32_t us;	//���ݵ�ֵ���м���
}systemtime;




//�豸��Ϣ���� 
//�ɶ�����д����Ҫ��ͨѶ����ʹ��
typedef struct 
{
	char strID[IDLEN];		//Ψһʶ��ţ���ȡоƬ��ID�Ż��  ÿ�����豸����һ���豸ID  ʹ��4���ֽڻ���64���ֽڣ�����
	char strSVRIP[IP_ADDR_LENGTH];	//��������IP��ַ
	uint32_t IPPort;		//������IP�˿ں�
	char strDevIP[IP_ADDR_LENGTH];	//����IP��ַ
	uint32_t DevIPPort;		//����IP�˿ں�
	uint32_t mainVer;		//���汾��
	uint32_t subVer;		//���汾��	
}DevInfo,*pDevInfo;






/*
//�ϰ汾  ��ǰ�����ݽṹ  
//���ò���
//�������ò�����
typedef struct 
{
	uint32_t SamplingFrequency;		//����Ƶ��
	uint32_t MaxHarm;			//������ߴ�г������
	uint32_t PhaseTag;			//�����ʶ���϶�ϸ��
	uint32_t SamplerLen;			//��������
	//uint16_t WeiDu_Du;	        //γ�� ��
	float   WeiDu ;  		
	//uint32_t WeiDu_Fen;		//γ�� ��	
	char     NS;			//γ�� ���� N ������ S  �ϰ���	
	//uint16_t JingDu_Du;		//���� ��	
	//uint32_t JingDu_Fen;		//���� ��
	float   JingDu;
	char     EW;	                //����  E ����  W ���� 
	float ZeroPoint[MAXCH];		//ͨ�����Ư����
	float ADRate[MAXCH][MAXHARM];		//������ϵ������Ҫ�궨
}ConfigParm ,*pConfParm;
*/






//�������ò���  ��Թ���ѹ������
//�������ò�����
typedef struct 
{
	char      strSVRIP[IP_ADDR_LENGTH];	//��������IP��ַ
	uint32_t  IPPort;		//������IP�˿ں�
	char      Store_Directory[IDLEN];//�洢Ŀ¼���ļ���
	char      Ui_Name[IDLEN];    //��ѹ�������� ?

	uint16_t  x_PT;//pt��� ��ѹ���
	uint16_t  x_CT;//ct��� �������
	
	uint16_t  u_pri_side;//��ѹ��ԭ�ߵ�ѹ 
	uint16_t  i_sec_side;//��ѹ�����ߵ�ѹ
	
  //uint16_t  i_pri_side;//��ѹ��ԭ�ߵ��� 
	//uint16_t  i_sec_side;//��ѹ�����ߵ���
	
	uint8_t     connection_type;//��ѹ������ӷ� �����λ�������
	float       over_u_value;// ��ѹ����ֵ ��Կͻ�
	float       over_i_value;// ��������ֵ ��Կͻ�
	float       peak_value;//��ֵ ��ѹ��ֵ���ֵ 
	float       overvoltage;//��ѹֵ RMS�벨��ѹ����ֵ
}ConfigParm_Protect ,*pConfParm_Protect;





//�������ò���  ���С�����ӵ�
//�������ò�����
typedef struct 
{
	char      strSVRIP[IP_ADDR_LENGTH];	//��������IP��ַ
	uint32_t  IPPort;		//������IP�˿ں�
	char      Store_Directory[IDLEN];//�洢Ŀ¼���ļ���
	char      Ui_Name[IDLEN];    //��ѹ�������� ?
	uint16_t  p_PT;//pt���
	uint16_t  p_CT;//ct���
	
	uint16_t u_pri_side;//��ѹ����ѹԭ�� 
	uint16_t u_sec_side;//��ѹ����ѹ����	
	uint16_t i_pri_side;//��ѹ����ѹԭ�� 
	uint16_t i_sec_side;//��ѹ����ѹ����
	uint8_t   connection_type;//��ѹ������ӷ������ͣ������ͣ�����ȡ�������
	// ����ֵ վ���û��ĽǶ�ȥ����
	float       over_u_value;// ��ѹ����ֵ  ��Կͻ��Ƕ�
	float       over_i_value;// ��������ֵ  ��Կͻ��Ƕ�	
	float       peak_value;//��ֵ  ��ѹ��ֵ���ֵ
	float       overvoltage;//��ѹֵ  RMS�벨��Чֵ
	float       u_drop_value;//��ѹ������ֵ 
	float       u0_limit_value;//3U0��ֵ  ֱ��
}ConfigParm_G_connection ,*pConfParm_Protect_G_connection;






//����¼��ֵ   ����ǰ4���ܲ� ���Ϻ�8���ܲ�˲̬����  �������ݱ��浽 SDRAM����ȥ �ٴ�SDRAM�洢��SD������  ��ֵ̬ 
//�����С�����ӵ�ѡ�� �ǲ���ֻ��������ݣ���
//�����߼���˲ֵ̬���꼰RMS�벨��Чֵ���ꣿ
typedef struct
{
  uint16_t    Syn_node;//ͬ����
	uint16_t    Start_node;//��ʼ��
	systemtime  stime;//ʱ��
	float  wdat[CHANNEL_NUM][SAMPLE_FREQUENCY*WAVE_NUM];//12���ܲ��Ĺ��ϲ������� ��50���ܲ������ݣ��ֲ�������������ʱ��
	/* �洢˳���� 0-0 1-0 2-0 7-0 0-1 1-1 2-1 ...7-1 ... 7-256*12-1*/
} FAULT_RECORD_WAVE;




//����ѹ�����������ʱ����
typedef struct
{
  uint8_t  start;
  uint8_t  cmd;//��ʱ����
  uint8_t  group;
  uint8_t  term;
  uint8_t  crc;
  uint8_t  end;
} OVERVOLTAGE_PROTECTOR_CHECK_TIME_SND;


//�����ظ�ʱ������ 
typedef struct
{
  uint8_t  start;
  uint8_t  cmd;//��ʱ����
  uint8_t  group;
  uint8_t  term;
  systemtime time;//����ʱ�� 
  uint8_t  crc;
  uint8_t  end;
} MAIN_DEVICE_SND_TIME;




//��Чֵ  ��̬��������ֵ ��ʱ�Ե�ȥ����һ����Щ��̬����
//����������4G�����ȽϹ� ����Ҫ��ʱ���ͣ���ֻ�Ƕ�ʱ�洢��SD�����棿�� ����1���ӻ���10���ӻ��������ȵȣ�����
typedef struct
{
   systemtime  stime;//ʱ��

   float Elec_U[U_CHANNEL_NUM];//4·��ѹ����Чֵ  

   float Elec_I[I_CHANNEL_NUM];//3·��������Чֵ

   float Elec_Thdu_Amp[U_CHANNEL_NUM][THD_CHANNEL_NUM];//4·��ѹ������г�� 1-50��
   float Elec_Thdu_Phose[U_CHANNEL_NUM][THD_CHANNEL_NUM];//4·��ѹ������г����λ 1-50��
   
   float Elec_Thdi_Amp[I_CHANNEL_NUM][THD_CHANNEL_NUM];//3·����������г�� 1-50��
   float Elec_Thdi_Phose[I_CHANNEL_NUM][THD_CHANNEL_NUM];//3·����������г����λ 1-50��

   float Elec_Power_Factor[POWER_FACTOR_NUM];//�������� ���������  ������ ���й�/���޹� 
   
   float Elec_Active_Power[POWER_FACTOR_NUM];//�й����� ������ƽ��
   
   float Elec_Reactive_Power[POWER_FACTOR_NUM];//�޹����� ������ƽ��
   
   float Elec_Apparent_Power[POWER_FACTOR_NUM];// ���ڹ��� ������ƽ��
   
   float positive_Usequence_components;  //��ѹ������̬����  
   float negative_Usequence_components;  //��ѹ������̬���� 
   float zero_Usequence_components;      //��ѹ������̬����
   float positive_Isequence_components;  //����������̬����
   float negative_Isequence_components;  //����������̬����
   float zero_Isequence_components;      //����������̬����
   float unbalance_u;//��ѹ��ƽ���
   float unbalance_i;//������ƽ���
} RMS_ELEC_VALUE;   








//�ӵع��� ͨ�������ϴ������ݽṹ  
typedef struct
{
  char  strID[IDLEN];		//Ψһʶ��� 
  FAULT_RECORD_WAVE fault_record_wave;//�����ӵ�ʱ�Ĳ���ֵ���� 
  //�����ѹ �� ������� 12���ܲ�˲̬����ֵ 
  int  u_sequence_components[SAMPLE_FREQUENCY*WAVE_NUM];//�����ӵ�ʱ�� ��ѹ���ϲ��������ֵ
  int  i_sequence_components[SAMPLE_FREQUENCY*WAVE_NUM];//�����ӵ�ʱ�� �������ϲ��������ֵ
  float Elec_U[U_CHANNEL_NUM];//4·��ѹRMS�벨��Чֵ Ӳ�����໷ 2�������֮��������ܲ�����RMS
  float Elec_I[I_CHANNEL_NUM];//3·����RMS�벨��Чֵ Ӳ�����໷ 2�������֮��������ܲ�����RMS
}EARTH_FAULT;






//����ѹ���������ݽṹ��  ��̬��˲̬���� ---��ѹ����������ϲ��β���
//�����߼����������¶�/ʪ�ȳ��ꣿ���� ˲ֵ̬���꼰RMS�벨��Чֵ���ꣿ������ѹ��������������  
typedef struct
{
  char strID[IDLEN];		//Ψһʶ��ţ�64���ֽڱ�ʾ��ÿ������ѹ����������һ��Ψһʶ���
  systemtime time;//����ʱ��
  float temp[3];//�¶� ABC
  float humi[3];//ʪ�� ABC
  uint8_t fault_time;//��������
  uint32_t  duration_time;//���ϳ���ʱ�� US����
  uint32_t  action_times[3];//ÿһ��Ķ�������ֵ ABC 
  FAULT_RECORD_WAVE fault_record_wave;//��������ʱ�Ĳ���ֵ���� 
}OVERVOLTAGE_PROTECTOR;






//������־  ��Ҫʹ��
typedef struct 
{
	char strID[IDLEN];		        //�����߱�ʶ��
	systemtime st;		            //������ʱ��
	uint32_t nResen;		          //��־ԭ���ʶ����
	char strDesrit[IDLEN+IDLEN];	//��ע������������ȫ�ֵı�ʶ���Ӧ�����Դ��
}LogData;



//��ע�����С�����ӵ�ѡ�߼�����ѹ��������⣬�������������㣬�豸�����ϴ����ݵ�ʱ��������
//���ϴ���־�����ļ���Ȼ�����ϴ���Ӧ�ľ�������ֵ��


//�Ľ������ �������̣�����֡������֡����ʽ,������sever ��client�������ߣ��ͽ����ߡ�
typedef struct 
{
	char       strID[IDLEN];			//������Ψһʶ���
	char       DevType;			    //������豸���� DevType = CLIENT_IP or SERVICE_IP;
	uint16_t   CommandID;			//�����ַ����ϸ��  
	systemtime st;			    //����ʱ��
	uint16_t   OverTime;			//��ʱ�޶�
	char       RepondMod; 	 //�Ƿ�ش�
	uint32_t   len;	          //��������֡�ĳ���  �����Ľṹ���������sizeof����Ҫ�Ľṹ�壩	
}CommandFrame;




//ͨѶ˵��
// ͨѶ��ʽ �ܵ�ԭ���ǲ��� CommandFrame������֡�� +����DevInfo��or ConfigParm or EARTH_FAULT  or RMS_ELEC_VALUE or OVERVOLTAGE_PROTECTOR�ȵȣ����ݽṹ�塡���ַ�ʽ��
//������ʾ��

//  �������˷��͸�λ�ͻ�������   
//�����ʽ ��CommandFrame +  ��
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪSERVICE_IP��+ int CommandID��ֵΪTagCommandSVR_RESET��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_Off ���ش��� + int len ��0���� ��
// 2��type struct  �� �� 





//  �������˷����������ò�������   
//�����ʽ ��CommandFrame + type struct DevInfo
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪSERVICE_IP��+ int CommandID��ֵΪTagCommandSVR_SETDEVINFO��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_Off ���ش��� + int len �� sizeof��DevInfo����
// 2��type struct  ��   DevInfo 





//  �������˷��͵õ����ò�������   
//�����ʽ ��CommandFrame + ��
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪSERVICE_IP��+ int CommandID��ֵΪTagCommandSVR_GETDEVINFO��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_On �ش��� + int len �� 0���գ�
// 2��type struct  ��  �� 





//  �ͻ��˻������ò�������   
//�����ʽ ��CommandFrame + type struct DevInfo
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪCLIENT_IP��+ int CommandID��ֵΪTagCommandSVR_GETDEVINFO��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_Off ���ش��� + int len �� sizeof��DevInfo����
// 2��type struct  ��   DevInfo 



//  �������˷����������в�������   
//�����ʽ ��CommandFrame + type struct ConfigParm
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪSERVICE_IP��+ int CommandID��ֵΪTagCommandSVR_SETCONFIGPARM��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_Off ���ش��� + int len �� sizeof��ConfigParm����
// 2��type struct  ��  ConfigParm




//  �������˷��͵õ����в�������   
//�����ʽ ��CommandFrame + ��
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪSERVICE_IP��+ int CommandID��ֵΪTagCommandSVR_GETCONFIGPARM��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_On �ش��� + int len �� 0 :�գ�
// 2��type struct  ��  �� 




//  �ͻ��˻������в�������   
//�����ʽ ��CommandFrame + type struct ConfigParm
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪCLIENT_IP��+ int CommandID��ֵΪTagCommandSVR_GETCONFIGPARM��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_Off ���ش��� + int len ��  sizeof��ConfigParm����
// 2��type struct  ��   ConfigParm



 


//  �������˷��Ͷ�ȡ�ӵع��ϲ������ݲ�������   ����ʱ������ļ�������Ȼ���ϴ����ϲ�������
//  �����ʽ ��CommandFrame + ��
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪSERVICE_IP��+ int CommandID��ֵΪTagCommandSVR_GETEARTHDATA��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_On �ش��� + int len �� 0���� ��
// 2��type struct  ��   ��





//  �ͻ��˻��ͽӵع��ϲ�����������   
//  �����ʽ ��CommandFrame + type struct EARTH_FAULT
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪCLIENT_IP��+ int CommandID��ֵΪTagCommandSVR_GETEARTHDATA��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_Off ���ش��� + int len ��  sizeof��EARTH_FAULT����
// 2��type struct  ��   EARTH_FAULT




//  �ͻ��������ϴ��ӵع��ϲ�����������  ��С�����ӵ�ѡ�߼�����ѹ������״̬��ⴥ����������ʱ�����������ϴ���־�ļ����ݣ�Ȼ��Ÿ��ϵ��Ǿ��岨������ֵ��
//�����ʽ ��CommandFrame + type struct EARTH_FAULT
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪCLIENT_IP��+ int CommandID��ֵΪTagCommandSVR_GETEARTHDATA��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_Off ���ش��� + int len ��  sizeof��EARTH_FAULT����
// 2��type struct  ��   EARTH_FAULT




//  �������˷��Ͷ�ȡ����ѹ��������������   ����ʱ������ļ�������Ȼ���ϴ�����?
//  �����ʽ ��CommandFrame + ��
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪSERVICE_IP��+ int CommandID��ֵΪTagCommandSVR_GETOVERVOLTAGEDATA��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_On �ش��� + int len �� 0���� ��
// 2��type struct  ��   ��





//  �ͻ��˻��͹���ѹ��������������   
//  �����ʽ ��CommandFrame + type struct OVERVOLTAGE_PROTECTOR
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪCLIENT_IP��+ int CommandID��ֵΪTagCommandSVR_GETOVERVOLTAGEDATA��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_Off ���ش��� + int len ��  sizeof��OVERVOLTAGE_PROTECTOR����
// 2��type struct  ��   OVERVOLTAGE_PROTECTOR




//  �ͻ��������ϴ�����ѹ��������������  ��С�����ӵ�ѡ�߼�����ѹ������״̬��ⴥ����������ʱ�����������ϴ���־�ļ����ݣ�Ȼ����Ǿ��岨������ֵ��
//�����ʽ ��CommandFrame + type struct OVERVOLTAGE_PROTECTOR
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪCLIENT_IP��+ int CommandID��ֵΪTagCommandSVR_GETOVERVOLTAGEDATA��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_Off ���ش��� + int len ��  sizeof��OVERVOLTAGE_PROTECTOR����
// 2��type struct  ��   OVERVOLTAGE_PROTECTOR





//  �������˷��Ͷ�ȡ������̬��������   �ϴ��ڴ��е����µ�һ�ε�����̬���� ��Ҫ��ȡ��ʷ��̬����������
//  �����ʽ ��CommandFrame + ��
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪSERVICE_IP��+ int CommandID��ֵΪTagCommandSVR_GETELECTIMEDATA��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_On �ش��� + int len �� 0���� ��
// 2��type struct  ��   ��





//  �ͻ��˻��͵�����̬��������   �ϴ��ڴ��е����µ�һ�ε�����̬���� ��Ҫ��ȡ��ʷ��̬����������
//  �����ʽ ��CommandFrame + type struct RMS_ELEC_VALUE
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪCLIENT_IP��+ int CommandID��ֵΪTagCommandSVR_GETELECTIMEDATA��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_Off ���ش��� + int len ��  sizeof��RMS_ELEC_VALUE����
// 2��type struct  ��   RMS_ELEC_VALUE




//  �ͻ��������ϴ���־�ļ���������   ��С�����ӵ�ѡ�߼�����ѹ������״̬��ⴥ����������ʱ�����������ϴ�����־�ļ����ݣ�Ȼ����Ǿ��岨������ֵ��
//�����ʽ ��CommandFrame + type struct LogData
// 1��CommandFrame  ��char strID[IDLEN] + int DevType��ֵΪCLIENT_IP��+ int CommandID��ֵΪTagCommandSVR_GETLOGDATA��+  st��ʱ��ֵ��\
// + int OverTime ��OVERTIME_15S��ѡ�� + int RepondMod ��RepondMod_Off ���ش��� + int len ��  sizeof��LogData����
// 2��type struct  ��   LogData



#endif









