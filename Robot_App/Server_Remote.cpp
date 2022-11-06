#pragma execution_character_set("utf-8")
#include "Server_Remote.h"

/*************************************************************************************************************************************************
**Function:    构造函数
**Others:      构造函数初始化在主线程实例化中完成，故在子线程中运行时，成员函数的初始化不能在构造函数中完成
*************************************************************************************************************************************************/
c_Server_Remote::c_Server_Remote(QObject *parent) : QObject(parent)
{

}
/*************************************************************************************************************************************************
**Function:    析构函数
*************************************************************************************************************************************************/
c_Server_Remote::~c_Server_Remote()
{
	delete m_Robot_Server;
}
/*************************************************************************************************************************************************
**Function:初始化接口
*************************************************************************************************************************************************/
void c_Server_Remote::Init()
{
	//初始化
	m_Robot_Server = new c_Robot_Server;
	m_Robot_Server->Init();
	//设备状态改变
	QObject::connect(m_Robot_Server, &c_Robot_Server::Connect_Done, this, &c_Server_Remote::Set_Working);
	QObject::connect(m_Robot_Server, &c_Robot_Server::Disconnect_Done, this, &c_Server_Remote::Set_Default);
	//读完成
	QObject::connect(m_Robot_Server, &c_Robot_Server::Read_Json_Done, this, &c_Server_Remote::Read_Json_Done);
	//键盘按键改变
	QObject::connect(this, &c_Server_Remote::Set_Working, this, [=] {emit setEnabled(true); });
	QObject::connect(this, &c_Server_Remote::Set_Default, this, [=] {emit setEnabled(false); });
	//向状态服务写入状态
	QObject::connect(m_Robot_Server, &c_Robot_Server::Connect_Done, this, &c_Server_Remote::Connect_Done);
	QObject::connect(m_Robot_Server, &c_Robot_Server::Disconnect_Done, this, &c_Server_Remote::Disconnect_Done);
	emit setEnabled(false);
}
/*************************************************************************************************************************************************
**Function: 公有接口
*************************************************************************************************************************************************/
void c_Server_Remote::System_Scan(QJsonObject db)
{
	m_State_DB = db;
}
/*************************************************************************************************************************************************
**Function: 虚函数
*************************************************************************************************************************************************/
void c_Server_Remote::Connect_Loop()
{

}
/*************************************************************************************************************************************************
**Function:私有接口
*************************************************************************************************************************************************/
void c_Server_Remote::Connect_Done()
{
	m_Server_State.insert("Connected", true);
	emit Write_Server_State(m_Server_State);
}
void c_Server_Remote::Disconnect_Done()
{
	m_Server_State.insert("Connected", false);
	emit Write_Server_State(m_Server_State);
}
//收到Ai服务器读请求，发送系统状态，解决粘包问题
void c_Server_Remote::Read_Json_Done(QJsonObject json)
{
	m_Cmd_Name = json.value("Cmd_Name").toString();
	m_Value = json.value("Value").toBool();
	if (m_Cmd_Name == "Read_Ready" && m_Value) { m_Robot_Server->Write_Json(m_State_DB); }
	if (m_Cmd_Name == "Work_Start") { emit Start_Cmd(json);  m_Robot_Server->Write_Json(json);}
	if (m_Cmd_Name == "00000" && m_Value) { emit Fuction_0_Set(); }
	if (m_Cmd_Name == "00000" && !m_Value) { emit Fuction_0_Reset(); }
	if (m_Cmd_Name == "00007" && m_Value) { emit Fuction_7();}
	if (m_Cmd_Name == "00008" && m_Value) { emit Fuction_8(); }
	if (m_Cmd_Name == "00009" && m_Value) { emit Fuction_9_Set(); }
	if (m_Cmd_Name == "00009" && !m_Value) { emit Fuction_9_Reset(); }
	if (m_Cmd_Name == "00010" && m_Value) { emit Fuction_10_Set(); }
	if (m_Cmd_Name == "00010" && !m_Value) { emit Fuction_10_Reset(); }
	if (m_Cmd_Name == "00011" && m_Value) { emit Fuction_11(); }
	if (m_Cmd_Name == "00012" && m_Value) { emit Fuction_12(); }
	if (m_Cmd_Name == "00013" && m_Value) { emit Fuction_13(); }
	if (m_Cmd_Name == "00014" && m_Value) { emit Fuction_14(); }
	if (m_Cmd_Name == "00015" && m_Value) { emit Fuction_15(); }
	if (m_Cmd_Name == "00016" && m_Value) { emit Fuction_16(); }
	if (m_Cmd_Name == "00017" && m_Value) { emit Fuction_17(); }
	if (m_Cmd_Name == "00018" && m_Value) { emit Fuction_18(); }
	if (m_Cmd_Name == "00019" && m_Value) { emit Fuction_19_Set(); }
	if (m_Cmd_Name == "00019" && !m_Value) { emit Fuction_19_Reset(); }
	if (m_Cmd_Name == "00020" && m_Value) { emit Fuction_20(); }
	if (m_Cmd_Name == "00021" && m_Value) { emit Fuction_21_Set(); }
	if (m_Cmd_Name == "00021" && !m_Value) { emit Fuction_21_Reset(); }
	if (m_Cmd_Name == "00022" && m_Value) { emit Fuction_22_Set(); }
	if (m_Cmd_Name == "00022" && !m_Value) { emit Fuction_22_Reset(); }
	if (m_Cmd_Name == "00024" && m_Value) { emit Fuction_24(json.value("Position").toInt()); }
	if (m_Cmd_Name == "00025" && m_Value) { emit Fuction_25(json.value("Position").toInt()); }
	if (m_Cmd_Name == "00026" && m_Value) { emit Fuction_26(); }
	if (m_Cmd_Name == "40000" && m_Value) { emit Date_0(json.value("Position").toInt(), json.value("Speed").toInt()); }
	if (m_Cmd_Name == "40004" && m_Value) { emit Date_4(json.value("Position").toInt(), json.value("Speed").toInt()); }
	if (m_Cmd_Name == "Jaka_120_power_on" && m_Value) { emit Jaka_120_power_on(); }
	if (m_Cmd_Name == "Jaka_120_power_off" && m_Value) { emit Jaka_120_power_off(); }
	if (m_Cmd_Name == "Jaka_120_disable_robot" && m_Value) { emit Jaka_120_disable(); }
	if (m_Cmd_Name == "Jaka_120_enable_robot" && m_Value) { emit Jaka_120_enable(); }
	if (m_Cmd_Name == "Jaka_120_play_program" && m_Value) { emit Jaka_120_play_program(json.value("programName").toString()); }
	if (m_Cmd_Name == "Jaka_120_stop_program" && m_Value) { emit Jaka_120_stop_program(); }
	if (m_Cmd_Name == "Jaka_121_power_on" && m_Value) { emit Jaka_121_power_on(); }
	if (m_Cmd_Name == "Jaka_121_power_off" && m_Value) { emit Jaka_121_power_off(); }
	if (m_Cmd_Name == "Jaka_121_disable_robot" && m_Value) { emit Jaka_121_disable(); }
	if (m_Cmd_Name == "Jaka_121_enable_robot" && m_Value) { emit Jaka_121_enable(); }
	if (m_Cmd_Name == "Jaka_121_play_program" && m_Value) { emit Jaka_121_play_program(json.value("programName").toString()); }
	if (m_Cmd_Name == "Jaka_121_stop_program" && m_Value) { emit Jaka_121_stop_program(); }
}