/*************************************************************************************************************************************************
**Copyright(C), 2020-2025, bjtds.
**Version:2.0
**Author: dz
**Date:2021.2.1
**Description:
**Others:                         "Meijidenki_Client"
							  ____________________________
			   connectDevice->|REQ          |        DONE|->Done
			disconnectDevice->|DISCONNECT   |       ERROR|->Error
				  read/write->|MB_MODE      |      STATUS|->status
					 ip,port->|CONNECT      |            |
					  value ->|DATA         |            |
							  ----------------------------
*************************************************************************************************************************************************/
#pragma once
#include "EquipmentComm.h"
#include "lim.h"
#include "Variable.h"
#include "Public_Header.h"

/*************************************************************************************************************************************************
**Function:线阵雷达IO映射
*************************************************************************************************************************************************/
#define IO_OUTNUM 3
#define IO_INNUM 2
#define PI 3.1415926
/*************************************************************************************************************************************************
**Function:注册结构体
*************************************************************************************************************************************************/
struct s_Meijidenki_DB {
	int cid = 0;
	LIM_HEAD *lim = NULL;
};
Q_DECLARE_METATYPE(s_Meijidenki_DB);
/*************************************************************************************************************************************************
**Function:线阵雷达回调函数
*************************************************************************************************************************************************/
class c_Meijidenki_CallBack : public QObject
{
	Q_OBJECT
public:
	explicit c_Meijidenki_CallBack(QObject * parent = nullptr);
	virtual ~c_Meijidenki_CallBack();
	//操作对象
	static c_Meijidenki_CallBack *g_Meijidenki_CallBack;
	//Debug信息回调函数，只需要将_state_code传递即可
	static void CALLBACK EqCommStateCallBack(int _cid, unsigned int _state_code, char* _ip, int _port, int _paddr);
	//输出事件回调函数，注意请不要在回调函数中进行耗时较长的操作
	static void CALLBACK EqCommDataCallBack(int _cid, unsigned int _lim_code, void* _lim, int _lim_len, int _paddr);
	public slots:
	void Meijidenki_20_Read_Ready(qint32 id);
	void Meijidenki_21_Read_Ready(qint32 id);
signals:
	//读准备完成
	void ReadReady(QVariant _db);
	//设备状态
	void State_Changed(qint32 _cid, quint32 _state_code);
private:
	bool m_Meijidenki_20_Read_Ready = true;
	bool m_Meijidenki_21_Read_Ready = true;
	qint32 m_Meijidenki_20_id;
	qint32 m_Meijidenki_21_id;
	s_Meijidenki_DB m_Meijidenki_DB;
};

class c_Meijidenki_Client : public QObject
{
	Q_OBJECT

public:
	explicit c_Meijidenki_Client(QObject * parent = nullptr);
	virtual ~c_Meijidenki_Client();
	bool m_State = false;

	public slots:
	void Connect_Device(int id, QString ip, int port);//连接到服务器
	void Disconnect_Device();//断开连接
	void Write(quint32 value); //写Json数据

signals:
	void Connect_Done();//连接到服务器完成
	void Disconnect_Done();//断开连接完成
	void Connect_Error();//连接到服务器失败
	void Disconnect_Error();//断开连接失败
	void Read_Json_Done(QJsonObject value);//读Json完成
	void Write_Done(); //写完成
	void Read_Json_Error();//读错误
	void Write_Error(); //写错误
	void Status(QString state);//通讯状态

private:
	int m_device_id = 99;
	QJsonObject m_Value;

	private slots :
	void Read_Json(QVariant db);//读Json数据
	void State_Changed(qint32 _cid, quint32 _state_code);//状态改变
	QJsonObject Meijidenki_LDBCONFIG_Decoding(LIM_HEAD *lim);
	QJsonObject Meijidenki_LMD_Decoding(LIM_HEAD *lim);
	QJsonObject Meijidenki_LMD_RSSI_Decoding(LIM_HEAD *lim);
	QJsonObject Meijidenki_FMSIG_Decoding(LIM_HEAD *lim);
	QJsonObject Meijidenki_IOSTATUS_Decoding(LIM_HEAD *lim);
	QString Meijidenki_ALARM_Decoding(LIM_HEAD *lim);
	QString Meijidenki_DISALARM_Decoding(LIM_HEAD *lim);
};