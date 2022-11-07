﻿#pragma execution_character_set("utf-8")
#include "Jaka_121_Monitor.h"
/*************************************************************************************************************************************************
**Function:    构造函数
**Others:      构造函数初始化在主线程实例化中完成，故在子线程中运行时，成员函数的初始化不能在构造函数中完成
*************************************************************************************************************************************************/
c_Jaka_121_Monitor::c_Jaka_121_Monitor(c_Jaka_Monitor *parent) : c_Jaka_Monitor(parent)
{
	
}
/*************************************************************************************************************************************************
**Function:    析构函数
*************************************************************************************************************************************************/
c_Jaka_121_Monitor::~c_Jaka_121_Monitor()
{
}
/*************************************************************************************************************************************************
**Function:    初始化函数
*************************************************************************************************************************************************/
void c_Jaka_121_Monitor::Init()
{
	//循环连接
	c_Jaka_Monitor::Init();
	//提示信息
	QObject::connect(m_Jaka_Monitor, &c_Jaka_Client::Status, this, [=](int value) {emit Status(c_Variable::g_Current_Time + "->右机械臂监视：" + c_Variable::TCP_Status(value)); });
	QObject::connect(m_Jaka_Monitor, &c_Jaka_Client::Connect_Loop, this, &c_Jaka_121_Monitor::Connect_Loop);
}
/*************************************************************************************************************************************************
**Function:    操作接口
*************************************************************************************************************************************************/
void c_Jaka_121_Monitor::Connect()
{
	if (m_Jaka_Monitor_State.value("Connected").toBool()) { return; }
	m_Ip = c_Variable::g_Communicate_DB.value("Jaka_121_Ip").toString();
	m_Port = c_Variable::g_Communicate_DB.value("Jaka_Monitor_Port").toInt();
	emit Connect_Device(m_Ip, m_Port);
}
/*************************************************************************************************************************************************
**Function:    循环连接
*************************************************************************************************************************************************/
void c_Jaka_121_Monitor::Connect_Loop()
{
	QTimer::singleShot(6000, this, &c_Jaka_121_Monitor::Connect);
}
