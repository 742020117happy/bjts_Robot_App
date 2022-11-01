﻿#pragma execution_character_set("utf-8")
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
void c_Local_Monitor::Run()
{
	//保持监视状态
	m_Ip = c_Variable::g_Communicate_DB.value("Local_Ip").toString();
	m_Port = c_Variable::g_Communicate_DB.value("Local_Monitor_Port").toInt();
	Init();
}