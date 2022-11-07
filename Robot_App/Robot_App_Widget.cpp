#pragma execution_character_set("utf-8")
#include "Robot_App_Widget.h"
/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_Robot_App_Widget::c_Robot_App_Widget(QWidget * parent) : QMainWindow(parent) {
	m_Time = new QElapsedTimer;
	m_Thread = new c_Thread;
	ui = new Ui_Robot_App_Widget;
	ui->setupUi(this);
	c_Variable::g_Hikvision_20_winId = (HWND)ui->Hikvision_20_Video->winId();
	c_Variable::g_Hikvision_21_winId = (HWND)ui->Hikvision_21_Video->winId();
	//UI输出绑定
	QObject::connect(m_Thread->m_RGV_Remote, &c_RGV_Remote::Set_Working, ui->RGV_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_RGV_Remote, &c_RGV_Remote::Set_Default, ui->RGV_Working_State, &c_Fr_Light::Set_Default);
	
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Set_Working, ui->Jaka_120_Remote_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Set_Default, ui->Jaka_120_Remote_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Write_Json_Error, ui->Jaka_120_Remote_Working_State, &c_Fr_Light::Set_Error);
	QObject::connect(m_Thread->m_Jaka_120_Monitor, &c_Jaka_120_Monitor::Set_Working, ui->Jaka_120_Monitor_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Jaka_120_Monitor, &c_Jaka_120_Monitor::Set_Default, ui->Jaka_120_Monitor_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::setEnabled, ui->Jaka_120_power_on, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::setEnabled, ui->Jaka_120_power_off, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::setEnabled, ui->Jaka_120_enable, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::setEnabled, ui->Jaka_120_disable, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::setEnabled, ui->Jaka_120_play_program, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::setEnabled, ui->Jaka_120_pause_program, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::setEnabled, ui->Jaka_120_resume_program, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::setEnabled, ui->Jaka_120_stop_program, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Set_Working, ui->Jaka_121_Remote_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Set_Default, ui->Jaka_121_Remote_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Write_Json_Error, ui->Jaka_121_Remote_Working_State, &c_Fr_Light::Set_Error);
	QObject::connect(m_Thread->m_Jaka_121_Monitor, &c_Jaka_121_Monitor::Set_Working, ui->Jaka_121_Monitor_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Jaka_121_Monitor, &c_Jaka_121_Monitor::Set_Default, ui->Jaka_121_Monitor_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::setEnabled, ui->Jaka_121_power_on, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::setEnabled, ui->Jaka_121_power_off, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::setEnabled, ui->Jaka_121_enable, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::setEnabled, ui->Jaka_121_disable, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::setEnabled, ui->Jaka_121_play_program, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::setEnabled, ui->Jaka_121_pause_program, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::setEnabled, ui->Jaka_121_resume_program, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::setEnabled, ui->Jaka_121_stop_program, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Hypersen_30_Remote, &c_Hypersen_30_Remote::Set_Working, ui->Hypersen_30_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Hypersen_30_Remote, &c_Hypersen_30_Remote::Set_Default, ui->Hypersen_30_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Hypersen_30_Remote, &c_Hypersen_30_Remote::setEnabled, ui->RUN_SINGLE_SHOT_30, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Hypersen_30_Remote, &c_Hypersen_30_Remote::setEnabled, this, [=](bool state) {
		ui->RUN_CONTINUOUS_30->setEnabled(state);
		c_Variable::msleep(3000);//等待3秒
		ui->RUN_CONTINUOUS_30->clicked();
	});
	QObject::connect(m_Thread->m_Hypersen_30_Remote, &c_Hypersen_30_Remote::setEnabled, ui->RUN_IDLE_30, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Hypersen_31_Remote, &c_Hypersen_31_Remote::Set_Working, ui->Hypersen_31_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Hypersen_31_Remote, &c_Hypersen_31_Remote::Set_Default, ui->Hypersen_31_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Hypersen_31_Remote, &c_Hypersen_31_Remote::setEnabled, ui->RUN_SINGLE_SHOT_31, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Hypersen_31_Remote, &c_Hypersen_31_Remote::setEnabled, this, [=](bool state) {
		ui->RUN_CONTINUOUS_31->setEnabled(state);
		c_Variable::msleep(3000);//等待3秒
		ui->RUN_CONTINUOUS_31->clicked();
	});
	QObject::connect(m_Thread->m_Hypersen_31_Remote, &c_Hypersen_31_Remote::setEnabled, ui->RUN_IDLE_31, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::Set_Working, ui->Meijidenki_20_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::Set_Default, ui->Meijidenki_20_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::setEnabled, this, [=](bool state) {
		ui->Meijidenki_20_START_LMD->setEnabled(state);
		c_Variable::msleep(3000);//等待3秒
		ui->Meijidenki_20_START_LMD->clicked();
	});
	QObject::connect(m_Thread->m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::setEnabled, ui->Meijidenki_20_STOP_LMD, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::Set_Working, ui->Meijidenki_21_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::Set_Default, ui->Meijidenki_21_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::setEnabled, this, [=](bool state) {
		ui->Meijidenki_21_START_LMD->setEnabled(state);
		c_Variable::msleep(3000);//等待3秒
		ui->Meijidenki_21_START_LMD->clicked();
	});
	QObject::connect(m_Thread->m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::setEnabled, ui->Meijidenki_21_STOP_LMD, &QPushButton::setEnabled);
	QObject::connect(m_Thread->m_Prec_Scan_120_Remote, &c_Prec_Scan_120_Remote::Set_Working, ui->Prec_Scan_120_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Prec_Scan_120_Remote, &c_Prec_Scan_120_Remote::Set_Default, ui->Prec_Scan_120_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Prec_Scan_121_Remote, &c_Prec_Scan_121_Remote::Set_Working, ui->Prec_Scan_121_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Prec_Scan_121_Remote, &c_Prec_Scan_121_Remote::Set_Default, ui->Prec_Scan_121_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Fast_Scan_Remote, &c_Fast_Scan_Remote::Set_Working, ui->Fast_Scan_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Fast_Scan_Remote, &c_Fast_Scan_Remote::Set_Default, ui->Fast_Scan_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Hikvision_20_Remote, &c_Hikvision_20_Remote::Connect_Done, ui->Hikvision_20_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Hikvision_20_Remote, &c_Hikvision_20_Remote::Disconnect_Done, ui->Hikvision_20_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Hikvision_21_Remote, &c_Hikvision_21_Remote::Connect_Done, ui->Hikvision_21_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Hikvision_21_Remote, &c_Hikvision_21_Remote::Disconnect_Done, ui->Hikvision_21_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Set_Working, ui->Local_Remote_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Set_Default, ui->Local_Remote_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_0_Set, ui->Fuction_0_Set, &QPushButton::click);//通信控制
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_0_Reset, ui->Fuction_0_Reset, &QPushButton::click);//通信控制
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_7, ui->Fuction_7, &QPushButton::click);//主动力位置清零
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_8, ui->Fuction_8, &QPushButton::click);//主动力故障复位
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_9_Set, ui->Fuction_9, &QPushButton::pressed);//主动力手动左行
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_9_Reset, ui->Fuction_9, &QPushButton::released);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_10_Set, ui->Fuction_10, &QPushButton::pressed);//主动力手动右行
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_10_Reset, ui->Fuction_10, &QPushButton::released);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_11, ui->Fuction_11, &QPushButton::click);//正向连续运行
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_12, ui->Fuction_12, &QPushButton::click);//返向连续运行
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_13, ui->Fuction_13, &QPushButton::click);//返向连续运行再启动
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_14, ui->Fuction_14, &QPushButton::click);//急停
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_15, ui->Fuction_15, &QPushButton::click);//左机器人开机
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_16, ui->Fuction_16, &QPushButton::click);//左机器人关机
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_17, ui->Fuction_17, &QPushButton::click);//右机器人开机
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_18, ui->Fuction_18, &QPushButton::click);//右机器人关机
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_19_Set, ui->Fuction_19_Set, &QPushButton::click);//上电
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_19_Reset, ui->Fuction_19_Reset, &QPushButton::click);//下电
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_20, ui->Fuction_20, &QPushButton::click);//充电回原点
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_21_Set, ui->Fuction_21_Set, &QPushButton::click);//清扫电机
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_21_Reset, ui->Fuction_21_Reset, &QPushButton::click);//清扫电机
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_22_Set, ui->Fuction_22_Set, &QPushButton::click);//风刀电机
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_22_Reset, ui->Fuction_22_Reset, &QPushButton::click);//风刀电机
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_26, ui->Fuction_26, &QPushButton::click);//开始充电
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_27, ui->Fuction_27, &QPushButton::click);//结束充电
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_24, this, [=](int position) {
		ui->is_RGV_Date_4->setText(QString::number(position));
		ui->Date_4->clicked();//写入数据
		c_Variable::msleep(500);
		ui->Fuction_24->clicked();//轴1移动
	});//轴1移动（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Fuction_25, this, [=](int position) {
		ui->is_RGV_Date_5->setText(QString::number(position));
		ui->Date_5->clicked();//写入数据
		c_Variable::msleep(500);
		ui->Fuction_25->clicked();//轴2移动
	});//轴2移动（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Date_0, this, [=](int position, int speed) {
		ui->is_RGV_Date_1->setText(QString::number(position));
		ui->Date_1->clicked();
		ui->is_RGV_Date_0->setText(QString::number(speed));
		ui->Date_0->clicked();
	});//左升降台
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Date_4, this, [=](int position, int speed) {
		ui->is_RGV_Date_3->setText(QString::number(position));
		ui->Date_3->clicked();
		ui->is_RGV_Date_2->setText(QString::number(speed));
		ui->Date_2->clicked();
	});//右升降台
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Jaka_120_power_on, ui->Jaka_120_power_on, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Jaka_120_power_off, ui->Jaka_120_power_off, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Jaka_120_enable, ui->Jaka_120_enable, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Jaka_120_disable, ui->Jaka_120_disable, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Jaka_120_play_program, this, [=](QString name) {ui->Jaka_120_program_name->setText(name); ui->Jaka_120_play_program->clicked(); });
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Jaka_121_power_on, ui->Jaka_121_power_on, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Jaka_121_power_off, ui->Jaka_121_power_off, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Jaka_121_enable, ui->Jaka_121_enable, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Jaka_121_disable, ui->Jaka_121_disable, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Jaka_121_play_program, this, [=](QString name) {ui->Jaka_121_program_name->setText(name); ui->Jaka_121_play_program->clicked(); });
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Set_Working, ui->Local_Monitor_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Set_Default, ui->Local_Monitor_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_0_Set, ui->Fuction_0_Set, &QPushButton::click);//通信控制
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_0_Reset, ui->Fuction_0_Reset, &QPushButton::click);//通信控制
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_7, ui->Fuction_7, &QPushButton::click);//主动力位置清零
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_8, ui->Fuction_8, &QPushButton::click);//主动力故障复位
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_9_Set, ui->Fuction_9, &QPushButton::pressed);//主动力手动左行
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_9_Reset, ui->Fuction_9, &QPushButton::released);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_10_Set, ui->Fuction_10, &QPushButton::pressed);//主动力手动右行
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_10_Reset, ui->Fuction_10, &QPushButton::released);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_11, ui->Fuction_11, &QPushButton::click);//正向连续运行
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_12, ui->Fuction_12, &QPushButton::click);//返向连续运行
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_13, ui->Fuction_13, &QPushButton::click);//返向连续运行再启动
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_14, ui->Fuction_14, &QPushButton::click);//急停
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_15, ui->Fuction_15, &QPushButton::click);//左机器人开机
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_16, ui->Fuction_16, &QPushButton::click);//左机器人关机
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_17, ui->Fuction_17, &QPushButton::click);//右机器人开机
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_18, ui->Fuction_18, &QPushButton::click);//右机器人关机
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_19_Set, ui->Fuction_19_Set, &QPushButton::click);//上电
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_19_Reset, ui->Fuction_19_Reset, &QPushButton::click);//下电
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_20, ui->Fuction_20, &QPushButton::click);//充电回原点
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_21_Set, ui->Fuction_21_Set, &QPushButton::click);//清扫电机
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_21_Reset, ui->Fuction_21_Reset, &QPushButton::click);//清扫电机
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_22_Set, ui->Fuction_22_Set, &QPushButton::click);//风刀电机
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_22_Reset, ui->Fuction_22_Reset, &QPushButton::click);//风刀电机
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_26, ui->Fuction_26, &QPushButton::click);//开始充电
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_27, ui->Fuction_27, &QPushButton::click);//结束充电
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_24, this, [=](int position) {
		ui->is_RGV_Date_4->setText(QString::number(position));
		ui->Date_4->clicked();//写入数据
		c_Variable::msleep(500);
		ui->Fuction_24->clicked();//轴1移动
	});//轴1移动（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Fuction_25, this, [=](int position) {
		ui->is_RGV_Date_5->setText(QString::number(position));
		ui->Date_5->clicked();//写入数据
		c_Variable::msleep(500);
		ui->Fuction_25->clicked();//轴2移动
	});//轴2移动（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Date_0, this, [=](int position, int speed) {
		ui->is_RGV_Date_1->setText(QString::number(position));
		ui->Date_1->clicked();
		c_Variable::msleep(100);
		ui->is_RGV_Date_0->setText(QString::number(speed));
		ui->Date_0->clicked();
	});//左升降台
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Date_4, this, [=](int position, int speed) {
		ui->is_RGV_Date_3->setText(QString::number(position));
		ui->Date_3->clicked();
		c_Variable::msleep(100);
		ui->is_RGV_Date_2->setText(QString::number(speed));
		ui->Date_2->clicked();
	});//右升降台
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Jaka_120_power_on, ui->Jaka_120_power_on, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Jaka_120_power_off, ui->Jaka_120_power_off, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Jaka_120_enable, ui->Jaka_120_enable, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Jaka_120_disable, ui->Jaka_120_disable, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Jaka_120_play_program, this, [=](QString name) {ui->Jaka_120_program_name->setText(name); ui->Jaka_120_play_program->clicked(); });
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Jaka_121_power_on, ui->Jaka_121_power_on, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Jaka_121_power_off, ui->Jaka_121_power_off, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Jaka_121_enable, ui->Jaka_121_enable, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Jaka_121_disable, ui->Jaka_121_disable, &QPushButton::click);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Jaka_121_play_program, this, [=](QString name) {ui->Jaka_121_program_name->setText(name); ui->Jaka_121_play_program->clicked(); });
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Set_Working, ui->App_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Set_Default, ui->App_Working_State, &c_Fr_Light::Set_Default);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_0_Set, ui->Fuction_0_Set, &QPushButton::click);//通信控制
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_0_Reset, ui->Fuction_0_Reset, &QPushButton::click);//通信控制
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_7, ui->Fuction_7, &QPushButton::click);//主动力位置清零
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_8, ui->Fuction_8, &QPushButton::click);//主动力故障复位
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_9_Set, ui->Fuction_9, &QPushButton::pressed);//主动力手动左行
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_9_Reset, ui->Fuction_9, &QPushButton::released);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_10_Set, ui->Fuction_10, &QPushButton::pressed);//主动力手动右行
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_10_Reset, ui->Fuction_10, &QPushButton::released);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_11, ui->Fuction_11, &QPushButton::click);//正向连续运行
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_12, ui->Fuction_12, &QPushButton::click);//返向连续运行
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_13, ui->Fuction_13, &QPushButton::click);//返向连续运行再启动
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_14, ui->Fuction_14, &QPushButton::click);//急停
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_15, ui->Fuction_15, &QPushButton::click);//左机器人开机
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_16, ui->Fuction_16, &QPushButton::click);//左机器人关机
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_17, ui->Fuction_17, &QPushButton::click);//右机器人开机
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_18, ui->Fuction_18, &QPushButton::click);//右机器人关机
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_19_Set, ui->Fuction_19_Set, &QPushButton::click);//上电
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_19_Reset, ui->Fuction_19_Reset, &QPushButton::click);//下电
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_20, ui->Fuction_20, &QPushButton::click);//充电回原点
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_21_Set, ui->Fuction_21_Set, &QPushButton::click);//清扫电机
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_21_Reset, ui->Fuction_21_Reset, &QPushButton::click);//清扫电机
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_22_Set, ui->Fuction_22_Set, &QPushButton::click);//风刀电机
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_22_Reset, ui->Fuction_22_Reset, &QPushButton::click);//风刀电机
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_26, ui->Fuction_26, &QPushButton::click);//开始充电
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_27, ui->Fuction_27, &QPushButton::click);//结束充电
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_24, this, [=](int position) {
		ui->is_RGV_Date_4->setText(QString::number(position));
		ui->Date_4->clicked();//写入数据
		c_Variable::msleep(500);
		ui->Fuction_24->clicked();//轴1移动
	});//轴1移动（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Fuction_25, this, [=](int position) {
		ui->is_RGV_Date_5->setText(QString::number(position));
		ui->Date_5->clicked();//写入数据
		c_Variable::msleep(500);
		ui->Fuction_25->clicked();//轴2移动
	});//轴2移动（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Date_0, this, [=](int position, int speed) {
		ui->is_RGV_Date_1->setText(QString::number(position));
		ui->Date_1->clicked();
		c_Variable::msleep(100);
		ui->is_RGV_Date_0->setText(QString::number(speed));
		ui->Date_0->clicked();
	});//左升降台
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Date_4, this, [=](int position, int speed) {
		ui->is_RGV_Date_3->setText(QString::number(position));
		ui->Date_3->clicked();
		c_Variable::msleep(100);
		ui->is_RGV_Date_2->setText(QString::number(speed));
		ui->Date_2->clicked();
	});//右升降台
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Jaka_120_power_on, ui->Jaka_120_power_on, &QPushButton::click);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Jaka_120_power_off, ui->Jaka_120_power_off, &QPushButton::click);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Jaka_120_enable, ui->Jaka_120_enable, &QPushButton::click);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Jaka_120_disable, ui->Jaka_120_disable, &QPushButton::click);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Jaka_120_play_program, this, [=](QString name) {ui->Jaka_120_program_name->setText(name); ui->Jaka_120_play_program->clicked(); });
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Jaka_121_power_on, ui->Jaka_121_power_on, &QPushButton::click);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Jaka_121_power_off, ui->Jaka_121_power_off, &QPushButton::click);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Jaka_121_enable, ui->Jaka_121_enable, &QPushButton::click);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Jaka_121_disable, ui->Jaka_121_disable, &QPushButton::click);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Jaka_121_play_program, this, [=](QString name) {ui->Jaka_121_program_name->setText(name); ui->Jaka_121_play_program->clicked(); });
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_0_Set, ui->Fuction_0_Set, &QPushButton::click);//通信控制（线圈）
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_7, ui->Fuction_7, &QPushButton::click);//主动力位置清零（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_8, ui->Fuction_8, &QPushButton::click);//主动力故障复位（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_11, ui->Fuction_11, &QPushButton::click);//正向连续运行（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_12, ui->Fuction_12, &QPushButton::click);//返向连续运行（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_13, ui->Fuction_13, &QPushButton::click);//返向连续运行再启动（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_14, ui->Fuction_14, &QPushButton::click);//急停
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_15, ui->Fuction_15, &QPushButton::click);//左机器人开机（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_15, ui->Fuction_17, &QPushButton::click);//右机器人开机（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_19_Set, ui->Fuction_19_Set, &QPushButton::click);//上电（线圈, 点控）
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_20, ui->Fuction_20, &QPushButton::click);//自动充电定位
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_22_Set, ui->Fuction_22_Set, &QPushButton::click);//风刀电机开
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_22_Reset, ui->Fuction_22_Reset, &QPushButton::click);//风刀电机关
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_27, ui->Fuction_27, &QPushButton::click);//结束自动充电
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_24, this, [=](int position) {
		ui->is_RGV_Date_4->setText(QString::number(position));
		ui->Date_4->clicked();//写入数据
		c_Variable::msleep(500);
		ui->Fuction_24->clicked();//轴1移动
	});//轴1移动（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fuction_25, this, [=](int position) {
		ui->is_RGV_Date_5->setText(QString::number(position));
		ui->Date_5->clicked();//写入数据
		c_Variable::msleep(500);
		ui->Fuction_25->clicked();//轴2移动
	});//轴2移动（线圈, 工控机立即复位）
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Date_0, this, [=](int speed) {
		ui->is_RGV_Date_0->setText(QString::number(speed));
		ui->Date_0->clicked();
	});//左升降台速度
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Date_1, this, [=](int position) {
		ui->is_RGV_Date_1->setText(QString::number(position));
		ui->Date_1->clicked();
	});//左升降台位置
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Date_2, this, [=](int speed) {
		ui->is_RGV_Date_2->setText(QString::number(speed));
		ui->Date_2->clicked();
	});//右升降台速度
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Date_3, this, [=](int position) {
		ui->is_RGV_Date_3->setText(QString::number(position));
		ui->Date_3->clicked();
	});//右升降台位置
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Jaka_120_power_on, ui->Jaka_120_power_on, &QPushButton::click);//上电
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Jaka_120_power_off, ui->Jaka_120_power_off, &QPushButton::click);//下电
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Jaka_120_enable, ui->Jaka_120_enable, &QPushButton::click);//使能
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Jaka_120_disable, ui->Jaka_120_disable, &QPushButton::click);//下使能
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Jaka_120_play_program, this, [=](QString name) {
		ui->Jaka_120_program_name->setText(name);//更新名
		ui->Jaka_120_play_program->clicked();//执行
	});
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Jaka_121_power_on, ui->Jaka_121_power_on, &QPushButton::click);//上电
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Jaka_121_power_off, ui->Jaka_121_power_off, &QPushButton::click);//下电
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Jaka_121_enable, ui->Jaka_121_enable, &QPushButton::click);//使能
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Jaka_121_disable, ui->Jaka_121_disable, &QPushButton::click);//下使能
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Jaka_121_play_program, this, [=](QString name) {
		ui->Jaka_121_program_name->setText(name);//更新名
		ui->Jaka_121_play_program->clicked();//执行
	});
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fast_Scan_Start, ui->Fast_Scan_Collection, &QPushButton::click);//快扫采集
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Fast_Scan_Stop, ui->Fast_Scan_Stop, &QPushButton::click);//快扫停止采集																				   
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Work_Init, ui->Work_Start, &QPushButton::click);//触发自动巡检按钮
	//系统消息
	QObject::connect(m_Thread->m_RGV_Remote, &c_RGV_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Jaka_120_Monitor, &c_Jaka_120_Monitor::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Jaka_121_Monitor, &c_Jaka_121_Monitor::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Hypersen_30_Remote, &c_Hypersen_30_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Hypersen_31_Remote, &c_Hypersen_31_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Prec_Scan_120_Remote, &c_Prec_Scan_120_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Prec_Scan_121_Remote, &c_Prec_Scan_121_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Fast_Scan_Remote, &c_Fast_Scan_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Hikvision_20_Remote, &c_Hikvision_20_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Hikvision_21_Remote, &c_Hikvision_21_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Local_Remote, &c_Local_Remote::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Local_Monitor, &c_Local_Monitor::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_App_Control, &c_App_Control::Status, ui->Worry_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Work_Remote, &c_Work_Remote::Status, ui->Work_List, &QTextEdit::append);
	QObject::connect(m_Thread->m_Prec_Scan_120_Remote, &c_Prec_Scan_120_Remote::Write_String, this, [=](QString ip, int port, QString value) {ui->Prec_Scan_120_Cmd->append(value); });
	QObject::connect(m_Thread->m_Prec_Scan_121_Remote, &c_Prec_Scan_121_Remote::Write_String, this, [=](QString ip, int port, QString value) {ui->Prec_Scan_121_Cmd->append(value); });
	QObject::connect(m_Thread->m_Fast_Scan_Remote, &c_Fast_Scan_Remote::Write_String, this, [=](QString ip, int port, QString value) {ui->Fast_Scan_Cmd->append(value); });
	//UI输入绑定
	QObject::connect(ui->Show_Work_Widget, &QPushButton::clicked, this, [=]() {ui->stackedWidget->setCurrentWidget(ui->Work_Widget); });
	QObject::connect(ui->Show_Setting_Widget, &QPushButton::clicked, this, [=]() {ui->stackedWidget->setCurrentWidget(ui->Setting_Widget); });
	QObject::connect(ui->Show_RGV_Remote_Widget, &QPushButton::clicked, this, [=]() {ui->stackedWidget->setCurrentWidget(ui->RGV_Remote_Widget); });
	QObject::connect(ui->Show_Jaka_120_Widget, &QPushButton::clicked, this, [=]() {ui->stackedWidget->setCurrentWidget(ui->Jaka_120_Widget); });
	QObject::connect(ui->Show_Scan_Widget, &QPushButton::clicked, this, [=]() {ui->stackedWidget->setCurrentWidget(ui->Scan_Widget); });
	QObject::connect(ui->Show_Hypersen_Widget, &QPushButton::clicked, this, [=]() {ui->stackedWidget->setCurrentWidget(ui->Hypersen_Widget); });
	QObject::connect(ui->Show_Meijidenki_Widget, &QPushButton::clicked, this, [=]() {ui->stackedWidget->setCurrentWidget(ui->Meijidenki_Widget); });
	QObject::connect(ui->RGV_Connect, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Connect);
	QObject::connect(ui->RGV_Disconnect, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Disconnect_Device);
	QObject::connect(ui->Fuction_0_Set, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_0_Set);//通信控制
	QObject::connect(ui->Fuction_0_Reset, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_0_Reset);//通信控制
	QObject::connect(ui->Fuction_7, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_7);//主动力位置清零
	QObject::connect(ui->Fuction_8, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_8);//主动力故障复位
	QObject::connect(ui->Fuction_9, &QPushButton::pressed, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_9_Set);//主动力手动左行
	QObject::connect(ui->Fuction_9, &QPushButton::released, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_9_Reset);//主动力手动左行结束
	QObject::connect(ui->Fuction_10, &QPushButton::pressed, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_10_Set);//主动力手动右行
	QObject::connect(ui->Fuction_10, &QPushButton::released, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_10_Reset);//主动力手动右行结束
	QObject::connect(ui->Fuction_11, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_11);//正向连续运行
	QObject::connect(ui->Fuction_12, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_12);//返向连续运行
	QObject::connect(ui->Fuction_13, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_13);//返向连续运行再启动
	QObject::connect(ui->Fuction_14, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_14);//急停
	QObject::connect(ui->Stop_working, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_14);//急停
	QObject::connect(ui->Fuction_15, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_15);//左机器人开机
	QObject::connect(ui->Fuction_16, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_16);//左机器人关机
	QObject::connect(ui->Fuction_17, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_17);//右机器人开机
	QObject::connect(ui->Fuction_18, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_18);//右机器人关机
	QObject::connect(ui->Fuction_19_Set, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_19_Set);//上电
	QObject::connect(ui->Fuction_19_Reset, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_19_Reset);//下电
	QObject::connect(ui->Fuction_20, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_20);//充电回原点
	QObject::connect(ui->Fuction_21_Set, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_21_Set);//清扫电机开
	QObject::connect(ui->Fuction_21_Reset, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_21_Reset);//清扫电机关
	QObject::connect(ui->Fuction_22_Set, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_22_Set);//风刀电机开
	QObject::connect(ui->Fuction_22_Reset, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_22_Reset);//风刀电机关
	QObject::connect(ui->Fuction_24, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_24);//轴1移动
	QObject::connect(ui->Fuction_25, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_25);//轴2移动
	QObject::connect(ui->Fuction_26, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_26);//开始充电
	QObject::connect(ui->Fuction_27, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Fuction_27);//结束充电
	QObject::connect(ui->Date_0, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Date_0);//左升降台速度
	QObject::connect(ui->Date_1, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Date_1);//左升降台位置
	QObject::connect(ui->Date_2, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Date_2);//右升降台速度
	QObject::connect(ui->Date_3, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Date_3);//右升降台位置
	QObject::connect(ui->Date_4, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Date_4);//轴1移动距离
	QObject::connect(ui->Date_5, &QPushButton::clicked, m_Thread->m_RGV_Remote, &c_RGV_Remote::Date_5);//轴2移动距离
	QObject::connect(ui->Jaka_120_Remote_Connect, &QPushButton::clicked, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Connect);
	QObject::connect(ui->Jaka_120_Remote_Disconnect, &QPushButton::clicked, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Disconnect_Device);
	QObject::connect(ui->Jaka_120_power_on, &QPushButton::clicked, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Jaka_power_on);
	QObject::connect(ui->Jaka_120_power_off, &QPushButton::clicked, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Jaka_power_off);
	QObject::connect(ui->Jaka_120_enable, &QPushButton::clicked, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Jaka_enable_robot);
	QObject::connect(ui->Jaka_120_disable, &QPushButton::clicked, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Jaka_disable_robot);
	QObject::connect(ui->Jaka_120_program_name, &QLineEdit::textChanged, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Jaka_program_name);
	QObject::connect(ui->Jaka_120_play_program, &QPushButton::clicked, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Jaka_play_program);
	QObject::connect(ui->Jaka_120_pause_program, &QPushButton::clicked, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Jaka_pause_program);
	QObject::connect(ui->Jaka_120_resume_program, &QPushButton::clicked, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Jaka_resume_program);
	QObject::connect(ui->Jaka_120_stop_program, &QPushButton::clicked, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Jaka_stop_program);
	QObject::connect(ui->Stop_working, &QPushButton::clicked, m_Thread->m_Jaka_120_Remote, &c_Jaka_120_Remote::Jaka_stop_program);
	QObject::connect(ui->Jaka_120_Monitor_Connect, &QPushButton::clicked, m_Thread->m_Jaka_120_Monitor, &c_Jaka_120_Monitor::Connect);
	QObject::connect(ui->Jaka_120_Monitor_Disconnect, &QPushButton::clicked, m_Thread->m_Jaka_120_Monitor, &c_Jaka_120_Monitor::Disconnect_Device);
	QObject::connect(ui->Jaka_121_Remote_Connect, &QPushButton::clicked, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Connect);
	QObject::connect(ui->Jaka_121_Remote_Disconnect, &QPushButton::clicked, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Disconnect_Device);
	QObject::connect(ui->Jaka_121_power_on, &QPushButton::clicked, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Jaka_power_on);
	QObject::connect(ui->Jaka_121_power_off, &QPushButton::clicked, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Jaka_power_off);
	QObject::connect(ui->Jaka_121_enable, &QPushButton::clicked, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Jaka_enable_robot);
	QObject::connect(ui->Jaka_121_disable, &QPushButton::clicked, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Jaka_disable_robot);
	QObject::connect(ui->Jaka_121_program_name, &QLineEdit::textChanged, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Jaka_program_name);
	QObject::connect(ui->Jaka_121_play_program, &QPushButton::clicked, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Jaka_play_program);
	QObject::connect(ui->Jaka_121_pause_program, &QPushButton::clicked, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Jaka_pause_program);
	QObject::connect(ui->Jaka_121_resume_program, &QPushButton::clicked, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Jaka_resume_program);
	QObject::connect(ui->Jaka_121_stop_program, &QPushButton::clicked, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Jaka_stop_program);
	QObject::connect(ui->Stop_working, &QPushButton::clicked, m_Thread->m_Jaka_121_Remote, &c_Jaka_121_Remote::Jaka_stop_program);
	QObject::connect(ui->Jaka_121_Monitor_Connect, &QPushButton::clicked, m_Thread->m_Jaka_121_Monitor, &c_Jaka_121_Monitor::Connect);
	QObject::connect(ui->Jaka_121_Monitor_Disconnect, &QPushButton::clicked, m_Thread->m_Jaka_121_Monitor, &c_Jaka_121_Monitor::Disconnect_Device);
	QObject::connect(ui->Hypersen_30_Connect, &QPushButton::clicked, m_Thread->m_Hypersen_30_Remote, &c_Hypersen_30_Remote::Connect);
	QObject::connect(ui->Hypersen_30_Disconnect, &QPushButton::clicked, m_Thread->m_Hypersen_30_Remote, &c_Hypersen_30_Remote::Disconnect_Device);
	QObject::connect(ui->RUN_SINGLE_SHOT_30, &QPushButton::clicked, m_Thread->m_Hypersen_30_Remote, &c_Hypersen_30_Remote::Run_Single_Shot);
	QObject::connect(ui->RUN_CONTINUOUS_30, &QPushButton::clicked, m_Thread->m_Hypersen_30_Remote, &c_Hypersen_30_Remote::Run_Continuous);
	QObject::connect(ui->RUN_IDLE_30, &QPushButton::clicked, m_Thread->m_Hypersen_30_Remote, &c_Hypersen_30_Remote::Run_Idle);
	QObject::connect(ui->Hypersen_31_Connect, &QPushButton::clicked, m_Thread->m_Hypersen_31_Remote, &c_Hypersen_31_Remote::Connect);
	QObject::connect(ui->Hypersen_31_Disconnect, &QPushButton::clicked, m_Thread->m_Hypersen_31_Remote, &c_Hypersen_31_Remote::Disconnect_Device);
	QObject::connect(ui->RUN_SINGLE_SHOT_31, &QPushButton::clicked, m_Thread->m_Hypersen_31_Remote, &c_Hypersen_31_Remote::Run_Single_Shot);
	QObject::connect(ui->RUN_CONTINUOUS_31, &QPushButton::clicked, m_Thread->m_Hypersen_31_Remote, &c_Hypersen_31_Remote::Run_Continuous);
	QObject::connect(ui->RUN_IDLE_31, &QPushButton::clicked, m_Thread->m_Hypersen_31_Remote, &c_Hypersen_31_Remote::Run_Idle);
	QObject::connect(ui->Meijidenki_20_Connect, &QPushButton::clicked, m_Thread->m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::Connect);
	QObject::connect(ui->Meijidenki_20_Disconnect, &QPushButton::clicked, m_Thread->m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::Disconnect_Device);
	QObject::connect(ui->Meijidenki_20_START_LMD, &QPushButton::clicked, m_Thread->m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::START_LMD);
	QObject::connect(ui->Meijidenki_20_STOP_LMD, &QPushButton::clicked, m_Thread->m_Meijidenki_20_Remote, &c_Meijidenki_20_Remote::STOP_LMD);
	QObject::connect(ui->Meijidenki_21_Connect, &QPushButton::clicked, m_Thread->m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::Connect);
	QObject::connect(ui->Meijidenki_21_Disconnect, &QPushButton::clicked, m_Thread->m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::Disconnect_Device);
	QObject::connect(ui->Meijidenki_21_START_LMD, &QPushButton::clicked, m_Thread->m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::START_LMD);
	QObject::connect(ui->Meijidenki_21_STOP_LMD, &QPushButton::clicked, m_Thread->m_Meijidenki_21_Remote, &c_Meijidenki_21_Remote::STOP_LMD);
	QObject::connect(ui->Fast_Scan_Connect, &QPushButton::clicked, m_Thread->m_Fast_Scan_Remote, &c_Fast_Scan_Remote::Connect);
	QObject::connect(ui->Fast_Scan_Disconnect, &QPushButton::clicked, m_Thread->m_Fast_Scan_Remote, &c_Fast_Scan_Remote::Disconnect_Device);
	QObject::connect(ui->Fast_Scan_Collection, &QPushButton::clicked, m_Thread->m_Fast_Scan_Remote, &c_Fast_Scan_Remote::Collection);
	QObject::connect(ui->Fast_Scan_Stop, &QPushButton::clicked, m_Thread->m_Fast_Scan_Remote, &c_Fast_Scan_Remote::Stop);
	QObject::connect(ui->Work_Start, &QPushButton::clicked, m_Thread->m_Work_Remote, &c_Work_Remote::Work_Start);
	QObject::connect(ui->m_State_224, &c_Fr_Light::Working_State, m_Thread->m_Hikvision_20_Remote, &c_Hikvision_20_Remote::Connect);//监控相机电源开
	QObject::connect(ui->m_State_224, &c_Fr_Light::Working_State, m_Thread->m_Hikvision_21_Remote, &c_Hikvision_21_Remote::Connect);//监控相机电源开
	QObject::connect(ui->m_State_224, &c_Fr_Light::Default_State, m_Thread->m_Hikvision_20_Remote, &c_Hikvision_20_Remote::Disconnect_Device); //监控相机电源关
	QObject::connect(ui->m_State_224, &c_Fr_Light::Default_State, m_Thread->m_Hikvision_21_Remote, &c_Hikvision_21_Remote::Disconnect_Device); //监控相机电源关
	QObject::connect(ui->m_State_15, &c_Fr_Light::Working_State, ui->Jaka_120_Monitor_Connect, &QPushButton::click);//左机器人开机
	QObject::connect(ui->m_State_15, &c_Fr_Light::Working_State, ui->Jaka_120_Remote_Connect, &QPushButton::click);//左机器人开机	
	QObject::connect(ui->m_State_15, &c_Fr_Light::Working_State, ui->Jaka_121_Monitor_Connect, &QPushButton::click);//右机器人开机
	QObject::connect(ui->m_State_15, &c_Fr_Light::Working_State, ui->Jaka_121_Remote_Connect, &QPushButton::click);//右机器人开机	
	QObject::connect(ui->m_State_16, &c_Fr_Light::Working_State, ui->Jaka_120_Monitor_Disconnect, &QPushButton::click);//左机器人关机
	QObject::connect(ui->m_State_16, &c_Fr_Light::Working_State, ui->Jaka_120_Remote_Disconnect, &QPushButton::click);//左机器人关机
	QObject::connect(ui->m_State_17, &c_Fr_Light::Working_State, ui->Jaka_121_Monitor_Disconnect, &QPushButton::click);//右机器人关机
	QObject::connect(ui->m_State_17, &c_Fr_Light::Working_State, ui->Jaka_121_Remote_Disconnect, &QPushButton::click);//右机器人关机
	QObject::connect(ui->m_State_225, &c_Fr_Light::Working_State, ui->Fast_Scan_Connect, &QPushButton::click);//采集机电源
	QObject::connect(ui->m_State_225, &c_Fr_Light::Default_State, ui->Fast_Scan_Disconnect, &QPushButton::click);//采集机电源
	QObject::connect(ui->m_State_230, &c_Fr_Light::Working_State, ui->Hypersen_30_Connect, &QPushButton::click);//传感器电源
	QObject::connect(ui->m_State_230, &c_Fr_Light::Working_State, ui->Hypersen_31_Connect, &QPushButton::click);//传感器电源
	QObject::connect(ui->m_State_230, &c_Fr_Light::Working_State, ui->Meijidenki_20_Connect, &QPushButton::click);//传感器电源
	QObject::connect(ui->m_State_230, &c_Fr_Light::Working_State, ui->Meijidenki_21_Connect, &QPushButton::click);//传感器电源
	QObject::connect(ui->m_State_230, &c_Fr_Light::Default_State, ui->Hypersen_30_Disconnect, &QPushButton::click);//传感器电源
	QObject::connect(ui->m_State_230, &c_Fr_Light::Default_State, ui->Hypersen_31_Disconnect, &QPushButton::click);//传感器电源
	QObject::connect(ui->m_State_230, &c_Fr_Light::Default_State, ui->Meijidenki_20_Disconnect, &QPushButton::click);//传感器电源
	QObject::connect(ui->m_State_230, &c_Fr_Light::Default_State, ui->Meijidenki_21_Disconnect, &QPushButton::click);//传感器电源
	QObject::connect(ui->RGV_Working_State, &c_Fr_Light::Working_State, ui->Fuction_0_Set, &QPushButton::setEnabled);//通信控制
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_0_Reset, &QPushButton::setEnabled);//通信控制
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_7, &QPushButton::setEnabled);//主动力位置清零
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_8, &QPushButton::setEnabled);//主动力故障复位
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_9, &QPushButton::setEnabled);//主动力手动左行
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_10, &QPushButton::setEnabled);//主动力手动右行
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_11, &QPushButton::setEnabled);//正向连续运行
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_12, &QPushButton::setEnabled);//返向连续运行
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_13, &QPushButton::setEnabled);//返向连续运行再启动
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_14, &QPushButton::setEnabled);//急停
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_15, &QPushButton::setEnabled);//左机器人开机
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_16, &QPushButton::setEnabled);//左机器人关机
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_17, &QPushButton::setEnabled);//右机器人开机
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_18, &QPushButton::setEnabled);//右机器人关机
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_19_Set, &QPushButton::setEnabled);//上电
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_19_Reset, &QPushButton::setEnabled);//下电
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_20, &QPushButton::setEnabled);//充电回原点
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_21_Set, &QPushButton::setEnabled);//清扫电机开
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_21_Reset, &QPushButton::setEnabled);//清扫电机关
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_22_Set, &QPushButton::setEnabled);//风刀电机开
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_22_Reset, &QPushButton::setEnabled);//风刀电机关
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_24, &QPushButton::setEnabled);//轴1移动
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_25, &QPushButton::setEnabled);//轴2移动
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_26, &QPushButton::setEnabled);//开始充电
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Fuction_27, &QPushButton::setEnabled);//结束充电
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Date_0, &QPushButton::setEnabled);//左升降台速度
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Date_1, &QPushButton::setEnabled);//左升降台位置
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Date_2, &QPushButton::setEnabled);//右升降台速度
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Date_3, &QPushButton::setEnabled);//右升降台位置
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Date_4, &QPushButton::setEnabled);//轴1移动距离
	QObject::connect(ui->m_State_0, &c_Fr_Light::Working_State, ui->Date_5, &QPushButton::setEnabled);//轴2移动距离
	QObject::connect(ui->RGV_Working_State, &c_Fr_Light::Default_State, ui->Fuction_0_Set, &QPushButton::setDisabled);//通信控制
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_0_Reset, &QPushButton::setDisabled);//通信控制
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_7, &QPushButton::setDisabled);//主动力位置清零
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_8, &QPushButton::setDisabled);//主动力故障复位
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_9, &QPushButton::setDisabled);//主动力手动左行
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_10, &QPushButton::setDisabled);//主动力手动右行
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_11, &QPushButton::setDisabled);//正向连续运行
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_12, &QPushButton::setDisabled);//返向连续运行
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_13, &QPushButton::setDisabled);//返向连续运行再启动
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_14, &QPushButton::setDisabled);//急停
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_15, &QPushButton::setDisabled);//左机器人开机
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_16, &QPushButton::setDisabled);//左机器人关机
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_17, &QPushButton::setDisabled);//右机器人开机
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_18, &QPushButton::setDisabled);//右机器人关机
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_19_Set, &QPushButton::setDisabled);//上电
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_19_Reset, &QPushButton::setDisabled);//下电
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_20, &QPushButton::setDisabled);//充电回原点
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_21_Set, &QPushButton::setDisabled);//清扫电机开
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_21_Reset, &QPushButton::setDisabled);//清扫电机关
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_22_Set, &QPushButton::setDisabled);//风刀电机开
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_22_Reset, &QPushButton::setDisabled);//风刀电机关
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_24, &QPushButton::setDisabled);//轴1移动
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_25, &QPushButton::setDisabled);//轴2移动
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_26, &QPushButton::setDisabled);//开始充电
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Fuction_27, &QPushButton::setDisabled);//结束充电
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Date_0, &QPushButton::setDisabled);//左升降台速度
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Date_1, &QPushButton::setDisabled);//左升降台位置
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Date_2, &QPushButton::setDisabled);//右升降台速度
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Date_3, &QPushButton::setDisabled);//右升降台位置
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Date_4, &QPushButton::setDisabled);//轴1移动距离
	QObject::connect(ui->m_State_0, &c_Fr_Light::Default_State, ui->Date_5, &QPushButton::setDisabled);//轴2移动距离
	QObject::connect(ui->Local_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Local_Ip", ip); });
	QObject::connect(ui->Collector_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Collector_Ip", ip); });
	QObject::connect(ui->Prec_Scan_120_Local_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Prec_Scan_120_Local_Port", port.toInt()); });
	QObject::connect(ui->Prec_Scan_121_Local_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Prec_Scan_121_Local_Port", port.toInt()); });
	QObject::connect(ui->Prec_Scan_120_Tran_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Prec_Scan_120_Tran_Port", port.toInt()); });
	QObject::connect(ui->Prec_Scan_121_Tran_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Prec_Scan_121_Tran_Port", port.toInt()); });
	QObject::connect(ui->Prec_Scan_120_Jaka_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Prec_Scan_120_Jaka_Port", port.toInt()); });
	QObject::connect(ui->Prec_Scan_121_Jaka_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Prec_Scan_121_Jaka_Port", port.toInt()); });
	QObject::connect(ui->Fast_Scan_Local_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Fast_Scan_Local_Port", port.toInt()); });
	QObject::connect(ui->Prec_Scan_120_Collector_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Prec_Scan_120_Collector_Port", port.toInt()); });
	QObject::connect(ui->Prec_Scan_121_Collector_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Prec_Scan_121_Collector_Port", port.toInt()); });
	QObject::connect(ui->Fast_Scan_Collector_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Fast_Scan_Collector_Port", port.toInt()); });
	QObject::connect(ui->Jaka_120_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Jaka_120_Ip", ip); });
	QObject::connect(ui->Jaka_121_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Jaka_121_Ip", ip); });
	QObject::connect(ui->Jaka_Remote_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Jaka_Remote_Port", port.toInt()); });
	QObject::connect(ui->Jaka_Monitor_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Jaka_Monitor_Port", port.toInt()); });
	QObject::connect(ui->RGV_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("RGV_Ip", ip); });
	QObject::connect(ui->RGV_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("RGV_Port", port.toInt()); });
	QObject::connect(ui->Mcgs_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Mcgs_Ip", ip); });
	QObject::connect(ui->Mcgs_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Mcgs_Port", port.toInt()); });
	QObject::connect(ui->Hypersen_30_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Hypersen_30_Ip", ip); });
	QObject::connect(ui->Hypersen_30_Id, &QLineEdit::textChanged, this, [=](QString id) {c_Variable::g_Communicate_DB.insert("Hypersen_30_Id", id.toInt()); });
	QObject::connect(ui->Hypersen_30_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Hypersen_30_Port", port.toInt()); });
	QObject::connect(ui->Hypersen_31_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Hypersen_31_Ip", ip); });
	QObject::connect(ui->Hypersen_31_Id, &QLineEdit::textChanged, this, [=](QString id) {c_Variable::g_Communicate_DB.insert("Hypersen_31_Id", id.toInt()); });
	QObject::connect(ui->Hypersen_31_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Hypersen_31_Port", port.toInt()); });
	QObject::connect(ui->Meijidenki_20_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Meijidenki_20_Ip", ip); });
	QObject::connect(ui->Meijidenki_20_Id, &QLineEdit::textChanged, this, [=](QString id) {c_Variable::g_Communicate_DB.insert("Meijidenki_20_Id", id.toInt()); });
	QObject::connect(ui->Meijidenki_20_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Meijidenki_20_Port", port.toInt()); });
	QObject::connect(ui->Meijidenki_21_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Meijidenki_21_Ip", ip); });
	QObject::connect(ui->Meijidenki_21_Id, &QLineEdit::textChanged, this, [=](QString id) {c_Variable::g_Communicate_DB.insert("Meijidenki_21_Id", id.toInt()); });
	QObject::connect(ui->Meijidenki_21_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Meijidenki_21_Port", port.toInt()); });
	QObject::connect(ui->Web_Server_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Web_Server_Ip", ip); });
	QObject::connect(ui->Ai_Server_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Ai_Server_Ip", ip); });
	QObject::connect(ui->App_Server_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("App_Server_Ip", ip); });
	QObject::connect(ui->Local_Monitor_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Local_Monitor_Port", port.toInt()); });
	QObject::connect(ui->Local_Remote_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Local_Remote_Port", port.toInt()); });
	QObject::connect(ui->App_Server_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("App_Server_Port", port.toInt()); });
	QObject::connect(ui->Hikvision_20_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Hikvision_20_Ip", ip); });
	QObject::connect(ui->Hikvision_21_Ip, &QLineEdit::textChanged, this, [=](QString ip) {c_Variable::g_Communicate_DB.insert("Hikvision_21_Ip", ip); });
	QObject::connect(ui->Hikvision_20_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Hikvision_20_Port", port.toInt()); });
	QObject::connect(ui->Hikvision_21_Port, &QLineEdit::textChanged, this, [=](QString port) {c_Variable::g_Communicate_DB.insert("Hikvision_21_Port", port.toInt()); });
	QObject::connect(ui->Hikvision_20_User, &QLineEdit::textChanged, this, [=](QString name) {c_Variable::g_Communicate_DB.insert("Hikvision_20_User", name); });
	QObject::connect(ui->Hikvision_21_User, &QLineEdit::textChanged, this, [=](QString name) {c_Variable::g_Communicate_DB.insert("Hikvision_21_User", name); });
	QObject::connect(ui->Hikvision_20_Pass, &QLineEdit::textChanged, this, [=](QString word) {c_Variable::g_Communicate_DB.insert("Hikvision_20_Pass", word); });
	QObject::connect(ui->Hikvision_21_Pass, &QLineEdit::textChanged, this, [=](QString word) {c_Variable::g_Communicate_DB.insert("Hikvision_21_Pass", word); });
	QObject::connect(ui->Write_Coils_Size, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Write_Coils_Size) {c_Variable::g_Communicate_DB.insert("Write_Coils_Size", Write_Coils_Size); });
	QObject::connect(ui->Read_DiscreteInputs_Size, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Read_DiscreteInputs_Size) {c_Variable::g_Communicate_DB.insert("Read_DiscreteInputs_Size", Read_DiscreteInputs_Size); });
	QObject::connect(ui->Read_InputRegisters_Size, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Read_InputRegisters_Size) {c_Variable::g_Communicate_DB.insert("Read_InputRegisters_Size", Read_InputRegisters_Size); });
	QObject::connect(ui->Write_HoldingRegisters_Size, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Write_HoldingRegisters_Size) {c_Variable::g_Communicate_DB.insert("Write_HoldingRegisters_Size", Write_HoldingRegisters_Size); });
	QObject::connect(ui->Write_Coils_Addr, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Write_Coils_Addr) {c_Variable::g_Communicate_DB.insert("Write_Coils_Addr", Write_Coils_Addr); });
	QObject::connect(ui->Read_DiscreteInputs_Addr, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Read_DiscreteInputs_Addr) {c_Variable::g_Communicate_DB.insert("Read_DiscreteInputs_Addr", Read_DiscreteInputs_Addr); });
	QObject::connect(ui->Read_InputRegisters_Addr, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Read_InputRegisters_Addr) {c_Variable::g_Communicate_DB.insert("Read_InputRegisters_Addr", Read_InputRegisters_Addr); });
	QObject::connect(ui->Write_HoldingRegisters_Addr, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Write_HoldingRegisters_Addr) {c_Variable::g_Communicate_DB.insert("Write_HoldingRegisters_Addr", Write_HoldingRegisters_Addr); });
	QObject::connect(ui->is_RGV_Date_0, &QLineEdit::textChanged, this, [=](QString date) {c_Variable::g_Communicate_DB.insert("RGV_Date_0", date.toInt()); });
	QObject::connect(ui->is_RGV_Date_1, &QLineEdit::textChanged, this, [=](QString date) {c_Variable::g_Communicate_DB.insert("RGV_Date_1", date.toInt()); });
	QObject::connect(ui->is_RGV_Date_2, &QLineEdit::textChanged, this, [=](QString date) {c_Variable::g_Communicate_DB.insert("RGV_Date_2", date.toInt()); });
	QObject::connect(ui->is_RGV_Date_3, &QLineEdit::textChanged, this, [=](QString date) {c_Variable::g_Communicate_DB.insert("RGV_Date_3", date.toInt()); });
	QObject::connect(ui->is_RGV_Date_4, &QLineEdit::textChanged, this, [=](QString date) {c_Variable::g_Communicate_DB.insert("RGV_Date_4", date.toInt()); });
	QObject::connect(ui->is_RGV_Date_5, &QLineEdit::textChanged, this, [=](QString date) {c_Variable::g_Communicate_DB.insert("RGV_Date_5", date.toInt()); });
	ui->Local_Ip->setText(c_Variable::g_Communicate_DB.value("Local_Ip").toString());
	ui->Collector_Ip->setText(c_Variable::g_Communicate_DB.value("Collector_Ip").toString());
	ui->Prec_Scan_120_Local_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Prec_Scan_120_Local_Port").toInt()));
	ui->Prec_Scan_121_Local_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Prec_Scan_121_Local_Port").toInt()));
	ui->Fast_Scan_Local_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Fast_Scan_Local_Port").toInt()));
	ui->Prec_Scan_120_Collector_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Prec_Scan_120_Collector_Port").toInt()));
	ui->Prec_Scan_121_Collector_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Prec_Scan_121_Collector_Port").toInt()));
	ui->Fast_Scan_Collector_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Fast_Scan_Collector_Port").toInt()));
	ui->Prec_Scan_120_Tran_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Prec_Scan_120_Tran_Port").toInt()));
	ui->Prec_Scan_121_Tran_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Prec_Scan_121_Tran_Port").toInt()));
	ui->Prec_Scan_120_Jaka_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Prec_Scan_120_Jaka_Port").toInt()));
	ui->Prec_Scan_121_Jaka_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Prec_Scan_121_Jaka_Port").toInt()));
	ui->Jaka_120_Ip->setText(c_Variable::g_Communicate_DB.value("Jaka_120_Ip").toString());
	ui->Jaka_121_Ip->setText(c_Variable::g_Communicate_DB.value("Jaka_121_Ip").toString());
	ui->Jaka_Remote_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Jaka_Remote_Port").toInt()));
	ui->Jaka_Monitor_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Jaka_Monitor_Port").toInt()));
	ui->RGV_Ip->setText(c_Variable::g_Communicate_DB.value("RGV_Ip").toString());
	ui->RGV_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("RGV_Port").toInt()));
	ui->Mcgs_Ip->setText(c_Variable::g_Communicate_DB.value("Mcgs_Ip").toString());
	ui->Mcgs_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Mcgs_Port").toInt()));
	ui->Hypersen_30_Ip->setText(c_Variable::g_Communicate_DB.value("Hypersen_30_Ip").toString());
	ui->Hypersen_30_Id->setText(QString::number(c_Variable::g_Communicate_DB.value("Hypersen_30_Id").toInt()));
	ui->Hypersen_30_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Hypersen_30_Port").toInt()));
	ui->Hypersen_31_Ip->setText(c_Variable::g_Communicate_DB.value("Hypersen_31_Ip").toString());
	ui->Hypersen_31_Id->setText(QString::number(c_Variable::g_Communicate_DB.value("Hypersen_31_Id").toInt()));
	ui->Hypersen_31_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Hypersen_31_Port").toInt()));
	ui->Meijidenki_20_Ip->setText(c_Variable::g_Communicate_DB.value("Meijidenki_20_Ip").toString());
	ui->Meijidenki_20_Id->setText(QString::number(c_Variable::g_Communicate_DB.value("Meijidenki_20_Id").toInt()));
	ui->Meijidenki_20_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Meijidenki_20_Port").toInt()));
	ui->Meijidenki_21_Ip->setText(c_Variable::g_Communicate_DB.value("Meijidenki_21_Ip").toString());
	ui->Meijidenki_21_Id->setText(QString::number(c_Variable::g_Communicate_DB.value("Meijidenki_21_Id").toInt()));
	ui->Meijidenki_21_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Meijidenki_21_Port").toInt()));
	ui->Web_Server_Ip->setText(c_Variable::g_Communicate_DB.value("Web_Server_Ip").toString());
	ui->Ai_Server_Ip->setText(c_Variable::g_Communicate_DB.value("Ai_Server_Ip").toString());
	ui->App_Server_Ip->setText(c_Variable::g_Communicate_DB.value("App_Server_Ip").toString());
	ui->Local_Monitor_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Local_Monitor_Port").toInt()));
	ui->Local_Remote_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Local_Remote_Port").toInt()));
	ui->App_Server_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("App_Server_Port").toInt()));
	ui->Hikvision_20_Ip->setText(c_Variable::g_Communicate_DB.value("Hikvision_20_Ip").toString());
	ui->Hikvision_21_Ip->setText(c_Variable::g_Communicate_DB.value("Hikvision_21_Ip").toString());
	ui->Hikvision_20_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Hikvision_20_Port").toInt()));
	ui->Hikvision_21_Port->setText(QString::number(c_Variable::g_Communicate_DB.value("Hikvision_21_Port").toInt()));
	ui->Hikvision_20_User->setText(c_Variable::g_Communicate_DB.value("Hikvision_20_User").toString());
	ui->Hikvision_21_User->setText(c_Variable::g_Communicate_DB.value("Hikvision_21_User").toString());
	ui->Hikvision_20_Pass->setText(c_Variable::g_Communicate_DB.value("Hikvision_20_Pass").toString());
	ui->Hikvision_21_Pass->setText(c_Variable::g_Communicate_DB.value("Hikvision_21_Pass").toString());
	ui->Write_Coils_Size->setValue(c_Variable::g_Communicate_DB.value("Write_Coils_Size").toInt());
	ui->Read_DiscreteInputs_Size->setValue(c_Variable::g_Communicate_DB.value("Read_DiscreteInputs_Size").toInt());
	ui->Read_InputRegisters_Size->setValue(c_Variable::g_Communicate_DB.value("Read_InputRegisters_Size").toInt());
	ui->Write_HoldingRegisters_Size->setValue(c_Variable::g_Communicate_DB.value("Write_HoldingRegisters_Size").toInt());
	ui->Write_Coils_Addr->setValue(c_Variable::g_Communicate_DB.value("Write_Coils_Addr").toInt());
	ui->Read_DiscreteInputs_Addr->setValue(c_Variable::g_Communicate_DB.value("Read_DiscreteInputs_Addr").toInt());
	ui->Read_DiscreteInputs_Addr->setValue(c_Variable::g_Communicate_DB.value("Read_DiscreteInputs_Addr").toInt());
	ui->Write_HoldingRegisters_Addr->setValue(c_Variable::g_Communicate_DB.value("Write_HoldingRegisters_Addr").toInt());
	ui->is_RGV_Date_0->setText(QString::number(c_Variable::g_Communicate_DB.value("RGV_Date_0").toInt()));
	ui->is_RGV_Date_1->setText(QString::number(c_Variable::g_Communicate_DB.value("RGV_Date_1").toInt()));
	ui->is_RGV_Date_2->setText(QString::number(c_Variable::g_Communicate_DB.value("RGV_Date_2").toInt()));
	ui->is_RGV_Date_3->setText(QString::number(c_Variable::g_Communicate_DB.value("RGV_Date_3").toInt()));
	ui->is_RGV_Date_4->setText(QString::number(c_Variable::g_Communicate_DB.value("RGV_Date_4").toInt()));
	ui->is_RGV_Date_5->setText(QString::number(c_Variable::g_Communicate_DB.value("RGV_Date_5").toInt()));
	ui->Worry_List->document()->setMaximumBlockCount(100);//只显示100条消息
	ui->stackedWidget->setCurrentWidget(ui->Work_Widget);
	//系统输出
	QObject::connect(m_Thread->m_State_DB, &c_State_DB::System_Scan, this, &c_Robot_App_Widget::System_Scan);
	QObject::connect(this, &c_Robot_App_Widget::System_Scan_Done, m_Thread->m_State_DB, &c_State_DB::Write_System_Time);
	//加载样式表
	keyPressEvent(c_Variable::Key_F6);
	//启动系统
	m_Time->start();
	m_Thread->Start();
	emit System_Scan_Done();
	ui->RGV_Connect->clicked();
}
/*************************************************************************************************************************************************
**Function:析构函数
*************************************************************************************************************************************************/
c_Robot_App_Widget::~c_Robot_App_Widget() {
	m_System_Scan = false;
	m_Thread->deleteLater();;
	delete m_Time;
	delete ui;
}
/*************************************************************************************************************************************************
**Function:系统状态扫描
*************************************************************************************************************************************************/
void c_Robot_App_Widget::System_Scan(QJsonObject db)
{
	/*************************************************************************************************************************************************
	**Function:更新状态
	*************************************************************************************************************************************************/
	QJsonObject Work_Remote_State = db.value("Work_Remote_State").toObject();//更新
	QJsonObject RGV_State = db.value("RGV_State").toObject();//更新
	QJsonObject Jaka_120_Remote_State = db.value("Jaka_120_Remote_State").toObject();//更新
	QJsonObject Jaka_121_Remote_State = db.value("Jaka_121_Remote_State").toObject();//更新
	QJsonObject Jaka_120_Monitor_State = db.value("Jaka_120_Monitor_State").toObject();//更新
	QJsonObject Jaka_121_Monitor_State = db.value("Jaka_121_Monitor_State").toObject();//更新
	QJsonObject Hypersen_30_State = db.value("Hypersen_30_State").toObject();//更新
	QJsonObject Hypersen_31_State = db.value("Hypersen_31_State").toObject();//更新
	QJsonObject Meijidenki_20_State = db.value("Meijidenki_20_State").toObject();//更新
	QJsonObject Meijidenki_21_State = db.value("Meijidenki_21_State").toObject();//更新
	QJsonObject Fast_Scan_State = db.value("Fast_Scan_State").toObject();//更新
	QJsonObject Prec_Scan_120_State = db.value("Prec_Scan_120_State").toObject();//更新
	QJsonObject Prec_Scan_121_State = db.value("Prec_Scan_121_State").toObject();//更新
	bool RGV_Connected = RGV_State.value("Connected").toBool();//状态
	bool Jaka_120_Remote_Connected = Jaka_120_Remote_State.value("Connected").toBool();//状态
	bool Jaka_121_Remote_Connected = Jaka_121_Remote_State.value("Connected").toBool();//状态
	bool Jaka_120_Monitor_Connected = Jaka_120_Monitor_State.value("Connected").toBool();//状态
	bool Jaka_121_Monitor_Connected = Jaka_121_Monitor_State.value("Connected").toBool();//状态
	bool Hypersen_30_Connected = Hypersen_30_State.value("Connected").toBool();//状态
	bool Hypersen_31_Connected = Hypersen_31_State.value("Connected").toBool();//状态
	bool Meijidenki_20_Connected = Meijidenki_20_State.value("Connected").toBool();//状态
	bool Meijidenki_21_Connected = Meijidenki_21_State.value("Connected").toBool();//状态
	bool Fast_Scan_Connected = Fast_Scan_State.value("Connected").toBool();//状态
	bool Work_Connected = Work_Remote_State.value("Connected").toBool();//状态
	/*************************************************************************************************************************************************
	**Function:执行计数
	*************************************************************************************************************************************************/
	ui->System_State->Set_State(
		RGV_Connected &&
		Jaka_120_Remote_Connected &&
		Jaka_121_Remote_Connected &&
		Jaka_120_Monitor_Connected &&
		Jaka_121_Monitor_Connected &&
		Hypersen_30_Connected &&
		Hypersen_31_Connected &&
		Meijidenki_20_Connected &&
		Meijidenki_21_Connected &&
		Fast_Scan_Connected &&
		Work_Connected
	);
	/*************************************************************************************************************************************************
	**Function:更新RGV状态显示
	*************************************************************************************************************************************************/
	if (RGV_Connected && RGV_State.value("DB") != QJsonValue::Undefined)
	{
		QJsonObject json = RGV_State.value("DB").toObject();

		QJsonObject InputRegisters = json.value("InputRegisters").toObject();
		int Date_High = InputRegisters.value("0").toInt();//前左轮对
		int Date_Low = InputRegisters.value("1").toInt();
		ui->RGV_State_1->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("2").toInt();//前右轮对
		Date_Low = InputRegisters.value("3").toInt();
		ui->RGV_State_2->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("4").toInt();//前左轮轴
		Date_Low = InputRegisters.value("5").toInt();
		ui->RGV_State_3->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("6").toInt();//前右轮轴
		Date_Low = InputRegisters.value("7").toInt();
		ui->RGV_State_4->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("8").toInt();//中前左轮对
		Date_Low = InputRegisters.value("9").toInt();
		ui->RGV_State_5->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("10").toInt();//中前右轮对
		Date_Low = InputRegisters.value("11").toInt();
		ui->RGV_State_6->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("12").toInt();//中后左轮对
		Date_Low = InputRegisters.value("13").toInt();
		ui->RGV_State_7->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("14").toInt();//中后右轮对
		Date_Low = InputRegisters.value("15").toInt();
		ui->RGV_State_8->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("16").toInt();//中左轮轴
		Date_Low = InputRegisters.value("17").toInt();
		ui->RGV_State_9->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("18").toInt();//中右轮轴
		Date_Low = InputRegisters.value("19").toInt();
		ui->RGV_State_10->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("20").toInt();//后左轮对
		Date_Low = InputRegisters.value("21").toInt();
		ui->RGV_State_11->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("22").toInt();//后右轮对
		Date_Low = InputRegisters.value("23").toInt();
		ui->RGV_State_12->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("24").toInt();//后左轮轴
		Date_Low = InputRegisters.value("25").toInt();
		ui->RGV_State_13->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("26").toInt();//后右轮轴
		Date_Low = InputRegisters.value("27").toInt();
		ui->RGV_State_14->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("28").toInt();//左跳动
		Date_Low = InputRegisters.value("29").toInt();
		ui->RGV_State_15->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("30").toInt();//右跳动
		Date_Low = InputRegisters.value("31").toInt();
		ui->RGV_State_16->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("32").toInt();//电源电流
		Date_Low = InputRegisters.value("33").toInt();
		ui->RGV_State_17->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("34").toInt();//电源电压
		Date_Low = InputRegisters.value("35").toInt();
		ui->RGV_State_18->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("36").toInt();//电池容量
		Date_Low = InputRegisters.value("37").toInt();
		ui->RGV_State_19->setValue(c_Variable::Short_To_Float(Date_High, Date_Low));

		Date_High = InputRegisters.value("38").toInt();//电池温度
		Date_Low = InputRegisters.value("39").toInt();
		ui->RGV_State_20->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("40").toInt();//主动力当前位置
		Date_Low = InputRegisters.value("41").toInt();
		ui->RGV_State_21->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("42").toInt();//车底空间距离
		Date_Low = InputRegisters.value("43").toInt();
		ui->RGV_State_22->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("46").toInt();//中轮对检测距离
		Date_Low = InputRegisters.value("47").toInt();
		ui->RGV_State_24->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("52").toInt();//主动力手动速度
		Date_Low = InputRegisters.value("53").toInt();
		ui->RGV_State_27->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("54").toInt();//主动力正向运行速度
		Date_Low = InputRegisters.value("55").toInt();
		ui->RGV_State_28->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("56").toInt();//主动力反向运行速度
		Date_Low = InputRegisters.value("57").toInt();
		ui->RGV_State_29->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("58").toInt();//主动力第一减速速度
		Date_Low = InputRegisters.value("59").toInt();
		ui->RGV_State_30->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("60").toInt();//主动力儒动速度
		Date_Low = InputRegisters.value("61").toInt();
		ui->RGV_State_31->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		Date_High = InputRegisters.value("63").toInt();//轮轴定位精度误差
		Date_Low = InputRegisters.value("64").toInt();
		ui->RGV_State_33->setText(QString::number(c_Variable::Short_To_Float(Date_High, Date_Low)));

		float Left_Axice = 0;//左中轴当前定位数据
		float Right_Axice = 0;//右中轴当前定位数据

		int Date = InputRegisters.value("62").toInt();//轮轴数量计数
		ui->RGV_State_32->setText(QString::number(Date));

		switch (Date)
		{
		case 0:
			Date_High = InputRegisters.value("65").toInt();//左中轴0数据
			Date_Low = InputRegisters.value("66").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("193").toInt();//右中轴0数据
			Date_Low = InputRegisters.value("194").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 1:
			Date_High = InputRegisters.value("67").toInt();//左中轴1数据
			Date_Low = InputRegisters.value("68").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("195").toInt();//右中轴1数据
			Date_Low = InputRegisters.value("196").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 2:
			Date_High = InputRegisters.value("69").toInt();//左中轴2数据
			Date_Low = InputRegisters.value("70").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("197").toInt();//右中轴2数据
			Date_Low = InputRegisters.value("198").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 3:
			Date_High = InputRegisters.value("71").toInt();//左中轴3数据
			Date_Low = InputRegisters.value("72").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("199").toInt();//右中轴3数据
			Date_Low = InputRegisters.value("200").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 4:
			Date_High = InputRegisters.value("73").toInt();//左中轴4数据
			Date_Low = InputRegisters.value("74").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("201").toInt();//右中轴4数据
			Date_Low = InputRegisters.value("202").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 5:
			Date_High = InputRegisters.value("75").toInt();//左中轴5数据
			Date_Low = InputRegisters.value("76").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("203").toInt();//右中轴5数据
			Date_Low = InputRegisters.value("204").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 6:
			Date_High = InputRegisters.value("77").toInt();//左中轴6数据
			Date_Low = InputRegisters.value("78").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("205").toInt();//右中轴6数据
			Date_Low = InputRegisters.value("206").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 7:
			Date_High = InputRegisters.value("79").toInt();//左中轴7数据
			Date_Low = InputRegisters.value("80").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("207").toInt();//右中轴7数据
			Date_Low = InputRegisters.value("208").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 8:
			Date_High = InputRegisters.value("81").toInt();//左中轴8数据
			Date_Low = InputRegisters.value("82").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("209").toInt();//右中轴8数据
			Date_Low = InputRegisters.value("210").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 9:
			Date_High = InputRegisters.value("83").toInt();//左中轴9数据
			Date_Low = InputRegisters.value("84").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("211").toInt();//右中轴9数据
			Date_Low = InputRegisters.value("212").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 10:
			Date_High = InputRegisters.value("85").toInt();//左中轴10数据
			Date_Low = InputRegisters.value("86").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("213").toInt();//右中轴10数据
			Date_Low = InputRegisters.value("214").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 11:
			Date_High = InputRegisters.value("87").toInt();//左中轴11数据
			Date_Low = InputRegisters.value("88").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("215").toInt();//右中轴11数据
			Date_Low = InputRegisters.value("216").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 12:
			Date_High = InputRegisters.value("89").toInt();//左中轴12数据
			Date_Low = InputRegisters.value("90").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("217").toInt();//右中轴12数据
			Date_Low = InputRegisters.value("218").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 13:
			Date_High = InputRegisters.value("91").toInt();//左中轴13数据
			Date_Low = InputRegisters.value("92").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("219").toInt();//右中轴13数据
			Date_Low = InputRegisters.value("220").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 14:
			Date_High = InputRegisters.value("93").toInt();//左中轴14数据
			Date_Low = InputRegisters.value("94").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("221").toInt();//右中轴14数据
			Date_Low = InputRegisters.value("222").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 15:
			Date_High = InputRegisters.value("95").toInt();//左中轴15数据
			Date_Low = InputRegisters.value("96").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("223").toInt();//右中轴15数据
			Date_Low = InputRegisters.value("224").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 16:
			Date_High = InputRegisters.value("97").toInt();//左中轴16数据
			Date_Low = InputRegisters.value("98").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("225").toInt();//右中轴16数据
			Date_Low = InputRegisters.value("226").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 17:
			Date_High = InputRegisters.value("99").toInt();//左中轴17数据
			Date_Low = InputRegisters.value("100").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("227").toInt();//右中轴17数据
			Date_Low = InputRegisters.value("228").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 18:
			Date_High = InputRegisters.value("101").toInt();//左中轴18数据
			Date_Low = InputRegisters.value("102").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("229").toInt();//右中轴18数据
			Date_Low = InputRegisters.value("230").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 19:
			Date_High = InputRegisters.value("103").toInt();//左中轴19数据
			Date_Low = InputRegisters.value("104").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("231").toInt();//右中轴19数据
			Date_Low = InputRegisters.value("232").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 20:
			Date_High = InputRegisters.value("105").toInt();//左中轴20数据
			Date_Low = InputRegisters.value("106").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("233").toInt();//右中轴20数据
			Date_Low = InputRegisters.value("234").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 21:
			Date_High = InputRegisters.value("107").toInt();//左中轴21数据
			Date_Low = InputRegisters.value("108").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("235").toInt();//右中轴21数据
			Date_Low = InputRegisters.value("236").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 22:
			Date_High = InputRegisters.value("109").toInt();//左中轴22数据
			Date_Low = InputRegisters.value("110").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("237").toInt();//右中轴22数据
			Date_Low = InputRegisters.value("238").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 23:
			Date_High = InputRegisters.value("111").toInt();//左中轴23数据
			Date_Low = InputRegisters.value("112").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("239").toInt();//右中轴23数据
			Date_Low = InputRegisters.value("240").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 24:
			Date_High = InputRegisters.value("113").toInt();//左中轴24数据
			Date_Low = InputRegisters.value("114").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("241").toInt();//右中轴24数据
			Date_Low = InputRegisters.value("242").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 25:
			Date_High = InputRegisters.value("115").toInt();//左中轴25数据
			Date_Low = InputRegisters.value("116").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("243").toInt();//右中轴25数据
			Date_Low = InputRegisters.value("244").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 26:
			Date_High = InputRegisters.value("117").toInt();//左中轴26数据
			Date_Low = InputRegisters.value("118").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("245").toInt();//右中轴26数据
			Date_Low = InputRegisters.value("246").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 27:
			Date_High = InputRegisters.value("119").toInt();//左中轴27数据
			Date_Low = InputRegisters.value("120").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("247").toInt();//右中轴27数据
			Date_Low = InputRegisters.value("248").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 28:
			Date_High = InputRegisters.value("121").toInt();//左中轴28数据
			Date_Low = InputRegisters.value("122").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("249").toInt();//右中轴28数据
			Date_Low = InputRegisters.value("250").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 29:
			Date_High = InputRegisters.value("123").toInt();//左中轴29数据
			Date_Low = InputRegisters.value("124").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("251").toInt();//右中轴29数据
			Date_Low = InputRegisters.value("252").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 30:
			Date_High = InputRegisters.value("125").toInt();//左中轴30数据
			Date_Low = InputRegisters.value("126").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("253").toInt();//右中轴30数据
			Date_Low = InputRegisters.value("254").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 31:
			Date_High = InputRegisters.value("127").toInt();//左中轴31数据
			Date_Low = InputRegisters.value("128").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("255").toInt();//右中轴31数据
			Date_Low = InputRegisters.value("256").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		case 32:
			Date_High = InputRegisters.value("129").toInt();//左中轴32数据
			Date_Low = InputRegisters.value("130").toInt();
			Left_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			Date_High = InputRegisters.value("257").toInt();//右中轴32数据
			Date_Low = InputRegisters.value("258").toInt();
			Right_Axice = c_Variable::Short_To_Float(Date_High, Date_Low);
			break;
		default:
			Left_Axice = 0;
			Right_Axice = 0;
			break;
		}
		ui->RGV_State_34->setText(QString::number(Left_Axice));//左中轴定位数据
		ui->RGV_State_35->setText(QString::number(Right_Axice));//右中轴定位数据
		Date_High = InputRegisters.value("330").toInt() * 65536;//左升降台当前位置
		Date_Low = InputRegisters.value("331").toInt();
		ui->RGV_State_36->setText(QString::number(Date_High + Date_Low));
		Date_High = InputRegisters.value("332").toInt() * 65536;//右升降台当前位置
		Date_Low = InputRegisters.value("333").toInt();
		ui->RGV_State_37->setText(QString::number(Date_High + Date_Low));
		Date_High = InputRegisters.value("334").toInt();//返向停车位置
		Date_Low = InputRegisters.value("335").toInt();
		ui->RGV_State_38->setText(QString::number(Date_High + Date_Low));

		Date = InputRegisters.value("336").toInt();//主动力电流
		ui->RGV_State_336->setText(QString::number(Date / 100.));
		Date = InputRegisters.value("337").toInt();//主动扭矩
		ui->RGV_State_337->setText(QString::number(Date / 100.));
		Date = InputRegisters.value("338").toInt();//前控制箱温度
		ui->RGV_State_338->setText(QString::number(Date));
		Date = InputRegisters.value("339").toInt();//工控机温度
		ui->RGV_State_339->setText(QString::number(Date));
		Date = InputRegisters.value("340").toInt();//电源模块温度
		ui->RGV_State_340->setText(QString::number(Date));
		Date = InputRegisters.value("341").toInt();//驱动器温度
		ui->RGV_State_341->setText(QString::number(Date));

		QJsonObject HoldingRegisters = json.value("HoldingRegisters").toObject();
		int RGV_Date = HoldingRegisters.value("1").toInt() * 65536 + HoldingRegisters.value("0").toInt();
		ui->RGV_State_342->setText(QString::number(RGV_Date));//左升降台速度
		RGV_Date = HoldingRegisters.value("5").toInt() * 65536 + HoldingRegisters.value("4").toInt();
		ui->RGV_State_343->setText(QString::number(RGV_Date));//右升降台速度
		RGV_Date = HoldingRegisters.value("8").toInt();
		ui->RGV_State_344->setText(QString::number(RGV_Date));//间1位移动距离
		RGV_Date = HoldingRegisters.value("9").toInt();
		ui->RGV_State_345->setText(QString::number(RGV_Date));//间2位移动距离

		QJsonObject DiscreteInputs = json.value("DiscreteInputs").toObject();
		bool State = DiscreteInputs.value("0").toInt();//通信控制
		ui->m_State_0->Set_State(State);
		State = DiscreteInputs.value("1").toInt();//中间门开
		ui->m_State_1->Set_State(State);
		State = DiscreteInputs.value("2").toInt();//中间门关
		ui->m_State_2->Set_State(State);
		State = DiscreteInputs.value("3").toInt();//左边门开
		ui->m_State_3->Set_State(State);
		State = DiscreteInputs.value("4").toInt();//左边门关
		ui->m_State_4->Set_State(State);
		State = DiscreteInputs.value("5").toInt();//右边门开
		ui->m_State_5->Set_State(State);
		State = DiscreteInputs.value("6").toInt();//右边门关
		ui->m_State_6->Set_State(State);
		State = DiscreteInputs.value("11").toInt();//正向连续运动
		ui->m_State_11->Set_State(State);
		State = DiscreteInputs.value("12").toInt();//返向断续运动
		ui->m_State_12->Set_State(State);
		State = DiscreteInputs.value("13").toInt();//返向断续再启动
		ui->m_State_13->Set_State(State);
		State = DiscreteInputs.value("14").toInt();//急停
		ui->m_State_14->Set_State(State);
		State = DiscreteInputs.value("15").toInt();//左机器人开机
		ui->m_State_15->Set_State(State);
		State = DiscreteInputs.value("16").toInt();//左机器人关机
		ui->m_State_16->Set_State(State);
		State = DiscreteInputs.value("17").toInt();//右机器人关机
		ui->m_State_17->Set_State(State);
		State = DiscreteInputs.value("18").toInt();//右机器人开机
		ui->m_State_18->Set_State(State);
		State = DiscreteInputs.value("19").toInt();//上电
		ui->m_State_19->Set_State(State);
		State = DiscreteInputs.value("20").toInt();//通讯完成
		ui->m_State_20->Set_State(State);
		State = DiscreteInputs.value("21").toInt();//正向完成停车
		ui->m_State_21->Set_State(State);
		State = DiscreteInputs.value("22").toInt();//反向完成停车
		ui->m_State_22->Set_State(State);
		State = DiscreteInputs.value("24").toInt();//轴1移动
		ui->m_State_24->Set_State(State);
		State = DiscreteInputs.value("25").toInt();//轴2移动
		ui->m_State_25->Set_State(State);
		State = DiscreteInputs.value("122").toInt();//"面板急停"按下
		ui->m_State_77->Set_State(!State);
		State = DiscreteInputs.value("123").toInt();//"左前急停"按下
		ui->m_State_78->Set_State(!State);
		State = DiscreteInputs.value("124").toInt();//"左后急停"按下
		ui->m_State_79->Set_State(!State);
		State = DiscreteInputs.value("125").toInt();//"右前急停"按下
		ui->m_State_80->Set_State(!State);
		State = DiscreteInputs.value("126").toInt();//"右后急停"按下
		ui->m_State_81->Set_State(!State);
		State = DiscreteInputs.value("90").toInt();//返向运行遇到雷达停车
		ui->m_State_90->Set_State(State);
		State = DiscreteInputs.value("91").toInt();//正向运行遇到雷达停车
		ui->m_State_91->Set_State(State);
		State = DiscreteInputs.value("92").toInt();//返向到达轮轴停车
		ui->m_State_92->Set_State(State);
		State = DiscreteInputs.value("93").toInt();//左升降台位置到达
		ui->m_State_93->Set_State(State);
		State = DiscreteInputs.value("94").toInt();//右升降台位置到达
		ui->m_State_94->Set_State(State);
		State = DiscreteInputs.value("129").toInt();//动力伺服就绪
		ui->m_State_129->Set_State(State);
		State = DiscreteInputs.value("131").toInt();//左升降台伺服就绪
		ui->m_State_131->Set_State(State);
		State = DiscreteInputs.value("133").toInt();//右升降台伺服就绪
		ui->m_State_133->Set_State(State);
		State = DiscreteInputs.value("144").toInt();//前点阵雷达 out1
		ui->m_State_144->Set_State(State);
		State = DiscreteInputs.value("145").toInt();//前点阵雷达 out2
		ui->m_State_145->Set_State(State);
		State = DiscreteInputs.value("146").toInt();//前点阵雷达 out3
		ui->m_State_146->Set_State(State);
		State = DiscreteInputs.value("147").toInt();//前点阵雷达 out4
		ui->m_State_147->Set_State(State);
		State = DiscreteInputs.value("148").toInt();//后点阵雷达 out1
		ui->m_State_148->Set_State(State);
		State = DiscreteInputs.value("149").toInt();//后点阵雷达 out2
		ui->m_State_149->Set_State(State);
		State = DiscreteInputs.value("150").toInt();//后点阵雷达 out3
		ui->m_State_150->Set_State(State);
		State = DiscreteInputs.value("151").toInt();//后点阵雷达 out4
		ui->m_State_151->Set_State(State);
		State = DiscreteInputs.value("216").toInt();//后-右门电磁锁
		ui->m_State_216->Set_State(State);
		State = DiscreteInputs.value("217").toInt();//后-中门电磁锁
		ui->m_State_217->Set_State(State);
		State = DiscreteInputs.value("218").toInt();//后-左门电磁锁
		ui->m_State_218->Set_State(State);
		State = DiscreteInputs.value("219").toInt();//前-左门电磁锁
		ui->m_State_219->Set_State(State);
		State = DiscreteInputs.value("220").toInt();//前-中门电磁锁
		ui->m_State_220->Set_State(State);
		State = DiscreteInputs.value("221").toInt();//前-右门电磁锁
		ui->m_State_221->Set_State(State);
		State = DiscreteInputs.value("204").toInt();//三色灯-红
		if (State) { ui->m_State_204->Set_Default(); }
		State = DiscreteInputs.value("205").toInt();//三色灯-黄
		if (State) { ui->m_State_204->Set_Error(); }
		State = DiscreteInputs.value("206").toInt();//三色灯-绿
		if (State) { ui->m_State_204->Set_Working(); }
		State = DiscreteInputs.value("223").toInt();//清扫电机
		ui->m_State_223->Set_State(State);
		State = DiscreteInputs.value("224").toInt();//监控相机 电源
		ui->m_State_224->Set_State(State);
		State = DiscreteInputs.value("225").toInt();//采集机 电源
		ui->m_State_225->Set_State(State);
		State = DiscreteInputs.value("226").toInt();//前快扫相机 电源
		ui->m_State_226->Set_State(State);
		State = DiscreteInputs.value("227").toInt();//后快扫相机 电源
		ui->m_State_227->Set_State(State);
		State = DiscreteInputs.value("228").toInt();//左机器人 电源
		ui->m_State_228->Set_State(State);
		State = DiscreteInputs.value("229").toInt();//右机器人 电源
		ui->m_State_229->Set_State(State);
		State = DiscreteInputs.value("230").toInt();//传感器 电源
		ui->m_State_230->Set_State(State);
		State = DiscreteInputs.value("253").toInt();//车尾面板灯-红
		if (State) { ui->m_State_253->Set_Default(); }
		State = DiscreteInputs.value("254").toInt();//车尾面板灯-黄
		if (State) { ui->m_State_253->Set_Error(); }
		State = DiscreteInputs.value("255").toInt();//车尾面板灯-绿
		if (State) { ui->m_State_253->Set_Working(); }
		State = DiscreteInputs.value("258").toInt();//左自动充电
		ui->m_State_258->Set_State(State);
		State = DiscreteInputs.value("260").toInt();//升降台与门电机 电源
		ui->m_State_260->Set_State(State);
		State = DiscreteInputs.value("261").toInt();//动力电机 电源
		ui->m_State_261->Set_State(State);
		State = DiscreteInputs.value("262").toInt();//风刀电机
		ui->m_State_262->Set_State(State);
	}
	/*************************************************************************************************************************************************
	**Function:更新机械臂120状态显示
	*************************************************************************************************************************************************/
	if (Jaka_120_Monitor_Connected && Jaka_120_Monitor_State.value("DB") != QJsonValue::Undefined)
	{
		QJsonObject json = Jaka_120_Monitor_State.value("DB").toObject();
		//机器人关节角度
		QJsonArray joint_actual_position = json.value("joint_actual_position").toArray();
		ui->Jaka_120_joint_1->setText(QString::number(joint_actual_position.at(0).toDouble()));
		ui->Jaka_120_joint_2->setText(QString::number(joint_actual_position.at(1).toDouble()));
		ui->Jaka_120_joint_3->setText(QString::number(joint_actual_position.at(2).toDouble()));
		ui->Jaka_120_joint_4->setText(QString::number(joint_actual_position.at(3).toDouble()));
		ui->Jaka_120_joint_5->setText(QString::number(joint_actual_position.at(4).toDouble()));
		ui->Jaka_120_joint_6->setText(QString::number(joint_actual_position.at(5).toDouble()));
		//机器人 tcp 的位姿
		QJsonArray actual_position = json.value("actual_position").toArray();
		ui->Jaka_120_TCP_x->setText(QString::number(actual_position.at(0).toDouble()));
		ui->Jaka_120_TCP_y->setText(QString::number(actual_position.at(1).toDouble()));
		ui->Jaka_120_TCP_z->setText(QString::number(actual_position.at(2).toDouble()));
		ui->Jaka_120_TCP_a->setText(QString::number(actual_position.at(3).toDouble()));
		ui->Jaka_120_TCP_b->setText(QString::number(actual_position.at(4).toDouble()));
		ui->Jaka_120_TCP_c->setText(QString::number(actual_position.at(5).toDouble()));
		//1 代表机器人下电 2 代表机器人上电 3 代表机器人下使能 4 代表机器人上使能
		int task_state = json.value("task_state").toInt();
		switch (task_state)
		{
		case 1:
			ui->Jaka_120_task_state->setText("下电");
			break;
		case 2:
			ui->Jaka_120_task_state->setText("上电");
			break;
		case 3:
			ui->Jaka_120_task_state->setText("下使能");
			break;
		case 4:
			ui->Jaka_120_task_state->setText("上使能");
			break;
		default:
			ui->Jaka_120_task_state->setText("未连接");
			break;
		}
		//机器人被设置的 home 点位
		QJsonArray homed = json.value("homed").toArray();
		ui->Jaka_120_home_x->setText(QString::number(homed.at(0).toDouble()));
		ui->Jaka_120_home_y->setText(QString::number(homed.at(1).toDouble()));
		ui->Jaka_120_home_z->setText(QString::number(homed.at(2).toDouble()));
		ui->Jaka_120_home_a->setText(QString::number(homed.at(3).toDouble()));
		ui->Jaka_120_home_b->setText(QString::number(homed.at(4).toDouble()));
		ui->Jaka_120_home_c->setText(QString::number(homed.at(5).toDouble()));
		//机器人的任务模式 1 代表手动模式 2 代表自动模式 3 代表 MDI(面板操作)模式 4 代表拖拽模式
		int task_mode = json.value("task_mode").toInt();
		switch (task_mode)
		{
		case 1:
			ui->Jaka_120_task_mode->setText("手动");
			break;
		case 2:
			ui->Jaka_120_task_mode->setText("自动");
			break;
		case 3:
			ui->Jaka_120_task_mode->setText("面板操作");
			break;
		case 4:
			ui->Jaka_120_task_mode->setText("拖拽");
			break;
		default:
			ui->Jaka_120_task_mode->setText("未连接");
			break;
		}
		//程序运行状态 0 代表空闲 1 代表正在加载 2 代表暂停 3 代表正在运行
		int interp_state = json.value("interp_state").toInt();
		switch (interp_state)
		{
		case 0:
			ui->Jaka_120_interp_state->setText("空闲");
			break;
		case 1:
			ui->Jaka_120_interp_state->setText("正在加载");
			break;
		case 2:
			ui->Jaka_120_interp_state->setText("暂停");
			break;
		case 3:
			ui->Jaka_120_interp_state->setText("正在运行");
			break;
		default:
			ui->Jaka_120_interp_state->setText("未连接");
			break;
		}
		//值为 1 时代表机器人正处于急停状态
		int protective_stop = json.value("protective_stop").toInt();
		if (protective_stop == 1) { ui->Jaka_120_protective_stop->setText("急停状态"); }
		else { ui->Jaka_120_protective_stop->setText("正常"); }
		//值为 1 时代表机器人正处于关节软限位
		int on_soft_limit = json.value("on_soft_limit").toInt();
		if (on_soft_limit == 1 ) { ui->Jaka_120_on_soft_limit->setText("关节软限位状态"); }
		else { ui->Jaka_120_on_soft_limit->setText("正常"); }
		//值为 1 时代表机器人正处于代表急停状态
		int emergency_stop = json.value("emergency_stop").toInt();
		if (emergency_stop == 1) { ui->Jaka_120_emergency_stop->setText("软件急停状态"); }
		else { ui->Jaka_120_emergency_stop->setText("正常"); }
		//值为 1 时代表机器人接近拖拽的极限位置
		QJsonArray drag_near_limit = json.value("drag_near_limit").toArray();
		if (drag_near_limit.at(0).toDouble() == 1) { ui->Jaka_120_drag_near_limit_1->setText("到达极限"); }
		else { ui->Jaka_120_drag_near_limit_1->setText("正常"); }
		if (drag_near_limit.at(1).toDouble() == 1) { ui->Jaka_120_drag_near_limit_2->setText("到达极限"); }
		else { ui->Jaka_120_drag_near_limit_2->setText("正常"); }
		if (drag_near_limit.at(2).toDouble() == 1) { ui->Jaka_120_drag_near_limit_3->setText("到达极限"); }
		else { ui->Jaka_120_drag_near_limit_3->setText("正常"); }
		if (drag_near_limit.at(3).toDouble() == 1) { ui->Jaka_120_drag_near_limit_4->setText("到达极限"); }
		else { ui->Jaka_120_drag_near_limit_4->setText("正常"); }
		if (drag_near_limit.at(4).toDouble() == 1) { ui->Jaka_120_drag_near_limit_5->setText("到达极限"); }
		else { ui->Jaka_120_drag_near_limit_5->setText("正常"); }
		if (drag_near_limit.at(5).toDouble() == 1) { ui->Jaka_120_drag_near_limit_6->setText("到达极限"); }
		else { ui->Jaka_120_drag_near_limit_6->setText("正常"); }
	}
	/*************************************************************************************************************************************************
	**Function:更新机械臂121状态显示
	*************************************************************************************************************************************************/
	if (Jaka_121_Monitor_Connected && Jaka_121_Monitor_State.value("DB") != QJsonValue::Undefined)
	{
		QJsonObject json = Jaka_121_Monitor_State.value("DB").toObject();
		//机器人关节角度
		QJsonArray joint_actual_position = json.value("joint_actual_position").toArray();
		ui->Jaka_121_joint_1->setText(QString::number(joint_actual_position.at(0).toDouble()));
		ui->Jaka_121_joint_2->setText(QString::number(joint_actual_position.at(1).toDouble()));
		ui->Jaka_121_joint_3->setText(QString::number(joint_actual_position.at(2).toDouble()));
		ui->Jaka_121_joint_4->setText(QString::number(joint_actual_position.at(3).toDouble()));
		ui->Jaka_121_joint_5->setText(QString::number(joint_actual_position.at(4).toDouble()));
		ui->Jaka_121_joint_6->setText(QString::number(joint_actual_position.at(5).toDouble()));
		//机器人 tcp 的位姿
		QJsonArray actual_position = json.value("actual_position").toArray();
		ui->Jaka_121_TCP_x->setText(QString::number(actual_position.at(0).toDouble()));
		ui->Jaka_121_TCP_y->setText(QString::number(actual_position.at(1).toDouble()));
		ui->Jaka_121_TCP_z->setText(QString::number(actual_position.at(2).toDouble()));
		ui->Jaka_121_TCP_a->setText(QString::number(actual_position.at(3).toDouble()));
		ui->Jaka_121_TCP_b->setText(QString::number(actual_position.at(4).toDouble()));
		ui->Jaka_121_TCP_c->setText(QString::number(actual_position.at(5).toDouble()));
		//1 代表机器人下电 2 代表机器人上电 3 代表机器人下使能 4 代表机器人上使能
		int task_state = json.value("task_state").toInt();
		switch (task_state)
		{
		case 1:
			ui->Jaka_121_task_state->setText("下电");
			break;
		case 2:
			ui->Jaka_121_task_state->setText("上电");
			break;
		case 3:
			ui->Jaka_121_task_state->setText("下使能");
			break;
		case 4:
			ui->Jaka_121_task_state->setText("上使能");
			break;
		default:
			ui->Jaka_121_task_state->setText("未连接");
			break;
		}
		//机器人被设置的 home 点位
		QJsonArray homed = json.value("homed").toArray();
		ui->Jaka_121_home_x->setText(QString::number(homed.at(0).toDouble()));
		ui->Jaka_121_home_y->setText(QString::number(homed.at(1).toDouble()));
		ui->Jaka_121_home_z->setText(QString::number(homed.at(2).toDouble()));
		ui->Jaka_121_home_a->setText(QString::number(homed.at(3).toDouble()));
		ui->Jaka_121_home_b->setText(QString::number(homed.at(4).toDouble()));
		ui->Jaka_121_home_c->setText(QString::number(homed.at(5).toDouble()));
		//机器人的任务模式 1 代表手动模式 2 代表自动模式 3 代表 MDI(面板操作)模式 4 代表拖拽模式
		int task_mode = json.value("task_mode").toInt();
		switch (task_mode)
		{
		case 1:
			ui->Jaka_121_task_mode->setText("手动");
			break;
		case 2:
			ui->Jaka_121_task_mode->setText("自动");
			break;
		case 3:
			ui->Jaka_121_task_mode->setText("面板操作");
			break;
		case 4:
			ui->Jaka_121_task_mode->setText("拖拽");
			break;
		default:
			ui->Jaka_121_task_mode->setText("未连接");
			break;
		}
		//程序运行状态 0 代表空闲 1 代表正在加载 2 代表暂停 3 代表正在运行
		int interp_state = json.value("interp_state").toInt();
		switch (interp_state)
		{
		case 0:
			ui->Jaka_121_interp_state->setText("空闲");
			break;
		case 1:
			ui->Jaka_121_interp_state->setText("正在加载");
			break;
		case 2:
			ui->Jaka_121_interp_state->setText("暂停");
			break;
		case 3:
			ui->Jaka_121_interp_state->setText("正在运行");
			break;
		default:
			ui->Jaka_121_interp_state->setText("未连接");
			break;
		}
		//值为 1 时代表机器人正处于急停状态
		int protective_stop = json.value("protective_stop").toInt();
		if (protective_stop == 1) { ui->Jaka_121_protective_stop->setText("急停状态"); }
		else { ui->Jaka_121_protective_stop->setText("正常"); }
		//值为 1 时代表机器人正处于关节软限位
		int on_soft_limit = json.value("on_soft_limit").toInt();
		if (on_soft_limit == 1) { ui->Jaka_121_on_soft_limit->setText("关节软限位状态"); }
		else { ui->Jaka_121_on_soft_limit->setText("正常"); }
		//值为 1 时代表机器人正处于代表急停状态
		int emergency_stop = json.value("emergency_stop").toInt();
		if (emergency_stop == 1) { ui->Jaka_121_emergency_stop->setText("软件急停状态"); }
		else { ui->Jaka_121_emergency_stop->setText("正常"); }
		//值为 1 时代表机器人接近拖拽的极限位置
		QJsonArray drag_near_limit = json.value("drag_near_limit").toArray();
		if (drag_near_limit.at(0).toDouble() == 1) { ui->Jaka_121_drag_near_limit_1->setText("到达极限"); }
		else { ui->Jaka_121_drag_near_limit_1->setText("正常"); }
		if (drag_near_limit.at(1).toDouble() == 1) { ui->Jaka_121_drag_near_limit_2->setText("到达极限"); }
		else { ui->Jaka_121_drag_near_limit_2->setText("正常"); }
		if (drag_near_limit.at(2).toDouble() == 1) { ui->Jaka_121_drag_near_limit_3->setText("到达极限"); }
		else { ui->Jaka_121_drag_near_limit_3->setText("正常"); }
		if (drag_near_limit.at(3).toDouble() == 1) { ui->Jaka_121_drag_near_limit_4->setText("到达极限"); }
		else { ui->Jaka_121_drag_near_limit_4->setText("正常"); }
		if (drag_near_limit.at(4).toDouble() == 1) { ui->Jaka_121_drag_near_limit_5->setText("到达极限"); }
		else { ui->Jaka_121_drag_near_limit_5->setText("正常"); }
		if (drag_near_limit.at(5).toDouble() == 1) { ui->Jaka_121_drag_near_limit_6->setText("到达极限"); }
		else { ui->Jaka_121_drag_near_limit_6->setText("正常"); }
	}
	/*************************************************************************************************************************************************
	**Function:更新线阵雷达状态显示
	*************************************************************************************************************************************************/
	if (Meijidenki_20_Connected && Meijidenki_20_State.value("DB") != QJsonValue::Undefined)
	{
		QJsonObject json = Meijidenki_20_State.value("DB").toObject();
		QJsonObject value = json.value("LIM_CODE_LDBCONFIG").toObject();

		int MR = value.value("MR").toInt();// 量程
		int ESAR = value.value("ESAR").toInt();// 设备角度范围
		int ESA0 = value.value("ESA0").toInt();// 设备角度起始角
		int ESA1 = value.value("ESA1").toInt();// 设备角度终止角
		int SAR = value.value("SAR").toInt();// 有效角度范围
		int SA0 = value.value("SA0").toInt();// 有效角度起始角
		int SA1 = value.value("SA1").toInt();// 有效角度终止角
		int SAV = value.value("SAV").toInt();// 扫描角速度（度/秒）
		int SAP = value.value("SAP").toDouble();// 扫描角度分辨率
		int PF = value.value("PF").toInt();// 测量频率(HZ)

		ui->ESAR_20->setText(QString::number(ESAR));//设备角度范围
		ui->MR_20->setText(QString::number(MR));//量程
		ui->ESA_20->setText(QString("(%1, %2)").arg(ESA0).arg(ESA1));//设备角度起始终止角
		ui->SAV_20->setText(QString::number(SAV));//扫描角速度
		ui->SAR_20->setText(QString::number(SAR));//有效角度范围
		ui->PF_20->setText(QString::number(PF));//测量频率(HZ)
		ui->SA_20->setText(QString("(%1, %2)").arg(SA0).arg(SA1));//有效角度起始终止角
		ui->SAP_20->setText(QString::number(SAP));//扫描角度分辨率

		value = json.value("LIM_CODE_LMD").toObject();
		double nRange = value.value("nRange").toDouble();// 设备量程.单位:cm
		double nBAngle = value.value("nBAngle").toDouble();// 测量数据起始角度, 可能为负值.单位:度
		double nEAngle = value.value("nEAngle").toDouble();// 测量数据结束角度, 可能为负值.单位:度
		double nAnglePrecision = value.value("nAnglePrecision").toDouble();// 角度精度.单位：1/1000 度
		double nRPM = value.value("nRPM").toDouble();// 扫描频率.单位：RPM (转/分钟)
		double nMDataNum = value.value("nMDataNum").toDouble();//测量数据的个数, 根据nBAngle、nEAngle和nAnglePrecision计算
		ui->nMDataNum_20->setText(QString::number(nMDataNum));
		QJsonArray distance = value.value("LMD_D").toArray();//测量距离数据.单位cm
		ui->Meijidenki_20_Chart->set_Date(distance);
		int i = ui->MDataNum_20->value();//当前点
		int idistance = 0;//模长
		double iAngle = 0;//角度
		if (i < distance.size())
		{
			idistance = distance.at(i).toDouble();//模长
			iAngle = nBAngle + i * nAnglePrecision;//角度
		}
		ui->LMD_20->setText(QString(" %1 °: %2 cm").arg(iAngle).arg(idistance));//极坐标
	}
	/*************************************************************************************************************************************************
	**Function:更新线阵雷达状态显示
	*************************************************************************************************************************************************/
	if (Meijidenki_21_Connected && Meijidenki_21_State.value("DB") != QJsonValue::Undefined)
	{
		QJsonObject json = Meijidenki_21_State.value("DB").toObject();
		QJsonObject value = json.value("LIM_CODE_LDBCONFIG").toObject();

		int MR = value.value("MR").toInt();// 量程
		int ESAR = value.value("ESAR").toInt();// 设备角度范围
		int ESA0 = value.value("ESA0").toInt();// 设备角度起始角
		int ESA1 = value.value("ESA1").toInt();// 设备角度终止角
		int SAR = value.value("SAR").toInt();// 有效角度范围
		int SA0 = value.value("SA0").toInt();// 有效角度起始角
		int SA1 = value.value("SA1").toInt();// 有效角度终止角
		int SAV = value.value("SAV").toInt();// 扫描角速度（度/秒）
		int SAP = value.value("SAP").toDouble();// 扫描角度分辨率
		int PF = value.value("PF").toInt();// 测量频率(HZ)

		ui->ESAR_21->setText(QString::number(ESAR));//设备角度范围
		ui->MR_21->setText(QString::number(MR));//量程
		ui->ESA_21->setText(QString("(%1, %2)").arg(ESA0).arg(ESA1));//设备角度起始终止角
		ui->SAV_21->setText(QString::number(SAV));//扫描角速度
		ui->SAR_21->setText(QString::number(SAR));//有效角度范围
		ui->PF_21->setText(QString::number(PF));//测量频率(HZ)
		ui->SA_21->setText(QString("(%1, %2)").arg(SA0).arg(SA1));//有效角度起始终止角
		ui->SAP_21->setText(QString::number(SAP));//扫描角度分辨率

		value = json.value("LIM_CODE_LMD").toObject();
		double nRange = value.value("nRange").toDouble();// 设备量程.单位:cm
		double nBAngle = value.value("nBAngle").toDouble();// 测量数据起始角度, 可能为负值.单位:度
		double nEAngle = value.value("nEAngle").toDouble();// 测量数据结束角度, 可能为负值.单位:度
		double nAnglePrecision = value.value("nAnglePrecision").toDouble();// 角度精度.单位：1/1000 度
		double nRPM = value.value("nRPM").toDouble();// 扫描频率.单位：RPM (转/分钟)
		double nMDataNum = value.value("nMDataNum").toDouble();//测量数据的个数, 根据nBAngle、nEAngle和nAnglePrecision计算
		ui->nMDataNum_21->setText(QString::number(nMDataNum));
		QJsonArray distance = value.value("LMD_D").toArray();//测量距离数据.单位cm
		ui->Meijidenki_21_Chart->set_Date(distance);
		int i = ui->MDataNum_21->value();//当前点
		int idistance = 0;//模长
		double iAngle = 0;//角度
		if (i < distance.size())
		{
			idistance = distance.at(i).toDouble();//模长
			iAngle = nBAngle + i * nAnglePrecision;//角度
		}
		ui->LMD_21->setText(QString(" %1 °: %2 cm").arg(iAngle).arg(idistance));//极坐标
	}
	/*************************************************************************************************************************************************
	**Function:更新面阵雷达状态显示
	*************************************************************************************************************************************************/
	if (Hypersen_30_Connected && Hypersen_30_State.value("DB") != QJsonValue::Undefined)
	{
		QJsonObject json = Hypersen_30_State.value("DB").toObject();
		QJsonArray Array = json.value("0").toArray();

		int distance = Array.at(0).toInt();
		ui->roi_aver_distance_30_0->setText(QString::number(distance));//ROI_0平均距离
		distance = Array.at(1).toInt();
		ui->roi_max_distance_30_0->setText(QString::number(distance));//ROI_0最大距离
		distance = Array.at(2).toInt();
		ui->roi_min_distance_30_0->setText(QString::number(distance));//ROI_0最小距离
		distance = Array.at(3).toInt();
		ui->roi_valid_aver_amp_30_0->setText(QString::number(distance));//ROI_0有效幅值
		distance = Array.at(4).toInt();
		ui->roi_all_aver_amp_30_0->setText(QString::number(distance));//ROI_0平均幅值

		Array = json.value("1").toArray();

		distance = Array.at(0).toInt();
		ui->roi_aver_distance_30_1->setText(QString::number(distance));//ROI_1平均距离
		distance = Array.at(1).toInt();
		ui->roi_max_distance_30_1->setText(QString::number(distance));//ROI_1最大距离
		distance = Array.at(2).toInt();
		ui->roi_min_distance_30_1->setText(QString::number(distance));//ROI_1最小距离
		distance = Array.at(3).toInt();
		ui->roi_valid_aver_amp_30_1->setText(QString::number(distance));//ROI_1有效幅值
		distance = Array.at(4).toInt();
		ui->roi_all_aver_amp_30_1->setText(QString::number(distance));//ROI_1平均幅值
	}
	/*************************************************************************************************************************************************
	**Function:更新面阵雷达状态显示
	*************************************************************************************************************************************************/
	if (Hypersen_31_Connected && Hypersen_31_State.value("DB") != QJsonValue::Undefined)
	{
		QJsonObject json = Hypersen_31_State.value("DB").toObject();
		QJsonArray Array = json.value("0").toArray();

		int distance = Array.at(0).toInt();
		ui->roi_aver_distance_31_0->setText(QString::number(distance));//ROI_0平均距离
		distance = Array.at(1).toInt();
		ui->roi_max_distance_31_0->setText(QString::number(distance));//ROI_0最大距离
		distance = Array.at(2).toInt();
		ui->roi_min_distance_31_0->setText(QString::number(distance));//ROI_0最小距离
		distance = Array.at(3).toInt();
		ui->roi_valid_aver_amp_31_0->setText(QString::number(distance));//ROI_0有效幅值
		distance = Array.at(4).toInt();
		ui->roi_all_aver_amp_31_0->setText(QString::number(distance));//ROI_0平均幅值

		Array = json.value("1").toArray();

		distance = Array.at(0).toInt();
		ui->roi_aver_distance_31_1->setText(QString::number(distance));//ROI_1平均距离
		distance = Array.at(1).toInt();
		ui->roi_max_distance_31_1->setText(QString::number(distance));//ROI_1最大距离
		distance = Array.at(2).toInt();
		ui->roi_min_distance_31_1->setText(QString::number(distance));//ROI_1最小距离
		distance = Array.at(3).toInt();
		ui->roi_valid_aver_amp_31_1->setText(QString::number(distance));//ROI_1有效幅值
		distance = Array.at(4).toInt();
		ui->roi_all_aver_amp_31_1->setText(QString::number(distance));//ROI_1平均幅值
	}
	/*************************************************************************************************************************************************
	**Function:更新工作状态显示
	*************************************************************************************************************************************************/
	if (Work_Connected)
	{
		QJsonObject json = Work_Remote_State.value("DB").toObject();
		ui->Robot_Num->setText(json.value("Robot_Num").toString());//机器人编号
		ui->Track_Rank->setText(json.value("Track_Rank").toString());//股道号
		ui->Begin_Time->setText(json.value("Begin_Time").toString());//开始时间
		ui->Work_Num->setText(json.value("Work_Num").toString());//开始时间
		ui->Car_Type->setText(json.value("Car_Type").toString());//任务车型
		ui->Car_Num->setText(json.value("Car_Num").toString());//车辆编号
		ui->Carbox_Num->setText(json.value("Carbox_Num").toString());//车厢号
		ui->Bogie_Num->setText(json.value("Bogie_Num").toString());//转向架号
		ui->Axis_Num->setText(json.value("Axis_Num").toString());//轮轴号
		ui->Work_Stage->setText(json.value("Work_Stage").toString());//巡检工作阶段
		ui->Wheelset_Num_120->setText(json.value("Wheelset_Num_120").toString());//机械臂120轮对号
		ui->Wheelset_Num_121->setText(json.value("Wheelset_Num_121").toString());//机械臂121轮对号
		ui->Point_Num_120->setText(json.value("Point_Num_120").toString());//机械臂120巡检点
		ui->Point_Num_121->setText(json.value("Point_Num_121").toString());//机械臂121巡检点
		ui->Primary_Components_120->setText(json.value("Primary_Components_120").toString());//机械臂120一级部件
		ui->Secondary_Components_120->setText(json.value("Secondary_Components_120").toString());//机械臂120二级部件
		ui->Primary_Components_121->setText(json.value("Primary_Components_121").toString());//机械臂121一级部件
		ui->Secondary_Components_121->setText(json.value("Secondary_Components_121").toString());//机械臂121二级部件
		if (ui->Carbox_Num->text() == "01" && ui->Bogie_Num->text() == "A") { ui->Num_1->Set_Working(); }
		else { ui->Num_1->Set_Default(); }
		if (ui->Carbox_Num->text() == "01" && ui->Bogie_Num->text() == "B") { ui->Num_2->Set_Working(); }
		else { ui->Num_2->Set_Default(); }
		if (ui->Carbox_Num->text() == "02" && ui->Bogie_Num->text() == "A") { ui->Num_3->Set_Working(); }
		else { ui->Num_3->Set_Default(); }
		if (ui->Carbox_Num->text() == "02" && ui->Bogie_Num->text() == "B") { ui->Num_4->Set_Working(); }
		else { ui->Num_4->Set_Default(); }
		if (ui->Carbox_Num->text() == "03" && ui->Bogie_Num->text() == "A") { ui->Num_5->Set_Working(); }
		else { ui->Num_5->Set_Default(); }
		if (ui->Carbox_Num->text() == "03" && ui->Bogie_Num->text() == "B") { ui->Num_6->Set_Working(); }
		else { ui->Num_6->Set_Default(); }
		if (ui->Carbox_Num->text() == "04" && ui->Bogie_Num->text() == "A") { ui->Num_7->Set_Working(); }
		else { ui->Num_7->Set_Default(); }
		if (ui->Carbox_Num->text() == "04" && ui->Bogie_Num->text() == "B") { ui->Num_8->Set_Working(); }
		else { ui->Num_8->Set_Default(); }
		if (ui->Carbox_Num->text() == "05" && ui->Bogie_Num->text() == "A") { ui->Num_9->Set_Working(); }
		else { ui->Num_9->Set_Default(); }
		if (ui->Carbox_Num->text() == "05" && ui->Bogie_Num->text() == "B") { ui->Num_10->Set_Working(); }
		else { ui->Num_10->Set_Default(); }
		if (ui->Carbox_Num->text() == "06" && ui->Bogie_Num->text() == "A") { ui->Num_11->Set_Working(); }
		else { ui->Num_11->Set_Default(); }
		if (ui->Carbox_Num->text() == "06" && ui->Bogie_Num->text() == "B") { ui->Num_12->Set_Working(); }
		else { ui->Num_12->Set_Default(); }
		if (ui->Carbox_Num->text() == "07" && ui->Bogie_Num->text() == "A") { ui->Num_13->Set_Working(); }
		else { ui->Num_13->Set_Default(); }
		if (ui->Carbox_Num->text() == "07" && ui->Bogie_Num->text() == "B") { ui->Num_14->Set_Working(); }
		else { ui->Num_14->Set_Default(); }
		if (ui->Carbox_Num->text() == "08" && ui->Bogie_Num->text() == "A") { ui->Num_15->Set_Working(); }
		else { ui->Num_15->Set_Default(); }
		if (ui->Carbox_Num->text() == "08" && ui->Bogie_Num->text() == "B") { ui->Num_16->Set_Working(); }
		else { ui->Num_16->Set_Default(); }
	}
	/*************************************************************************************************************************************************
	**Function:跟新系统时间
	*************************************************************************************************************************************************/
	ui->Status_Bar->showMessage("系统时间：" + c_Variable::g_Current_Time + "              " + "刷新帧率：" + QString::number(m_Current_FPS));
	if (m_FPS == 50) {
		m_Current_FPS = m_Time->restart() / 50;
		m_Current_FPS = 1000 / m_Current_FPS;
		m_FPS = 0;
	}
	m_FPS += 1;
	if (m_System_Scan) { QTimer::singleShot(30, this, &c_Robot_App_Widget::System_Scan_Done); }
}
/*************************************************************************************************************************************************
**Function:刷新样式表
*************************************************************************************************************************************************/
void c_Robot_App_Widget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_F6) {
		QFile file(QDir::currentPath() + "/Robot_App/stuqss.css");
		file.open(QIODevice::ReadOnly);
		QString strQss = file.readAll();
		this->setStyleSheet(strQss);
	}
}

