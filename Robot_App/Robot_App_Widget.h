#pragma once
#include "Ui_Robot_App_Widget.h"
#include "Public_Header.h"
#include "Thread.h"


class c_Robot_App_Widget : public QMainWindow {
	Q_OBJECT

public:
	c_Robot_App_Widget(QWidget * parent = nullptr);
	virtual ~c_Robot_App_Widget();
	
	public slots:
	void keyPressEvent(QKeyEvent *event);

signals:
	void System_Scan_Done();

private:
	Ui_Robot_App_Widget *ui;
	c_Thread *m_Thread;//子类线程
	QElapsedTimer *m_Time;//计时器

	QString m_Current_Time = "";//系统时间
	int m_Current_FPS = 0;//当前帧率
	int m_FPS = 0;//帧率计数
	int m_Connect_Count = 0;//连接计数

	QJsonObject m_Work_Remote_State;//获取到的工作状态参数
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
	QJsonObject m_Prec_Scan_120_State;//左精扫采集状态
	QJsonObject m_Prec_Scan_121_State;//右精扫采集状态
	QJsonObject m_Hikvision_State;//声纹采集状态

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
	bool m_Hikvision_Connected = false;//状态
	bool m_Work_Connected = false;//状态
	bool m_START_LMD_20 = false;//状态
	bool m_START_LMD_21 = false;//状态
	bool m_RUN_CONTINUOUS_30 = false;//状态
	bool m_RUN_CONTINUOUS_31 = false;//状态

	QJsonObject m_DiscreteInputs ;//RGV离散输入
	QJsonObject m_InputRegisters;//RGV输入寄存器
	QJsonObject m_HoldingRegisters;//RGV保持寄存器

	int m_RGV_Date = 0;//RGV保持寄存器数据（只读一次）

	bool m_State = false;//离散数据
	bool m_RGV_Ready = false;//状态
	bool m_Jaka_120_Power = false;//状态
	bool m_Jaka_121_Power = false;//状态
	bool m_RGV_Power = false;//状态

	int m_Date = 0;//输入寄存器数据
	int m_Date_High = 0;//浮点数高位
	int m_Date_Low = 0;//浮点数低位

	float m_Left_Axice = 0;//左中轴当前定位数据
	float m_Right_Axice = 0;//右中轴当前定位数据

	QJsonObject m_json;//机械臂参数
	QJsonArray m_joint_actual_position;//前六个值为机器人关节角度，最后三个数值无用
	QJsonArray m_actual_position;// 前六个值为机器人 tcp 的位姿，最后三个数值无用
	int m_task_state = 0;//1 代表机器人下电 2 代表机器人上电 3 代表机器人下使能 4 代表机器人上使能
	QJsonArray m_homed;//输出值表示机器人被设置的 home 点位
	int m_task_mode = 0;//机器人的任务模式 1 代表手动模式 2 代表自动模式 3 已不使用 4 代表拖
	int m_interp_state = 0;//程序运行状态 0 代表空闲 1 代表正在加载 2 代表暂停
	int m_current_tool_id = 0;//机器人当前使用的工具 ID
	int m_protective_stop = 0;//值为 1 时代表机器人正处于急停状态
	int m_on_soft_limit = 0;//值为 1 时代表机器人正处于关节软
	int m_emergency_stop = 0;//值为 1 时代表机器人正处于代表急
	QJsonArray m_drag_near_limit;//值为 1 时代表机器人接近拖拽的极限位
	
	QJsonObject  m_value;//线阵雷达参数
	int m_MR = 0;// 量程
	int m_ESAR = 0;// 设备角度范围
	int m_ESA0 = 0;// 设备角度起始角
	int m_ESA1 = 0;// 设备角度终止角
	int m_SAR = 0;// 有效角度范围
	int m_SA0 = 0;// 有效角度起始角
	int m_SA1 = 0;// 有效角度终止角
	int m_SAV = 0;// 扫描角速度（度/秒）
	double m_SAP = 0;// 扫描角度分辨率
	int m_PF = 0;// 测量频率(HZ)
	double m_nRange = 0;// 设备量程.单位:cm
	double m_nBAngle = 0;// 测量数据起始角度，可能为负值.单位:度
	double m_nEAngle = 0;// 测量数据结束角度，可能为负值.单位:度
	double m_nAnglePrecision = 0;// 角度精度.单位：1/1000 度
	double m_nRPM = 0;// 扫描频率.单位：RPM (转/分钟)
	double m_nMDataNum = 0;//测量数据的个数，根据nBAngle、nEAngle和nAnglePrecision计算
	QJsonArray m_distance;//测量距离数据.单位cm
	int m_idistance = 0;//模长
	double m_iAngle = 0;//角度

	QJsonArray m_Array;//面阵雷达参数
	int m_roi_aver_distance = 0;//平均距离
	int m_roi_max_distance = 0;//最大距离
	int m_roi_min_distance = 0;//最小距离
	int m_roi_valid_aver_amp = 0;//有效幅值
	int m_roi_all_aver_amp = 0;//平均幅值

	private slots:
	void System_Scan(QJsonObject db);//系统扫描
};
