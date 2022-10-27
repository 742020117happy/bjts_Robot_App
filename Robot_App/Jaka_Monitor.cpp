#pragma execution_character_set("utf-8")
#include "Jaka_Monitor.h"

/*************************************************************************************************************************************************
**Function:构造函数(状态监视)
*************************************************************************************************************************************************/
c_Jaka_Monitor::c_Jaka_Monitor(QObject *parent) : QObject(parent)
{

}
/*************************************************************************************************************************************************
**Function:析构
*************************************************************************************************************************************************/
c_Jaka_Monitor::~c_Jaka_Monitor()
{
	//线程中断
	m_Jaka_Monitor_Thread->requestInterruption();
	//线程退出
	m_Jaka_Monitor_Thread->quit();
	//线程等待
	m_Jaka_Monitor_Thread->wait();
}
/*************************************************************************************************************************************************
**Function:初始化接口
*************************************************************************************************************************************************/
void c_Jaka_Monitor::Init()
{
	//实例化
	m_Jaka_Monitor = new c_Jaka_Client;
	m_Jaka_Monitor_Thread = new QThread;
	m_Jaka_Monitor->moveToThread(m_Jaka_Monitor_Thread);
	//初始化数据交换层
	QObject::connect(m_Jaka_Monitor_Thread, &QThread::started, m_Jaka_Monitor, &c_Jaka_Client::Init);
	QObject::connect(m_Jaka_Monitor_Thread, &QThread::finished, m_Jaka_Monitor, &c_Jaka_Client::deleteLater);
	//连接设备
	QObject::connect(this, &c_Jaka_Monitor::Connect_Device, m_Jaka_Monitor, &c_Jaka_Client::Connect_Device);
	QObject::connect(this, &c_Jaka_Monitor::Disconnect_Device, m_Jaka_Monitor, &c_Jaka_Client::Disconnect_Device);
	//设备状态改变
	QObject::connect(m_Jaka_Monitor, &c_Jaka_Client::Connect_Done, this, &c_Jaka_Monitor::Set_Working);
	QObject::connect(m_Jaka_Monitor, &c_Jaka_Client::Disconnect_Done, this, &c_Jaka_Monitor::Set_Default);
	//键盘按键改变
	QObject::connect(this, &c_Jaka_Monitor::Set_Working, this, [=] {emit setEnabled(true); });
	QObject::connect(this, &c_Jaka_Monitor::Set_Default, this, [=] {emit setEnabled(false); });
	//向状态服务写入状态
	QObject::connect(m_Jaka_Monitor, &c_Jaka_Client::Connect_Done, this, &c_Jaka_Monitor::Connect_Done);
	QObject::connect(m_Jaka_Monitor, &c_Jaka_Client::Disconnect_Done, this, &c_Jaka_Monitor::Disconnect_Done);
	//写指令与校验
	QObject::connect(m_Jaka_Monitor, &c_Jaka_Client::Read_Json_Done, this, &c_Jaka_Monitor::Read_Json_Done);
	//读准备同步
	QObject::connect(this, &c_Jaka_Monitor::Read_Ready, m_Jaka_Monitor, &c_Jaka_Client::Read_Ready);
	//提示信息
	QObject::connect(m_Jaka_Monitor, &c_Jaka_Client::Status, this, [=](int value) {emit Status(c_Variable::TCP_Status(value)); });
	//启动线程
	m_Jaka_Monitor_Thread->start();
	emit setEnabled(false);
}
/*************************************************************************************************************************************************
**Function:虚函数
*************************************************************************************************************************************************/
void c_Jaka_Monitor::Connect()
{
}
/*************************************************************************************************************************************************
**Function:父类私有的读状态函数，将状态量提取到公有成员变量
*************************************************************************************************************************************************/
void c_Jaka_Monitor::Read_Json_Done(QJsonObject json)
{
	m_Jaka_Monitor_State.insert("DB", json);
	//机器人关节角度
	joint_actual_position = json.value("joint_actual_position").toArray();
	//机器人 tcp 的位姿
	actual_position = json.value("actual_position").toArray();
	//1 代表机器人下电 2 代表机器人上电 3 代表机器人下使能 4 代表机器人上使能
	task_state = json.value("task_state").toInt();
	//机器人被设置的 home 点位
	homed = json.value("homed").toArray();
	//机器人的任务模式 1 代表手动模式 2 代表自动模式 3 代表 MDI(面板操作)模式 4 代表拖拽模式
	task_mode = json.value("task_mode").toInt();
	//程序运行状态 0 代表空闲 1 代表正在加载 2 代表暂停 3 代表正在运行
	interp_state = json.value("interp_state").toInt();
	//机器人当前使用的工具 ID
	current_tool_id = json.value("current_tool_id").toInt();
	//值为 1 时代表机器人正处于急停状态
	protective_stop = json.value("protective_stop").toInt();
	//值为 1 时代表机器人正处于关节软限位
	on_soft_limit = json.value("on_soft_limit").toInt();
	//值为 1 时代表机器人正处于代表急停状态
	emergency_stop = json.value("emergency_stop").toInt();
	//值为 1 时代表机器人接近拖拽的极限位置
	drag_near_limit = json.value("drag_near_limit").toArray();
	if (m_Power_Monitor && task_state == 1) {
		emit Power_Off();
	}
	if (m_Power_Monitor && task_state == 2) {
		emit Power_On();
	}
	if (m_Power_Monitor && task_state == 3) {
		emit Enable_Robot();
	}
	if (m_Power_Monitor && task_state == 4) {
		emit Disable_Robot();
	}
	if (m_Origin_Monitor && Origin_Position(joint_actual_position)) {
		emit Origin_Moved();
	}
	emit Write_Jaka_Monitor_State(m_Jaka_Monitor_State);
}
/*************************************************************************************************************************************************
**Function:监控功能接口
*************************************************************************************************************************************************/
void c_Jaka_Monitor::Power_Monitor()
{
	m_Power_Monitor = true;
}
void c_Jaka_Monitor::Enable_Monitor()
{
	m_Able_Monitor = true;
}
void c_Jaka_Monitor::Origin_Monitor()
{
	m_Origin_Monitor = true;
}
bool c_Jaka_Monitor::Origin_Position(QJsonArray joint)
{
	double a[6] = { -90, 40, 150, 120, 0, 5 };
	double b[6];
	for (int i = 0; i<6; i++)
	{
		b[i] = joint.at(i).toDouble();
	}
	bool arraysEqual = qFuzzyCompare(a[0], b[0]) && qFuzzyCompare(a[1], b[1]) && qFuzzyCompare(a[2], b[2]) && qFuzzyCompare(a[3], b[3]) && qFuzzyCompare(a[4], b[4]) && qFuzzyCompare(a[5], b[5]); // 标志变量
	return arraysEqual;
}
/*************************************************************************************************************************************************
**Function:父类私有连接状态写入
*************************************************************************************************************************************************/
void c_Jaka_Monitor::Connect_Done()
{
	m_Jaka_Monitor_State.insert("Connected", true);
	emit Write_Jaka_Monitor_State(m_Jaka_Monitor_State);
}
void c_Jaka_Monitor::Disconnect_Done()
{
	m_Jaka_Monitor_State.insert("Connected", false);
	emit Write_Jaka_Monitor_State(m_Jaka_Monitor_State);
}

