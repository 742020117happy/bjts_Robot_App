#pragma execution_character_set("utf-8")
#include "Jaka_Remote.h"

/*************************************************************************************************************************************************
**Function:构造函数(控制指令)
*************************************************************************************************************************************************/
c_Jaka_Remote::c_Jaka_Remote(QObject *parent) : QObject(parent)
{
	power_on.insert("cmdName", "power_on");
	power_off.insert("cmdName", "power_off");
	enable_robot.insert("cmdName", "enable_robot");
	disable_robot.insert("cmdName", "disable_robot");
	load_program.insert("cmdName", "load_program");
	play_program.insert("cmdName", "play_program");
	pause_program.insert("cmdName", "pause_program");
	resume_program.insert("cmdName", "resume_program");
	stop_program.insert("cmdName", "stop_program");
}
/*************************************************************************************************************************************************
**Function:析构
*************************************************************************************************************************************************/
c_Jaka_Remote::~c_Jaka_Remote()
{
	//线程中断
	m_Jaka_Remote_Thread->requestInterruption();
	//线程退出
	m_Jaka_Remote_Thread->quit();
	//线程等待
	m_Jaka_Remote_Thread->wait();
}
/*************************************************************************************************************************************************
**Function:初始化接口
*************************************************************************************************************************************************/
void c_Jaka_Remote::Init()
{
	//实例化
	m_Jaka_Remote = new c_Jaka_Client;
	m_Jaka_Remote_Thread = new QThread;
	m_Jaka_Remote->moveToThread(m_Jaka_Remote_Thread);
	//初始化数据交换层
	QObject::connect(m_Jaka_Remote_Thread, &QThread::started, m_Jaka_Remote, &c_Jaka_Client::Init);
	QObject::connect(m_Jaka_Remote_Thread, &QThread::finished, m_Jaka_Remote, &c_Jaka_Client::deleteLater);
	//连接设备
	QObject::connect(this, &c_Jaka_Remote::Connect_Device, m_Jaka_Remote, &c_Jaka_Client::Connect_Device);
	QObject::connect(this, &c_Jaka_Remote::Disconnect_Device, m_Jaka_Remote, &c_Jaka_Client::Disconnect_Device);
	//设备状态改变
	QObject::connect(m_Jaka_Remote, &c_Jaka_Client::Connect_Done, this, &c_Jaka_Remote::Set_Working);
	QObject::connect(m_Jaka_Remote, &c_Jaka_Client::Disconnect_Done, this, &c_Jaka_Remote::Set_Default);
	//键盘按键改变
	QObject::connect(this, &c_Jaka_Remote::Set_Working, this, [=] {emit setEnabled(true); });
	QObject::connect(this, &c_Jaka_Remote::Set_Default, this, [=] {emit setEnabled(false); });
	//向状态服务写入状态
	QObject::connect(m_Jaka_Remote, &c_Jaka_Client::Connect_Done, this, &c_Jaka_Remote::Connect_Done);
	QObject::connect(m_Jaka_Remote, &c_Jaka_Client::Disconnect_Done, this, &c_Jaka_Remote::Disconnect_Done);
	//写指令与校验
	QObject::connect(this, &c_Jaka_Remote::Write_Json, m_Jaka_Remote, &c_Jaka_Client::Write_Json);
	QObject::connect(m_Jaka_Remote, &c_Jaka_Client::Read_Json_Done, this, &c_Jaka_Remote::Read_Json_Done);
	//启动线程
	m_Jaka_Remote_Thread->start();
	emit setEnabled(false);
}
/*************************************************************************************************************************************************
**Function:连接虚函数
*************************************************************************************************************************************************/
void c_Jaka_Remote::Connect()
{
}
void c_Jaka_Remote::Connect_Loop()
{
}
/*************************************************************************************************************************************************
**Function:子类公有遥控方法接口
*************************************************************************************************************************************************/
void c_Jaka_Remote::Jaka_power_on()
{
	Write(power_on);
}
void c_Jaka_Remote::Jaka_power_off()
{
	Write(power_off);
}
void c_Jaka_Remote::Jaka_enable_robot()
{
	Write(enable_robot);
}
void c_Jaka_Remote::Jaka_disable_robot()
{
	Write(disable_robot);
}
void c_Jaka_Remote::Jaka_play_program()
{
	Jaka_load_program();
	Jaka_origin_move();
	Write(play_program);
}
void c_Jaka_Remote::Jaka_pause_program()
{
	Write(pause_program);
}
void c_Jaka_Remote::Jaka_resume_program()
{
	Write(resume_program);
}
void c_Jaka_Remote::Jaka_stop_program()
{
	Write(stop_program);
}
void c_Jaka_Remote::Jaka_program_name(QString name)
{
	load_program.insert("programName", name + ".jks");
}
/*************************************************************************************************************************************************
**Function:父类私有的写数据方法
*************************************************************************************************************************************************/
void c_Jaka_Remote::Write(QJsonObject json)
{
	if (!m_Jaka_Remote_State.value("Connected").toBool()){ 
		return;
	}
	QString cmdName = json.value("cmdName").toString();
	if (m_Writing) {
		emit Write_Json_Error(cmdName);
		return;
	}
	m_Writing = true;
	QEventLoop *Write_Loop = new QEventLoop;
	QObject::connect(this, &c_Jaka_Remote::Write_Json_Done, Write_Loop, &QEventLoop::quit);
	QObject::connect(this, &c_Jaka_Remote::Set_Default, Write_Loop, &QEventLoop::quit);
	emit Write_Json(json);
	Write_Loop->exec();
	emit Status(c_Variable::g_Current_Time + m_Name + cmdName + "指令校验完成");
	m_Writing = false;
}
/*************************************************************************************************************************************************
**Function:父类私有的写反馈校验
** errorCode               errorMsg                           Description
**    0                   返回消息为空                       指令执行成功
**    1           Exception:function call failed             程序发生异常
**    2                 返回具体的错误消息                     错误信息
*************************************************************************************************************************************************/
void c_Jaka_Remote::Read_Json_Done(QJsonObject json)
{
	QString cmdName = json.value("cmdName").toString();
	QString errorCode = json.value("errorCode").toString();
	QString errorMsg = json.value("errorMsg").toString();
	emit Status(c_Variable::g_Current_Time + m_Name + cmdName + "指令校验开始");
	if (errorCode == "0") {
		emit Status(c_Variable::g_Current_Time + m_Name + cmdName + "指令执行成功");
		emit Write_Json_Done(cmdName);
	}
	if (errorCode == "1") {
		emit Status(c_Variable::g_Current_Time + m_Name + cmdName + "发送失败，程序发生异常,请断开设备重新连接");
		emit Write_Json_Error(cmdName);
		emit setEnabled(false);
	}
	if (errorCode == "2") {
		emit Status(c_Variable::g_Current_Time + m_Name + cmdName + "发送失败，错误信息：" + errorMsg + ",请断开设备重新连接");
		emit Write_Json_Error(cmdName);
		emit setEnabled(false);
	}
}
/*************************************************************************************************************************************************
**Function:子类私有遥控方法接口
*************************************************************************************************************************************************/
void c_Jaka_Remote::Jaka_joint_move(double J1, double J2, double J3, double J4, double J5, double J6)
{
	joint_move.insert("cmdName", "joint_move");
	QJsonArray jointPosition;
	jointPosition.append(J1);
	jointPosition.append(J2);
	jointPosition.append(J3);
	jointPosition.append(J4);
	jointPosition.append(J5);
	jointPosition.append(J6);
	joint_move.insert("jointPosition", jointPosition);
	joint_move.insert("speed", 30);
	joint_move.insert("accel", 30);
	joint_move.insert("relFlag", 0);
	Write(joint_move);
}
void c_Jaka_Remote::Jaka_origin_move()
{
	QEventLoop *origin_move_Loop = new QEventLoop;
	Jaka_joint_move(-90, 40, 150, 120, 0, 5);
	QObject::connect(this, &c_Jaka_Remote::Origin_Moved, origin_move_Loop, &QEventLoop::quit);
	emit Origin_Monitor();
	origin_move_Loop->exec();
}
void c_Jaka_Remote::Jaka_load_program()
{
	Write(load_program);
}
/*************************************************************************************************************************************************
**Function:父类私有连接状态写入
*************************************************************************************************************************************************/
void c_Jaka_Remote::Connect_Done()
{
	m_Jaka_Remote_State.insert("Connected", true);
	emit Write_Jaka_Remote_State(m_Jaka_Remote_State);
}
void c_Jaka_Remote::Disconnect_Done()
{
	m_Jaka_Remote_State.insert("Connected", false);
	emit Write_Jaka_Remote_State(m_Jaka_Remote_State);
}