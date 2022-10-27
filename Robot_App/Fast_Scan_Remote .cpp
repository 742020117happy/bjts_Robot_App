#pragma execution_character_set("utf-8")
#include "Fast_Scan_Remote.h"
/*************************************************************************************************************************************************
**Function:构造函数(状态监视)
*************************************************************************************************************************************************/
c_Fast_Scan_Remote::c_Fast_Scan_Remote(c_Scan_Remote *parent) : c_Scan_Remote(parent)
{

}
/*************************************************************************************************************************************************
**Function:析构
*************************************************************************************************************************************************/
c_Fast_Scan_Remote::~c_Fast_Scan_Remote()
{

}
/*************************************************************************************************************************************************
**Function:操作接口
*************************************************************************************************************************************************/
void c_Fast_Scan_Remote::Connect()
{
	if (m_Scan_Remote_State.value("Connected").toBool()) { return; }
	QString ip = c_Variable::g_Communicate_DB.value("Local_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("Fast_Scan_Local_Port").toInt();
	emit Connect_Device(ip, port);
}
/*************************************************************************************************************************************************
**Function:采集
*************************************************************************************************************************************************/
void c_Fast_Scan_Remote::Collection()
{
	QString ip = c_Variable::g_Communicate_DB.value("Collector_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("Fast_Scan_Collector_Port").toInt();
	//CS&OPEN&CurrentImagePath（CurrentImagePath为当前图像存储路径）
	//C:\Image\FastScan_raw\ID股道号\年月日\时分\车型车号\任务ID(年月日时分车型车号)
	QString yyyyMMdd = m_Begin_Time.left(8);//年月日
	QString hhmm = m_Begin_Time.right(4);//时分
	QString CurrentImagePath = QString("C:\\Image\\FastScan_raw\\%1\\%2\\%3\\%4\\%5")
		.arg(m_Track_Rank)//股道ID
		.arg(yyyyMMdd)//年月日
		.arg(hhmm)//时分
		.arg(m_Car_Type + m_Car_Num)//车型车号
		.arg(m_Work_Num);//任务编号
	//CS&OPEN&CurrentImagePath（CurrentImagePath为当前图像存储路径）
	QString Cmd = QString("CS&OPEN&%1")
		.arg(CurrentImagePath);
	emit Write_String(ip, port, Cmd);
}
/*************************************************************************************************************************************************
**Function:停止采集
*************************************************************************************************************************************************/
void c_Fast_Scan_Remote::Stop()
{
	QString ip = c_Variable::g_Communicate_DB.value("Collector_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("Fast_Scan_Collector_Port").toInt();
	emit Write_String(ip, port, "PHOTO&FINISH");//发送采集结束指令
}
 