// TryLIM.cpp : 定义控制台应用程序的入口点。

#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>

#include "EquipmentComm.h"
#include "lim.h"

// IO_OUTNUM,IO_INNUM由设备信号确定，运行此程序前请阅读使用手册或者联系厂家确定
#define IO_OUTNUM  3
#define IO_INNUM   2
char *szIP = "192.168.1.201";

int g_IO_OutSts[IO_OUTNUM];
int g_IO_InSts[IO_INNUM];

bool connected = false;
int nFrameNum = 0;

#define	MAX_FRAME_NUM    100
#define	USER_DATA        NULL
#define	USER_TASK_CID    1

// LIM_CODE_LDBCONFIG LIM包解码
void LIM_CODE_LDBCONFIG_Decoding(LIM_HEAD* lim)
{
	if (lim->nCode != LIM_CODE_LDBCONFIG)
		return;
	printf("#%d LMD Frame: LIM_CODE_LDBCONFIG--------------------\n", nFrameNum);
	ULDINI_Type *ldtype = (ULDINI_Type*)LIM_ExData(lim);
	printf("----Product Info\n");
	printf("Type: %s\n", ldtype->szType);
	printf("Manufacturer: %s\n", ldtype->szManufacturer);
	printf("ReleaseDate: %s\n", ldtype->szReleaseDate);
	printf("SerialNo: %s\n", ldtype->szSerialNo);

	printf("----Network Configuration\n");
	printf("szMAC: %s\n", ldtype->szMAC);
	printf("szIP: %s\n", ldtype->szIP);
	printf("szMask: %s\n", ldtype->szMask);
	printf("szGate: %s\n", ldtype->szGate);
	printf("szDNS: %s\n", ldtype->szDNS);

	printf("----Measurement Parameters\n");
	printf("MR: %d cm\n", ldtype->nMR);
	printf("ESAR: %d°(%d°~%d°) \n", ldtype->nESAR, ldtype->nESA[0]/1000, ldtype->nESA[1]/1000);
	printf("SAR: %d°(%d°~%d°) \n", ldtype->nSAR, ldtype->nSA[0], ldtype->nSA[1]);
	printf("SAV: %d \n", ldtype->nSAV);
	printf("SAP: %3.3f° \n", ldtype->nSAP / 1000.);
	printf("PF: %d \n", ldtype->nPF);

	printf("\t------------------------------------------\n\n");
}

// LIM_CODE_LMD LIM包解码
void LIM_CODE_LMD_Decoding(LIM_HEAD*lim)
{
	if (lim->nCode != LIM_CODE_LMD)
		return;
	LMD_INFO* lmd_info = LMD_Info(lim);  // 获取LMD_INFO指针
	LMD_D_Type* lmd = LMD_D(lim);        // 获取LMD数据指针

	//打印LMD_INFO
	printf("#%d LMD Frame: LIM_CODE_LMD--------------------\n", nFrameNum);

	printf("Range<%5.2fM> Angle<%5.3f°- %5.3f°> AngleStep<%3.3f°>\nRPM<%drpm> MeasurementData_Num<%d>\n", \
		lmd_info->nRange / 100., lmd_info->nBAngle / 1000., \
		lmd_info->nEAngle / 1000., lmd_info->nAnglePrecision / 1000., \
		lmd_info->nRPM, lmd_info->nMDataNum);

	//打印部分角度上的LMD（极坐标）
	printf("    LMD  %5.3f°:%d cm", (float)(lmd_info->nBAngle + 0 * (float)lmd_info->nAnglePrecision) / 1000.0, lmd[0]);
	printf("    %5.3f°:%d cm", (float)(lmd_info->nBAngle + 1 * (float)lmd_info->nAnglePrecision) / 1000.0, lmd[1]);
	printf("... ... ... ...");
	printf("%5.3f°:%d cm\n",
		(float)(lmd_info->nBAngle + (lmd_info->nMDataNum - 1) * (float)lmd_info->nAnglePrecision) / 1000.0, lmd[(lmd_info->nMDataNum - 1)]);
	printf("\t------------------------------------------\n\n");
}

// LIM_CODE_LMD_RSSI LIM包解码
void LIM_CODE_LMD_RSSI_Decoding(LIM_HEAD *lim)
{
	if (lim->nCode != LIM_CODE_LMD_RSSI)
		return;
	LMD_INFO* lmd_info = LMD_Info(lim);  //获取LMD_INFO指针
	LMD_D_Type* lmd = LMD_D(lim);        //获取LMD数据指针
	LMD_D_RSSI_Type *lmdrssi = LMD_D_RSSI(lim);//获取RSSI数据指针

	printf("#%d LMD Frame: LIM_CODE_LMD_RSSI--------------------\n", nFrameNum);

	//打印LMD_INFO
	printf("Range<%5.2fM> Angle<%5.3f°- %5.3f°> AngleStep<%3.3f°>\nRPM<%drpm> MeasurementData_Num<%d>\n", \
		lmd_info->nRange / 100., lmd_info->nBAngle / 1000., \
		lmd_info->nEAngle / 1000., lmd_info->nAnglePrecision / 1000., \
		lmd_info->nRPM, lmd_info->nMDataNum);
	//打印部分角度上的LMD（极坐标）和RSSI
	printf("    LMD  %5.3f°:%d cm RSSI:%d", (float)(lmd_info->nBAngle + 0 * (float)lmd_info->nAnglePrecision) / 1000.0, lmd[0], lmdrssi[0]);
	printf("    %5.3f°:%d cm RSSI:%d", (float)(lmd_info->nBAngle + 1 * (float)lmd_info->nAnglePrecision) / 1000.0, lmd[1], lmdrssi[1]);
	printf("... ... ... ...");
	printf("%5.3f°:%d cm RSSI:%d\n",
		(float)(lmd_info->nBAngle + (lmd_info->nMDataNum - 1) * (float)lmd_info->nAnglePrecision) / 1000.0, lmd[(lmd_info->nMDataNum - 1)], lmdrssi[(lmd_info->nMDataNum - 1)]);
	printf("\t------------------------------------------\n\n");
}

// LIM_CODE_FMSIG LIM包解码
void LIM_CODE_FMSIG_Decoding(LIM_HEAD *lim)
{
	if (lim->nCode != LIM_CODE_FMSIG)
		return;
	int fieldIdx;
	int alm;
	int attentionW, attentionA;
	int alertW, alertA;
	int alarmW, alarmA;

	fieldIdx = lim->Data[0];
	alm = lim->Data[1];

	alarmA = (alm & 0x01);
	alarmW = (alm & 0x02) >> 1;
	alertA = (alm & 0x04) >> 2;
	alertW = (alm & 0x08) >> 3;
	attentionA = (alm & 0x10) >> 4;
	attentionW = (alm & 0x20) >> 5;

	printf("#%d LMD Frame: LIM_CODE_FMSIG--------------------\n", nFrameNum);
	printf("field_%d FMSIG: %d  %d  %d\n", fieldIdx, alarmA, alertA, attentionA);
	printf("  whole FMSIG: %d  %d  %d\n", alarmW, alertW, attentionW);
	printf("\t------------------------------------------\n\n");
}

// LIM_CODE_IOSTATUS LIM包解码
void LIM_CODE_IOSTATUS_Decoding(LIM_HEAD *lim)
{
	if (lim->nCode != LIM_CODE_IOSTATUS)
		return;

	int iosts = lim->Data[0];

	for (int i = 0; i < IO_OUTNUM; i++)
		g_IO_OutSts[i] = ((iosts & (1 << i)) >> i);

	for (int i = IO_OUTNUM; i < IO_OUTNUM + IO_INNUM; i++)
		g_IO_InSts[i - IO_OUTNUM] = ((iosts & (1 << i)) >> i);

	printf("#%d LMD Frame: LIM_CODE_IOSTATUS--------------------\n", nFrameNum);
	for (int i = 0; i < IO_OUTNUM; i++)
		printf(" OUT_%d: %d", i + 1, g_IO_OutSts[i]);
	printf("\n");
	for (int i = 0; i < IO_INNUM; i++)
		printf(" IN_%d: %d", i + 1, g_IO_InSts[i]);
	printf("\n\t------------------------------------------\n\n");
}

// LIM_CODE_ALARM LIM包解码
void LIM_CODE_ALARM_Decoding(LIM_HEAD *lim)
{
	int type;
	if (lim->nCode != LIM_CODE_ALARM)
		return;
	type = lim->Data[0];
	printf("#%d LMD Frame: LIM_CODE_ALARM--------------------\n", nFrameNum);
	if (type == LIM_DATA_ALARMCODE_INTERNAL)
		printf("Equipment Alarm: Internal error\n");
	else if (type == LIM_DATA_ALARMCODE_Occluded)
		printf("Equipment Alarm: Occluded or too dirty\n");
	else if (type == LIM_DATA_ALARMCODE_High_Temperature)
		printf("Equipment Alarm: Temperature too high\n");
	else if (type == LIM_DATA_ALARMCODE_Low_Temperature)
		printf("Equipment Alarm: Temperature too low\n");

	printf("\t------------------------------------------\n\n");
}

// LIM_CODE_DISALARM LIM包解码
void LIM_CODE_DISALARM_Decoding(LIM_HEAD *lim)
{
	int type;
	if (lim->nCode != LIM_CODE_DISALARM)
		return;
	type = lim->Data[0];
	printf("#%d LMD Frame: LIM_CODE_DISALARM--------------------\n", nFrameNum);
	if (type == LIM_DATA_ALARMCODE_INTERNAL)
		printf("Equipment DisAlarm: Internal error\n");
	else if (type == LIM_DATA_ALARMCODE_Occluded)
		printf("Equipment DisAlarm: Occluded or too dirty\n");
	else if (type == LIM_DATA_ALARMCODE_High_Temperature)
		printf("Equipment DisAlarm: Temperature too high\n");
	else if (type == LIM_DATA_ALARMCODE_Low_Temperature)
		printf("Equipment DisAlarm: Temperature too low\n");

	printf("\t------------------------------------------\n\n");
}

//设备通讯数据回调函数
void CALLBACK EqCommDataCallBack(int _cid, unsigned int _lim_code, void* _lim, int _lim_len, int _paddr)
{
	printf("EqCommDataCallBack _cid=%d, _lim_code=%d, _lim_len=%d, _paddr=%d\n",
		_cid, _lim_code, _lim_len, _paddr);

	LIM_HEAD *lim = (LIM_HEAD*)_lim;
	unsigned int checksum = LIM_CheckSum(lim);
	if (checksum != lim->CheckSum)           //校验数据是否正确
	{
		printf("\tLIM checksum error!\n");
		return;
	}

	if (LIM_CODE_LDBCONFIG == lim->nCode)
		LIM_CODE_LDBCONFIG_Decoding(lim);

	if (LIM_CODE_LMD == lim->nCode)
	{
		nFrameNum++;
		LIM_CODE_LMD_Decoding(lim);
	}

	if (LIM_CODE_LMD_RSSI == lim->nCode)
	{
		nFrameNum++;
		LIM_CODE_LMD_RSSI_Decoding(lim);
	}

	if (LIM_CODE_FMSIG == lim->nCode)
		LIM_CODE_FMSIG_Decoding(lim);

	if (LIM_CODE_IOSTATUS == lim->nCode)
		LIM_CODE_IOSTATUS_Decoding(lim);

	if (LIM_CODE_ALARM == lim->nCode)
		LIM_CODE_ALARM_Decoding(lim);

	if (LIM_CODE_DISALARM == lim->nCode)
		LIM_CODE_DISALARM_Decoding(lim);

}

//设备通讯状态回调函数
void CALLBACK EqCommStateCallBack(int _cid, unsigned int _state_code, char* _ip, int _port, int _paddr)
{
	printf("EqCommStateCallBack _cid=%d, _state_code=%d, _ip=%s, _port=%d _paddr=%d\n",
		_cid, _state_code, _ip, _port, _paddr);

	if (_state_code == EQCOMM_STATE_OK)   //连接成功：请求LMD数据
	{
		LIM_HEAD* lim = NULL;
		LIM_Pack(lim, USER_TASK_CID, LIM_CODE_GET_LDBCONFIG, NULL); //获取设备信息
		SendLIM(USER_TASK_CID, lim, lim->nLIMLen);        //发送LIM
		LIM_Release(lim);                                 //释放LIM

		connected = true;
	}
	else
	{
		connected = false;

		printf("Connection failed.code:%d\n",_state_code);
	}
}


//int _tmain(int argc, _TCHAR* argv[])
//{
//	LIM_HEAD* lim = NULL;
//
//	//设备数据通讯初始化操作
//	printf("GetEquipmentCommVersion version =%d\n", GetEquipmentCommVersion()); //获取SDK版本
//
//	EquipmentCommInit(USER_DATA, EqCommDataCallBack, EqCommStateCallBack);      //通讯库初始化
//
//	OpenEquipmentComm(USER_TASK_CID, szIP, LIM_USER_PORT);                      //连接雷达
//
//	while (MAX_FRAME_NUM > nFrameNum)
//	{
//		Sleep(5000); //等待5秒；
//
//		if (connected == true)
//		{
//			LIM_HEAD* lim = NULL;
//			LIM_Pack(lim, USER_TASK_CID, LIM_CODE_FMSIG_QUERY,NULL); // 获取区域监测信号的LIM组包
//			lim->Data[0] = 0;
//			SendLIM(USER_TASK_CID, lim, lim->nLIMLen);               // 发送LIM
//			LIM_Release(lim);                                        // 释放LIM
//			Sleep(1000);
//
//			LIM_Pack(lim, USER_TASK_CID, LIM_CODE_HB, NULL); // 获取区域监测信号的LIM组包
//			SendLIM(USER_TASK_CID, lim, lim->nLIMLen);
//			LIM_Release(lim);
//			Sleep(1000);
//
//			
//			LIM_Pack(lim, USER_TASK_CID, LIM_CODE_IOREAD,NULL); // 读取当前IO状态的LIM组包
//			SendLIM(USER_TASK_CID, lim, lim->nLIMLen);
//			LIM_Release(lim);
//			Sleep(1000);
//
//			LIM_Pack(lim, USER_TASK_CID, LIM_CODE_IOSET, NULL); // 设置IO状态的LIM组包
//			lim->Data[0] = 0; // value
//			lim->Data[1] = 1; // index
//			SendLIM(USER_TASK_CID, lim, lim->nLIMLen);
//			LIM_Release(lim);
//			Sleep(1000);
//
//			LIM_Pack(lim, USER_TASK_CID, LIM_CODE_IOSET_RELEASE, NULL); // IO状态设置权的释放的LIM组包
//			SendLIM(USER_TASK_CID, lim, lim->nLIMLen);
//			LIM_Release(lim);
//			Sleep(1000);
//
//			LIM_Pack(lim, USER_TASK_CID, LIM_CODE_START_LMD); // 请求LMD数据LIM组包
//			SendLIM(USER_TASK_CID, lim, lim->nLIMLen);
//			LIM_Release(lim);
//			Sleep(500);
//
//			LIM_Pack(lim, USER_TASK_CID, LIM_CODE_STOP_LMD); // 停止接收LMD数据LIM组包
//			SendLIM(USER_TASK_CID, lim, lim->nLIMLen);
//			LIM_Release(lim);
//
//		}
//
//		if (!connected)
//			break;
//	}
//
//	if (connected)
//	{
//		
//		LIM_HEAD* lim = NULL;
//		// IO状态设置权的释放
//		LIM_Pack(lim, USER_TASK_CID, LIM_CODE_IOSET_RELEASE, NULL); // IO状态设置权的释放的LIM组包
//		SendLIM(USER_TASK_CID, lim, lim->nLIMLen);
//		LIM_Release(lim);
//		//停止LMD数据
//		LIM_Pack(lim, USER_TASK_CID, LIM_CODE_STOP_LMD);  // 停止LMD数据的LIM组包
//		SendLIM(USER_TASK_CID, lim, lim->nLIMLen);        // 发送LIM
//		LIM_Release(lim);                                 // 释放LIM
//		Sleep(1000);
//	}
//
//	CloseEquipmentComm(USER_TASK_CID); // 关闭当前设备
//
//	EquipmentCommDestory(); //关闭通讯库
//
//	printf("press any key to exit\n");
//	getchar();
//
//	return 0;
//}

