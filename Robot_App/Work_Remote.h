#pragma once
#include "Variable.h"
#include "Public_Header.h"

class c_Work_Remote : public QObject
{
	Q_OBJECT
public:
	explicit c_Work_Remote(QObject *parent = nullptr);
	virtual ~c_Work_Remote();
	QJsonObject m_Work_Program;
	int m_Axis_1_Position;
	int m_Axis_2_Position;
	int m_Header_Position;

	public slots:
	void Init();
	void Start_Cmd(QJsonObject object);
	void Work_Start();
	void System_Scan(QJsonObject object);
	void Prec_Scan_Write_Done(QString ip, int port, QString value);
	void Load_Status(QString state);
	void Step_0(QString Carbox_Num, QString Bogie_Num, QString Axis_Num);//直到第一轴巡检结束
	void Step_1(QString Carbox_Num, QString Bogie_Num, QString Axis_Num);//直到间二位巡检结束
	void Step_2(QString Carbox_Num, QString Bogie_Num, QString Axis_Num);//直到间一位巡检结束
	void Step_3(QString Carbox_Num, QString Bogie_Num, QString Axis_Num);//直到巡检结束
	void State_1_Loop();//通信控制（线圈）
	void State_2_Loop();//主动力位置清零（线圈，工控机立即复位）
	void State_3_Loop();//主动力故障复位（线圈，工控机立即复位）
	void State_4_Loop();//正向连续运行（线圈，工控机立即复位）
	void State_5_Loop();//返向连续运行（线圈，工控机立即复位）
	void State_6_Loop();//返向连续运行再启动（线圈，工控机立即复位）
	void State_7_Loop();//机器人开机（线圈，工控机立即复位）
	void State_8_Loop();//上电（线圈，点控）
	void State_9_Loop(int position);//轴1移动（线圈，工控机立即复位）
	void State_10_Loop(int position);//轴2移动（线圈，工控机立即复位）
	void State_11_Loop(int speed, int position);//升降台移动
	void State_12_Loop();//机械臂上电
	void State_13_Loop();//机械臂下电
	void State_14_Loop();//机械臂上使能
	void State_15_Loop();//机械臂下使能
	void State_16_Loop(QString name);//机械臂运行程序	
	void State_17_Loop();//巡检完成
	void State_18_Loop();//正向障碍停车
	void State_19_Loop();//反向障碍停车
	void State_20_Loop();//自动充电解除
	void State_21_Loop();//风刀打开
	void State_22_Loop();//系统就绪
signals:
	void Status(QString state);
	void Write_Work_Remote_State(QJsonObject db);
	void Work_Init();
	void Fuction_0_Set();//通信控制（线圈）
	void Fuction_7();//主动力位置清零（线圈，工控机立即复位）
	void Fuction_8();//主动力故障复位（线圈，工控机立即复位）
	void Fuction_14();//急停
	void Fuction_11();//正向连续运行（线圈，工控机立即复位）
	void Fuction_12();//返向连续运行（线圈，工控机立即复位）
	void Fuction_13();//返向连续运行再启动（线圈，工控机立即复位）
	void Fuction_15();//机器人开机（线圈，工控机立即复位）
	void Fuction_19_Set();//上电（线圈，点控）
	void Fuction_20();//自动充电定位
	void Fuction_22_Set();//风刀电源开
	void Fuction_22_Reset();//风刀电源关
	void Fuction_24(int position);//轴1移动（线圈，工控机立即复位）
	void Fuction_25(int position);//轴2移动（线圈，工控机立即复位）
	void Fuction_27();//自动充电结束
	void Date_0(int speed);//左升降台速度
	void Date_1(int position);//左升降台位置
	void Date_2(int speed);//右升降台速度
	void Date_3(int position);//右升降台位置

	void Jaka_120_power_on();//机械臂上电
    void Jaka_120_power_off();//机械臂下电
    void Jaka_120_enable();//机械臂使能
    void Jaka_120_disable();//机械臂下使能
	void Jaka_120_play_program(QString name);//机械臂运行程序
	void Jaka_121_power_on();//机械臂上电
	void Jaka_121_power_off();//机械臂下电
	void Jaka_121_enable();//机械臂使能
	void Jaka_121_disable();//机械臂下使能
	void Jaka_121_play_program(QString name);//机械臂运行程序

	void Fast_Scan_Start();//快扫开始采集
	void Fast_Scan_Stop();//快扫停止采集
	void Hikvision_Start();//声纹开始采集
	void Hikvision_Stop();//声纹停止采集
	void Prec_Scan_Done();//精扫采集完成状态

	void is_State_1();//通信控制完成状态
	void is_State_2();//正向连续运动完成状态
	void is_State_3();//返向断续运动完成状态
	void is_State_4();//返向断续再启动完成状态
	void is_State_5();//主动力位置清零完成状态
	void is_State_6();//机器人开机完成状态
	void is_State_7();//上电完成状态
	void is_State_8();//通讯完成
	void is_State_9();//正向完成停车完成状态
	void is_State_10();//反向完成停车完成状态
	void is_State_11();//轴1移动完成状态
	void is_State_12();//轴2移动完成状态
	void is_State_13();//返向运行遇到雷达停车完成状态
	void is_State_14();//正向运行遇到雷达停车完成状态
	void is_State_15();//返向到达轮轴停车完成状态
	void is_State_16();//升降台位置到达完成状态
	void is_State_17();//动力伺服就绪完成状态
	void is_State_18();//风刀电机完成状态
	void is_State_19();//机械臂上电
	void is_State_20();//机械臂上使能
	void is_State_21();//机械臂下电
	void is_State_22();//机械臂下使能
	void is_State_23();//正向面阵雷达停车
	void is_State_24();//正向面阵雷达停车解除
	void is_State_25();//反向面阵雷达停车
	void is_State_26();//反向面阵雷达停车解除
	void is_State_27();//自动充电解除
	void is_State_28();//系统就绪

private:
	QJsonObject m_Work_Remote_State;
	QJsonArray m_Status;
	QString m_Robot_Num = "01";//机器人编号
	QString m_Track_Rank = "D98";//股道号
	QString m_Begin_Time = "202211021651";//开始时间
	QString m_Work_Num = "202211021651CHR380A2411";//任务编号
	QString m_Car_Type = "CHR380A";//任务车型
	QString m_Car_Num = "2411";//车辆编号
	QString m_Carbox_Num = "01";//车厢号
	QString m_Bogie_Num = "A";//转向架号
	QString m_Axis_Num = "01";//轮轴号
	QString m_Work_Stage = "Debuge";//巡检工作阶段
	QString m_Wheelset_Num_120 = "L";//机械臂120轮对号
	QString m_Wheelset_Num_121 = "L";//机械臂121轮对号
	QString m_Point_Num_120 = "01";//机械臂120巡检点
	QString m_Point_Num_121 = "O1";//机械臂121巡检点
	QString m_Primary_Components_120 = "ZDJQ";//机械臂120巡检一级部件
	QString m_Secondary_Components_120 = "01";//机械臂120巡检二级部件
	QString m_Primary_Components_121 = "ZDJQ";//机械臂121巡检一级部件
	QString m_Secondary_Components_121 = "01";//机械臂121巡检二级部件
	bool m_Work_Start = false;
	bool m_Jaka_120 = false;
	bool m_Jaka_121 = false;

	QJsonObject m_json;//参数

	QJsonObject m_RGV_State;//RGV状态
	QJsonObject m_Jaka_120_Remote_State;//左机械臂控制状态
	QJsonObject m_Jaka_121_Remote_State;//右机械臂控制状态
	QJsonObject m_Jaka_120_Monitor_State;//左机械臂监视状态
	QJsonObject m_Jaka_121_Monitor_State;//右机械臂监视状态
	QJsonObject m_Hypersen_30_State;//左面阵相机状态
	QJsonObject m_Hypersen_31_State;//右面阵相机状态
	QJsonObject m_Meijidenki_20_State;//左线阵相机状态
	QJsonObject m_Meijidenki_21_State;//右线阵相机状态
	QJsonObject m_Fast_Scan_State;//快扫采集状态

	bool m_RGV_Connected = false;//状态
	bool m_Jaka_120_Remote_Connected = false;//状态
	bool m_Jaka_121_Remote_Connected = false;//状态
	bool m_Jaka_120_Monitor_Connected = false;//状态
	bool m_Jaka_121_Monitor_Connected = false;//状态
	bool m_Hypersen_30_Connected = false;//状态
	bool m_Hypersen_31_Connected = false;//状态
	bool m_Meijidenki_20_Connected = false;//状态
	bool m_Meijidenki_21_Connected = false;//状态
	bool m_Fast_Scan_Connected = false;//状态

	QJsonObject m_DiscreteInputs;
	QJsonObject m_InputRegisters;
	int m_RGV_Position;

	int m_task_state;

	QJsonArray m_Array;
	int m_count_1;
	int m_count_2;
	int m_count_3;
	int m_count_4;
	int m_roi_aver_distance;//ROI_0平均距离
	int m_roi_max_distance;//ROI_0最大距离
	int m_roi_min_distance;//ROI_0最小距离
	int m_roi_valid_aver_amp;//ROI_0有效幅值
	int m_roi_all_aver_amp;//ROI_0平均幅值

	bool m_State_1 = false;//通信控制
	bool m_State_2 = false;//正向连续运动
	bool m_State_3 = false;//返向断续运动
	bool m_State_4 = false;//返向断续再启动
	bool m_State_5 = false;//主动力位置清零
	bool m_State_6 = false;//机器人开机
	bool m_State_7 = false;//上电
	bool m_State_8 = false;//通讯完成
	bool m_State_9 = false;//正向完成停车
	bool m_State_10 = false;//反向完成停车
	bool m_State_11 = false;//轴1移动
	bool m_State_12 = false;//轴2移动
	bool m_State_13 = false;//返向运行遇到雷达停车
	bool m_State_14 = false;//正向运行遇到雷达停车
	bool m_State_15 = false;//返向到达轮轴停车
	bool m_State_16 = false;//升降台位置到达
	bool m_State_17 = false;//动力伺服就绪
	bool m_State_18 = false;//风刀电机
	bool m_State_19= false;//左机械臂上电
	bool m_State_20 = false;//左机械臂上使能
	bool m_State_21 = false;//右机械臂上电
	bool m_State_22 = false;//右机械臂上使能
	bool m_State_23 = false;//机械臂上电
	bool m_State_24 = false;//机械臂上使能
	bool m_State_25 = false;//正向面阵雷达停车1
	bool m_State_26 = false;//正向面阵雷达停车2
	bool m_State_27 = false;//反向面阵雷达停车1
	bool m_State_28 = false;//反向面阵雷达停车2
	bool m_State_29 = false;//正向障碍停车中
	bool m_State_30 = false;//反向障碍停车中
	bool m_State_31 = false;//自动在充电状态
	bool m_State_32 = false;//系统就绪状态
};


