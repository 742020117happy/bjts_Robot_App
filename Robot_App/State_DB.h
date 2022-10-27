/*************************************************************************************************************************************************
**Copyright(C), 2020-2025, bjtds.
**Version:2.0
**Author: dz
**Date:2021.2.1
**Description:
**Others:                                                           "State_DB"
							       ____________________	
								->|Done
			                  DB->|DATA      |         |
						  device->|Thread    |     scan|->
							       --------------------
*************************************************************************************************************************************************/
#pragma once
#include "Variable.h"
#include "Public_Header.h"

class c_State_DB : public QObject
{
	Q_OBJECT
public:
	explicit c_State_DB(QObject *parent = nullptr);
	virtual ~c_State_DB();

	//线程操作接口
	public slots:
	void Init();
	void Write_System_Time();
	void Write_RGV_State(QJsonObject db);
	void Write_Jaka_120_Remote_State(QJsonObject db);
	void Write_Jaka_121_Remote_State(QJsonObject db);
	void Write_Jaka_120_Monitor_State(QJsonObject db);
	void Write_Jaka_121_Monitor_State(QJsonObject db);
	void Write_Hypersen_30_State(QJsonObject db);
	void Write_Hypersen_31_State(QJsonObject db);
	void Write_Meijidenki_20_State(QJsonObject db);
	void Write_Meijidenki_21_State(QJsonObject db);
	void Write_Hikvision_State(QJsonObject db);
	void Write_Fast_Scan_State(QJsonObject db);
	void Write_Prec_Scan_120_State(QJsonObject db);
	void Write_Prec_Scan_121_State(QJsonObject db);
	void Write_Local_Remote_State(QJsonObject db);
	void Write_Local_Monitor_State(QJsonObject db);
	void Write_App_Control_State(QJsonObject db);
	void Write_Work_Remote_State(QJsonObject db);

signals:
	void System_Scan(QJsonObject db);
	void RGV_Remote_Read_Ready();//同步
	void Jaka_120_Monitor_Read_Ready();//同步
	void Jaka_121_Monitor_Read_Ready();//同步
	void Hypersen_30_Read_Ready();//同步
	void Hypersen_31_Read_Ready();//同步
	void Meijidenki_20_Read_Ready();//同步
	void Meijidenki_21_Read_Ready();//同步

private:
	QJsonObject m_State_DB;
	QString m_Current_Time = "";//系统时间
};