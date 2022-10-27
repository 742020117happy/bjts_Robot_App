#pragma once
#include "Public_Header.h"
#include "State_DB.h"
#include "RGV_Remote.h"
#include "App_Control.h"
#include "Work_Remote.h"
#include "Local_Remote.h"
#include "Local_Monitor.h"
#include "Jaka_120_Remote.h"
#include "Jaka_121_Remote.h"
#include "Jaka_120_Monitor.h"
#include "Jaka_121_Monitor.h"
#include "Hypersen_30_Remote.h"
#include "Hypersen_31_Remote.h"
#include "Meijidenki_20_Remote.h"
#include "Meijidenki_21_Remote.h"
#include "Prec_Scan_120_Remote.h"
#include "Prec_Scan_121_Remote.h"
#include "Hikvision_Remote.h"
#include "Fast_Scan_Remote.h"

/*************************************************************************************************************************************************
**Function:线程对象与接口
*************************************************************************************************************************************************/
class c_Thread : public QObject
{
	Q_OBJECT
public:
	explicit c_Thread(QObject *parent = nullptr);
	virtual ~c_Thread();
	c_State_DB *m_State_DB;//状态参数
	c_RGV_Remote *m_RGV_Remote;//RGV控制
	c_Jaka_120_Remote *m_Jaka_120_Remote;//左机械臂控制
	c_Jaka_121_Remote *m_Jaka_121_Remote;//右机械臂控制
	c_Jaka_120_Monitor *m_Jaka_120_Monitor;//左机械臂监视
	c_Jaka_121_Monitor *m_Jaka_121_Monitor;//右机械臂监视
	c_Hypersen_30_Remote *m_Hypersen_30_Remote;//左面阵雷达控制
	c_Hypersen_31_Remote *m_Hypersen_31_Remote;//右面阵雷达控制
	c_Meijidenki_20_Remote *m_Meijidenki_20_Remote;//左线阵雷达控制
	c_Meijidenki_21_Remote *m_Meijidenki_21_Remote;//右线阵雷达控制
	c_Prec_Scan_120_Remote *m_Prec_Scan_120_Remote;//左精扫相机转发
	c_Prec_Scan_121_Remote *m_Prec_Scan_121_Remote;//右精扫相机转发
	c_Fast_Scan_Remote  *m_Fast_Scan_Remote;//快扫相机控制
	c_Hikvision_Remote *m_Hikvision_Remote;//海康声纹采集
	c_Local_Remote *m_Local_Remote;//控制服务
	c_Local_Monitor *m_Local_Monitor;//监视服务
	c_App_Control *m_App_Control;//调试App服务
	c_Work_Remote *m_Work_Remote;//巡检控制
	public slots:
	void Start();
private:
	QThread *m_State_DB_Thread;//状态参数线程
	QThread *m_RGV_Remote_Thread;//RGV控制线程
	QThread *m_Jaka_120_Remote_Thread;//左机械臂控制线程
	QThread *m_Jaka_121_Remote_Thread;//右机械臂控制线程
	QThread *m_Jaka_120_Monitor_Thread;//左机械臂监视线程
	QThread *m_Jaka_121_Monitor_Thread;//右机械臂监视线程
	QThread *m_Hypersen_30_Remote_Thread;//左面阵雷达控制线程
	QThread *m_Hypersen_31_Remote_Thread;//右面阵雷达控制线程
	QThread *m_Meijidenki_20_Remote_Thread;//左线阵雷达控制线程
	QThread *m_Meijidenki_21_Remote_Thread;//右线阵雷达控制线程
	QThread *m_Prec_Scan_120_Remote_Thread;//左精扫相机转发线程
	QThread *m_Prec_Scan_121_Remote_Thread;//右精扫相机转发线程
	QThread *m_Fast_Scan_Remote_Thread;//快扫相机控制线程
	QThread *m_Hikvision_Remote_Thread;//海康声纹采集线程
	QThread *m_Local_Remote_Thread;//控制服务线程
	QThread *m_Local_Monitor_Thread;//监视服务线程
	QThread *m_App_Control_Thread;//调试App线程
	QThread *m_Work_Remote_Thread;//巡检控制线程
};
