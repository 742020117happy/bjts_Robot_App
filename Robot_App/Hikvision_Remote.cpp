#include "Hikvision_Remote.h"
/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_Hikvision_Remote::c_Hikvision_Remote(QObject * parent) : QObject(parent)
{	
}
/*************************************************************************************************************************************************
**Function:析构
*************************************************************************************************************************************************/
c_Hikvision_Remote::~c_Hikvision_Remote() 
{
	m_Hikvision->deleteLater();
}
/*************************************************************************************************************************************************
**Function:初始化
*************************************************************************************************************************************************/
void c_Hikvision_Remote::Init()
{
	m_Hikvision = new c_Hikvision_Client;
	QObject::connect(this, &c_Hikvision_Remote::Connect_Device, m_Hikvision, &c_Hikvision_Client::Connect_Device);
	QObject::connect(this, &c_Hikvision_Remote::Disconnect_Device, m_Hikvision, &c_Hikvision_Client::Disconnect_Device);
	QObject::connect(m_Hikvision, &c_Hikvision_Client::Connect_Done,this, &c_Hikvision_Remote::Connect_Done);
	QObject::connect(m_Hikvision, &c_Hikvision_Client::Disconnect_Done,this, &c_Hikvision_Remote::Disconnect_Done);
}
/*************************************************************************************************************************************************
**Function:虚函数
*************************************************************************************************************************************************/
void c_Hikvision_Remote::Connect()
{
	
}
void c_Hikvision_Remote::Connect_Loop()
{

}