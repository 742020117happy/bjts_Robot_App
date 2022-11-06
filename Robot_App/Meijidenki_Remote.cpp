#pragma execution_character_set("utf-8")
#include "Meijidenki_Remote.h"
/*************************************************************************************************************************************************
**Function:构造函数(状态监视)
*************************************************************************************************************************************************/
c_Meijidenki_Remote::c_Meijidenki_Remote(QObject *parent) : QObject(parent)
{

}
/*************************************************************************************************************************************************
**Function:析构
*************************************************************************************************************************************************/
c_Meijidenki_Remote::~c_Meijidenki_Remote()
{
	//线程中断
	m_Meijidenki_Remote_Thread->requestInterruption();
	//线程退出
	m_Meijidenki_Remote_Thread->quit();
	//线程等待
	m_Meijidenki_Remote_Thread->wait();
}
/*************************************************************************************************************************************************
**Function:初始化接口
*************************************************************************************************************************************************/
void c_Meijidenki_Remote::Init()
{
	//实例化
	m_Meijidenki_Remote = new c_Meijidenki_Client;
	m_Meijidenki_Remote_Thread = new QThread;
	m_Meijidenki_Remote->moveToThread(m_Meijidenki_Remote_Thread);
	//初始化数据交换层
	QObject::connect(m_Meijidenki_Remote_Thread, &QThread::started, m_Meijidenki_Remote, &c_Meijidenki_Client::Init);
	QObject::connect(m_Meijidenki_Remote_Thread, &QThread::finished, m_Meijidenki_Remote, &c_Meijidenki_Client::deleteLater);
	//连接设备
	QObject::connect(this, &c_Meijidenki_Remote::Connect_Device, m_Meijidenki_Remote, &c_Meijidenki_Client::Connect_Device);
	QObject::connect(this, &c_Meijidenki_Remote::Disconnect_Device, m_Meijidenki_Remote, &c_Meijidenki_Client::Disconnect_Device);
	//循环连接
	QObject::connect(m_Meijidenki_Remote, &c_Meijidenki_Client::Connect_Loop, this, &c_Meijidenki_Remote::Connect_Loop);
	//写数据
	QObject::connect(this, &c_Meijidenki_Remote::Write, m_Meijidenki_Remote, &c_Meijidenki_Client::Write);
	//设备状态改变
	QObject::connect(m_Meijidenki_Remote, &c_Meijidenki_Client::Connect_Done, this, &c_Meijidenki_Remote::Set_Working);
	QObject::connect(m_Meijidenki_Remote, &c_Meijidenki_Client::Disconnect_Done, this, &c_Meijidenki_Remote::Set_Default);
	//键盘按键改变
	QObject::connect(this, &c_Meijidenki_Remote::Set_Working, this, [=] {emit setEnabled(true); });
	QObject::connect(this, &c_Meijidenki_Remote::Set_Default, this, [=] {emit setEnabled(false); });
	//读到消息
	QObject::connect(m_Meijidenki_Remote, &c_Meijidenki_Client::Read_Json_Done, this, &c_Meijidenki_Remote::Read_Json_Done);
	//向状态服务写入状态
	QObject::connect(m_Meijidenki_Remote, &c_Meijidenki_Client::Connect_Done, this, &c_Meijidenki_Remote::Connect_Done);
	QObject::connect(m_Meijidenki_Remote, &c_Meijidenki_Client::Disconnect_Done, this, &c_Meijidenki_Remote::Disconnect_Done);
	//提示信息
	QObject::connect(m_Meijidenki_Remote, &c_Meijidenki_Client::Status, this, &c_Meijidenki_Remote::Status);
	m_Meijidenki_Remote_Thread->start();
	emit setEnabled(false);
}
/*************************************************************************************************************************************************
**Function:虚函数
*************************************************************************************************************************************************/
void c_Meijidenki_Remote::Connect()
{
}
/*************************************************************************************************************************************************
**Function:操作接口
*************************************************************************************************************************************************/
void c_Meijidenki_Remote::START_LMD()
{
	emit Write(LIM_CODE_START_LMD);
}
void c_Meijidenki_Remote::STOP_LMD()
{
	emit Write(LIM_CODE_STOP_LMD);
}
/*************************************************************************************************************************************************
**Function:父类私有的读状态函数，将状态量提取到公有成员变量
*************************************************************************************************************************************************/
void c_Meijidenki_Remote::Read_Json_Done(QJsonObject json)
{
	m_Meijidenki_Remote_State.insert("DB", json);
	emit Write_Meijidenki_Remote_State(m_Meijidenki_Remote_State);
}
/*************************************************************************************************************************************************
**Function:父类私有连接状态写入
*************************************************************************************************************************************************/
void c_Meijidenki_Remote::Connect_Done()
{
	m_Meijidenki_Remote_State.insert("Connected", true);
	emit Write_Meijidenki_Remote_State(m_Meijidenki_Remote_State);
}
void c_Meijidenki_Remote::Disconnect_Done()
{
	m_Meijidenki_Remote_State.insert("Connected", false);
	emit Write_Meijidenki_Remote_State(m_Meijidenki_Remote_State);
}
/*************************************************************************************************************************************************
**Function:循环连接
*************************************************************************************************************************************************/
void c_Meijidenki_Remote::Connect_Loop(int id, QString ip, int port)
{
	QThread::sleep(6);//等待6秒
	emit Connect_Device(id, ip, port);
}
