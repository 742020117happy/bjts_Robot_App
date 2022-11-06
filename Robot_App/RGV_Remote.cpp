#pragma execution_character_set("utf-8")
#include "RGV_Remote.h"
/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_RGV_Remote::c_RGV_Remote(QObject *parent) : QObject(parent)
{

}
/*************************************************************************************************************************************************
**Function:析构函数
*************************************************************************************************************************************************/
c_RGV_Remote::~c_RGV_Remote()
{
	//线程中断
	m_RGV_Remote_Thread->requestInterruption();
	//线程退出
	m_RGV_Remote_Thread->quit();
	//线程等待
	m_RGV_Remote_Thread->wait();
}
/*************************************************************************************************************************************************
**Function:初始化函数
*************************************************************************************************************************************************/
void c_RGV_Remote::Init()
{
	//实例化
	m_RGV_Remote = new c_RGV_Client;
	m_RGV_Remote_Thread = new QThread;
	m_RGV_Remote->moveToThread(m_RGV_Remote_Thread);
	//初始化数据交换层
	QObject::connect(m_RGV_Remote_Thread, &QThread::started, m_RGV_Remote, &c_RGV_Client::Init);
	QObject::connect(m_RGV_Remote_Thread, &QThread::finished, m_RGV_Remote, &c_RGV_Client::deleteLater);
	//连接设备
	QObject::connect(this, &c_RGV_Remote::Connect_Device, m_RGV_Remote, &c_RGV_Client::Connect_Device);
	QObject::connect(this, &c_RGV_Remote::Disconnect_Device, m_RGV_Remote, &c_RGV_Client::Disconnect_Device);
	//循环连接设备
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Connect_Loop, this, &c_RGV_Remote::Connect);
	//读数据
	QObject::connect(this, &c_RGV_Remote::Read_Coils, m_RGV_Remote, &c_RGV_Client::Read_Coils);
	QObject::connect(this, &c_RGV_Remote::Read_HoldingRegisters, m_RGV_Remote, &c_RGV_Client::Read_HoldingRegisters);
	QObject::connect(this, &c_RGV_Remote::Read_DiscreteInputs, m_RGV_Remote, &c_RGV_Client::Read_DiscreteInputs);
	QObject::connect(this, &c_RGV_Remote::Read_InputRegisters, m_RGV_Remote, &c_RGV_Client::Read_InputRegisters);
	//写数据
	QObject::connect(this, &c_RGV_Remote::Write_Coils, m_RGV_Remote, &c_RGV_Client::Write_Coils);
	QObject::connect(this, &c_RGV_Remote::Write_HoldingRegisters, m_RGV_Remote, &c_RGV_Client::Write_HoldingRegisters);
	//设备状态改变
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Connect_Done, this, &c_RGV_Remote::Set_Working);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Disconnect_Done, this, &c_RGV_Remote::Set_Default);
	//键盘按键改变
	QObject::connect(this, &c_RGV_Remote::Set_Working, this, [=] {emit setEnabled(true); });
	QObject::connect(this, &c_RGV_Remote::Set_Default, this, [=] {emit setEnabled(false); });
	//读到消息
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Write_Coils_Done, this, &c_RGV_Remote::Write_Coils_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Write_HoldingRegisters_Done, this, &c_RGV_Remote::Write_HoldingRegisters_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_Coils_Done, this, &c_RGV_Remote::Read_Coils_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_HoldingRegisters_Done, this, &c_RGV_Remote::Read_HoldingRegisters_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_DiscreteInputs_Done, this, &c_RGV_Remote::Read_DiscreteInputs_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_InputRegisters_Done, this, &c_RGV_Remote::Read_InputRegisters_Done);
	//向状态服务写入状态
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Connect_Done, this, &c_RGV_Remote::Connect_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Disconnect_Done, this, &c_RGV_Remote::Disconnect_Done);
	//提示信息
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Status, this, [=](int state) {emit Status(c_Variable::Modbus_Status(state)); });
	//启动线程
	m_RGV_Remote_Thread->start();
	emit setEnabled(false);
	c_Variable::msleep(500);
	Connect();
}
/*************************************************************************************************************************************************
**Function:连接设备
*************************************************************************************************************************************************/
void c_RGV_Remote::Connect()
{
	if (m_RGV_Remote_State.value("Connected").toBool()) { return; }
	QString ip = c_Variable::g_Communicate_DB.value("RGV_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("RGV_Port").toInt();
	emit Connect_Device(ip, port);
}
/*************************************************************************************************************************************************
**断开连接
************************************************************************************************************************************************/
void c_RGV_Remote::Disconnect_Done()
{
	m_RGV_Remote_State.insert("Connected", false);
	Write_RGV_State();
}
/*************************************************************************************************************************************************
**置位复位赋值
************************************************************************************************************************************************/
bool c_RGV_Remote::Set_Coils(int addr)
{
	m_Coils.insert(QString::number(addr), 1);
	emit Write_Coils(addr, m_Coils, 1);
	return false;
}
bool c_RGV_Remote::Reset_Coils(int addr)
{
	m_Coils.insert(QString::number(addr), 0);
	emit Write_Coils(addr, m_Coils, 1);
	return false;
}
bool c_RGV_Remote::Set_HoldingRegisters_16(int addr, int value)
{
	m_HoldingRegisters.insert(QString::number(addr), value);
	emit Write_HoldingRegisters(addr, m_HoldingRegisters, 1);
	return false;
}
bool c_RGV_Remote::Set_HoldingRegisters_32(int addr, int low, int high)
{
	m_HoldingRegisters.insert(QString::number(addr), low);
	m_HoldingRegisters.insert(QString::number(addr + 1), high);
	emit Write_HoldingRegisters(addr, m_HoldingRegisters, 2);
	return false;
}
void c_RGV_Remote::Write_RGV_State()
{
	QEventLoop *loop = new QEventLoop;
	QObject::connect(this, &c_RGV_Remote::RGV_Remote_Read_Ready, loop, &QEventLoop::quit);
	emit Write_RGV_Remote_State(m_RGV_Remote_State);//发送到缓存区
	loop->exec();
}
/*************************************************************************************************************************************************
**轮询操作
************************************************************************************************************************************************/
//完成连接->读线圈
void c_RGV_Remote::Connect_Done()
{
	m_RGV_Remote_State.insert("Connected", true);
	Write_RGV_State();
	//更新轮询参数
	m_Coils_Addr = c_Variable::g_Communicate_DB.value("Write_Coils_Addr").toInt();
	m_Coils_Size = c_Variable::g_Communicate_DB.value("Write_Coils_Size").toInt();
	m_Read_Coils_Count = 1;
	if (m_Coils_Size > 1000) {
		emit Read_Coils(m_DiscreteInputs_Addr, 1000);
		return;
	}
	emit Read_Coils(m_Coils_Addr, m_Coils_Size);
}
//读线圈完成->写线圈复位
//直到线圈全部复位->读保持存器
void c_RGV_Remote::Read_Coils_Done(QJsonObject value)
{
	m_Coils = value;
	m_RGV_Remote_DB.insert("Coils", value);//保存离散数据
	m_RGV_Remote_State.insert("DB", m_RGV_Remote_DB);//编入通讯协议
	Write_RGV_State();//发送到缓存区
	
	//当前需要读取的线圈起始地址 = 读取次数 * 每次读取长度 (0~999=1000)
	int Read_Coils_Addr = m_Read_Coils_Count * 1000;
	//当前需要读取的线圈数据长度 = 总数据长度（1000） - 当前起始地址（0）
	int Read_Coils_Size = m_Coils_Size - Read_Coils_Addr;
	//如果当前地址 < 总长度 且 还需读取长度大于124 则 从当前地址开始再读124个长度 计数加1 并返回
	if (Read_Coils_Addr < m_Coils_Size && Read_Coils_Size > 1000) {
		emit Read_Coils(Read_Coils_Addr, 1000);
		m_Read_Coils_Count += 1;
		return;
	}
	//如果当前地址 < 总长度 且 还需读取长度小于124 则 从当前地址开始读完剩余数据 并返回
	if (Read_Coils_Addr < m_Coils_Size && Read_Coils_Size < 1000) {
		emit Read_Coils(Read_Coils_Addr, Read_Coils_Size);
		m_Read_Coils_Count += 1;
		return;
	}
	//如果读到需要复位的线圈，进行复位,并返回写线圈完成
	if (If_Coils_Set()) { return; }
	//更新轮询参数
	m_HoldingRegisters_Addr = c_Variable::g_Communicate_DB.value("Write_HoldingRegisters_Addr").toInt();
	m_HoldingRegisters_Size = c_Variable::g_Communicate_DB.value("Write_HoldingRegisters_Size").toInt();
	m_Read_HoldingRegisters_Count = 1;
	if (m_HoldingRegisters_Size > 124) {
		emit Read_HoldingRegisters(m_HoldingRegisters_Addr, 124);
		return;
	}
	emit Read_HoldingRegisters(m_HoldingRegisters_Addr, m_HoldingRegisters_Size);
}
//写线圈完成->读线圈是否写完
void c_RGV_Remote::Write_Coils_Done()
{
	c_Variable::msleep(2);
	//更新轮询参数
	m_Coils_Addr = c_Variable::g_Communicate_DB.value("Write_Coils_Addr").toInt();
	m_Coils_Size = c_Variable::g_Communicate_DB.value("Write_Coils_Size").toInt();
	m_Read_Coils_Count = 1;
	if (m_Coils_Size > 1000) {
		emit Read_Coils(m_DiscreteInputs_Addr, 1000);
		return;
	}
	emit Read_Coils(m_Coils_Addr, m_Coils_Size);
}
//读保持寄存器完成->读离散输入
void c_RGV_Remote::Read_HoldingRegisters_Done(QJsonObject value)
{
	m_HoldingRegisters = value;
	m_RGV_Remote_DB.insert("HoldingRegisters", value);//保存保持数据
	m_RGV_Remote_State.insert("DB", m_RGV_Remote_DB);//编入通讯协议
	Write_RGV_State();//发送到缓存区
	
	//当前需要读取的保持寄存器起始地址 = 读取次数 * 每次读取长度 (0~123=124)
	int Read_HoldingRegisters_Addr = m_Read_HoldingRegisters_Count * 124;
	//当前需要读取的保持寄存器数据长度 = 总数据长度（250） - 当前起始地址（124）
	int Read_HoldingRegisters_Size = m_HoldingRegisters_Size - Read_HoldingRegisters_Addr;
	//如果当前地址 < 总长度 且 还需读取长度大于124 则 从当前地址开始再读124个长度 计数加1 并返回
	if (Read_HoldingRegisters_Addr < m_HoldingRegisters_Size && Read_HoldingRegisters_Size > 124) {
		emit Read_HoldingRegisters(Read_HoldingRegisters_Addr, 124);
		m_Read_HoldingRegisters_Count += 1;
		return;
	}
	//如果当前地址 < 总长度 且 还需读取长度小于124 则 从当前地址开始读完剩余数据 并返回
	if (Read_HoldingRegisters_Addr < m_HoldingRegisters_Size && Read_HoldingRegisters_Size < 124) {
		emit Read_HoldingRegisters(Read_HoldingRegisters_Addr, Read_HoldingRegisters_Size);
		m_Read_HoldingRegisters_Count += 1;
		return;
	}
	//读离散输入更新轮询参数
	m_DiscreteInputs_Addr = c_Variable::g_Communicate_DB.value("Read_DiscreteInputs_Addr").toInt();
	m_DiscreteInputs_Size = c_Variable::g_Communicate_DB.value("Read_DiscreteInputs_Size").toInt();
	m_Read_DiscreteInputs_Count = 1;
	if (m_DiscreteInputs_Size > 1000) {
		emit Read_DiscreteInputs(m_DiscreteInputs_Addr, 1000);
		return;
	}
	emit Read_DiscreteInputs(m_DiscreteInputs_Addr, m_DiscreteInputs_Size);
}
//需要写线圈时：读离散输入完成->写线圈
//不需要写线圈时：读离散输入完成->读输入存器
void c_RGV_Remote::Read_DiscreteInputs_Done(QJsonObject value)
{
	m_DiscreteInputs = value;
	m_RGV_Remote_DB.insert("DiscreteInputs", value);//保存离散数据
	m_RGV_Remote_State.insert("DB", m_RGV_Remote_DB);//编入通讯协议
	Write_RGV_State();//发送到缓存区
	
	//当前需要读取的离散起始地址 = 读取次数 * 每次读取长度 (0~999=1000)
	int Read_DiscreteInputs_Addr = m_Read_DiscreteInputs_Count * 1000;
	//当前需要读取的离散数据长度 = 总数据长度（1000） - 当前起始地址（0）
	int Read_DiscreteInputs_Size = m_DiscreteInputs_Size - Read_DiscreteInputs_Addr;
	//如果当前地址 < 总长度 且 还需读取长度大于124 则 从当前地址开始再读124个长度 计数加1 并返回
	if (Read_DiscreteInputs_Addr < m_DiscreteInputs_Size && Read_DiscreteInputs_Size > 1000) {
		emit Read_DiscreteInputs(Read_DiscreteInputs_Addr, 1000);
		m_Read_DiscreteInputs_Count += 1;
		return;
	}
	//如果当前地址 < 总长度 且 还需读取长度小于124 则 从当前地址开始读完剩余数据 并返回
	if (Read_DiscreteInputs_Addr < m_DiscreteInputs_Size && Read_DiscreteInputs_Size < 1000) {
		emit Read_DiscreteInputs(Read_DiscreteInputs_Addr, Read_DiscreteInputs_Size);
		m_Read_DiscreteInputs_Count += 1;
		return;
	}
	//如果线圈改变则等待线圈写完成后，读线圈
	if (If_Write_Coils()) { return; }
	//如果线圈不改变则不读线圈，直接读输入寄存器，更新轮询参数
	m_InputRegisters_Addr = c_Variable::g_Communicate_DB.value("Read_InputRegisters_Addr").toInt();
	m_InputRegisters_Size = c_Variable::g_Communicate_DB.value("Read_InputRegisters_Size").toInt();
	//如果读取的数据长度大于124则读124个数据,并初始化计数
	m_Read_InputRegisters_Count = 1;
	if (m_InputRegisters_Size > 124) {
		emit Read_InputRegisters(m_InputRegisters_Addr, 124);
		return;
	}
	emit Read_InputRegisters(m_InputRegisters_Addr, m_InputRegisters_Size);
}
//需写保持寄存器：读输入寄存器完成—>写保持寄存器
//无需写保持寄存器：读输入寄存器完成->读离散
void c_RGV_Remote::Read_InputRegisters_Done(QJsonObject value)
{
	m_InputRegisters = value;
	m_RGV_Remote_DB.insert("InputRegisters", value);//保存输入数据
	m_RGV_Remote_State.insert("DB", m_RGV_Remote_DB);//编入通讯协议
	Write_RGV_State();//发送到缓存区
	
	//当前需要读取的输入寄存器起始地址 = 读取次数 * 每次读取长度 (0~123=124)
	int Read_InputRegisters_Addr = m_Read_InputRegisters_Count * 124;
	//当前需要读取的输入寄存器数据长度 = 总数据长度（250） - 当前起始地址（124）
	int Read_InputRegisters_Size = m_InputRegisters_Size - Read_InputRegisters_Addr;
	//如果当前地址 < 总长度 且 还需读取长度大于124 则 从当前地址开始再读124个长度 计数加1 并返回
	if (Read_InputRegisters_Addr < m_InputRegisters_Size && Read_InputRegisters_Size > 124) {
		emit Read_InputRegisters(Read_InputRegisters_Addr, 124);
		m_Read_InputRegisters_Count += 1;
		return;
	}
	//如果当前地址 < 总长度 且 还需读取长度小于124 则 从当前地址开始读完剩余数据 并返回
	if (Read_InputRegisters_Addr < m_InputRegisters_Size && Read_InputRegisters_Size < 124) {
		emit Read_InputRegisters(Read_InputRegisters_Addr, Read_InputRegisters_Size);
		m_Read_InputRegisters_Count += 1;
		return;
	}
	//完成所有输入数据读取后，继续执行如下程序
	//如果保持寄存器改变则等待保持寄存器写完成后，读保持寄存器
	if (If_Write_HoldingRegisters()) { return; }
	//如果保持寄存器没有改变，则直接读离散输入
	//更新轮询参数
	m_DiscreteInputs_Addr = c_Variable::g_Communicate_DB.value("Read_DiscreteInputs_Addr").toInt();
	m_DiscreteInputs_Size = c_Variable::g_Communicate_DB.value("Read_DiscreteInputs_Size").toInt();
	m_Read_DiscreteInputs_Count = 1;
	if (m_DiscreteInputs_Size > 1000) {
		emit Read_DiscreteInputs(m_DiscreteInputs_Addr, 1000);
		return;
	}
	emit Read_DiscreteInputs(m_DiscreteInputs_Addr, m_DiscreteInputs_Size);
}
//写保持寄存器完成->读保持寄存器
void c_RGV_Remote::Write_HoldingRegisters_Done()
{
	c_Variable::msleep(2);
	//更新轮询参数
	m_HoldingRegisters_Addr = c_Variable::g_Communicate_DB.value("Write_HoldingRegisters_Addr").toInt();
	m_HoldingRegisters_Size = c_Variable::g_Communicate_DB.value("Write_HoldingRegisters_Size").toInt();
	m_Read_HoldingRegisters_Count = 1;
	if (m_HoldingRegisters_Size > 124) {
		emit Read_HoldingRegisters(m_HoldingRegisters_Addr, 124);
		return;
	}
	emit Read_HoldingRegisters(m_HoldingRegisters_Addr, m_HoldingRegisters_Size);
}
/*************************************************************************************************************************************************
**线圈操作
************************************************************************************************************************************************/
//通信控制（线圈）（离散：为1时远程控制，PLC端无控制）
void c_RGV_Remote::Fuction_0_Set()
{
	m_RGV_Fuction_0_Set = true;
}
void c_RGV_Remote::Fuction_0_Reset()
{
	m_RGV_Fuction_0_Reset = true;
}
//主动力位置清零（线圈，工控机立即复位）（离散）
void c_RGV_Remote::Fuction_7()
{
	m_RGV_Fuction_7 = true;
}
//主动力故障复位（线圈，工控机立即复位）（离散）
void c_RGV_Remote::Fuction_8()
{
	m_RGV_Fuction_8 = true;
}
//主动力手动左行（线圈，点控）（离散：为1运行中）
void c_RGV_Remote::Fuction_9_Set()
{
	m_RGV_Fuction_9_Set = true;
}
//主动力手动左行（线圈，点控）（离散：为1运行中）
void c_RGV_Remote::Fuction_9_Reset()
{
	m_RGV_Fuction_9_Reset = true;
}
//主动力手动右行（线圈，点控）（离散：为1运行中）
void c_RGV_Remote::Fuction_10_Set()
{
	m_RGV_Fuction_10_Set = true;
}
//主动力手动右行（线圈，点控）（离散：为1运行中）
void c_RGV_Remote::Fuction_10_Reset()
{
	m_RGV_Fuction_10_Reset = true;
}
//正向连续运行（线圈，工控机立即复位）（离散：为1运行中）
void c_RGV_Remote::Fuction_11()
{
	m_RGV_Fuction_11 = true;
}
//返向连续运行（线圈，工控机立即复位）（离散：为1运行中）
void c_RGV_Remote::Fuction_12()
{
	m_RGV_Fuction_12 = true;
}
//返向连续运行再启动（线圈，工控机立即复位）（离散：为1运行中）
void c_RGV_Remote::Fuction_13()
{
	m_RGV_Fuction_13 = true;
}//
 //急停（线圈，工控机立即复位）（离散）
void c_RGV_Remote::Fuction_14()
{
	m_RGV_Fuction_14 = true;
}
//左机器人开机（线圈，工控机立即复位）（离散：为1完成开机）
void c_RGV_Remote::Fuction_15()
{
	m_RGV_Fuction_15 = true;
}
//左机器人关机（线圈，工控机立即复位）（离散：为1完成关机）
void c_RGV_Remote::Fuction_16()
{
	m_RGV_Fuction_16 = true;
}
//右机器人开机（线圈，工控机立即复位）（离散：为1完成开机）
void c_RGV_Remote::Fuction_17()
{
	m_RGV_Fuction_17 = true;
}
//右机器人关机（线圈，工控机立即复位）（离散：为1完成关机）
void c_RGV_Remote::Fuction_18()
{
	m_RGV_Fuction_18 = true;
}
//上电（线圈，点控）（离散：为1所有设备上电处于待机状态）
void c_RGV_Remote::Fuction_19_Set()
{
	m_RGV_Fuction_19_Set = true;
}
//上电（线圈，点控）（离散：为1所有设备上电处于待机状态）
void c_RGV_Remote::Fuction_19_Reset()
{
	m_RGV_Fuction_19_Reset = true;
}
//充电回原点（线圈，工控机立即复位）
void c_RGV_Remote::Fuction_20()
{
	m_RGV_Fuction_20 = true;
}
//风刀电机开
void c_RGV_Remote::Fuction_21_Set()
{
	m_RGV_Fuction_21_Set = true;
}
//风刀电机关
void c_RGV_Remote::Fuction_21_Reset()
{
	m_RGV_Fuction_21_Reset = true;
}
//清扫电机开
void c_RGV_Remote::Fuction_22_Set()
{
	m_RGV_Fuction_22_Set = true;
}
//清扫电机关
void c_RGV_Remote::Fuction_22_Reset()
{
	m_RGV_Fuction_22_Reset = true;
}
//轴1移动（线圈，工控机立即复位）（离散：为为1移动中）
void c_RGV_Remote::Fuction_24()
{
	m_RGV_Fuction_24 = true;
}
//轴2移动（线圈，工控机立即复位）（离散：为1移动中）
void c_RGV_Remote::Fuction_25()
{
	m_RGV_Fuction_25 = true;
}
//开始充电（线圈，工控机立即复位）（离散：为1时远程控制，PLC端无控制）
void c_RGV_Remote::Fuction_26()
{
	m_RGV_Fuction_26 = true;
}
//结束充电（线圈，工控机立即复位）（离散：为1时远程控制，PLC端无控制）
void c_RGV_Remote::Fuction_27()
{
	m_RGV_Fuction_27 = true;
}
/*************************************************************************************************************************************************
**保持寄存器操作
************************************************************************************************************************************************/
//左升降台速度
void c_RGV_Remote::Date_0()
{
	m_RGV_Date_0 = true;
}
//左升降台位置
void c_RGV_Remote::Date_1()
{
	m_RGV_Date_1 = true;
}
//右升降台速度
void c_RGV_Remote::Date_2()
{
	m_RGV_Date_2 = true;
}
//右升降台位置
void c_RGV_Remote::Date_3()
{
	m_RGV_Date_3 = true;
}
//轴1移动距离
void c_RGV_Remote::Date_4()
{
	m_RGV_Date_4 = true;
}
//轴2移动距离
void c_RGV_Remote::Date_5()
{
	m_RGV_Date_5 = true;
}
/*************************************************************************************************************************************************
**判断是否写线圈
************************************************************************************************************************************************/
bool c_RGV_Remote::If_Write_Coils()
{
	if (m_RGV_Fuction_0_Set) {
		m_RGV_Fuction_0_Set = Set_Coils(RGV_Fuction_0);
		return true;
	}
	if (m_RGV_Fuction_0_Reset) {
		m_RGV_Fuction_0_Reset = Reset_Coils(RGV_Fuction_0);
		return true;
	}
	if (m_RGV_Fuction_7) {
		m_RGV_Fuction_7 = Set_Coils(RGV_Fuction_7);
		return true;
	}
	if (m_RGV_Fuction_8) {
		m_RGV_Fuction_8 = Set_Coils(RGV_Fuction_8);
		return true;
	}
	if (m_RGV_Fuction_9_Set) {
		m_RGV_Fuction_9_Set = Set_Coils(RGV_Fuction_9);
		return true;
	}
	if (m_RGV_Fuction_9_Reset) {
		m_RGV_Fuction_9_Reset = Reset_Coils(RGV_Fuction_9);
		return true;
	}
	if (m_RGV_Fuction_10_Set) {
		m_RGV_Fuction_10_Set = Set_Coils(RGV_Fuction_10);
		return true;
	}
	if (m_RGV_Fuction_10_Reset) {
		m_RGV_Fuction_10_Reset = Reset_Coils(RGV_Fuction_10);
		return true;
	}
	if (m_RGV_Fuction_11) {
		m_RGV_Fuction_11 = Set_Coils(RGV_Fuction_11);
		return true;
	}
	if (m_RGV_Fuction_12) {
		m_RGV_Fuction_12 = Set_Coils(RGV_Fuction_12);
		return true;
	}
	if (m_RGV_Fuction_13) {
		m_RGV_Fuction_13 = Set_Coils(RGV_Fuction_13);
		return true;
	}
	if (m_RGV_Fuction_14) {
		m_RGV_Fuction_14 = Set_Coils(RGV_Fuction_14);
		return true;
	}
	if (m_RGV_Fuction_15) {
		m_RGV_Fuction_15 = Set_Coils(RGV_Fuction_15);
		return true;
	}
	if (m_RGV_Fuction_16) {
		m_RGV_Fuction_16 = Set_Coils(RGV_Fuction_16);
		return true;
	}
	if (m_RGV_Fuction_17) {
		m_RGV_Fuction_17 = Set_Coils(RGV_Fuction_17);
		return true;
	}
	if (m_RGV_Fuction_18) {
		m_RGV_Fuction_18 = Set_Coils(RGV_Fuction_18);
		return true;
	}
	if (m_RGV_Fuction_19_Set) {
		m_RGV_Fuction_19_Set = Set_Coils(RGV_Fuction_19);
		return true;
	}
	if (m_RGV_Fuction_19_Reset) {
		m_RGV_Fuction_19_Reset = Reset_Coils(RGV_Fuction_19);
		return true;
	}
	if (m_RGV_Fuction_20) {
		m_RGV_Fuction_20 = Set_Coils(RGV_Fuction_20);
		return true;
	}
	if (m_RGV_Fuction_21_Set) {
		m_RGV_Fuction_21_Set = Set_Coils(RGV_Fuction_21);
		return true;
	}
	if (m_RGV_Fuction_21_Reset) {
		m_RGV_Fuction_21_Reset = Reset_Coils(RGV_Fuction_21);
		return true;
	}
	if (m_RGV_Fuction_22_Set) {
		m_RGV_Fuction_22_Set = Set_Coils(RGV_Fuction_22);
		return true;
	}
	if (m_RGV_Fuction_22_Reset) {
		m_RGV_Fuction_22_Reset = Reset_Coils(RGV_Fuction_22);
		return true;
	}
	if (m_RGV_Fuction_24) {
		m_RGV_Fuction_24 = Set_Coils(RGV_Fuction_24);
		return true;
	}
	if (m_RGV_Fuction_25) {
		m_RGV_Fuction_25 = Set_Coils(RGV_Fuction_25);
		return true;
	}
	if (m_RGV_Fuction_26) {
		m_RGV_Fuction_26 = Set_Coils(RGV_Fuction_26);
		return true;
	}
	if (m_RGV_Fuction_27) {
		m_RGV_Fuction_27 = Set_Coils(RGV_Fuction_27);
		return true;
	}
	else {
		return false;
	}
}
/*************************************************************************************************************************************************
**判断是否写保持寄存器
************************************************************************************************************************************************/
bool c_RGV_Remote::If_Write_HoldingRegisters()
{
	//左升降台速度
	if (m_RGV_Date_0) {
		int Date = c_Variable::g_Communicate_DB.value("RGV_Date_0").toInt();
		int high = Date / 65536;
		int low = Date % 65536;
		m_RGV_Date_0 = Set_HoldingRegisters_32(RGV_Date_0, low, high);
		return true;
	}
	//左升降台位置
	if (m_RGV_Date_1) {
		int Date = c_Variable::g_Communicate_DB.value("RGV_Date_1").toInt();
		int high = Date / 65536;
		int low = Date % 65536;
		m_RGV_Date_1 = Set_HoldingRegisters_32(RGV_Date_1, low, high);
		return true;
	}
	//右升降台速度
	if (m_RGV_Date_2) {
		int Date = c_Variable::g_Communicate_DB.value("RGV_Date_2").toInt();
		int high = Date / 65536;
		int low = Date % 65536;
		m_RGV_Date_2 = Set_HoldingRegisters_32(RGV_Date_2, low, high);
		return true;
	}
	//右升降台位置
	if (m_RGV_Date_3) {
		int Date = c_Variable::g_Communicate_DB.value("RGV_Date_3").toInt();
		int high = Date / 65536;
		int low = Date % 65536;
		m_RGV_Date_3 = Set_HoldingRegisters_32(RGV_Date_3, low, high);
		return true;
	}
	//轴1移动距离
	if (m_RGV_Date_4) {
		int Date = c_Variable::g_Communicate_DB.value("RGV_Date_4").toInt();
		m_RGV_Date_4 = Set_HoldingRegisters_16(RGV_Date_4, Date);
		return true;
	}
	//轴2移动距离
	if (m_RGV_Date_5) {
		int Date = c_Variable::g_Communicate_DB.value("RGV_Date_5").toInt();
		m_RGV_Date_5 = Set_HoldingRegisters_16(RGV_Date_5, Date);
		return true;
	}
	//否则返回false
	else {
		return false;
	}
}
/*************************************************************************************************************************************************
**判断线圈是否置位(点控)
************************************************************************************************************************************************/
bool c_RGV_Remote::If_Coils_Set()
{
	//主动力位置清零
	if (m_Coils.value(QString::number(RGV_Fuction_7)).toInt() == 1 ) {
		Reset_Coils(RGV_Fuction_7);
		return true;
	}
	//主动力故障复位
	if (m_Coils.value(QString::number(RGV_Fuction_8)).toInt() == 1 ) {
		Reset_Coils(RGV_Fuction_8);
		return true;
	}
	//正向连续运行
	if (m_Coils.value(QString::number(RGV_Fuction_11)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_11);
		return true;
	}
	//返向连续运行
	if (m_Coils.value(QString::number(RGV_Fuction_12)).toInt() == 1 ) {
		Reset_Coils(RGV_Fuction_12);
		return true;
	}
	//返向连续运行再启动
	if (m_Coils.value(QString::number(RGV_Fuction_13)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_13);
		return true;
	}
	//急停
	if (m_Coils.value(QString::number(RGV_Fuction_14)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_14);
		return true;
	}
	//左机器人开机
	if (m_Coils.value(QString::number(RGV_Fuction_15)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_15);
		return true;
	}
	//左机器人关机
	if (m_Coils.value(QString::number(RGV_Fuction_16)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_16);
		return true;
	}
	//右机器人开机
	if (m_Coils.value(QString::number(RGV_Fuction_17)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_17);
		return true;
	}
	//右机器人关机
	if (m_Coils.value(QString::number(RGV_Fuction_18)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_18);
		return true;
	}
	//充电回原点（线圈，工控机立即复位）
	if (m_Coils.value(QString::number(RGV_Fuction_20)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_20);
		return true;
	}
	//轴1移动
	if (m_Coils.value(QString::number(RGV_Fuction_24)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_24);
		return true;
	}
	//轴2移动
	if (m_Coils.value(QString::number(RGV_Fuction_25)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_25);
		return true;
	}
	//开始充电（线圈，工控机立即复位）
	if (m_Coils.value(QString::number(RGV_Fuction_26)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_26);
		return true;
	}
	//结束充电（线圈，工控机立即复位）
	if (m_Coils.value(QString::number(RGV_Fuction_27)).toInt() == 1) {
		Reset_Coils(RGV_Fuction_27);
		return true;
	}
	//否则返回false
	else {
		return false;
	}
}



