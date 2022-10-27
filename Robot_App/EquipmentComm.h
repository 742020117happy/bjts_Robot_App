#ifndef __EQUIPMENTCOMMDLL_H__
#define __EQUIPMENTCOMMDLL_H__

#include <WINDOWS.H>
#include <stdio.h>

#ifdef EQUIPMENTDLL
#define EQUIPMENTCOMM_DLL_API extern "C" __declspec(dllexport)
#else
#define EQUIPMENTCOMM_DLL_API extern "C" __declspec(dllimport)
#endif
/*************************************************************************
 �������ݻص�����
void (CALLBACK *EQCOMMDataCallBack)(int _cid, unsigned int _lim_code, void* _lim, int _lim_len, int _paddr)
_cid�����ӱ��
_lim_code��LIM Code (LIM.nCode)
_lim��LIM
_lim_len��LIM������ (LIM.nLIMLen)
_paddr���û�����
*************************************************************************/
typedef void (CALLBACK *EQCOMMDataCallBack)(int _cid, unsigned int _lim_code, void* _lim, int _lim_len, int _paddr); 
/*************************************************************************
 ����״̬�ص�����
(CALLBACK *EQCOMMSTATECallBack)(int _cid, unsigned int _state_code, char* _ip, int _port, int _paddr)
_cid�����ӱ��
_state_code������״̬ (EQCOMM_STATE_XXX)
_ip��IP��ַ
_port���˿ں�
_paddr���û�����
*************************************************************************/
#define EQCOMM_STATE_OK					2001	//ͨѶ�������״�ɹ�
#define EQCOMM_STATE_ERR				2002	//ͨѶ�������״�ʧ��
#define EQCOMM_STATE_LOST				2003	//ͨѶ�������״����ӶϿ�

typedef void (CALLBACK *EQCOMMStateCallBack)(int _cid, unsigned int _state_code, char* _ip, int _port, int _paddr); 

/*************************************************************************
��ȡ�汾��
int __stdcall		GetEquipmentCommVersion()
����ֵ���汾��
*************************************************************************/
EQUIPMENTCOMM_DLL_API int __stdcall	GetEquipmentCommVersion();

/*************************************************************************
��ʼ�����Ӷ�̬��
EQUIPMENTCOMM_DLL_API bool __stdcall EquipmentCommInit(int _paddr, EQCOMMDataCallBack _feqdata, EQCOMMStateCallBack _feqstate)
_paddr���û����ݣ��ڻص�������Ѵ����ݴ���
_feqdata���������ݻص�����
_feqstate������״̬�ص�����
����ֵ��false��ʧ�� true���ɹ�
*************************************************************************/
EQUIPMENTCOMM_DLL_API bool __stdcall EquipmentCommInit(int _paddr, EQCOMMDataCallBack _feqdata, EQCOMMStateCallBack _feqstate);

/*************************************************************************
�˳����Ӷ�̬��
bool __stdcall EquipmentCommDestory();
����ֵ��false��ʧ�� true���ɹ�
*************************************************************************/
EQUIPMENTCOMM_DLL_API bool __stdcall EquipmentCommDestory();

/*************************************************************************
���豸����
bool __stdcall	OpenEquipmentComm(int _cid, char* _ip, int _port);
_cid�����ӱ��
_ip������IP
_port�����Ӷ˿�
����ֵ��false��ʧ�� true���ɹ�
*************************************************************************/
EQUIPMENTCOMM_DLL_API bool __stdcall OpenEquipmentComm(int _cid, char* _ip, int _port);

/*************************************************************************
�ر��豸����
bool __stdcall	CloseEquipmentComm(int _cid);
_cid�����ӱ��
����ֵ��false��ʧ�� true���ɹ�
*************************************************************************/
EQUIPMENTCOMM_DLL_API bool __stdcall CloseEquipmentComm(int _cid);

/*************************************************************************
����TIP
bool __stdcall	SendLIM(int _cid, void* _lim, int _lim_len);
_cid�����ӱ��
_lim��LIM
_lim_len��LIM������ (LIM.nLIMLen)
����ֵ��false��ʧ�� true���ɹ�
*************************************************************************/
EQUIPMENTCOMM_DLL_API bool __stdcall SendLIM(int _cid, void* _lim, int _lim_len);

#endif


