#ifndef struct_H_INCLUDED
#define struct_H_INCLUDED
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif


#include "bsp.h"


//主要常用定值定义
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




#define  SERVICE_IP      1      // 服务器IP 标识
#define  CLIENT_IP       2      // 客户端IP 标识
#define  RepondMod_On    1       //回传标志
#define  RepondMod_Off   2       //不回传标志
#define  OVERTIME_5S     5      //超时时间值
#define  OVERTIME_10S    10
#define  OVERTIME_15S    15
#define  OVERTIME_20S    20


//工作配置参数宏定义
#define   PT                   100    //后面确定 PT变比
#define   CT                   100    // CT变比
#define   PRIMARY_SIDE         1   //原边
#define   SECONDARY_SIDE       2   //副边
#define   TRIGON_CONNECTION    3//三角形连接
#define   STAR_CONNECTION      4//星形连接
#define   OVER_U_VALUE         30000 //后面确定  电压超标值
#define   OVER_I_VALUE         900    //后面确定 电流超标值
#define   PEAK_VALUE_U         1000 //  电压峰值 ？电压峰值
#define   PEAK_VALUE_I         1000 //  电流峰值 ？电流峰值
#define   OVERVOLTAGE          10000// 过压值 ？ 




//过压保护器故障类型
#define      A_GROUND          1 
#define      B_GROUND          2
#define      C_GROUND          3
#define      AB_GROUND         4
#define      BC_GROUND         5
#define      AC_GROUND         6
#define      ABC_GROUND        7
#define      NO_GROUND         8





//通讯部分主要常量定义
//触发运行日志的相关操作
#define TagCommandSVR_RESET     			0	//复位客户端
#define TagCommandSVR_SETTIME					11	//设置终端时间
#define TagCommandSVR_GETTIME					12	//读取终端时间

#define TagCommandSVR_SETDEVINFO			23 //设置所有配置参数 可配置的域。DevInfo
#define TagCommandSVR_GETDEVINFO			24 //得到配置参数。 DevInfo
#define TagCommandSVR_SETCONFIGPARM  	35 //设置运行参数	ConfigParm
#define TagCommandSVR_GETCONFIGPARM  	36	//读取运行参数	ConfigParm
#define TagCommandSVR_GETLOGDATA			47 //读取日志文件
#define TagCommandSVR_GETLOGDATAANDDELETE			48 //读取日志文件并且删除
#define TagCommandSVR_GETRAWDATA      59				//读取波形数据
#define TagCommandSVR_GETEARTHDATA    59       //读取接地故障波形数据
#define TagCommandSVR_GETOVERVOLTAGEDATA      60       //读取过电压保护器故障数据
#define TagCommandSVR_GETELECTIMEDATA      61       //读取定时性的FFT分析的电气参数值数据 
#define TagCommandSVR_GETTESTDATA			81				//读取测试报告






//时间结构体 年月日时分秒 微妙 只有在知道采样点值的时候 才能知道微秒时间值
typedef struct
{
	uint16_t year;
	uint16_t month;
	uint16_t day;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
	uint32_t us;	//根据点值进行计算
}systemtime;




//设备信息参数 
//可读，可写，主要是通讯功能使用
typedef struct 
{
	char strID[IDLEN];		//唯一识别号，读取芯片的ID号获得  每个主设备都有一个设备ID  使用4个字节还是64个字节？？？
	char strSVRIP[IP_ADDR_LENGTH];	//服务器端IP地址
	uint32_t IPPort;		//服务器IP端口号
	char strDevIP[IP_ADDR_LENGTH];	//本机IP地址
	uint32_t DevIPPort;		//本机IP端口号
	uint32_t mainVer;		//主版本号
	uint32_t subVer;		//辅版本号	
}DevInfo,*pDevInfo;






/*
//老版本  以前的数据结构  
//配置参数
//运行配置参数表
typedef struct 
{
	uint32_t SamplingFrequency;		//采样频率
	uint32_t MaxHarm;			//分析最高次谐波次数
	uint32_t PhaseTag;			//相序标识，较多细分
	uint32_t SamplerLen;			//采样长度
	//uint16_t WeiDu_Du;	        //纬度 度
	float   WeiDu ;  		
	//uint32_t WeiDu_Fen;		//纬度 分	
	char     NS;			//纬度 半球 N 北半球 S  南半球	
	//uint16_t JingDu_Du;		//经度 度	
	//uint32_t JingDu_Fen;		//经度 分
	float   JingDu;
	char     EW;	                //经度  E 东经  W 西经 
	float ZeroPoint[MAXCH];		//通道零点漂移量
	float ADRate[MAXCH][MAXHARM];		//码点比例系数，需要标定
}ConfigParm ,*pConfParm;
*/






//运行配置参数  针对过电压保护器
//运行配置参数表
typedef struct 
{
	char      strSVRIP[IP_ADDR_LENGTH];	//服务器端IP地址
	uint32_t  IPPort;		//服务器IP端口号
	char      Store_Directory[IDLEN];//存储目录及文件名
	char      Ui_Name[IDLEN];    //电压电流名称 ?

	uint16_t  x_PT;//pt变比 电压变比
	uint16_t  x_CT;//ct变比 电流变比
	
	uint16_t  u_pri_side;//变压器原边电压 
	uint16_t  i_sec_side;//变压器副边电压
	
  //uint16_t  i_pri_side;//变压器原边电流 
	//uint16_t  i_sec_side;//变压器副边电流
	
	uint8_t     connection_type;//变压器相序接法 三角形或者星形
	float       over_u_value;// 电压超标值 针对客户
	float       over_i_value;// 电流超标值 针对客户
	float       peak_value;//峰值 电压峰值最大值 
	float       overvoltage;//过压值 RMS半波过压超标值
}ConfigParm_Protect ,*pConfParm_Protect;





//运行配置参数  针对小电流接地
//运行配置参数表
typedef struct 
{
	char      strSVRIP[IP_ADDR_LENGTH];	//服务器端IP地址
	uint32_t  IPPort;		//服务器IP端口号
	char      Store_Directory[IDLEN];//存储目录及文件名
	char      Ui_Name[IDLEN];    //电压电流名称 ?
	uint16_t  p_PT;//pt变比
	uint16_t  p_CT;//ct变比
	
	uint16_t u_pri_side;//变压器电压原边 
	uint16_t u_sec_side;//变压器电压副边	
	uint16_t i_pri_side;//变压器电压原边 
	uint16_t i_sec_side;//变压器电压副边
	uint8_t   connection_type;//变压器相序接法，星型，三角型，单相等。。。。
	// 浮点值 站在用户的角度去设置
	float       over_u_value;// 电压超标值  针对客户角度
	float       over_i_value;// 电流超标值  针对客户角度	
	float       peak_value;//峰值  电压峰值最大值
	float       overvoltage;//过压值  RMS半波有效值
	float       u_drop_value;//电压跌落限值 
	float       u0_limit_value;//3U0限值  直接
}ConfigParm_G_connection ,*pConfParm_Protect_G_connection;






//故障录波值   故障前4个周波 故障后8个周波瞬态数据  该组数据保存到 SDRAM里面去 再从SDRAM存储到SD卡里面  暂态值 
//如果是小电流接地选线 是不是只传输此数据？？
//触发逻辑：瞬态值超标及RMS半波有效值超标？
typedef struct
{
  uint16_t    Syn_node;//同步点
	uint16_t    Start_node;//起始点
	systemtime  stime;//时间
	float  wdat[CHANNEL_NUM][SAMPLE_FREQUENCY*WAVE_NUM];//12个周波的故障波形数据 ？50个周波的数据？弥补保护器与主机时间差？
	/* 存储顺序是 0-0 1-0 2-0 7-0 0-1 1-1 2-1 ...7-1 ... 7-256*12-1*/
} FAULT_RECORD_WAVE;




//过电压保护器申请对时命令
typedef struct
{
  uint8_t  start;
  uint8_t  cmd;//对时命令
  uint8_t  group;
  uint8_t  term;
  uint8_t  crc;
  uint8_t  end;
} OVERVOLTAGE_PROTECTOR_CHECK_TIME_SND;


//主机回复时间命令 
typedef struct
{
  uint8_t  start;
  uint8_t  cmd;//对时命令
  uint8_t  group;
  uint8_t  term;
  systemtime time;//发生时间 
  uint8_t  crc;
  uint8_t  end;
} MAIN_DEVICE_SND_TIME;




//有效值  稳态电气参数值 定时性的去计算一次这些稳态数据
//触发条件：4G流量比较贵 不需要定时发送？？只是定时存储到SD卡里面？？ 比如1分钟或者10分钟或者其它等等？？？
typedef struct
{
   systemtime  stime;//时间

   float Elec_U[U_CHANNEL_NUM];//4路电压总有效值  

   float Elec_I[I_CHANNEL_NUM];//3路电流总有效值

   float Elec_Thdu_Amp[U_CHANNEL_NUM][THD_CHANNEL_NUM];//4路电压基波及谐波 1-50次
   float Elec_Thdu_Phose[U_CHANNEL_NUM][THD_CHANNEL_NUM];//4路电压基波及谐波相位 1-50次
   
   float Elec_Thdi_Amp[I_CHANNEL_NUM][THD_CHANNEL_NUM];//3路电流基波及谐波 1-50次
   float Elec_Thdi_Phose[I_CHANNEL_NUM][THD_CHANNEL_NUM];//3路电流基波及谐波相位 1-50次

   float Elec_Power_Factor[POWER_FACTOR_NUM];//功率因数 三相与合相  合相是 总有功/总无功 
   
   float Elec_Active_Power[POWER_FACTOR_NUM];//有功功率 三相与平均
   
   float Elec_Reactive_Power[POWER_FACTOR_NUM];//无功功率 三相与平均
   
   float Elec_Apparent_Power[POWER_FACTOR_NUM];// 视在功率 三相与平均
   
   float positive_Usequence_components;  //电压正序稳态分量  
   float negative_Usequence_components;  //电压负序稳态分量 
   float zero_Usequence_components;      //电压零序稳态分量
   float positive_Isequence_components;  //电流正序稳态分量
   float negative_Isequence_components;  //电流负序稳态分量
   float zero_Isequence_components;      //电流零序稳态分量
   float unbalance_u;//电压不平衡度
   float unbalance_i;//电流不平衡度
} RMS_ELEC_VALUE;   








//接地故障 通过网络上传的数据结构  
typedef struct
{
  char  strID[IDLEN];		//唯一识别号 
  FAULT_RECORD_WAVE fault_record_wave;//发生接地时的波形值数据 
  //零序电压 与 零序电流 12个周波瞬态波形值 
  int  u_sequence_components[SAMPLE_FREQUENCY*WAVE_NUM];//发生接地时的 电压故障波形序分量值
  int  i_sequence_components[SAMPLE_FREQUENCY*WAVE_NUM];//发生接地时的 电流故障波形序分量值
  float Elec_U[U_CHANNEL_NUM];//4路电压RMS半波有效值 硬件锁相环 2个过零点之间的上下周波计算RMS
  float Elec_I[I_CHANNEL_NUM];//3路电流RMS半波有效值 硬件锁相环 2个过零点之间的上下周波计算RMS
}EARTH_FAULT;






//过电压保护器数据结构体  稳态与瞬态数据 ---过压保护器与故障波形参数
//触发逻辑：保护器温度/湿度超标？？？ 瞬态值超标及RMS半波有效值超标？？过电压保护器动作？？  
typedef struct
{
  char strID[IDLEN];		//唯一识别号，64个字节表示，每个过电压保护器都有一个唯一识别号
  systemtime time;//发生时间
  float temp[3];//温度 ABC
  float humi[3];//湿度 ABC
  uint8_t fault_time;//故障类型
  uint32_t  duration_time;//故障持续时间 US级别
  uint32_t  action_times[3];//每一相的动作次数值 ABC 
  FAULT_RECORD_WAVE fault_record_wave;//发生动作时的波形值数据 
}OVERVOLTAGE_PROTECTOR;






//运行日志  需要使用
typedef struct 
{
	char strID[IDLEN];		        //发生者标识码
	systemtime st;		            //发生的时刻
	uint32_t nResen;		          //日志原因标识代码
	char strDesrit[IDLEN+IDLEN];	//备注或描述，或定义全局的标识码对应表达资源。
}LogData;



//备注：针对小电流接地选线及过电压保护器监测，当触发条件满足，设备主动上传数据的时候，流程是
//先上传日志数据文件，然后再上传相应的具体数据值。


//改进版设计 总体流程，命令帧，数据帧，方式,不区分sever 和client。发起者，和接受者。
typedef struct 
{
	char       strID[IDLEN];			//发起者唯一识别号
	char       DevType;			    //命令的设备类型 DevType = CLIENT_IP or SERVICE_IP;
	uint16_t   CommandID;			//命令字分配表细分  
	systemtime st;			    //命令时间
	uint16_t   OverTime;			//超时限度
	char       RepondMod; 	 //是否回传
	uint32_t   len;	          //后面数据帧的长度  定长的结构方便解析。sizeof（主要的结构体）	
}CommandFrame;




//通讯说明
// 通讯方式 总的原则是采用 CommandFrame（命令帧） +　（DevInfo　or ConfigParm or EARTH_FAULT  or RMS_ELEC_VALUE or OVERVOLTAGE_PROTECTOR等等）数据结构体　这种方式．
//下面是示例

//  服务器端发送复位客户端命令   
//命令格式 ：CommandFrame +  空
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为SERVICE_IP）+ int CommandID（值为TagCommandSVR_RESET）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_Off 不回传） + int len （0：空 ）
// 2：type struct  ： 空 





//  服务器端发送设置配置参数命令   
//命令格式 ：CommandFrame + type struct DevInfo
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为SERVICE_IP）+ int CommandID（值为TagCommandSVR_SETDEVINFO）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_Off 不回传） + int len （ sizeof（DevInfo））
// 2：type struct  ：   DevInfo 





//  服务器端发送得到配置参数命令   
//命令格式 ：CommandFrame + 空
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为SERVICE_IP）+ int CommandID（值为TagCommandSVR_GETDEVINFO）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_On 回传） + int len （ 0：空）
// 2：type struct  ：  空 





//  客户端回送配置参数命令   
//命令格式 ：CommandFrame + type struct DevInfo
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为CLIENT_IP）+ int CommandID（值为TagCommandSVR_GETDEVINFO）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_Off 不回传） + int len （ sizeof（DevInfo））
// 2：type struct  ：   DevInfo 



//  服务器端发送设置运行参数命令   
//命令格式 ：CommandFrame + type struct ConfigParm
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为SERVICE_IP）+ int CommandID（值为TagCommandSVR_SETCONFIGPARM）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_Off 不回传） + int len （ sizeof（ConfigParm））
// 2：type struct  ：  ConfigParm




//  服务器端发送得到运行参数命令   
//命令格式 ：CommandFrame + 空
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为SERVICE_IP）+ int CommandID（值为TagCommandSVR_GETCONFIGPARM）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_On 回传） + int len （ 0 :空）
// 2：type struct  ：  空 




//  客户端回送运行参数命令   
//命令格式 ：CommandFrame + type struct ConfigParm
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为CLIENT_IP）+ int CommandID（值为TagCommandSVR_GETCONFIGPARM）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_Off 不回传） + int len （  sizeof（ConfigParm））
// 2：type struct  ：   ConfigParm



 


//  服务器端发送读取接地故障波形数据参数命令   根据时间进行文件搜索，然后上传故障波形数据
//  命令格式 ：CommandFrame + 空
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为SERVICE_IP）+ int CommandID（值为TagCommandSVR_GETEARTHDATA）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_On 回传） + int len （ 0：空 ）
// 2：type struct  ：   空





//  客户端回送接地故障波形数据命令   
//  命令格式 ：CommandFrame + type struct EARTH_FAULT
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为CLIENT_IP）+ int CommandID（值为TagCommandSVR_GETEARTHDATA）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_Off 不回传） + int len （  sizeof（EARTH_FAULT））
// 2：type struct  ：   EARTH_FAULT




//  客户端主动上传接地故障波形数据命令  当小电流接地选线及过电压保护器状态监测触发条件满足时，首先主动上传日志文件数据，然后才跟上的是具体波形数据值。
//命令格式 ：CommandFrame + type struct EARTH_FAULT
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为CLIENT_IP）+ int CommandID（值为TagCommandSVR_GETEARTHDATA）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_Off 不回传） + int len （  sizeof（EARTH_FAULT））
// 2：type struct  ：   EARTH_FAULT




//  服务器端发送读取过电压保护器数据命令   根据时间进行文件搜索，然后上传数据?
//  命令格式 ：CommandFrame + 空
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为SERVICE_IP）+ int CommandID（值为TagCommandSVR_GETOVERVOLTAGEDATA）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_On 回传） + int len （ 0：空 ）
// 2：type struct  ：   空





//  客户端回送过电压保护器数据命令   
//  命令格式 ：CommandFrame + type struct OVERVOLTAGE_PROTECTOR
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为CLIENT_IP）+ int CommandID（值为TagCommandSVR_GETOVERVOLTAGEDATA）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_Off 不回传） + int len （  sizeof（OVERVOLTAGE_PROTECTOR））
// 2：type struct  ：   OVERVOLTAGE_PROTECTOR




//  客户端主动上传过电压保护器数据命令  当小电流接地选线及过电压保护器状态监测触发条件满足时，首先主动上传日志文件数据，然后才是具体波形数据值。
//命令格式 ：CommandFrame + type struct OVERVOLTAGE_PROTECTOR
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为CLIENT_IP）+ int CommandID（值为TagCommandSVR_GETOVERVOLTAGEDATA）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_Off 不回传） + int len （  sizeof（OVERVOLTAGE_PROTECTOR））
// 2：type struct  ：   OVERVOLTAGE_PROTECTOR





//  服务器端发送读取电气稳态数据命令   上传内存中的最新的一次电气稳态参数 需要读取历史稳态电气参数？
//  命令格式 ：CommandFrame + 空
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为SERVICE_IP）+ int CommandID（值为TagCommandSVR_GETELECTIMEDATA）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_On 回传） + int len （ 0：空 ）
// 2：type struct  ：   空





//  客户端回送电气稳态数据命令   上传内存中的最新的一次电气稳态参数 需要读取历史稳态电气参数？
//  命令格式 ：CommandFrame + type struct RMS_ELEC_VALUE
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为CLIENT_IP）+ int CommandID（值为TagCommandSVR_GETELECTIMEDATA）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_Off 不回传） + int len （  sizeof（RMS_ELEC_VALUE））
// 2：type struct  ：   RMS_ELEC_VALUE




//  客户端主动上传日志文件数据命令   当小电流接地选线及过电压保护器状态监测触发条件满足时，首先主动上传此日志文件数据，然后才是具体波形数据值。
//命令格式 ：CommandFrame + type struct LogData
// 1：CommandFrame  ：char strID[IDLEN] + int DevType（值为CLIENT_IP）+ int CommandID（值为TagCommandSVR_GETLOGDATA）+  st（时间值）\
// + int OverTime （OVERTIME_15S可选） + int RepondMod （RepondMod_Off 不回传） + int len （  sizeof（LogData））
// 2：type struct  ：   LogData



#endif









