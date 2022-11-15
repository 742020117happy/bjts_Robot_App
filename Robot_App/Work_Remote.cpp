#pragma execution_character_set("utf-8")
#include "Work_Remote.h"
/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_Work_Remote::c_Work_Remote(QObject *parent) : QObject(parent)
{

}
/*************************************************************************************************************************************************
**Function:析构函数
*************************************************************************************************************************************************/
c_Work_Remote::~c_Work_Remote()
{

}
/*************************************************************************************************************************************************
**Function:初始化函数
*************************************************************************************************************************************************/
void c_Work_Remote::Init()
{
	QObject::connect(this, &c_Work_Remote::is_State_23, this, &c_Work_Remote::State_18_Loop);
	QObject::connect(this, &c_Work_Remote::is_State_25, this, &c_Work_Remote::State_19_Loop);
	c_Variable::msleep(500);
	Load_Status("等待巡检任务中");
}
/*************************************************************************************************************************************************
**Function:开始任务
*************************************************************************************************************************************************/
void c_Work_Remote::Start_Cmd(QJsonObject object)
{
	if (m_Work_Start) { return; }//只能触发一次
	QString Cmd_Name = object.value("Cmd_Name").toString();
	m_Work_Start = object.value("Value").toBool();
	//{"Cmd_Name":"Work_Start", "Track_Rank":"D98","Begin_Time":"202111060940","Car_Type":"CRH380A","Car_Num":"2411"，"Value":true}
	if (Cmd_Name == "Work_Start") {
		Load_Status("生成巡检任务" + QString(QJsonDocument(object).toJson()));
		m_Begin_Time = object.value("Begin_Time").toString();
		m_Car_Type = object.value("Car_Type").toString();
		m_Car_Num = object.value("Car_Num").toString();
		m_Work_Num = m_Begin_Time + m_Car_Type + m_Car_Num;
		Load_Status("初始化参数");
	}
	if (m_Work_Start) {
		Load_Status("加载巡检任务程序");
		QFile file;
		file.setFileName(QDir::currentPath() + "/Work_DB/Axis_Position.json");
		file.open(QFile::ReadOnly | QIODevice::Text);
		QByteArray Data = file.readAll();
		file.close();
		QJsonDocument Doc(QJsonDocument::fromJson(Data));
		QJsonObject Object = Doc.object();
		m_Work_Program = Object.value(m_Car_Type).toObject();
		m_Axis_1_Position = m_Work_Program.value("Axis_1_Position").toInt();
		m_Axis_2_Position = m_Work_Program.value("Axis_2_Position").toInt();
		m_Header_Position = m_Work_Program.value("Header_Position").toInt();
		c_Variable::msleep(1000);//等待1秒
		emit Work_Init();
	}
}
/*************************************************************************************************************************************************
**Function:执行任务（还需改良编组适应能力）
*************************************************************************************************************************************************/
void c_Work_Remote::Work_Start()
{
	if (!m_Work_Start) { return; }
	Load_Status("巡检任务开始执行");
	Step_0("1", "A", "1");//1-A-1   完成第1个间1位巡检
	Step_1("1", "A", "2");//1-A-2	完成第1个间2位巡检
	Step_2("1", "B", "1");//1-B-1  	完成第2个间1位巡检
/**
	Step_1("1", "B", "2");//1-B-2   完成第2个间2位巡检
	Step_2("2", "A", "1");//2-A-1  	完成第3个间1位巡检
	Step_1("2", "A", "2");//2-A-2   完成第3个间2位巡检
	Step_2("2", "B", "1");//2-B-1  	完成第4个间1位巡检
	Step_1("2", "B", "2");//2-B-2   完成第4个间2位巡检
	Step_2("3", "A", "1");//3-A-1  	完成第5个间1位巡检
	Step_1("3", "A", "2");//3-A-2   完成第5个间2位巡检
	Step_2("3", "B", "1");//3-B-1  	完成第6个间1位巡检
	Step_1("3", "B", "2");//3-B-2   完成第6个间2位巡检
	Step_2("4", "A", "1");//4-A-1  	完成第7个间1位巡检
	Step_1("4", "A", "2");//4-A-2   完成第7个间2位巡检
	Step_2("4", "B", "1");//4-B-1  	完成第8个间1位巡检
	Step_1("4", "B", "2");//4-B-2   完成第8个间2位巡检
	Step_2("5", "A", "1");//5-A-1  	完成第9个间1位巡检
	Step_1("5", "A", "2");//5-A-2   完成第9个间2位巡检
	Step_2("5", "B", "1");//5-B-1  	完成第10个间1位巡检
	Step_1("5", "B", "2");//5-B-2   完成第10个间2位巡检
	Step_2("6", "A", "1");//6-A-1  	完成第11个间1位巡检
	Step_1("6", "A", "2");//6-A-2   完成第11个间2位巡检
	Step_2("6", "B", "1");//6-B-1  	完成第12个间1位巡检
	Step_1("6", "B", "2");//6-B-2   完成第12个间2位巡检
	Step_2("7", "A", "1");//7-A-1  	完成第13个间1位巡检
	Step_1("7", "A", "2");//7-A-2   完成第13个间2位巡检
	Step_2("7", "B", "1");//7-B-1  	完成第14个间1位巡检
	Step_1("7", "B", "2");//7-B-2   完成第14个间2位巡检
	Step_2("8", "A", "1");//8-A-1  	完成第15个间1位巡检
	Step_1("8", "A", "2");//8-A-2   完成第15个间2位巡检
	Step_2("8", "B", "1");//8-B-1  	完成第16个间1位巡检
**/
	Step_3("8", "B", "2");//8-B-2   完成最后一个间2位巡检
}
/*************************************************************************************************************************************************
**Function:直到第一轴巡检结束
*************************************************************************************************************************************************/
void c_Work_Remote::Step_0(QString Carbox_Num, QString Bogie_Num, QString Axis_Num)
{
	m_Work_Stage = "Fast_Scan";
	m_Carbox_Num = Carbox_Num;
	m_Bogie_Num = Bogie_Num;
	m_Axis_Num = Axis_Num;
	Load_Status("更新巡检状态");
	if (!m_Work_Start) { return; }

	Load_Status("检查通讯控制");
	if (!m_State_1) { State_1_Loop(); }//通讯
	c_Variable::msleep(500);//等待0.5秒

	Load_Status("检查上电状态");
	if (!m_State_7) { State_8_Loop(); }//上电
	c_Variable::msleep(500);//等待0.5秒

	Load_Status("检查充电状态");
	if (m_State_31) { State_20_Loop(); }//结束自动充电
	c_Variable::msleep(500);//等待0.5秒

	Load_Status("检查机械臂控制柜是否开机");
	if (!m_State_6) { State_7_Loop(); }//机械臂开机
	c_Variable::msleep(500);//等待0.5秒

	Load_Status("系统自检");
	if (!m_State_32) { State_22_Loop(); }
	c_Variable::msleep(500);//等待0.5秒

	Load_Status("检查主动力当前位置");
	if (!m_State_5) { State_2_Loop(); }//位置清零
	c_Variable::msleep(500);//等待0.5秒

	Load_Status("检查风刀状态");
	if (!m_State_18) { State_21_Loop(); }//打开风刀
	c_Variable::msleep(500);//等待0.5秒

	Load_Status("清除历史定位数据");
	emit Fuction_8();//故障复位
	c_Variable::msleep(500);//等待0.5秒

	Load_Status("系统初始化完成");

	emit Fast_Scan_Start();//快扫开始采集
	Load_Status("快扫开始采集");
	c_Variable::msleep(500);//等待0.5秒

	State_4_Loop();//正向连续
	c_Variable::msleep(500);//等待0.5秒

	emit Fuction_14();//停止
	Load_Status("正向连续运行停止");
	c_Variable::msleep(500);//等待0.5秒

	emit Fast_Scan_Stop();//快扫结束采集
	Load_Status("快扫结束采集");
	c_Variable::msleep(500);//等待0.5秒

	emit Fuction_22_Reset();//关闭风刀
	Load_Status("风刀关闭");
	c_Variable::msleep(500);//等待0.5秒

	if (!m_State_23) { State_12_Loop(); }//机械臂上电
	c_Variable::msleep(500);//等待0.5秒

	State_5_Loop();//反向断续
	c_Variable::msleep(500);//等待0.5秒

	if (!m_State_24) { State_14_Loop(); }//机械臂上使能
	c_Variable::msleep(500);//等待0.5秒

	State_9_Loop(m_Axis_1_Position);//轴一运动
	c_Variable::msleep(1000);//等待1秒

	State_11_Loop(20, 200);//升降台上升
	c_Variable::msleep(1000);//等待1秒

	Load_Status("间1位巡检");
	QString program_name = QString("%1_%2_%3_间1").arg(m_Car_Type).arg(m_Carbox_Num + m_Bogie_Num).arg(m_Axis_Num);//CRH380A_01A_1_间1
	//State_16_Loop(program_name);
	c_Variable::msleep(1000);//等待1秒

	State_11_Loop(20, 0);//升降台下降升
	c_Variable::msleep(1000);//等待1秒
}
/*************************************************************************************************************************************************
**Function:直到间二位巡检结束
*************************************************************************************************************************************************/
void c_Work_Remote::Step_1(QString Carbox_Num, QString Bogie_Num, QString Axis_Num)
{
	m_Work_Stage = "Prec_Scan";
	m_Carbox_Num = Carbox_Num;
	m_Bogie_Num = Bogie_Num;
	m_Axis_Num = Axis_Num;
	Load_Status("更新巡检状态");
	if (!m_Work_Start) { return; }

	State_6_Loop();//返向连续运行再启动
	c_Variable::msleep(1000);//等待1秒

	State_10_Loop(m_Axis_2_Position);//轴二运动
	c_Variable::msleep(1000);//等待1秒

	State_11_Loop(20, 200);//升降台上升
	c_Variable::msleep(1000);//等待1秒

	Load_Status("间2位巡检");
	QString program_name = QString("%1_%2_%3_间2").arg(m_Car_Type).arg(m_Carbox_Num + m_Bogie_Num).arg(m_Axis_Num);//CRH380A_01A_2_间2
	//State_16_Loop(program_name);
	c_Variable::msleep(1000);//等待1秒

	State_11_Loop(20, 0);//升降台下降升
	c_Variable::msleep(1000);//等待1秒
}
/*************************************************************************************************************************************************
**Function:直到间一位巡检结束
*************************************************************************************************************************************************/
void c_Work_Remote::Step_2(QString Carbox_Num, QString Bogie_Num, QString Axis_Num)
{
	m_Work_Stage = "Prec_Scan";
	m_Carbox_Num = Carbox_Num;
	m_Bogie_Num = Bogie_Num;
	m_Axis_Num = Axis_Num;
	Load_Status("更新巡检状态");
	if (!m_Work_Start) { return; }

	State_6_Loop();//返向连续运行再启动
	c_Variable::msleep(1000);//等待1秒

	State_9_Loop(m_Axis_1_Position);//轴一运动
	c_Variable::msleep(1000);//等待1秒

	State_11_Loop(20, 200);//升降台上升
	c_Variable::msleep(1000);//等待1秒

	Load_Status("间1位巡检");
	QString program_name = QString("%1_%2_%3_间1").arg(m_Car_Type).arg(m_Carbox_Num + m_Bogie_Num).arg(m_Axis_Num);//CRH380A_01A_1_间1
	//State_16_Loop(program_name);
	c_Variable::msleep(1000);//等待1秒

	State_11_Loop(20, 0);//升降台下降升
	c_Variable::msleep(1000);//等待1秒
}
/*************************************************************************************************************************************************
**Function:直到巡检结束
*************************************************************************************************************************************************/
void c_Work_Remote::Step_3(QString Carbox_Num, QString Bogie_Num, QString Axis_Num)
{
	m_Work_Stage = "Prec_Scan";
	m_Carbox_Num = Carbox_Num;
	m_Bogie_Num = Bogie_Num;
	m_Axis_Num = Axis_Num;
	Load_Status("更新巡检状态");
	if (!m_Work_Start) { return; }

	State_6_Loop();//返向连续运行再启动
	c_Variable::msleep(1000);//等待1秒

	//State_11_Loop(20, 200);//升降台上升
	c_Variable::msleep(1000);//等待1秒

	Load_Status("南头主排障器检测");
	QString program_name = QString("%1_南头主排障器").arg(m_Car_Type);//CRH380A_南头主排障器
	//State_16_Loop(program_name);
	c_Variable::msleep(1000);//等待1秒

	//State_11_Loop(20, 0);//升降台下降升
	c_Variable::msleep(1000);//等待1秒

	State_10_Loop(m_Axis_2_Position);//轴二运动
	c_Variable::msleep(1000);//等待1秒

	State_11_Loop(20, 200);//升降台上升
	c_Variable::msleep(1000);//等待1秒

	Load_Status("间2位巡检");
	program_name = QString("%1_%2_%3_间2").arg(m_Car_Type).arg(m_Carbox_Num + m_Bogie_Num).arg(m_Axis_Num);//CRH380A_08B_2_间2
	//State_16_Loop(program_name);
	c_Variable::msleep(1000);//等待1秒

	State_11_Loop(20, 0);//升降台下降升
	c_Variable::msleep(1000);//等待1秒

	State_9_Loop(m_Header_Position);//轴一运动
	c_Variable::msleep(1000);//等待1秒

	//State_11_Loop(20, 200);//升降台上升
	c_Variable::msleep(1000);//等待1秒

	Load_Status("南头车头巡检");
	program_name = QString("%1_南头车头").arg(m_Car_Type);//CRH380A_南头车头
	//State_16_Loop(program_name);
	c_Variable::msleep(1000);//等待1秒

	//State_11_Loop(20, 0);//升降台下降升
	c_Variable::msleep(1000);//等待1秒

	State_17_Loop();//巡检完成停车
	c_Variable::msleep(500);//等待0.5秒

	emit Fuction_14();//急停
	c_Variable::msleep(1000);//等待1秒

	//emit Fuction_20();//返回充电原点
	c_Variable::msleep(500);//等待0.5秒

	m_Work_Stage = "Debug";
	m_Carbox_Num = "1";
	m_Bogie_Num = "A";
	m_Axis_Num = "1";
	for (int i = 0; i<m_Status.size(); i++){
		m_Status.removeFirst();
	}
	Load_Status("巡检任务完成，返回充电位置，检查充电状态");
	m_Work_Start = false;
	Load_Status("等待巡检任务中");
}
/*************************************************************************************************************************************************
**精扫相机执行情况
*************************************************************************************************************************************************/
void c_Work_Remote::Prec_Scan_Write_Done(QString ip, int port, QString value)
{
	if (!m_Work_Start) { return; }
	QString cmd_name = value.split("&", QString::SkipEmptyParts).at(0);
	QString cmd_value = value.split("&", QString::SkipEmptyParts).at(1);
	if (cmd_name == "PHOTO" && cmd_value != "FINISH")
	{
		QString photo_name = value.split("&", QString::SkipEmptyParts).at(2);
		m_Work_Stage = "Prec_Scan";//巡检工作阶段
		m_Carbox_Num = photo_name.split("-", QString::SkipEmptyParts).at(1);//车厢号
		m_Bogie_Num = photo_name.split("-", QString::SkipEmptyParts).at(2);//转向架号
		m_Axis_Num = photo_name.split("-", QString::SkipEmptyParts).at(3);//轮轴号
		if (port == 7001)
		{
			m_Wheelset_Num_120 = photo_name.split("-", QString::SkipEmptyParts).at(4);//机械臂120轮对号
			m_Primary_Components_120 = photo_name.split("-", QString::SkipEmptyParts).at(5);//机械臂120巡检一级部件
			m_Secondary_Components_120 = photo_name.split("-", QString::SkipEmptyParts).at(6);//机械臂120巡检二级部件
			m_Point_Num_120 = photo_name.split("-", QString::SkipEmptyParts).at(7);//机械臂120巡检点
			Load_Status("左精扫采集");
		}
		if (port == 7101)
		{
			m_Wheelset_Num_121 = photo_name.split("-", QString::SkipEmptyParts).at(4);//机械臂121轮对号
			m_Primary_Components_121 = photo_name.split("-", QString::SkipEmptyParts).at(5);//机械臂121巡检一级部件
			m_Secondary_Components_121 = photo_name.split("-", QString::SkipEmptyParts).at(6);//机械臂121巡检二级部件
			m_Point_Num_121 = photo_name.split("-", QString::SkipEmptyParts).at(7);//机械臂121巡检点
			Load_Status("右精扫采集");
		}
	}
	if (cmd_name == "PHOTO" && cmd_value == "FINISH")
	{
		if (port == 7001) { m_Jaka_120 = true; }
		if (port == 7101) { m_Jaka_121 = true; }
	}
	if (m_Jaka_120 && m_Jaka_121)
	{
		m_Jaka_120 = false;
		m_Jaka_121 = false;
		Load_Status("精扫采集完成");
		emit Prec_Scan_Done();
	}
}
//获取系统状态
void c_Work_Remote::System_Scan(QJsonObject db)
{
	if (!m_Work_Start) { return; }
	m_RGV_State = db.value("RGV_State").toObject();//更新
	m_Jaka_120_Remote_State = db.value("Jaka_120_Remote_State").toObject();//更新
	m_Jaka_121_Remote_State = db.value("Jaka_121_Remote_State").toObject();//更新
	m_Jaka_120_Monitor_State = db.value("Jaka_120_Monitor_State").toObject();//更新
	m_Jaka_121_Monitor_State = db.value("Jaka_121_Monitor_State").toObject();//更新
	m_Hypersen_30_State = db.value("Hypersen_30_State").toObject();//更新
	m_Hypersen_31_State = db.value("Hypersen_31_State").toObject();//更新
	m_Meijidenki_20_State = db.value("Meijidenki_20_State").toObject();//更新
	m_Meijidenki_21_State = db.value("Meijidenki_21_State").toObject();//更新
	m_Fast_Scan_State = db.value("Fast_Scan_State").toObject();//更新

	m_RGV_Connected = m_RGV_State.value("Connected").toBool();//状态
	m_Jaka_120_Remote_Connected = m_Jaka_120_Remote_State.value("Connected").toBool();//状态
	m_Jaka_121_Remote_Connected = m_Jaka_121_Remote_State.value("Connected").toBool();//状态
	m_Jaka_120_Monitor_Connected = m_Jaka_120_Monitor_State.value("Connected").toBool();//状态
	m_Jaka_121_Monitor_Connected = m_Jaka_121_Monitor_State.value("Connected").toBool();//状态
	m_Hypersen_30_Connected = m_Hypersen_30_State.value("Connected").toBool();//状态
	m_Hypersen_31_Connected = m_Hypersen_31_State.value("Connected").toBool();//状态
	m_Meijidenki_20_Connected = m_Meijidenki_20_State.value("Connected").toBool();//状态
	m_Meijidenki_21_Connected = m_Meijidenki_21_State.value("Connected").toBool();//状态
	m_Fast_Scan_Connected = m_Fast_Scan_State.value("Connected").toBool();//状态

	if (m_RGV_Connected &&
		m_Jaka_120_Remote_Connected &&
		m_Jaka_121_Remote_Connected &&
		m_Jaka_120_Monitor_Connected &&
		m_Jaka_121_Monitor_Connected &&
		m_Hypersen_30_Connected &&
		m_Hypersen_31_Connected &&
		m_Meijidenki_20_Connected &&
		m_Meijidenki_21_Connected &&
		m_Fast_Scan_Connected && !m_State_32) {
		emit is_State_28();
		m_State_32 = true;
	}
	else {
		m_State_32 = false;
	}
	if (m_RGV_Connected)
	{
		m_json = m_RGV_State.value("DB").toObject();
		m_DiscreteInputs = m_json.value("DiscreteInputs").toObject();

		if (!m_State_1 && m_DiscreteInputs.value("0").toInt() == 1) { emit is_State_1(); }//通信控制
		if (!m_State_2 && m_DiscreteInputs.value("11").toInt() == 1) { emit is_State_2(); }//正向连续运动
		if (!m_State_3 && m_DiscreteInputs.value("12").toInt() == 1) { emit is_State_3(); }//返向断续运动
		if (!m_State_4 && m_DiscreteInputs.value("13").toInt() == 1) { emit is_State_4(); }//返向断续再启动
		if (!m_State_6 &&  m_DiscreteInputs.value("15").toInt() == 1 && m_DiscreteInputs.value("18").toInt() == 1) { emit is_State_6(); }//机器人开机
		if (!m_State_7 && m_DiscreteInputs.value("19").toInt() == 1) { emit is_State_7(); }//上电
		if (!m_State_8 && m_DiscreteInputs.value("20").toInt() == 1) { emit is_State_8(); }//通讯完成
		if (!m_State_9 && m_DiscreteInputs.value("21").toInt() == 1) { emit is_State_9(); }//正向完成停车
		if (!m_State_10 && m_DiscreteInputs.value("22").toInt() == 1) { emit is_State_10(); }//反向完成停车
		if (m_State_11 && m_DiscreteInputs.value("24").toInt() == 0) { emit is_State_11(); }//轴1移动
		if (m_State_12 && m_DiscreteInputs.value("25").toInt() == 0) { emit is_State_12(); }//轴2移动
		if (!m_State_13 && m_DiscreteInputs.value("90").toInt() == 1) { emit is_State_13(); }//返向运行遇到雷达停车
		if (!m_State_14 && m_DiscreteInputs.value("91").toInt() == 1) { emit is_State_14(); }//正向运行遇到雷达停车
		if (!m_State_15 && m_DiscreteInputs.value("92").toInt() == 1) { emit is_State_15(); }//返向到达轮轴停车
		if (!m_State_16 && m_DiscreteInputs.value("93").toInt() == 1 && m_DiscreteInputs.value("94").toInt() == 1) { emit is_State_16(); }//升降台位置到达
		if (!m_State_17 && m_DiscreteInputs.value("129").toInt() == 1) { emit is_State_17(); }//动力伺服就绪
		if (!m_State_18 && m_DiscreteInputs.value("262").toInt() == 1) { emit is_State_18(); }//风刀电机
		if (m_State_31 && m_DiscreteInputs.value("258").toInt() == 0) { emit is_State_27(); } //自动充电结束

		m_State_1 = m_DiscreteInputs.value("0").toInt();//通信控制
		m_State_2 = m_DiscreteInputs.value("11").toInt();//正向连续运动
		m_State_3 = m_DiscreteInputs.value("12").toInt();//返向断续运动
		m_State_4 = m_DiscreteInputs.value("13").toInt();//返向断续再启动
		m_State_6 = m_DiscreteInputs.value("15").toInt() == 1 && m_DiscreteInputs.value("18").toInt() == 1;//左机器人开机
		m_State_7 = m_DiscreteInputs.value("19").toInt();//上电
		m_State_8 = m_DiscreteInputs.value("20").toInt();//通讯完成
		m_State_9 = m_DiscreteInputs.value("21").toInt();//正向完成停车
		m_State_10 = m_DiscreteInputs.value("22").toInt();//反向完成停车
		m_State_11 = m_DiscreteInputs.value("24").toInt();//轴1移动
		m_State_12 = m_DiscreteInputs.value("25").toInt();//轴2移动
		m_State_13 = m_DiscreteInputs.value("90").toInt();//返向运行遇到雷达停车
		m_State_14 = m_DiscreteInputs.value("91").toInt();//正向运行遇到雷达停车
		m_State_15 = m_DiscreteInputs.value("92").toInt();//返向到达轮轴停车
		m_State_16 = m_DiscreteInputs.value("93").toInt() == 1 && m_DiscreteInputs.value("94").toInt() == 1;//升降台位置到达
		m_State_17 = m_DiscreteInputs.value("129").toInt();//动力伺服就绪
		m_State_18 = m_DiscreteInputs.value("262").toInt();//风刀电机
		m_State_31 = m_DiscreteInputs.value("258").toInt();//自动充电状态

		m_InputRegisters = m_json.value("InputRegisters").toObject();
		m_RGV_Position = c_Variable::Short_To_Float(m_InputRegisters.value("40").toInt(), m_InputRegisters.value("41").toInt());
		if (!m_State_5 && m_RGV_Position == 0) { emit is_State_5(); }//主动力位置清零
		if (m_RGV_Position == 0){m_State_5 = true;}
		if (m_RGV_Position != 0){m_State_5 = false;}
	}
	if (m_Jaka_120_Monitor_Connected && m_Jaka_121_Monitor_Connected)
	{
		m_json = m_Jaka_120_Monitor_State.value("DB").toObject();
		//1 代表机器人下电 2 代表机器人上电 3 代表机器人下使能 4 代表机器人上使能
		m_task_state = m_json.value("task_state").toInt();
		switch (m_task_state)
		{
		case 1:
			m_State_19 = false;//下电状态
			m_State_20 = false;//下使能状态
			break;
		case 2:
			m_State_19 = true;//上电状态
			m_State_20 = false;//下使能状态
			break;
		case 3:
			m_State_19 = true;//上电状态
			m_State_20 = false;//下使能状态
			break;
		case 4:
			m_State_19 = true;//上电状态
			m_State_20 = true;//上使能状态
			break;
		default:
			break;
		}

		m_json = m_Jaka_121_Monitor_State.value("DB").toObject();
		//1 代表机器人下电 2 代表机器人上电 3 代表机器人下使能 4 代表机器人上使能
		m_task_state = m_json.value("task_state").toInt();
		switch (m_task_state)
		{
		case 1:
			m_State_21 = false;//下电状态
			m_State_22 = false;//下使能状态
			break;
		case 2:
			m_State_21 = true;//上电状态
			m_State_22 = false;//下使能状态
			break;
		case 3:
			m_State_21 = true;//上电状态
			m_State_22 = false;//下使能状态
			break;
		case 4:
			m_State_21 = true;//上电状态
			m_State_22 = true;//上使能状态
			break;
		default:
			break;
		}

		if (!m_State_23 && m_State_19 && m_State_21) { emit is_State_19(); }//机器人上电
		if (!m_State_24 && m_State_20 && m_State_22) { emit is_State_20(); }//机器人上使能
		if (m_State_23 && !m_State_19 && !m_State_21) { emit is_State_21(); }//机器人下电
		if (m_State_24 && !m_State_20 && !m_State_22) { emit is_State_22(); }//机器人下使能
		if (m_State_19 && m_State_21) { m_State_23 = true; }
		else { m_State_23 = false; }//上电状态更新
		if (m_State_20 && m_State_22) { m_State_24 = true; }
		else { m_State_24 = false; }//使能状态更新	
	}
	if (m_Hypersen_30_Connected && m_State_2 && !m_State_9) {
		m_json = m_Hypersen_30_State.value("DB").toObject();

		m_Array = m_json.value("0").toArray();
		m_roi_all_aver_amp = m_Array.at(4).toInt();//ROI_0平均幅值

		if (m_State_25 && m_count_1 == 30) { emit is_State_23(); m_count_1 = 40; }//检测到障碍，持续1分钟，发送障碍信号，计数障碍解除
		if (!m_State_25 && m_count_1 == 70) { emit is_State_24(); m_count_1 = 0; }//障碍解除，持续1分钟，发送障碍解除信号，计数障碍检测

		if (m_roi_all_aver_amp > 30 && m_count_1 < 40) { m_State_25 = true;  m_count_1++; }//如果到达阈值，且次数未到达，检测到障碍，且累计次数
		if (m_roi_all_aver_amp < 30 && m_count_1 >= 40) { m_State_25 = false;  m_count_1++; }//如果未到达阈值，且以检测到障碍，障碍接触，且累计次数

		if (m_State_25 && m_roi_all_aver_amp < 30) { m_count_1 = 40; }//如果监测到障碍，累计次数中断，则重新累计
		if (!m_State_25 && m_roi_all_aver_amp > 30) { m_count_1 = 0; }//如果障碍解除，累计次数中断，则重新累计

		m_Array = m_json.value("1").toArray();
		m_roi_all_aver_amp = m_Array.at(4).toInt();//ROI_0平均幅值

		if (m_State_26 && m_count_2 == 30) { emit is_State_23(); m_count_2 = 40; }//检测到障碍，持续1分钟，发送障碍信号，计数障碍解除
		if (!m_State_26 && m_count_2 == 70) { emit is_State_24(); m_count_2 = 0; }//障碍解除，持续1分钟，发送障碍解除信号，计数障碍检测

		if (m_roi_all_aver_amp > 30 && m_count_2 < 40) { m_State_26 = true;  m_count_2++; }//如果到达阈值，且次数未到达，检测到障碍，且累计次数
		if (m_roi_all_aver_amp < 30 && m_count_2 >= 40) { m_State_26 = false;  m_count_2++; }//如果未到达阈值，且以检测到障碍，障碍接触，且累计次数

		if (m_State_26 && m_roi_all_aver_amp < 30) { m_count_2 = 40; }//如果监测到障碍，累计次数中断，则重新累计
		if (!m_State_26 && m_roi_all_aver_amp > 30) { m_count_2 = 0; }//如果障碍解除，累计次数中断，则重新累计
	}
	if (m_Hypersen_31_Connected && !m_State_10 && !m_State_15 && m_State_3 || m_State_4) {
		m_json = m_Hypersen_30_State.value("DB").toObject();

		m_Array = m_json.value("0").toArray();
		m_roi_all_aver_amp = m_Array.at(4).toInt();//ROI_0平均幅值

		if (m_State_27 && m_count_3 == 30) { emit is_State_25(); m_count_3 = 40; }//检测到障碍，持续1分钟，发送障碍信号，计数障碍解除
		if (!m_State_27 && m_count_3 == 70) { emit is_State_26(); m_count_3 = 0; }//障碍解除，持续1分钟，发送障碍解除信号，计数障碍检测

		if (m_roi_all_aver_amp > 30 && m_count_3 < 40) { m_State_27 = true;  m_count_3++; }//如果到达阈值，且次数未到达，检测到障碍，且累计次数
		if (m_roi_all_aver_amp < 30 && m_count_3 >= 40) { m_State_27 = false;  m_count_3++; }//如果未到达阈值，且以检测到障碍，障碍接触，且累计次数

		if (m_State_27 && m_roi_all_aver_amp < 30) { m_count_3 = 40; }//如果监测到障碍，累计次数中断，则重新累计
		if (!m_State_27 && m_roi_all_aver_amp > 30) { m_count_3 = 0; }//如果障碍解除，累计次数中断，则重新累计

		m_Array = m_json.value("1").toArray();
		m_roi_all_aver_amp = m_Array.at(4).toInt();//ROI_0平均幅值

		if (m_State_28 && m_count_4 == 30) { emit is_State_25(); m_count_4 = 40; }//检测到障碍，持续1分钟，发送障碍信号，计数障碍解除
		if (!m_State_28 && m_count_4 == 70) { emit is_State_26(); m_count_4 = 0; }//障碍解除，持续1分钟，发送障碍解除信号，计数障碍检测

		if (m_roi_all_aver_amp > 30 && m_count_4 < 40) { m_State_28 = true;  m_count_4++; }//如果到达阈值，且次数未到达，检测到障碍，且累计次数
		if (m_roi_all_aver_amp < 30 && m_count_4 >= 40) { m_State_28 = false;  m_count_4++; }//如果未到达阈值，且以检测到障碍，障碍接触，且累计次数

		if (m_State_28 && m_roi_all_aver_amp < 30) { m_count_4 = 40; }//如果监测到障碍，累计次数中断，则重新累计
		if (!m_State_28 && m_roi_all_aver_amp > 30) { m_count_4 = 0; }//如果障碍解除，累计次数中断，则重新累计
	}
}
//通信控制（线圈，工控机立即复位）
void c_Work_Remote::State_1_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_1, &loop, &QEventLoop::quit);
	emit Fuction_0_Set();
	Load_Status("通信控制开始");
	loop.exec();
	Load_Status("通信控制完成");
}
//主动力位置清零（线圈，工控机立即复位）
void c_Work_Remote::State_2_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_5, &loop, &QEventLoop::quit);
	emit Fuction_7();
	Load_Status("主动力位置清零中");
	loop.exec();
	Load_Status("主动力位置清零完成");
}
//主动力故障复位（线圈，工控机立即复位）
void c_Work_Remote::State_3_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_17, &loop, &QEventLoop::quit);
	emit Fuction_8();
	Load_Status("主动力故障复位中");
	loop.exec();
	Load_Status("主动力故障复位完成");
}
//正向连续运行（线圈，工控机立即复位）
void c_Work_Remote::State_4_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_9, &loop, &QEventLoop::quit);
	emit Fuction_11();
	Load_Status("正向连续运行开始");
	loop.exec();
	Load_Status("正向连续运行完成");
}
//返向连续运行（线圈，工控机立即复位）
void c_Work_Remote::State_5_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_15, &loop, &QEventLoop::quit);
	emit Fuction_12();
	Load_Status("返向连续运行开始");
	loop.exec();
	Load_Status("返向连续运行完成");
}
//返向连续运行再启动（线圈，工控机立即复位）
void c_Work_Remote::State_6_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_15, &loop, &QEventLoop::quit);
	emit Fuction_13();
	Load_Status("返向连续运行再启动开始");
	loop.exec();
	Load_Status("返向连续运行再启动完成");
}
//机器人开机（线圈，工控机立即复位）
void c_Work_Remote::State_7_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_6, &loop, &QEventLoop::quit);
	emit Fuction_15();
	Load_Status("机器人开机中");
	loop.exec();
	Load_Status("机器人开机完成");
}
//上电（线圈，点控）
void c_Work_Remote::State_8_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_7, &loop, &QEventLoop::quit);
	emit Fuction_19_Set();
	Load_Status("上电中");
	loop.exec();
	c_Variable::msleep(10000);//等待10秒
	Load_Status("上电完成");
}
//轴1移动（线圈，工控机立即复位）
void c_Work_Remote::State_9_Loop(int position)
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_11, &loop, &QEventLoop::quit);
	emit Fuction_24(position);
	Load_Status("轴1移动开始");
	loop.exec();
	Load_Status("轴1移动完成");
}
//轴2移动（线圈，工控机立即复位）
void c_Work_Remote::State_10_Loop(int position)
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_12, &loop, &QEventLoop::quit);
	emit Fuction_25(position);
	Load_Status("轴2移动开始");
	loop.exec();
	Load_Status("轴2移动完成");
}
//升降台移动
void c_Work_Remote::State_11_Loop(int speed, int position)
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_16, &loop, &QEventLoop::quit);
	emit Date_1(position);//左升降台位置
	emit Date_3(position);//右升降台位置
	emit Date_0(speed);//左升降台速度
	emit Date_2(speed);//右升降台速度
	Load_Status("升降台移动开始");
	loop.exec();
	c_Variable::msleep(1000);//等待1秒
	emit Date_0(0);//左升降台速度
	emit Date_2(0);//右升降台速度
	Load_Status("升降台移动完成");
}
//机械臂上电
void c_Work_Remote::State_12_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_19, &loop, &QEventLoop::quit);
	emit Jaka_120_power_on();
	emit Jaka_121_power_on();
	Load_Status("机械臂上电中");
	c_Variable::msleep(500);//等待0.5秒
	loop.exec();
	Load_Status("机械臂上电完成");
}
//机械臂下电
void c_Work_Remote::State_13_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_21, &loop, &QEventLoop::quit);
	emit Jaka_120_power_off();
	emit Jaka_121_power_on();
	Load_Status("机械臂下电中");
	c_Variable::msleep(500);//等待0.5秒
	loop.exec();
	Load_Status("机械臂下电完成");
}
//机械臂使能
void c_Work_Remote::State_14_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_20, &loop, &QEventLoop::quit);
	emit Jaka_120_enable();
	emit Jaka_121_enable();
	Load_Status("机械臂使能中");
	c_Variable::msleep(500);//等待0.5秒
	loop.exec();
	Load_Status("机械臂使能完成");
}
//机械臂下使能
void c_Work_Remote::State_15_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_22, &loop, &QEventLoop::quit);
	emit Jaka_120_disable();
	emit Jaka_121_disable();
	Load_Status("机械臂下使能中");
	c_Variable::msleep(500);//等待0.5秒
	loop.exec();
	Load_Status("机械臂下使能完成");
}
//机械臂运行程序
void c_Work_Remote::State_16_Loop(QString name)
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::Prec_Scan_Done, &loop, &QEventLoop::quit);
	Jaka_120_play_program(name);
	Jaka_121_play_program(name);
	Load_Status("机械臂运行程序中：" + name);
	c_Variable::msleep(500);//等待0.5秒
	loop.exec();
	Load_Status("机械臂运行程序完成");
}
//巡检完成停车
void c_Work_Remote::State_17_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_10, &loop, &QEventLoop::quit);
	emit Fuction_13();
	Load_Status("巡检即将完成");
	loop.exec();
	Load_Status("巡检完成停车");
}
//正向障碍停车
void c_Work_Remote::State_18_Loop()
{
	if (!m_Work_Start || m_State_29) { return; }
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_24, &loop, &QEventLoop::quit);
	emit Fuction_14();
	Load_Status("面阵雷达正向运行检查到障碍");
	m_State_29 = true;
	loop.exec();
	m_State_29 = false;
	Load_Status("面阵雷达正向运行检查到障碍解除");
	// emit Fuction_11();
}
//反向障碍停车
void c_Work_Remote::State_19_Loop()
{
	if (!m_Work_Start || m_State_30) { return; }
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_26, &loop, &QEventLoop::quit);
	emit Fuction_14();
	Load_Status("面阵雷达反向运行检查到障碍");
	m_State_30 = true;
	loop.exec();
	m_State_30 = false;
	Load_Status("面阵雷达反向运行检查到障碍解除");
	//if (m_State_4) { emit Fuction_13(); }//继续反向断续再启动
	//if (!m_State_4) { emit Fuction_12(); }//继续反向断续运动
}
//结束自动充电
void c_Work_Remote::State_20_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_27, &loop, &QEventLoop::quit);
	emit Fuction_27();
	Load_Status("断开充电继电器");
	loop.exec();
	Load_Status("等待充电桩缩回");
	c_Variable::msleep(10000);//等待10秒
	Load_Status("自动充电结束");
}
//风刀电源
void c_Work_Remote::State_21_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_18, &loop, &QEventLoop::quit);
	emit Fuction_22_Set();
	Load_Status("打开风刀");
	loop.exec();
	Load_Status("风刀打开完成");
}
//系统就绪
void c_Work_Remote::State_22_Loop()
{
	QEventLoop loop;
	QObject::connect(this, &c_Work_Remote::is_State_28, &loop, &QEventLoop::quit);
	Load_Status("等待系统自检");
	loop.exec();
	Load_Status("系统自检完成");
}
//载入巡检信息
void c_Work_Remote::Load_Status(QString state)
{
	m_Status.append(state);
	m_Work_Remote_State.insert("Connected", true);
	QJsonObject db;
	db.insert("Robot_Num", m_Robot_Num);
	db.insert("Robot_Num", m_Robot_Num);
	db.insert("Track_Rank", m_Track_Rank);
	db.insert("Begin_Time", m_Begin_Time);
	db.insert("Work_Num", m_Work_Num);
	db.insert("Car_Type", m_Car_Type);
	db.insert("Car_Num", m_Car_Num);
	db.insert("Carbox_Num", m_Carbox_Num);
	db.insert("Bogie_Num", m_Bogie_Num);
	db.insert("Axis_Num", m_Axis_Num);
	db.insert("Work_Stage", m_Work_Stage);
	db.insert("Wheelset_Num_120", m_Wheelset_Num_120);
	db.insert("Wheelset_Num_121", m_Wheelset_Num_121);
	db.insert("Point_Num_120", m_Point_Num_120);
	db.insert("Point_Num_121", m_Point_Num_121);
	db.insert("Primary_Components_120", m_Primary_Components_120);
	db.insert("Secondary_Components_120", m_Secondary_Components_120);
	db.insert("Primary_Components_121", m_Primary_Components_121);
	db.insert("Secondary_Components_121", m_Secondary_Components_121);
	db.insert("Status", m_Status);
	m_Work_Remote_State.insert("DB", db);
	emit Write_Work_Remote_State(m_Work_Remote_State);
	emit Status(c_Variable::g_Current_Time + "->当前巡检任务：" + state);
}