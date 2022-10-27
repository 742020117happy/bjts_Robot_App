#pragma execution_character_set("utf-8")
#include "Thread.h"

/*************************************************************************************************************************************************
**Function:构造函数(线程对象与接口)
*************************************************************************************************************************************************/
c_Thread::c_Thread(QObject *parent) : QObject(parent)
{
	m_State_DB_Thread = new QThread;//状态参数线程
	m_RGV_Remote_Thread = new QThread;//RGV控制线程
	m_Jaka_120_Remote_Thread = new QThread;//左机械臂控制线程
	m_Jaka_121_Remote_Thread = new QThread;//右机械臂控制线程
	m_Jaka_120_Monitor_Thread = new QThread;//左机械臂监视线程
	m_Jaka_121_Monitor_Thread = new QThread;//右机械臂监视线程
	m_Hypersen_30_Remote_Thread = new QThread;//左面阵雷达控制线程
	m_Hypersen_31_Remote_Thread = new QThread;//右面阵雷达控制线程
	m_Meijidenki_20_Remote_Thread = new QThread;//左线阵雷达控制线程
	m_Meijidenki_21_Remote_Thread = new QThread;//右线阵雷达控制线程
	m_Prec_Scan_120_Remote_Thread = new QThread;//左精扫相机转发线程
	m_Prec_Scan_121_Remote_Thread = new QThread;//右精扫相机转发线程
	m_Fast_Scan_Remote_Thread = new QThread;//快扫相机控制线程
	m_Hikvision_Remote_Thread = new QThread;//海康声纹采集线程
	m_Local_Remote_Thread = new QThread;//控制服务线程
	m_Local_Monitor_Thread = new QThread;//监视服务线程
	m_App_Control_Thread = new QThread;//调试App线程
	m_Work_Remote_Thread = new QThread;//巡检控制线程
	m_State_DB = new c_State_DB;//状态参数
	m_RGV_Remote = new c_RGV_Remote;//RGV控制
	m_Jaka_120_Remote = new c_Jaka_120_Remote;//左机械臂控制
	m_Jaka_121_Remote = new c_Jaka_121_Remote;//右机械臂控制
	m_Jaka_120_Monitor = new c_Jaka_120_Monitor;//左机械臂监视
	m_Jaka_121_Monitor = new c_Jaka_121_Monitor;//右机械臂监视
	m_Hypersen_30_Remote = new c_Hypersen_30_Remote;//左面阵雷达控制
	m_Hypersen_31_Remote = new c_Hypersen_31_Remote;//右面阵雷达控制
	m_Meijidenki_20_Remote = new c_Meijidenki_20_Remote;//左线阵雷达控制
	m_Meijidenki_21_Remote = new c_Meijidenki_21_Remote;//右线阵雷达控制
	m_Prec_Scan_120_Remote = new c_Prec_Scan_120_Remote;//左精扫相机转发
	m_Prec_Scan_121_Remote = new c_Prec_Scan_121_Remote;//右精扫相机转发
	m_Fast_Scan_Remote = new c_Fast_Scan_Remote;//快扫相机控制
	m_Hikvision_Remote = new c_Hikvision_Remote;//海康声纹采集
	m_Local_Remote = new c_Local_Remote;//控制服务
	m_Local_Monitor = new c_Local_Monitor;//监视服务
	m_App_Control = new c_App_Control;//调试App服务
	m_Work_Remote = new c_Work_Remote;//巡检控制
	m_State_DB->moveToThread(m_State_DB_Thread);//状态参数
	m_RGV_Remote->moveToThread(m_RGV_Remote_Thread);//RGV控制
	m_Jaka_120_Remote->moveToThread(m_Jaka_120_Remote_Thread);//左机械臂控制
	m_Jaka_121_Remote->moveToThread(m_Jaka_121_Remote_Thread);//右机械臂控制
	m_Jaka_120_Monitor->moveToThread(m_Jaka_120_Monitor_Thread);//左机械臂监视
	m_Jaka_121_Monitor->moveToThread(m_Jaka_121_Monitor_Thread);//右机械臂监视
	m_Hypersen_30_Remote->moveToThread(m_Hypersen_30_Remote_Thread);//左面阵雷达控制
	m_Hypersen_31_Remote->moveToThread(m_Hypersen_31_Remote_Thread);//右面阵雷达控制
	m_Meijidenki_20_Remote->moveToThread(m_Meijidenki_20_Remote_Thread);//左线阵雷达控制
	m_Meijidenki_21_Remote->moveToThread(m_Meijidenki_21_Remote_Thread);//右线阵雷达控制
	m_Prec_Scan_120_Remote->moveToThread(m_Prec_Scan_120_Remote_Thread);//左精扫相机转发
	m_Prec_Scan_121_Remote->moveToThread(m_Prec_Scan_121_Remote_Thread);//右精扫相机转发
	m_Fast_Scan_Remote->moveToThread(m_Fast_Scan_Remote_Thread);//快扫相机控制
	m_Hikvision_Remote->moveToThread(m_Hikvision_Remote_Thread);//海康声纹采集
	m_Local_Remote->moveToThread(m_Local_Remote_Thread);//控制服务
	m_Local_Monitor->moveToThread(m_Local_Monitor_Thread);//监视服务
	m_App_Control->moveToThread(m_App_Control_Thread);//调试App服务
	m_Work_Remote->moveToThread(m_Work_Remote_Thread);//巡检控制
	QObject::connect(m_State_DB_Thread, &QThread::started, m_State_DB, &c_State_DB::Init);
	QObject::connect(m_RGV_Remote_Thread, &QThread::started, m_RGV_Remote, &c_RGV_Remote::Init);
	QObject::connect(m_Jaka_120_Remote_Thread, &QThread::started, m_Jaka_120_Remote, &c_Jaka_120_Remote::Init);
	QObject::connect(m_Jaka_120_Monitor_Thread, &QThread::started, m_Jaka_120_Monitor, &c_Jaka_120_Monitor::Init);
	QObject::connect(m_Jaka_121_Remote_Thread, &QThread::started, m_Jaka_121_Remote, &c_Jaka_121_Remote::Init);
	QObject::connect(m_Jaka_121_Monitor_Thread, &QThread::started, m_Jaka_121_Monitor, &c_Jaka_121_Monitor::Init);
	QObject::connect(m_Hypersen_30_Remote_Thread, &QThread::started, m_Hypersen_30_Remote, &c_Hypersen_30_Remote::Init);
	QObject::connect(m_Hypersen_31_Remote_Thread, &QThread::started, m_Hypersen_31_Remote, &c_Hypersen_31_Remote::Init);
	QObject::connect(m_Meijidenki_20_Remote_Thread, &QThread::started, m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::Init);
	QObject::connect(m_Meijidenki_21_Remote_Thread, &QThread::started, m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::Init);
	QObject::connect(m_Prec_Scan_120_Remote_Thread, &QThread::started, m_Prec_Scan_120_Remote, &c_Prec_Scan_120_Remote::Run);
	QObject::connect(m_Prec_Scan_121_Remote_Thread, &QThread::started, m_Prec_Scan_121_Remote, &c_Prec_Scan_121_Remote::Run);
	QObject::connect(m_Fast_Scan_Remote_Thread, &QThread::started, m_Fast_Scan_Remote, &c_Fast_Scan_Remote::Init);
	QObject::connect(m_Hikvision_Remote_Thread, &QThread::started, m_Hikvision_Remote, &c_Hikvision_Remote::Init);
	QObject::connect(m_Local_Remote_Thread, &QThread::started, m_Local_Remote, &c_Local_Remote::Run);
	QObject::connect(m_Local_Monitor_Thread, &QThread::started, m_Local_Monitor, &c_Local_Monitor::Run);
	QObject::connect(m_App_Control_Thread, &QThread::started, m_App_Control, &c_App_Control::Run);
	QObject::connect(m_Work_Remote_Thread, &QThread::started, m_Work_Remote, &c_Work_Remote::Init);
	QObject::connect(m_State_DB_Thread, &QThread::finished, m_State_DB, &c_State_DB::deleteLater);
	QObject::connect(m_RGV_Remote_Thread, &QThread::finished, m_RGV_Remote, &c_RGV_Remote::deleteLater);
	QObject::connect(m_Jaka_120_Remote_Thread, &QThread::finished, m_Jaka_120_Remote, &c_Jaka_120_Remote::deleteLater);
	QObject::connect(m_Jaka_120_Monitor_Thread, &QThread::finished, m_Jaka_120_Monitor, &c_Jaka_120_Monitor::deleteLater);
	QObject::connect(m_Jaka_121_Remote_Thread, &QThread::finished, m_Jaka_121_Remote, &c_Jaka_121_Remote::deleteLater);
	QObject::connect(m_Jaka_121_Monitor_Thread, &QThread::finished, m_Jaka_121_Monitor, &c_Jaka_121_Monitor::deleteLater);
	QObject::connect(m_Hypersen_30_Remote_Thread, &QThread::finished, m_Hypersen_30_Remote, &c_Hypersen_30_Remote::deleteLater);
	QObject::connect(m_Hypersen_31_Remote_Thread, &QThread::finished, m_Hypersen_31_Remote, &c_Hypersen_31_Remote::deleteLater);
	QObject::connect(m_Meijidenki_20_Remote_Thread, &QThread::finished, m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::deleteLater);
	QObject::connect(m_Meijidenki_21_Remote_Thread, &QThread::finished, m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::deleteLater);
	QObject::connect(m_Prec_Scan_120_Remote_Thread, &QThread::finished, m_Prec_Scan_120_Remote, &c_Prec_Scan_120_Remote::deleteLater);
	QObject::connect(m_Prec_Scan_121_Remote_Thread, &QThread::finished, m_Prec_Scan_121_Remote, &c_Prec_Scan_121_Remote::deleteLater);
	QObject::connect(m_Fast_Scan_Remote_Thread, &QThread::finished, m_Fast_Scan_Remote, &c_Fast_Scan_Remote::deleteLater);
	QObject::connect(m_Hikvision_Remote_Thread, &QThread::finished, m_Hikvision_Remote, &c_Hikvision_Remote::deleteLater);
	QObject::connect(m_Local_Remote_Thread, &QThread::finished, m_Local_Remote, &c_Local_Remote::deleteLater);
	QObject::connect(m_Local_Monitor_Thread, &QThread::finished, m_Local_Monitor, &c_Local_Monitor::deleteLater);
	QObject::connect(m_App_Control_Thread, &QThread::finished, m_App_Control, &c_App_Control::deleteLater);
	QObject::connect(m_Work_Remote_Thread, &QThread::finished, m_Work_Remote, &c_Work_Remote::deleteLater);
	QObject::connect(m_State_DB, &c_State_DB::System_Scan, m_Local_Remote, &c_Local_Remote::System_Scan);
	QObject::connect(m_State_DB, &c_State_DB::System_Scan, m_Local_Monitor, &c_Local_Monitor::System_Scan);
	QObject::connect(m_State_DB, &c_State_DB::System_Scan, m_Work_Remote, &c_Work_Remote::System_Scan);
	QObject::connect(m_State_DB, &c_State_DB::System_Scan, m_Local_Remote, &c_Local_Remote::System_Scan);
	QObject::connect(m_State_DB, &c_State_DB::RGV_Remote_Read_Ready, m_RGV_Remote, &c_RGV_Remote::RGV_Remote_Read_Ready);
	QObject::connect(m_State_DB, &c_State_DB::Jaka_120_Monitor_Read_Ready, m_Jaka_120_Monitor, &c_Jaka_120_Monitor::Read_Ready);
	QObject::connect(m_State_DB, &c_State_DB::Jaka_121_Monitor_Read_Ready, m_Jaka_121_Monitor, &c_Jaka_121_Monitor::Read_Ready);
	QObject::connect(m_State_DB, &c_State_DB::Hypersen_30_Read_Ready, m_Hypersen_30_Remote, &c_Hypersen_30_Remote::Hypersen_30_Read_Ready);
	QObject::connect(m_State_DB, &c_State_DB::Hypersen_31_Read_Ready, m_Hypersen_31_Remote, &c_Hypersen_31_Remote::Hypersen_31_Read_Ready);
	QObject::connect(m_State_DB, &c_State_DB::Meijidenki_20_Read_Ready, m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::Meijidenki_20_Read_Ready);
	QObject::connect(m_State_DB, &c_State_DB::Meijidenki_21_Read_Ready, m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::Meijidenki_21_Read_Ready);
	QObject::connect(m_RGV_Remote, &c_RGV_Remote::Write_RGV_Remote_State, m_State_DB, &c_State_DB::Write_RGV_State);
	QObject::connect(m_Jaka_120_Remote, &c_Jaka_120_Remote::Write_Jaka_Remote_State, m_State_DB, &c_State_DB::Write_Jaka_120_Remote_State);
	QObject::connect(m_Jaka_121_Remote, &c_Jaka_121_Remote::Write_Jaka_Remote_State, m_State_DB, &c_State_DB::Write_Jaka_121_Remote_State);
	QObject::connect(m_Jaka_120_Monitor, &c_Jaka_120_Monitor::Write_Jaka_Monitor_State, m_State_DB, &c_State_DB::Write_Jaka_120_Monitor_State);
	QObject::connect(m_Jaka_121_Monitor, &c_Jaka_121_Monitor::Write_Jaka_Monitor_State, m_State_DB, &c_State_DB::Write_Jaka_121_Monitor_State);
	QObject::connect(m_Hypersen_30_Remote, &c_Hypersen_30_Remote::Write_Hypersen_Remote_State, m_State_DB, &c_State_DB::Write_Hypersen_30_State);
	QObject::connect(m_Hypersen_31_Remote, &c_Hypersen_31_Remote::Write_Hypersen_Remote_State, m_State_DB, &c_State_DB::Write_Hypersen_31_State);
	QObject::connect(m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::Write_Meijidenki_Remote_State, m_State_DB, &c_State_DB::Write_Meijidenki_20_State);
	QObject::connect(m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::Write_Meijidenki_Remote_State, m_State_DB, &c_State_DB::Write_Meijidenki_21_State);
	QObject::connect(m_Prec_Scan_120_Remote, &c_Prec_Scan_120_Remote::Write_Scan_Remote_State, m_State_DB, &c_State_DB::Write_Prec_Scan_120_State);
	QObject::connect(m_Prec_Scan_121_Remote, &c_Prec_Scan_121_Remote::Write_Scan_Remote_State, m_State_DB, &c_State_DB::Write_Prec_Scan_121_State);
	QObject::connect(m_Fast_Scan_Remote, &c_Fast_Scan_Remote::Write_Scan_Remote_State, m_State_DB, &c_State_DB::Write_Fast_Scan_State);
	QObject::connect(m_Hikvision_Remote, &c_Hikvision_Remote::Write_Scan_Remote_State, m_State_DB, &c_State_DB::Write_Hikvision_State);
	QObject::connect(m_Local_Remote, &c_Local_Remote::Write_Server_State, m_State_DB, &c_State_DB::Write_Local_Remote_State);
	QObject::connect(m_Local_Monitor, &c_Local_Monitor::Write_Server_State, m_State_DB, &c_State_DB::Write_Local_Monitor_State);
	QObject::connect(m_App_Control, &c_App_Control::Write_Server_State, m_State_DB, &c_State_DB::Write_App_Control_State);
	QObject::connect(m_Work_Remote, &c_Work_Remote::Write_Work_Remote_State, m_State_DB, &c_State_DB::Write_Work_Remote_State);
	QObject::connect(m_Jaka_120_Remote, &c_Jaka_120_Remote::Origin_Monitor, m_Jaka_120_Monitor, &c_Jaka_120_Monitor::Origin_Monitor);
	QObject::connect(m_Jaka_121_Remote, &c_Jaka_121_Remote::Origin_Monitor, m_Jaka_121_Monitor, &c_Jaka_121_Monitor::Origin_Monitor);
	QObject::connect(m_Jaka_120_Monitor, &c_Jaka_120_Monitor::Origin_Moved, m_Jaka_120_Remote, &c_Jaka_120_Remote::Origin_Moved);
	QObject::connect(m_Jaka_121_Monitor, &c_Jaka_121_Monitor::Origin_Moved, m_Jaka_121_Remote, &c_Jaka_121_Remote::Origin_Moved);
	QObject::connect(m_Prec_Scan_120_Remote, &c_Prec_Scan_120_Remote::Write_String,m_Work_Remote, &c_Work_Remote::Prec_Scan_Write_Done);//左精扫采集指令
	QObject::connect(m_Prec_Scan_121_Remote, &c_Prec_Scan_121_Remote::Write_String, m_Work_Remote, &c_Work_Remote::Prec_Scan_Write_Done);//右精扫采集指令
	QObject::connect(m_Local_Remote, &c_Local_Remote::Start_Cmd, m_Work_Remote, &c_Work_Remote::Start_Cmd);//自动巡检程序第一步：服务器下发巡检开始指令
	QObject::connect(m_Work_Remote, &c_Work_Remote::Write_Work_Remote_State, m_Prec_Scan_120_Remote, &c_Prec_Scan_120_Remote::Start_Cmd);
	QObject::connect(m_Work_Remote, &c_Work_Remote::Write_Work_Remote_State, m_Prec_Scan_121_Remote, &c_Prec_Scan_121_Remote::Start_Cmd);
	QObject::connect(m_Work_Remote, &c_Work_Remote::Write_Work_Remote_State, m_Fast_Scan_Remote, &c_Fast_Scan_Remote::Start_Cmd);
	QObject::connect(m_Work_Remote, &c_Work_Remote::Write_Work_Remote_State, m_Hikvision_Remote, &c_Hikvision_Remote::Start_Cmd);
}
/*************************************************************************************************************************************************
**Function:析构函数
*************************************************************************************************************************************************/
c_Thread::~c_Thread()
{
	m_Local_Remote_Thread->requestInterruption();
	m_Local_Remote_Thread->quit();
	m_Local_Remote_Thread->wait();

	m_Local_Monitor_Thread->requestInterruption();
	m_Local_Monitor_Thread->quit();
	m_Local_Monitor_Thread->wait();

	m_App_Control_Thread->requestInterruption();
	m_App_Control_Thread->quit();
	m_App_Control_Thread->wait();

	m_Prec_Scan_120_Remote_Thread->requestInterruption();
	m_Prec_Scan_120_Remote_Thread->quit();
	m_Prec_Scan_120_Remote_Thread->wait();

	m_Prec_Scan_121_Remote_Thread->requestInterruption();
	m_Prec_Scan_121_Remote_Thread->quit();
	m_Prec_Scan_121_Remote_Thread->wait();

	m_Fast_Scan_Remote_Thread->requestInterruption();
	m_Fast_Scan_Remote_Thread->quit();
	m_Fast_Scan_Remote_Thread->wait();

	m_Hikvision_Remote_Thread->requestInterruption();
	m_Hikvision_Remote_Thread->quit();
	m_Hikvision_Remote_Thread->wait();

	m_Jaka_120_Monitor_Thread->requestInterruption();
	m_Jaka_120_Monitor_Thread->quit();
	m_Jaka_120_Monitor_Thread->wait();

	m_Jaka_121_Monitor_Thread->requestInterruption();
	m_Jaka_121_Monitor_Thread->quit();
	m_Jaka_121_Monitor_Thread->wait();

	m_Jaka_120_Remote_Thread->requestInterruption();
	m_Jaka_120_Remote_Thread->quit();
	m_Jaka_120_Remote_Thread->wait();

	m_Jaka_121_Remote_Thread->requestInterruption();
	m_Jaka_121_Remote_Thread->quit();
	m_Jaka_121_Remote_Thread->wait();

	m_Hypersen_30_Remote_Thread->requestInterruption();
	m_Hypersen_30_Remote_Thread->quit();
	m_Hypersen_30_Remote_Thread->wait();

	m_Hypersen_31_Remote_Thread->requestInterruption();
	m_Hypersen_31_Remote_Thread->quit();
	m_Hypersen_31_Remote_Thread->wait();

	m_Meijidenki_20_Remote_Thread->requestInterruption();
	m_Meijidenki_20_Remote_Thread->quit();
	m_Meijidenki_20_Remote_Thread->wait();

	m_Meijidenki_21_Remote_Thread->requestInterruption();
	m_Meijidenki_21_Remote_Thread->quit();
	m_Meijidenki_21_Remote_Thread->wait();

	m_RGV_Remote_Thread->requestInterruption();
	m_RGV_Remote_Thread->quit();
	m_RGV_Remote_Thread->wait();

	m_State_DB_Thread->requestInterruption();
	m_State_DB_Thread->quit();
	m_State_DB_Thread->wait();
}
/*************************************************************************************************************************************************
**Function:启动
*************************************************************************************************************************************************/
void c_Thread::Start()
{
	m_State_DB_Thread->start();//状态参数线程
	m_Jaka_120_Remote_Thread->start();//左机械臂控制线程
	m_Jaka_121_Remote_Thread->start();//右机械臂控制线程
	m_Jaka_120_Monitor_Thread->start();//左机械臂监视线程
	m_Jaka_121_Monitor_Thread->start();//右机械臂监视线程
	m_Hypersen_30_Remote_Thread->start();//左面阵雷达控制线程
	m_Hypersen_31_Remote_Thread->start();//右面阵雷达控制线程
	m_Meijidenki_20_Remote_Thread->start();//左线阵雷达控制线程
	m_Meijidenki_21_Remote_Thread->start();//右线阵雷达控制线程
	m_Prec_Scan_120_Remote_Thread->start();//左精扫相机转发线程
	m_Prec_Scan_121_Remote_Thread->start();//右精扫相机转发线程
	m_Fast_Scan_Remote_Thread->start();//快扫相机控制线程
	m_Hikvision_Remote_Thread->start();//海康声纹采集线程
	m_Work_Remote_Thread->start();//巡检控制线程
	m_Local_Remote_Thread->start();//控制服务线程
	m_Local_Monitor_Thread->start();//监视服务线程
	m_App_Control_Thread->start();//调试App线程
	m_RGV_Remote_Thread->start();//RGV控制线程
}
