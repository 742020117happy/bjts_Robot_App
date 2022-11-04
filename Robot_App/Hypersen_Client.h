/*************************************************************************************************************************************************
**Copyright(C), 2020-2025, bjtds.
**Version:2.0
**Author: dz
**Date:2021.2.1
**Description:
**Others:                         "Hypersen_Client"
							  ____________________________
			   connectDevice->|REQ          |        DONE|->Done
			disconnectDevice->|DISCONNECT   |       ERROR|->Error
				  read/write->|MB_MODE      |      STATUS|->status
					 ip,port->|CONNECT      |            |
					  value ->|DATA         |            |
							  ----------------------------
*************************************************************************************************************************************************/
#pragma once
#include "HPS3D_IF.h"
#include "HPS3D_DEFINE.h"
#include "Variable.h"
#include "Public_Header.h"
/*************************************************************************************************************************************************
**Function:面阵雷达回调函数
*************************************************************************************************************************************************/
class c_Hypersen_CallBack : public QObject
{
	Q_OBJECT
public:
	explicit c_Hypersen_CallBack(QObject * parent = nullptr);
	virtual ~c_Hypersen_CallBack();
	//操作对象
	static c_Hypersen_CallBack *g_Hypersen_CallBack;
	//Debug信息回调函数，只需要将str打印即可
	static void Hypersen_DebugFunc(char *str, void *contex);
	//输出事件回调函数，注意请不要在回调函数中进行耗时较长的操作
	static void Hypersen_OutputEventFunc(uint8_t out_id, uint8_t event, void *contex);
	public slots:
	void Hypersen_30_Read_Ready(quint8 id);
	void Hypersen_31_Read_Ready(quint8 id);
signals:
	void ReadReady(quint8 out_id);//读准备完成
	void State_Changed(QString str);//设备状态改变
private:
	bool m_Hypersen_30_Read_Ready = true;
	bool m_Hypersen_31_Read_Ready = true;
	quint8 m_Hypersen_30_id;
	quint8 m_Hypersen_31_id;
};
/*************************************************************************************************************************************************
**Function:面阵雷达网络通讯
*************************************************************************************************************************************************/
class c_Hypersen_Client : public QObject
{
	Q_OBJECT
public:
	explicit c_Hypersen_Client(QObject * parent = nullptr);
	virtual ~c_Hypersen_Client();
	bool m_State = false;
	public slots:
	void Connect_Device(QString ip, int port);//连接到服务器
	void Disconnect_Device();//断开连接
	void Write(quint8 value); //写Json数据
signals:
	void Connect_Done(quint8 id);//连接到服务器完成
	void Disconnect_Done();//断开连接完成
	void Connect_Error();//连接到服务器失败
	void Disconnect_Error();//断开连接失败
	void Read_Json_Done(QJsonObject value);//读Json完成
	void Write_Done(); //写完成
	void Read_Json_Error();//读错误
	void Write_Error(); //写错误
	void Status(QString state);//通讯状态
private:
	QString m_ip = nullptr;
	quint8 m_device_id = 99;
	uint8_t m_sdk_version[6] = { 0 };
	uint8_t m_device_version[6] = { 0 };
	char m_SN[64] = { 0 };
	quint8 m_group_id = 0;
	quint8 m_roi_number = 0;
	quint8 m_roi_id[8] = { 0 };
	quint8 m_roi_current_id = 0;
	quint16 m_roi_aver_distance = 0;
	quint16 m_roi_max_distance = 0;
	quint16 m_roi_min_distance = 0;
	quint16 m_roi_valid_aver_amp = 0;
	quint16 m_roi_all_aver_amp = 0;
	private slots:
	void Read_Json(quint8 id);//读Json数据
	void State_Changed(QString str);//状态改变
};