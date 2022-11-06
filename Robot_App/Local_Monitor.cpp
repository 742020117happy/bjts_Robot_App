#pragma execution_character_set("utf-8")
#include "Local_Monitor.h"
 
/*************************************************************************************************************************************************
**Function:    构造函数
**Others:      构造函数初始化在主线程实例化中完成，故在子线程中运行时，成员函数的初始化不能在构造函数中完成
*************************************************************************************************************************************************/
c_Local_Monitor::c_Local_Monitor(c_Server_Remote *parent) : c_Server_Remote(parent)
{

}
/*************************************************************************************************************************************************
**Function:    析构函数
*************************************************************************************************************************************************/
c_Local_Monitor::~c_Local_Monitor()
{
	
}
/*************************************************************************************************************************************************
**Function:初始化接口
*************************************************************************************************************************************************/
void c_Local_Monitor::Init()
{
	//保持监视状态
	c_Server_Remote::Init();
	QObject::connect(m_Robot_Server, &c_Robot_Server::Connect_Loop, this, &c_Local_Monitor::Connect_Loop);
	m_Ip = c_Variable::g_Communicate_DB.value("Local_Ip").toString();
	m_Port = c_Variable::g_Communicate_DB.value("Local_Monitor_Port").toInt();
	c_Server_Remote::m_Robot_Server->Connect_Device(m_Ip, m_Port);
}
/*************************************************************************************************************************************************
**Function:循环连接
*************************************************************************************************************************************************/
void c_Local_Monitor::Connect_Loop()
{
	c_Variable::msleep(6000);//等待6秒
	qDebug() << "重新监听监视端口";
	m_Ip = c_Variable::g_Communicate_DB.value("Local_Ip").toString();
	m_Port = c_Variable::g_Communicate_DB.value("Local_Monitor_Port").toInt();
	c_Server_Remote::m_Robot_Server->Connect_Device(m_Ip, m_Port);
}