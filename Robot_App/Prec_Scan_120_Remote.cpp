#pragma execution_character_set("utf-8")
#include "Prec_Scan_120_Remote.h"

/*************************************************************************************************************************************************
**Function:构造函数(状态监视)
*************************************************************************************************************************************************/
c_Prec_Scan_120_Remote::c_Prec_Scan_120_Remote(c_Scan_Remote *parent) : c_Scan_Remote(parent)
{

}
/*************************************************************************************************************************************************
**Function:析构
*************************************************************************************************************************************************/
c_Prec_Scan_120_Remote::~c_Prec_Scan_120_Remote()
{
	m_Prec_Scan_Tran->deleteLater();
}
/*************************************************************************************************************************************************
**Function:开始运行
*************************************************************************************************************************************************/
void c_Prec_Scan_120_Remote::Init()
{
	m_Prec_Scan_Tran = new c_Robot_Server;
	QObject::connect(m_Prec_Scan_Tran, &c_Robot_Server::Init_Done, this, &c_Scan_Remote::Init);
	m_Prec_Scan_Tran->Init();
	//提示信息
	QObject::connect(m_Prec_Scan_Tran, &c_Robot_Server::Status, this, [=](int state) {emit Status(c_Variable::g_Current_Time + "->左精扫相机指令：" + c_Variable::TCP_Status(state)); });
	QObject::connect(m_Scan_Remote, &c_Scan_Client::Status, this, [=](int state) {emit Status(c_Variable::g_Current_Time + "->左精扫相机采集：" + c_Variable::TCP_Status(state)); });
	QObject::connect(m_Prec_Scan_Tran, &c_Robot_Server::Connect_Done, this, &c_Prec_Scan_120_Remote::Connect);
	QObject::connect(m_Prec_Scan_Tran, &c_Robot_Server::Connect_Loop, this, &c_Prec_Scan_120_Remote::Connect_Loop);
	QObject::connect(m_Prec_Scan_Tran, &c_Robot_Server::Disconnect_Done, this, &c_Scan_Remote::Disconnect_Device);
	QObject::connect(m_Prec_Scan_Tran, &c_Robot_Server::Read_String_Done, this, &c_Prec_Scan_120_Remote::Tran);
	QObject::connect(this, &c_Scan_Remote::Read_String_Done, m_Prec_Scan_Tran, &c_Robot_Server::Write_String);
	c_Prec_Scan_120_Remote::Connect_Server();
}
/*************************************************************************************************************************************************
**Function:操作接口
*************************************************************************************************************************************************/
void c_Prec_Scan_120_Remote::Connect()
{
	if (m_Scan_Remote_State.value("Connected").toBool()) { return; }
	QString ip = c_Variable::g_Communicate_DB.value("Local_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("Prec_Scan_120_Local_Port").toInt();
	emit Connect_Device(ip, port);
}
void c_Prec_Scan_120_Remote::Connect_Server()
{
	QString ip = c_Variable::g_Communicate_DB.value("Local_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("Prec_Scan_120_Tran_Port").toInt();
	m_Prec_Scan_Tran->Connect_Device(ip, port);
}
/*************************************************************************************************************************************************
**Function:循环连接
*************************************************************************************************************************************************/
void c_Prec_Scan_120_Remote::Connect_Loop()
{
	emit Status(c_Variable::g_Current_Time + "->左精扫服务：建立监听中");
	QTimer::singleShot(6000, this, &c_Prec_Scan_120_Remote::Connect_Server);
}
/*************************************************************************************************************************************************
**Function:自动采集转发协议
*************************************************************************************************************************************************/
void c_Prec_Scan_120_Remote::Tran(QString value)
{
	QString cmd;
	QString ip = c_Variable::g_Communicate_DB.value("Collector_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("Prec_Scan_120_Collector_Port").toInt();
	QString cmd_name = value.split("&", QString::SkipEmptyParts).at(0);
	if (cmd_name == "START")
	{
		/*START&股道ID&年月日&时分&车型车号&任务编号（年月日时分车型车号）
		*例：START&ID0&20211106&0940& CRH2A2411&202111060940CRH2A2411*/
		QString yyyyMMdd = m_Begin_Time.left(8);//年月日
		QString hhmm = m_Begin_Time.right(4);//时分
		cmd = QString("%1&%2&%3&%4&%5&%6").arg(cmd_name).arg(m_Track_Rank).arg(yyyyMMdd).arg(hhmm).arg(m_Car_Type + m_Car_Num).arg(m_Work_Num);
	}
	if (cmd_name == "PHOTO")
	{
		QString cmd_value = value.split("&", QString::SkipEmptyParts).at(1);
		if (cmd_value != "FINISH")
		{
			/*PHOTO&车厢号转向架号&原始图像名称
			例：PHOTO & 0101&T1-20211106- CRH2A2411-01-01-01-L-ZDZZ-05-08*/
			QString box_bio_num = value.split("&", QString::SkipEmptyParts).at(1);//车厢号转向架号
			QString photo_name = value.split("&", QString::SkipEmptyParts).at(2);//原始图像名称
			QString camera_num = photo_name.split("-", QString::SkipEmptyParts).at(0);//相机编号
			QString yyyyMMdd = QDateTime::currentDateTime().toString("yyyyMMdd");
			QString box_num = photo_name.split("-", QString::SkipEmptyParts).at(1);//车厢号
			QString bio_num = photo_name.split("-", QString::SkipEmptyParts).at(2);//转向架号
			QString axi_num = photo_name.split("-", QString::SkipEmptyParts).at(3);//轮轴号
			QString whe_num = photo_name.split("-", QString::SkipEmptyParts).at(4);//轮对号
			QString fir_num = photo_name.split("-", QString::SkipEmptyParts).at(5);//一级部件
			QString sec_num = photo_name.split("-", QString::SkipEmptyParts).at(6);//二级部件
			QString poi_num = photo_name.split("-", QString::SkipEmptyParts).at(7);//巡检点编号
			cmd = QString("%1&%2&%3-%4-%5-%6-%7-%8-%9-%10-%11-%12").arg(cmd_name).arg(box_bio_num)
				.arg(camera_num).arg(yyyyMMdd).arg(m_Car_Type + m_Car_Num).arg(box_num).arg(bio_num).arg(axi_num).arg(whe_num).arg(fir_num).arg(sec_num).arg(poi_num);
		}
	}
	emit Write_String(ip, port, cmd);
}