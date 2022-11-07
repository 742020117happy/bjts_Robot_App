#pragma execution_character_set("utf-8")
#include "Scan_Remote.h"

/*************************************************************************************************************************************************
**Function:构造函数(状态监视)
*************************************************************************************************************************************************/
c_Scan_Remote::c_Scan_Remote(QObject *parent) : QObject(parent)
{

}
/*************************************************************************************************************************************************
**Function:析构
*************************************************************************************************************************************************/
c_Scan_Remote::~c_Scan_Remote()
{
	//线程中断
	m_Scan_Remote_Thread->requestInterruption();
	//线程退出
	m_Scan_Remote_Thread->quit();
	//线程等待
	m_Scan_Remote_Thread->wait();
}
/*************************************************************************************************************************************************
**Function:初始化接口
*************************************************************************************************************************************************/
void c_Scan_Remote::Init()
{
	//实例化
	m_Scan_Remote = new c_Scan_Client;
	m_Scan_Remote_Thread = new QThread;
	m_Scan_Remote->moveToThread(m_Scan_Remote_Thread);
	//初始化数据交换层
	QObject::connect(m_Scan_Remote_Thread, &QThread::started, m_Scan_Remote, &c_Scan_Client::Init);
	QObject::connect(m_Scan_Remote_Thread, &QThread::finished, m_Scan_Remote, &c_Scan_Client::deleteLater);
	//连接设备
	QObject::connect(this, &c_Scan_Remote::Connect_Device, m_Scan_Remote, &c_Scan_Client::Connect_Device);
	QObject::connect(this, &c_Scan_Remote::Disconnect_Device, m_Scan_Remote, &c_Scan_Client::Disconnect_Device);
	//写数据
	QObject::connect(this, &c_Scan_Remote::Write_String, m_Scan_Remote, &c_Scan_Client::Write_String);
	//设备状态改变
	QObject::connect(m_Scan_Remote, &c_Scan_Client::Connect_Done, this, &c_Scan_Remote::Set_Working);
	QObject::connect(m_Scan_Remote, &c_Scan_Client::Disconnect_Done, this, &c_Scan_Remote::Set_Default);
	//键盘按键改变
	QObject::connect(this, &c_Scan_Remote::Set_Working, this, [=] {emit setEnabled(true); });
	QObject::connect(this, &c_Scan_Remote::Set_Default, this, [=] {emit setEnabled(false); });
	//读到消息
	QObject::connect(m_Scan_Remote, &c_Scan_Client::Read_String_Done, this, &c_Scan_Remote::Read_String_Done);
	//向状态服务写入状态
	QObject::connect(m_Scan_Remote, &c_Scan_Client::Connect_Done, this, &c_Scan_Remote::Connect_Done);
	QObject::connect(m_Scan_Remote, &c_Scan_Client::Disconnect_Done, this, &c_Scan_Remote::Disconnect_Done);
	//启动线程
	m_Scan_Remote_Thread->start();
	emit setEnabled(false);
}
void c_Scan_Remote::Start_Cmd(QJsonObject object)
{
	QJsonObject value = object.value("DB").toObject();
	m_Track_Rank = value.value("Track_Rank").toString();//股道号
	m_Begin_Time = value.value("Begin_Time").toString();//开始时间
	m_Car_Type = value.value("Car_Type").toString();//任务车型
	m_Car_Num = value.value("Car_Num").toString();//任务车号
	m_Carbox_Num = value.value("Carbox_Num").toString();//任务车厢号
	m_Bogie_Num = value.value("Bogie_Num").toString();//任务转向架号
	m_Axis_Num = value.value("Axis_Num").toString();//任务车轴号
	m_Work_Num = m_Begin_Time + m_Car_Type + m_Car_Num;//任务编号
}
/*************************************************************************************************************************************************
**Function:虚函数操作接口
*************************************************************************************************************************************************/
void c_Scan_Remote::Connect()
{

}
void c_Scan_Remote::Connect_Loop()
{

}
void c_Scan_Remote::Start()
{

}
void c_Scan_Remote::Collection()
{

}
void c_Scan_Remote::Stop()
{

}
void c_Scan_Remote::Tran(QString value)
{

}
/*************************************************************************************************************************************************
**Function:私有连接状态写入
*************************************************************************************************************************************************/
void c_Scan_Remote::Connect_Done()
{
	m_Scan_Remote_State.insert("Connected", true);
	emit Write_Scan_Remote_State(m_Scan_Remote_State);
}
void c_Scan_Remote::Disconnect_Done()
{
	m_Scan_Remote_State.insert("Connected", false);
	emit Write_Scan_Remote_State(m_Scan_Remote_State);
}