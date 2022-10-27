#ifndef __LIM_H__
#define __LIM_H__

#if defined (LINUX)

#ifndef STRUCTPACKED
#define STRUCTPACKED __attribute__((__packed__))
#endif

#elif defined(WIN32)||defined(WINNT)

#ifndef STRUCTPACKED
#define STRUCTPACKED
#endif

#endif

#define LIM_DT_IP      "237.1.1.200"  // 设备广播地址
#define LIM_DT_PORT    2111           // UDP端口号

/*
	LIM(Lidar Interaction Message)的基本结构:
	LIM ::= LIM_HEAD + Extended_Data
	Remark:
	1) Extended_Data : optional.
	
	Extended_Data:
	1) Lidar Measurement Data (LMD) Structure:
	   LMD ::= LMD_INFO + Data_Array
	   Remark:
	   1) Lidar mesaurement: distance value, unit: cm;
	   2) Data_Array: array of measurement; type: LMD_D_Type; length: LMD_INFO.nMDataNum.
*/




#define LIM_USER_PORT    2112           // TCP通讯, SOCKET端口号

/*
	LIM报文信息
*/
#define LIM_TAG          0xF5EC96A5     // 报文识别码
#define	LIM_VER          0x01000000     // 报文版本
#define LIM_DATA_LEN     4              // LIM_HEAD 基本数据长度，单位为32位WORD

typedef struct
{
	unsigned int TAG;                   // 报文识别码
	unsigned int VER;                   // 报文版本
	unsigned int nCID;                  // 连接编号，见下文说明.
	unsigned int nCode;                 // 报文代码
	unsigned int Data[LIM_DATA_LEN];    // 报文基本数据
	unsigned int nLIMLen;               // 报文总长度，包括Extended_Data
	unsigned int CheckSum;              // 校验和
}STRUCTPACKED LIM_HEAD;

/*
nCID说明：
1. 多客户端连接单台雷达时，nCID为每个客户端的TCP连接的唯一的、保持不变的ID；
2. 客户端在与雷达建立TCP连接后，在发送给雷达的LIM报文中指定其对应的nCID，雷达回复给客户端的每个报文也使用此ID
*/

#define LIM_CODE_LDBCONFIG          111        // 设备配置信息.设备端发送
#define LIM_CODE_START_LDBCONFIG    110        // 启动设备配置信息广播.应用端发送
#define LIM_CODE_STOP_LDBCONFIG     112        // 停止设备配置信息广播.应用端发送
#define LIM_CODE_GET_LDBCONFIG      114        // 获取设备配置信息.应用端发送

//LIM Code.
#define LIM_CODE_HB                10              // 心跳.应用端发送
#define LIM_CODE_HBACK             11              // 心跳回复.设备端发送
#define LIM_CODE_LMD               901             // LMD：雷达测量数据，参考下文LMD的说明.设备端发送
#define LIM_CODE_LMD_RSSI          911             // LMD-RSSI：带反射率的雷达测量数据.设备端发送
#define LIM_CODE_START_LMD         1900            // 请求LMD数据代码.应用端发送
#define LIM_CODE_STOP_LMD          1902            // 停止LMD数据代码.应用端发送
#define LIM_CODE_ALARM             9001            // 运行状态报警，参考下文ALARM/DISALARM的说明.设备端发送
#define LIM_CODE_DISALARM          9003            // 状态报警取消，参考下文ALARM/DISALARM的说明.设备端发送

#define LIM_CODE_FMSIG_QUERY       1912            // 查询区域监测信号.应用端发送
#define LIM_CODE_FMSIG             1911            // 区域监测信号.设备端发送
/*
LIM_CODE_FMSIG_QUERY说明:
要查询的区域组ID号在LIM_HEAD.Data[0]承载

LIM_CODE_FMSIG说明:
区域组ID在LIM_HEAD.Data[0]中承载.检测信号状态在LIM_HEAD.Data[1]承载.

1. 区域组ID为LIM_HEAD.Data[0]的检测信号为LIM_HEAD.Data[1]的bit0(报警区)，bit2(预警区), bit4(注意区)(1/0, 有无告警)
2. 所有区域组的检测信号总状态的或分别为bit1(报警区)，bit,3(预警区), bit5(注意区)(1/0, 有无告警)

*/

#define LIM_CODE_IOREAD            1920            // 读取I/O端子状态.应用端发送
#define LIM_CODE_IOSET             1922            // 设置I/O端子状态（仅限输出端子）.应用端发送
#define LIM_CODE_IOSET_RELEASE     1924            // 解除设置I/O端子状态.应用端发送
#define LIM_CODE_IOSTATUS          1921            // IO状态回复应答报文.设备端发送
/*
LIM_CODE_IOSTATUS说明:
    I/O端子状态在LIM_HEAD.Data[0]中承载.令设备的I口个数为INNum,O口个数为OUTNum，
    bit（0）...bit(OUTNum-1)为OUT(1)...OUT(OUTNum)的状态（0低电平，1高电平）.
    bit（OUTNum）...bit(OUTNum-1+INNum)为IN(1)...IN(INNum)的状态（0低电平，1高电平）
*/

/*
	雷达测量数据LMD的信息头
*/
typedef struct
{
	unsigned int nRange;             // 设备量程.单位:cm
	int          nBAngle;            // 测量数据起始角度，可能为负值.单位:1/1000度
	int          nEAngle;            // 测量数据结束角度，可能为负值.单位:1/1000度
	unsigned int nAnglePrecision;    // 角度精度.单位：1/1000 度
	unsigned int nRPM;               // 扫描频率.单位：RPM (转/分钟)
	unsigned int nMDataNum;          // 测量数据的个数，根据nBAngle、nEAngle和nAnglePrecision计算
}STRUCTPACKED LMD_INFO;

/*
	LMD数据类型
*/
typedef unsigned short LMD_D_Type;             // 测量距离数据.单位cm

/*
	RSSI数据类型
*/
typedef unsigned short LMD_D_RSSI_Type;        // 测量物体的RSSI数值(0-1000)

/*
	雷达运行状态报警代码，在LIM_HEAD.Data[0]中承载
*/
#define LIM_DATA_ALARMCODE_INTERNAL            1       // 内部错误
#define LIM_DATA_ALARMCODE_Occluded            101     // 设备出光面被遮挡或者透过罩太脏
#define LIM_DATA_ALARMCODE_High_Temperature    1001    // 高温报警
#define LIM_DATA_ALARMCODE_Low_Temperature     1002    // 低温报警


#define ULDINI_MAX_ATTR_STR_LEN                0x20    // ULDINI_Type中字符数组长度

typedef struct
{
	// 产品信息
	char szType[ULDINI_MAX_ATTR_STR_LEN];            // 产品型号
	char szManufacturer[ULDINI_MAX_ATTR_STR_LEN];    // 制造商
	char szReleaseDate[ULDINI_MAX_ATTR_STR_LEN];     // 出厂日期
	char szSerialNo[ULDINI_MAX_ATTR_STR_LEN];        // 产品序列号

	// 网络配置
	char szMAC[ULDINI_MAX_ATTR_STR_LEN];     // MAC地址
	char szIP[ULDINI_MAX_ATTR_STR_LEN];      // IP地址
	char szMask[ULDINI_MAX_ATTR_STR_LEN];    // 子网掩码
	char szGate[ULDINI_MAX_ATTR_STR_LEN];    // 默认网关
	char szDNS[ULDINI_MAX_ATTR_STR_LEN];     // DNS服务器

	// 测量参数
	int nMR;        // 量程
	int nESAR;      // 设备角度范围
	int nESA[2];    // 设备角度起始终止角
	int nSAR;       // 有效角度范围
	int nSA[2];     // 有效角度起始终止角
	int nSAV;       // 扫描角速度（度/秒）
	int nSAP;       // 扫描角度分辨率
	int nPF;        // 测量频率(HZ)

}ULDINI_Type;
#ifdef __cplusplus
extern "C"{
#endif

/*
	LIM utilities.
*/

/*
	LIM_CheckSum:
	Function: calculating the checksum of LIM.
	Return value: checksum of _lim.
	Remark:
	1) Checksum calculation for LIM creating & sending;
	2) Checksum checking for received LIM.
*/
unsigned int LIM_CheckSum(LIM_HEAD * _lim);

/*
LIM_ExData:
Function: memory address of Extended_Data of LIM.
Return value: pointer to Extended_Data.
Remark:
1) When a LIM has extended data, e.g., LMD LIM, use LIM_ExData to obtain the memory address;
2) LIM_HEAD and Extended_Data locate in continuous memory, so the address of Extended_Data equals to (void*)(_lim + 1).
*/
void* LIM_ExData(LIM_HEAD* _lim);

/*
LMD_Info & LMD_D:
Function: memory address of LMD_INFO & measurement data array in LIM.
Return value: pointer to LMD_INFO & measurement data array.
Remark:
1) For an LMD LIM, address of LMD_INFO equals to (LMD_INFO*)LIM_ExData(_lim);
2) The whole LMD LIM locates in continuous memory, so the address of measurement data equals to (LMD_D_Type*)(LMD_Info(_lim) + 1).
*/
LMD_INFO* LMD_Info(LIM_HEAD* _lim);
LMD_D_Type* LMD_D(LIM_HEAD* _lim);
LMD_D_RSSI_Type* LMD_D_RSSI(LIM_HEAD* _lim);

/*
LIM_Pack:
Function: compose a LIM.
return value: true / false
Remark:
1) composed LIM is returned in _lim.
*/
bool LIM_Pack(LIM_HEAD*& _lim, unsigned int _cid, unsigned int _code, unsigned int* _data = NULL, unsigned int _ext_data_len = 0, void* _ext_data = NULL);
/*
LIM_Copy:
Function: copy a LIM.
return value: true / false
Remark:
1) LIM copied from _slim is returned in _dlim.
*/
bool LIM_Copy(LIM_HEAD*& _dlim, LIM_HEAD* _slim);
/*
LIM_Release:
Function: release a LIM.
Remark:
1) memory of _lim is released, and _lim is cleared to NULL.
*/
void LIM_Release(LIM_HEAD*& _lim);

#ifdef __cplusplus
}
#endif

#endif
