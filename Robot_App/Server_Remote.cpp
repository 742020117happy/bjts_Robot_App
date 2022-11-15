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
void c_Server_Remote::Connect()
{

}
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
	qDebug() << json;
	m_Cmd_Name = json.value("Cmd_Name").toString();
	m_Value = json.value("Value").toBool();
	if (m_Cmd_Name == "Read_Ready" && m_Value) { m_Robot_Server->Write_Json(m_State_DB); }//服务器读准备就绪
	if (m_Cmd_Name == "Work_Start") { emit Start_Cmd(json); }//开始巡检
	if (m_Cmd_Name == "00000" && m_Value) { emit Fuction_0_Set(); }//通信控制
	if (m_Cmd_Name == "00000" && !m_Value) { emit Fuction_0_Reset(); }//通信控制
	if (m_Cmd_Name == "00007" && m_Value) { emit Fuction_7(); }//主动力位置清零
	if (m_Cmd_Name == "00008" && m_Value) { emit Fuction_8(); }//主动力故障复位
	if (m_Cmd_Name == "00009" && m_Value) { emit Fuction_9_Set(); }//主动力手动左行
	if (m_Cmd_Name == "00009" && !m_Value) { emit Fuction_9_Reset(); }//主动力手动左行
	if (m_Cmd_Name == "00010" && m_Value) { emit Fuction_10_Set(); }//主动力手动右行
	if (m_Cmd_Name == "00010" && !m_Value) { emit Fuction_10_Reset(); }//主动力手动右行
	if (m_Cmd_Name == "00011" && m_Value) { emit Fuction_11(); }//正向连续运行
	if (m_Cmd_Name == "00012" && m_Value) { emit Fuction_12(); }//返向连续运行
	if (m_Cmd_Name == "00013" && m_Value) { emit Fuction_13(); }//返向连续运行再启动
	if (m_Cmd_Name == "00014" && m_Value) { emit Fuction_14(); }//急停
	if (m_Cmd_Name == "00015" && m_Value) { emit Fuction_15(); }//左机器人开机
	if (m_Cmd_Name == "00016" && m_Value) { emit Fuction_16(); }//左机器人关机
	if (m_Cmd_Name == "00017" && m_Value) { emit Fuction_17(); }//右机器人开机
	if (m_Cmd_Name == "00018" && m_Value) { emit Fuction_18(); }//右机器人关机
	if (m_Cmd_Name == "00019" && m_Value) { emit Fuction_19_Set(); }//上电
	if (m_Cmd_Name == "00019" && !m_Value) { emit Fuction_19_Reset(); }//下电
	if (m_Cmd_Name == "00020" && m_Value) { emit Fuction_20(); }//充电回原点
	if (m_Cmd_Name == "00021" && m_Value) { emit Fuction_21_Set(); }//清扫电机
	if (m_Cmd_Name == "00021" && !m_Value) { emit Fuction_21_Reset(); }//清扫电机
	if (m_Cmd_Name == "00022" && m_Value) { emit Fuction_22_Set(); }//风刀电机
	if (m_Cmd_Name == "00022" && !m_Value) { emit Fuction_22_Reset(); }//风刀电机
	if (m_Cmd_Name == "00024" && m_Value) { emit Fuction_24(json.value("Position").toInt()); }//轴1移动
	if (m_Cmd_Name == "00025" && m_Value) { emit Fuction_25(json.value("Position").toInt()); }//轴2移动
	if (m_Cmd_Name == "00026" && m_Value) { emit Fuction_26(); }//开始充电
	if (m_Cmd_Name == "00027" && m_Value) { emit Fuction_27(); }//结束充电
	if (m_Cmd_Name == "40000" && m_Value) { emit Date_0(json.value("Position").toInt(), json.value("Speed").toInt()); }//左升降台
	if (m_Cmd_Name == "40004" && m_Value) { emit Date_4(json.value("Position").toInt(), json.value("Speed").toInt()); }//右升降台
	if (m_Cmd_Name == "Jaka_120_power_on" && m_Value) { emit Jaka_120_power_on(); }//左机械臂打开电源
	if (m_Cmd_Name == "Jaka_120_power_off" && m_Value) { emit Jaka_120_power_off(); }//左机械臂关闭电源
	if (m_Cmd_Name == "Jaka_120_disable_robot" && m_Value) { emit Jaka_120_disable(); }//左机器人下使能
	if (m_Cmd_Name == "Jaka_120_enable_robot" && m_Value) { emit Jaka_120_enable(); }//左机器人上使能
	if (m_Cmd_Name == "Jaka_120_play_program" && m_Value) { emit Jaka_120_play_program(json.value("programName").toString()); }//左机械臂运行轨迹文件
	if (m_Cmd_Name == "Jaka_120_stop_program" && m_Value) { emit Jaka_120_stop_program(); }//左机械臂停止运行轨迹文件
	if (m_Cmd_Name == "Jaka_121_power_on" && m_Value) { emit Jaka_121_power_on(); }//右机械臂打开电源
	if (m_Cmd_Name == "Jaka_121_power_off" && m_Value) { emit Jaka_121_power_off(); }//右机械臂关闭电源
	if (m_Cmd_Name == "Jaka_121_disable_robot" && m_Value) { emit Jaka_121_disable(); }//右机器人下使能
	if (m_Cmd_Name == "Jaka_121_enable_robot" && m_Value) { emit Jaka_121_enable(); }//右机器人上使能
	if (m_Cmd_Name == "Jaka_121_play_program" && m_Value) { emit Jaka_121_play_program(json.value("programName").toString()); }//右机械臂运行轨迹文件
	if (m_Cmd_Name == "Jaka_121_stop_program" && m_Value) { emit Jaka_121_stop_program(); }//右机械臂停止运行轨迹文件
}