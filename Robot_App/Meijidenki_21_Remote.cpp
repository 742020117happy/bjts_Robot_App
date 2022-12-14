#pragma execution_character_set("utf-8")
#include "Meijidenki_21_Remote.h"
 
/*************************************************************************************************************************************************
**Function:    构造函数
**Others:      构造函数初始化在主线程实例化中完成，故在子线程中运行时，成员函数的初始化不能在构造函数中完成
*************************************************************************************************************************************************/
c_Meijidenki_21_Remote::c_Meijidenki_21_Remote(c_Meijidenki_Remote *parent) : c_Meijidenki_Remote(parent)
{

}
/*************************************************************************************************************************************************
**Function:    析构函数
*************************************************************************************************************************************************/
c_Meijidenki_21_Remote::~c_Meijidenki_21_Remote()
{
}
/*************************************************************************************************************************************************
**Function:    初始化函数
*************************************************************************************************************************************************/
void c_Meijidenki_21_Remote::Init()
{
	//循环连接
	c_Meijidenki_Remote::Init();
	//提示信息
	QObject::connect(m_Meijidenki_Remote, &c_Meijidenki_Client::Status, [=](QString state) {emit Status(c_Variable::g_Current_Time + "->右线阵激光雷达：" + state); });
	QObject::connect(m_Meijidenki_Remote, &c_Meijidenki_Client::Connect_Loop, this, &c_Meijidenki_21_Remote::Connect_Loop);
	QObject::connect(this, &c_Meijidenki_21_Remote::Read_Ready, c_Meijidenki_CallBack::g_Meijidenki_CallBack, &c_Meijidenki_CallBack::Meijidenki_21_Read_Ready);
}
/*************************************************************************************************************************************************
**Function:    操作接口
*************************************************************************************************************************************************/
void c_Meijidenki_21_Remote::Connect()
{
	m_device_id = c_Variable::g_Communicate_DB.value("Meijidenki_21_Id").toInt();
	QString ip = c_Variable::g_Communicate_DB.value("Meijidenki_21_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("Meijidenki_21_Port").toInt();
	c_Variable::msleep(1000);
	qDebug() << "c_Meijidenki_21_Remote::Connect";
	emit Connect_Device(m_device_id, ip, port);
}
/*************************************************************************************************************************************************
**Function:循环连接
*************************************************************************************************************************************************/
void c_Meijidenki_21_Remote::Connect_Loop()
{
	QTimer::singleShot(6000, this, &c_Meijidenki_21_Remote::Connect);
}
/*************************************************************************************************************************************************
**Function:   同步接口
*************************************************************************************************************************************************/
void c_Meijidenki_21_Remote::Meijidenki_21_Read_Ready()
{
	emit Read_Ready(m_device_id);
}