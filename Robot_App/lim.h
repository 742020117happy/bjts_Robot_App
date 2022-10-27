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

#define LIM_DT_IP      "237.1.1.200"  // �豸�㲥��ַ
#define LIM_DT_PORT    2111           // UDP�˿ں�

/*
	LIM(Lidar Interaction Message)�Ļ����ṹ:
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




#define LIM_USER_PORT    2112           // TCPͨѶ, SOCKET�˿ں�

/*
	LIM������Ϣ
*/
#define LIM_TAG          0xF5EC96A5     // ����ʶ����
#define	LIM_VER          0x01000000     // ���İ汾
#define LIM_DATA_LEN     4              // LIM_HEAD �������ݳ��ȣ���λΪ32λWORD

typedef struct
{
	unsigned int TAG;                   // ����ʶ����
	unsigned int VER;                   // ���İ汾
	unsigned int nCID;                  // ���ӱ�ţ�������˵��.
	unsigned int nCode;                 // ���Ĵ���
	unsigned int Data[LIM_DATA_LEN];    // ���Ļ�������
	unsigned int nLIMLen;               // �����ܳ��ȣ�����Extended_Data
	unsigned int CheckSum;              // У���
}STRUCTPACKED LIM_HEAD;

/*
nCID˵����
1. ��ͻ������ӵ�̨�״�ʱ��nCIDΪÿ���ͻ��˵�TCP���ӵ�Ψһ�ġ����ֲ����ID��
2. �ͻ��������״ｨ��TCP���Ӻ��ڷ��͸��״��LIM������ָ�����Ӧ��nCID���״�ظ����ͻ��˵�ÿ������Ҳʹ�ô�ID
*/

#define LIM_CODE_LDBCONFIG          111        // �豸������Ϣ.�豸�˷���
#define LIM_CODE_START_LDBCONFIG    110        // �����豸������Ϣ�㲥.Ӧ�ö˷���
#define LIM_CODE_STOP_LDBCONFIG     112        // ֹͣ�豸������Ϣ�㲥.Ӧ�ö˷���
#define LIM_CODE_GET_LDBCONFIG      114        // ��ȡ�豸������Ϣ.Ӧ�ö˷���

//LIM Code.
#define LIM_CODE_HB                10              // ����.Ӧ�ö˷���
#define LIM_CODE_HBACK             11              // �����ظ�.�豸�˷���
#define LIM_CODE_LMD               901             // LMD���״�������ݣ��ο�����LMD��˵��.�豸�˷���
#define LIM_CODE_LMD_RSSI          911             // LMD-RSSI���������ʵ��״��������.�豸�˷���
#define LIM_CODE_START_LMD         1900            // ����LMD���ݴ���.Ӧ�ö˷���
#define LIM_CODE_STOP_LMD          1902            // ֹͣLMD���ݴ���.Ӧ�ö˷���
#define LIM_CODE_ALARM             9001            // ����״̬�������ο�����ALARM/DISALARM��˵��.�豸�˷���
#define LIM_CODE_DISALARM          9003            // ״̬����ȡ�����ο�����ALARM/DISALARM��˵��.�豸�˷���

#define LIM_CODE_FMSIG_QUERY       1912            // ��ѯ�������ź�.Ӧ�ö˷���
#define LIM_CODE_FMSIG             1911            // �������ź�.�豸�˷���
/*
LIM_CODE_FMSIG_QUERY˵��:
Ҫ��ѯ��������ID����LIM_HEAD.Data[0]����

LIM_CODE_FMSIG˵��:
������ID��LIM_HEAD.Data[0]�г���.����ź�״̬��LIM_HEAD.Data[1]����.

1. ������IDΪLIM_HEAD.Data[0]�ļ���ź�ΪLIM_HEAD.Data[1]��bit0(������)��bit2(Ԥ����), bit4(ע����)(1/0, ���޸澯)
2. ����������ļ���ź���״̬�Ļ�ֱ�Ϊbit1(������)��bit,3(Ԥ����), bit5(ע����)(1/0, ���޸澯)

*/

#define LIM_CODE_IOREAD            1920            // ��ȡI/O����״̬.Ӧ�ö˷���
#define LIM_CODE_IOSET             1922            // ����I/O����״̬������������ӣ�.Ӧ�ö˷���
#define LIM_CODE_IOSET_RELEASE     1924            // �������I/O����״̬.Ӧ�ö˷���
#define LIM_CODE_IOSTATUS          1921            // IO״̬�ظ�Ӧ����.�豸�˷���
/*
LIM_CODE_IOSTATUS˵��:
    I/O����״̬��LIM_HEAD.Data[0]�г���.���豸��I�ڸ���ΪINNum,O�ڸ���ΪOUTNum��
    bit��0��...bit(OUTNum-1)ΪOUT(1)...OUT(OUTNum)��״̬��0�͵�ƽ��1�ߵ�ƽ��.
    bit��OUTNum��...bit(OUTNum-1+INNum)ΪIN(1)...IN(INNum)��״̬��0�͵�ƽ��1�ߵ�ƽ��
*/

/*
	�״��������LMD����Ϣͷ
*/
typedef struct
{
	unsigned int nRange;             // �豸����.��λ:cm
	int          nBAngle;            // ����������ʼ�Ƕȣ�����Ϊ��ֵ.��λ:1/1000��
	int          nEAngle;            // �������ݽ����Ƕȣ�����Ϊ��ֵ.��λ:1/1000��
	unsigned int nAnglePrecision;    // �ǶȾ���.��λ��1/1000 ��
	unsigned int nRPM;               // ɨ��Ƶ��.��λ��RPM (ת/����)
	unsigned int nMDataNum;          // �������ݵĸ���������nBAngle��nEAngle��nAnglePrecision����
}STRUCTPACKED LMD_INFO;

/*
	LMD��������
*/
typedef unsigned short LMD_D_Type;             // ������������.��λcm

/*
	RSSI��������
*/
typedef unsigned short LMD_D_RSSI_Type;        // ���������RSSI��ֵ(0-1000)

/*
	�״�����״̬�������룬��LIM_HEAD.Data[0]�г���
*/
#define LIM_DATA_ALARMCODE_INTERNAL            1       // �ڲ�����
#define LIM_DATA_ALARMCODE_Occluded            101     // �豸�����汻�ڵ�����͸����̫��
#define LIM_DATA_ALARMCODE_High_Temperature    1001    // ���±���
#define LIM_DATA_ALARMCODE_Low_Temperature     1002    // ���±���


#define ULDINI_MAX_ATTR_STR_LEN                0x20    // ULDINI_Type���ַ����鳤��

typedef struct
{
	// ��Ʒ��Ϣ
	char szType[ULDINI_MAX_ATTR_STR_LEN];            // ��Ʒ�ͺ�
	char szManufacturer[ULDINI_MAX_ATTR_STR_LEN];    // ������
	char szReleaseDate[ULDINI_MAX_ATTR_STR_LEN];     // ��������
	char szSerialNo[ULDINI_MAX_ATTR_STR_LEN];        // ��Ʒ���к�

	// ��������
	char szMAC[ULDINI_MAX_ATTR_STR_LEN];     // MAC��ַ
	char szIP[ULDINI_MAX_ATTR_STR_LEN];      // IP��ַ
	char szMask[ULDINI_MAX_ATTR_STR_LEN];    // ��������
	char szGate[ULDINI_MAX_ATTR_STR_LEN];    // Ĭ������
	char szDNS[ULDINI_MAX_ATTR_STR_LEN];     // DNS������

	// ��������
	int nMR;        // ����
	int nESAR;      // �豸�Ƕȷ�Χ
	int nESA[2];    // �豸�Ƕ���ʼ��ֹ��
	int nSAR;       // ��Ч�Ƕȷ�Χ
	int nSA[2];     // ��Ч�Ƕ���ʼ��ֹ��
	int nSAV;       // ɨ����ٶȣ���/�룩
	int nSAP;       // ɨ��Ƕȷֱ���
	int nPF;        // ����Ƶ��(HZ)

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
