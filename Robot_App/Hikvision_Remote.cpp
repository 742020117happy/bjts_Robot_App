#pragma execution_character_set("utf-8")
#include "Hikvision_Remote.h"
/*************************************************************************************************************************************************
**Function:    构造函数
**Others:      构造函数初始化在主线程实例化中完成，故在子线程中运行时，成员函数的初始化不能在构造函数中完成
*************************************************************************************************************************************************/
c_Hikvision_Remote::c_Hikvision_Remote(c_Scan_Remote *parent) : c_Scan_Remote(parent)
{

}
/*************************************************************************************************************************************************
**Function:    析构函数
*************************************************************************************************************************************************/
c_Hikvision_Remote::~c_Hikvision_Remote()
{

}
/*************************************************************************************************************************************************
**Function:    操作接口
*************************************************************************************************************************************************/
void c_Hikvision_Remote::Connect()
{
	if (m_Scan_Remote_State.value("Connected").toBool()) { return; }
	QString ip = c_Variable::g_Communicate_DB.value("Local_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("Hikvision_Local_Port").toInt();
	emit Connect_Device(ip, port);
}
/*************************************************************************************************************************************************
**Function:开始采集
*************************************************************************************************************************************************/
void c_Hikvision_Remote::Start()
{
	QString ip = c_Variable::g_Communicate_DB.value("Collector_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("Hikvision_Collector_Port").toInt();
	//START&股道ID&年月日&时分&车型车号&任务编号（年月日时分车型车号）
	QString yyyyMMdd = m_Begin_Time.left(8);//年月日
	QString hhmm = m_Begin_Time.right(4);//时分
	QString Cmd = QString("Start&%1&%2&%3&%4&%5&%6&%7&%8")
		.arg(m_Track_Rank)//股道ID
		.arg(yyyyMMdd)//年月日
		.arg(hhmm)//时分
		.arg(m_Car_Type + m_Car_Num)//车型车号
		.arg(m_Work_Num)//年月日时分车型车号
		.arg(m_Carbox_Num)//车厢号
		.arg(m_Bogie_Num)//转向架号
		.arg(m_Axis_Num);//轮轴号
	emit Write_String(ip, port, Cmd);//发送开始采集指令
}
/*************************************************************************************************************************************************
**Function:停止采集
*************************************************************************************************************************************************/
void c_Hikvision_Remote::Stop()
{
	QString ip = c_Variable::g_Communicate_DB.value("Collector_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("Hikvision_Collector_Port").toInt();
	//PHOTO&车厢号转向架号&原始图像名称
	QString Cmd = QString("Stop");//原始图像名称
	emit Write_String(ip, port, Cmd);//发送采集指令
}