/*************************************************************************************************************************************************
**Copyright(C), 2020-2025, bjtds.
**Version:2.0
**Author: dz
**Date:2021.2.1
**Description: 轮询模式
*************************************************************************************************************************************************/
#pragma once
#include "RGV_Client.h"

class c_RGV_Remote : public QObject
{
	Q_OBJECT
public:
	explicit c_RGV_Remote(QObject *parent = nullptr);
	virtual ~c_RGV_Remote();
	QThread *m_RGV_Remote_Thread;
	c_RGV_Client *m_RGV_Remote;

	public slots:
	void Init();
	void Connect();
	void Connect_Loop();
	void Fuction_0_Set();//通信控制（线圈）
	void Fuction_0_Reset();//通信控制（线圈）
	void Fuction_7();//主动力位置清零（线圈，工控机立即复位）
	void Fuction_8();//主动力故障复位（线圈，工控机立即复位）
	void Fuction_9_Set();//主动力手动左行（线圈，点控）
	void Fuction_9_Reset();//主动力手动左行（线圈，点控）
	void Fuction_10_Set();//主动力手动右行（线圈，点控） 
	void Fuction_10_Reset();//主动力手动右行（线圈，点控） 
	void Fuction_11();//正向连续运行（线圈，工控机立即复位）
	void Fuction_12();//返向连续运行（线圈，工控机立即复位）
	void Fuction_13();//返向连续运行再启动（线圈，工控机立即复位）
	void Fuction_14();//急停（线圈，工控机立即复位）（离散）
	void Fuction_15();//左机器人开机（线圈，工控机立即复位）
	void Fuction_16();//左机器人关机（线圈，工控机立即复位）
	void Fuction_17();//右机器人开机（线圈，工控机立即复位）
	void Fuction_18();//右机器人关机（线圈，工控机立即复位）
	void Fuction_19_Set();//上电（线圈，点控）
	void Fuction_19_Reset();//上电（线圈，点控）
	void Fuction_20();//充电回原点（线圈，工控机立即复位）
	void Fuction_21_Set();//风刀电机开
	void Fuction_21_Reset();//风刀电机关
	void Fuction_22_Set();//清扫电机开
	void Fuction_22_Reset();//清扫电机关
	void Fuction_24();//轴1移动（线圈，工控机立即复位）
	void Fuction_25();//轴2移动（线圈，工控机立即复位）
	void Fuction_26();//开始充电（线圈，工控机立即复位）
	void Fuction_27();//结束充电（线圈，工控机立即复位）
	void Date_0();//左升降台速度
	void Date_1();//左升降台位置
	void Date_2();//右升降台速度
	void Date_3();//右升降台位置
	void Date_4();//轴1移动距离
	void Date_5();//轴2移动距离

signals:
	void Connect_Device(QString ip, int port);//连接到服务器
	void Disconnect_Device();//断开连接
	void Write_RGV_Remote_State(QJsonObject db);//写状态
	void RGV_Remote_Read_Ready();//写入缓存区
	void Set_Working();//工作状态
	void Set_Default();//非工作状态
	void setEnabled(bool ready);//写消息状态
	void Status(QString status);//监视器状态
	void Write_Coils(int addr, QJsonObject value, int size);//写线圈
	void Write_HoldingRegisters(int addr, QJsonObject value, int size);//写输入寄存器
	void Read_Coils(int addr, int size);//读线圈
	void Read_HoldingRegisters(int addr, int size);//读输入寄存器
	void Read_DiscreteInputs(int addr, int size); //读离散输入
	void Read_InputRegisters(int addr, int size);//读输入寄存器

private:
	QJsonObject m_RGV_Remote_DB;
	QJsonObject m_RGV_Remote_State;
	QJsonObject m_Coils;
	QJsonObject m_DiscreteInputs;
	QJsonObject m_InputRegisters;
	QJsonObject m_HoldingRegisters;

	bool m_RGV_Fuction_0_Set = false;//通信控制（线圈）
	bool m_RGV_Fuction_0_Reset = false;//通信控制（线圈）
	bool m_RGV_Fuction_7 = false;//主动力位置清零（线圈，工控机立即复位）
	bool m_RGV_Fuction_8 = false;//主动力故障复位（线圈，工控机立即复位）
	bool m_RGV_Fuction_9_Set = false;//主动力手动左行（线圈，点控）
	bool m_RGV_Fuction_9_Reset = false;//主动力手动左行（线圈，点控）
	bool m_RGV_Fuction_10_Set = false;//主动力手动右行（线圈，点控）
	bool m_RGV_Fuction_10_Reset = false;//主动力手动右行（线圈，点控）
	bool m_RGV_Fuction_11 = false;//正向连续运行（线圈，工控机立即复位）
	bool m_RGV_Fuction_12 = false;//返向连续运行（线圈，工控机立即复位）
	bool m_RGV_Fuction_13 = false;//返向连续运行再启动（线圈，工控机立即复位）
	bool m_RGV_Fuction_14 = false;//急停（线圈，工控机立即复位）
	bool m_RGV_Fuction_15 = false;//左机器人开机（线圈，工控机立即复位）
	bool m_RGV_Fuction_16 = false;//左机器人关机（线圈，工控机立即复位）
	bool m_RGV_Fuction_17 = false;//右机器人开机（线圈，工控机立即复位）
	bool m_RGV_Fuction_18 = false;//右机器人关机（线圈，工控机立即复位）
	bool m_RGV_Fuction_19_Set = false;//上电（线圈，点控）
	bool m_RGV_Fuction_19_Reset = false;//上电（线圈，点控）
	bool m_RGV_Fuction_20 = false;//充电回原点（线圈，工控机立即复位）
	bool m_RGV_Fuction_21_Set = false;//风刀电机开
	bool m_RGV_Fuction_21_Reset = false;//风刀电机关
	bool m_RGV_Fuction_22_Set = false;//清扫电机开
	bool m_RGV_Fuction_22_Reset = false;//清扫电机关
	bool m_RGV_Fuction_24 = false;//轴1移动（线圈，工控机立即复位）
	bool m_RGV_Fuction_25 = false;//轴2移动（线圈，工控机立即复位）
	bool m_RGV_Fuction_26 = false;//开始充电（线圈，工控机立即复位）
	bool m_RGV_Fuction_27 = false;//结束充电（线圈，工控机立即复位）
	bool m_RGV_Date_0 = false;//左升降台速度
	bool m_RGV_Date_1 = false;//左升降台位置
	bool m_RGV_Date_2 = false;//右升降台速度
	bool m_RGV_Date_3 = false;//右升降台位置
	bool m_RGV_Date_4 = false;//轴1移动距离
	bool m_RGV_Date_5 = false;//轴2移动距离
	int m_DiscreteInputs_Addr;//离散起始地址
	int m_DiscreteInputs_Size;//离散数据长度
	int m_Coils_Addr;//线圈起始地址
	int m_Coils_Size;//线圈数据长度
	int m_InputRegisters_Addr;//输入起始地址
	int m_InputRegisters_Size;//输入数据长度
	int m_HoldingRegisters_Addr;//保持起始地址
	int m_HoldingRegisters_Size;//保持数据长度
	int m_Read_Coils_Count;//读线圈长度
	int m_Read_DiscreteInputs_Count;//读离散次数
	int m_Read_HoldingRegisters_Count;//读保持次数
	int m_Read_InputRegisters_Count;//读输入次数
	int m_Carbox_Number;//车厢号
	int m_Bogie_Number;//转向架号
	int m_Axis_Number;//轮轴号

	private slots:
	void Connect_Done();
	void Disconnect_Done();
	void Write_Coils_Done();//读线圈
	void Write_HoldingRegisters_Done();//读保持寄存器
	void Read_Coils_Done(QJsonObject value);//读线圈
	void Read_HoldingRegisters_Done(QJsonObject value);//读输入寄存器
	void Read_DiscreteInputs_Done(QJsonObject value);//读离散输入
	void Read_InputRegisters_Done(QJsonObject value);//读输入寄存器
	void Write_RGV_State();
	bool Set_Coils(int addr);
	bool Reset_Coils(int addr);
	bool Set_HoldingRegisters_16(int addr, int value);
	bool Set_HoldingRegisters_32(int addr, int low, int high);
	bool If_Write_Coils();
	bool If_Write_HoldingRegisters();
	bool If_Coils_Set();
};

