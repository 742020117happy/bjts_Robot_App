﻿#pragma execution_character_set("utf-8")
#include "Local_Remote.h"

/*************************************************************************************************************************************************
**Function:    构造函数
**Others:      构造函数初始化在主线程实例化中完成，故在子线程中运行时，成员函数的初始化不能在构造函数中完成
*************************************************************************************************************************************************/
c_Local_Remote::c_Local_Remote(c_Server_Remote *parent) : c_Server_Remote(parent)
{

}
/*************************************************************************************************************************************************
**Function:    析构函数
*************************************************************************************************************************************************/
c_Local_Remote::~c_Local_Remote()
{

}
/*************************************************************************************************************************************************
**Function:初始化接口
*************************************************************************************************************************************************/
void c_Local_Remote::Init()
{
	//保持监视状态
	c_Server_Remote::Init();
	//提示信息
	QObject::connect(m_Robot_Server, &c_Robot_Server::Status, this, [=](int state) {emit Status(c_Variable::g_Current_Time + "->控制服务：" + c_Variable::TCP_Status(state)); });
	QObject::connect(m_Robot_Server, &c_Robot_Server::Connect_Loop, this, &c_Local_Remote::Connect_Loop);
	c_Local_Remote::Connect();
}
/*************************************************************************************************************************************************
**Function:连接系统
*************************************************************************************************************************************************/
void c_Local_Remote::Connect()
{
	m_Ip = c_Variable::g_Communicate_DB.value("Local_Ip").toString();
	m_Port = c_Variable::g_Communicate_DB.value("Local_Remote_Port").toInt();
	c_Server_Remote::m_Robot_Server->Connect_Device(m_Ip, m_Port);
}
/*************************************************************************************************************************************************
**Function:循环连接
*************************************************************************************************************************************************/
void c_Local_Remote::Connect_Loop()
{
	emit Status(c_Variable::g_Current_Time + "->控制服务：建立监听中");
	QTimer::singleShot(6000, this, &c_Local_Remote::Connect);
}
