#pragma execution_character_set("utf-8")
#include "State_DB.h"
/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_State_DB::c_State_DB(QObject *parent) : QObject(parent)
{
}
/*************************************************************************************************************************************************
**Function:析构函数
*************************************************************************************************************************************************/
c_State_DB::~c_State_DB()
{
}
/*************************************************************************************************************************************************
**Function:初始化函数
*************************************************************************************************************************************************/
void c_State_DB::Init()
{
	QJsonObject object;
	object.insert("Connected", false);
	m_State_DB.insert("System_Time", NULL);
	m_State_DB.insert("RGV_State", object);
	m_State_DB.insert("Jaka_120_Remote_State", object);
	m_State_DB.insert("Jaka_121_Remote_State", object);
	m_State_DB.insert("Jaka_120_Monitor_State", object);
	m_State_DB.insert("Jaka_121_Monitor_State", object);
	m_State_DB.insert("Hypersen_30_State", object);
	m_State_DB.insert("Hypersen_31_State", object);
	m_State_DB.insert("Meijidenki_20_State", object);
	m_State_DB.insert("Meijidenki_21_State", object);
	m_State_DB.insert("Hikvision_State", object);
	m_State_DB.insert("Fast_Scan_State", object);
	m_State_DB.insert("Prec_Scan_120_State", object);
	m_State_DB.insert("Prec_Scan_121_State", object);
	m_State_DB.insert("Local_Remote_State", object);
	m_State_DB.insert("Local_Monitor_State", object);
	m_State_DB.insert("App_Control_State", object);
}
/*************************************************************************************************************************************************
**Function:线程操作变量接口
*************************************************************************************************************************************************/
void c_State_DB::Write_System_Time()
{
	m_Current_Time = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");//更新
	m_State_DB.insert("System_Time", m_Current_Time);
	emit System_Scan(m_State_DB);
}
void c_State_DB::Write_RGV_State(QJsonObject db)
{
	m_State_DB.insert("RGV_State", db);
	emit RGV_Remote_Read_Ready();
}
void c_State_DB::Write_Jaka_120_Remote_State(QJsonObject db)
{
	m_State_DB.insert("Jaka_120_Remote_State", db);
}
void c_State_DB::Write_Jaka_121_Remote_State(QJsonObject db)
{
	m_State_DB.insert("Jaka_121_Remote_State", db);
}
void c_State_DB::Write_Jaka_120_Monitor_State(QJsonObject db)
{
	m_State_DB.insert("Jaka_120_Monitor_State", db);
	emit Jaka_120_Monitor_Read_Ready();
}
void c_State_DB::Write_Jaka_121_Monitor_State(QJsonObject db)
{
	m_State_DB.insert("Jaka_121_Monitor_State", db);
	emit Jaka_121_Monitor_Read_Ready();
}
void c_State_DB::Write_Hypersen_30_State(QJsonObject db)
{
	m_State_DB.insert("Hypersen_30_State", db);
	emit Hypersen_30_Read_Ready();
}
void c_State_DB::Write_Hypersen_31_State(QJsonObject db)
{
	m_State_DB.insert("Hypersen_31_State", db);
	emit Hypersen_31_Read_Ready();
}
void c_State_DB::Write_Meijidenki_20_State(QJsonObject db)
{
	m_State_DB.insert("Meijidenki_20_State", db);
	emit Meijidenki_20_Read_Ready();
}
void c_State_DB::Write_Meijidenki_21_State(QJsonObject db)
{
	m_State_DB.insert("Meijidenki_21_State", db);
	emit Meijidenki_21_Read_Ready();
}
void c_State_DB::Write_Hikvision_State(QJsonObject db)
{
	m_State_DB.insert("Hikvision_State", db);
}
void c_State_DB::Write_Fast_Scan_State(QJsonObject db)
{
	m_State_DB.insert("Fast_Scan_State", db);
}
void c_State_DB::Write_Prec_Scan_120_State(QJsonObject db)
{
	m_State_DB.insert("Prec_Scan_120_State", db);
}
void c_State_DB::Write_Prec_Scan_121_State(QJsonObject db)
{
	m_State_DB.insert("Prec_Scan_121_State", db);
}
void c_State_DB::Write_Local_Remote_State(QJsonObject db)
{
	m_State_DB.insert("Local_Remote_State", db);
}
void c_State_DB::Write_Local_Monitor_State(QJsonObject db)
{
	m_State_DB.insert("Local_Monitor_State", db);
}
void c_State_DB::Write_Work_Remote_State(QJsonObject db)
{
	m_State_DB.insert("Work_Remote_State", db);
}
void c_State_DB::Write_App_Control_State(QJsonObject db)
{
	m_State_DB.insert("App_Control_State", db);
}