#pragma execution_character_set("utf-8")
#include "Hypersen_Remote.h"

/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_Hypersen_Remote::c_Hypersen_Remote(QObject *parent) : QObject(parent)
{

}
/*************************************************************************************************************************************************
**Function:析构
*************************************************************************************************************************************************/
c_Hypersen_Remote::~c_Hypersen_Remote()
{
	delete m_Hypersen_Remote;
}
/*************************************************************************************************************************************************
**Function:初始化接口
*************************************************************************************************************************************************/
void c_Hypersen_Remote::Init()
{
	//实例化
	m_Hypersen_Remote = new c_Hypersen_Client;
	//连接设备
	QObject::connect(this, &c_Hypersen_Remote::Connect_Device, m_Hypersen_Remote, &c_Hypersen_Client::Connect_Device);
	QObject::connect(this, &c_Hypersen_Remote::Disconnect_Device, m_Hypersen_Remote, &c_Hypersen_Client::Disconnect_Device);
	//写数据
	QObject::connect(this, &c_Hypersen_Remote::Write, m_Hypersen_Remote, &c_Hypersen_Client::Write);
	//设备状态改变
	QObject::connect(m_Hypersen_Remote, &c_Hypersen_Client::Connect_Done, this, &c_Hypersen_Remote::Set_Working);
	QObject::connect(m_Hypersen_Remote, &c_Hypersen_Client::Disconnect_Done, this, &c_Hypersen_Remote::Set_Default);
	//键盘按键改变
	QObject::connect(this, &c_Hypersen_Remote::Set_Working, this, [=] {emit setEnabled(true); });
	QObject::connect(this, &c_Hypersen_Remote::Set_Default, this, [=] {emit setEnabled(false); });
	//读到消息
	QObject::connect(m_Hypersen_Remote, &c_Hypersen_Client::Read_Json_Done, this, &c_Hypersen_Remote::Read_Json_Done);
	//向状态服务写入状态
	QObject::connect(m_Hypersen_Remote, &c_Hypersen_Client::Connect_Done, this, &c_Hypersen_Remote::Connect_Done);
	QObject::connect(m_Hypersen_Remote, &c_Hypersen_Client::Disconnect_Done, this, &c_Hypersen_Remote::Disconnect_Done);
	//提示信息
	QObject::connect(m_Hypersen_Remote, &c_Hypersen_Client::Status, this, &c_Hypersen_Remote::Status);
	emit setEnabled(false);
}
/*************************************************************************************************************************************************
**Function:虚函数
*************************************************************************************************************************************************/
void c_Hypersen_Remote::Connect()
{
}
/*************************************************************************************************************************************************
**Function:操作接口
*************************************************************************************************************************************************/
void c_Hypersen_Remote::Run_Single_Shot()
{
	emit Write(RUN_SINGLE_SHOT);
}
void c_Hypersen_Remote::Run_Continuous()
{
	emit Write(RUN_CONTINUOUS);
}
void c_Hypersen_Remote::Run_Idle()
{
	emit Write(RUN_IDLE);
}
/*************************************************************************************************************************************************
**Function:父类私有的读状态函数，将状态量提取到公有成员变量
*************************************************************************************************************************************************/
void c_Hypersen_Remote::Read_Json_Done(QJsonObject json)
{
	m_Hypersen_Remote_State.insert("DB", json);
	emit Write_Hypersen_Remote_State(m_Hypersen_Remote_State);
}
/*************************************************************************************************************************************************
**Function:父类私有连接状态写入
*************************************************************************************************************************************************/
void c_Hypersen_Remote::Connect_Done(quint8 id)
{
	m_device_id = id;
	m_Hypersen_Remote_State.insert("Connected", true);
	emit Write_Hypersen_Remote_State(m_Hypersen_Remote_State);
}
void c_Hypersen_Remote::Disconnect_Done()
{
	m_Hypersen_Remote_State.insert("Connected", false);
	emit Write_Hypersen_Remote_State(m_Hypersen_Remote_State);
}

