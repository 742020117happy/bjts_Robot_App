#pragma execution_character_set("utf-8")
#include "Robot_Logon_Widget.h"
//构造函数
c_Robot_Logon_Widget::c_Robot_Logon_Widget(QWidget *parent) : QWidget(parent)
{
	//实列化
	ui = new Ui_Robot_Logon_Widget;
	Robot_App_Widget = new c_Robot_App_Widget;
	ui->setupUi(this);
	//默认用户名
	ui->lineEdit_Login->setText("root");
	//默认密码
	ui->lineEdit_Pwd->setText("123456");
	//登陆密码
	QObject::connect(ui->pB_OK, &QPushButton::clicked, this, &c_Robot_Logon_Widget::Logon_Robot_App);
	//取消退出
	QObject::connect(ui->pB_Exit, &QPushButton::clicked, this, &c_Robot_Logon_Widget::close);
	//显示界面
	QObject::connect(this, &c_Robot_Logon_Widget::Show_Robot_App, Robot_App_Widget, &c_Robot_App_Widget::showMaximized);
}
//析构函数
c_Robot_Logon_Widget::~c_Robot_Logon_Widget()
{
	delete Robot_App_Widget;
	delete ui;
}
//登录界面
void c_Robot_Logon_Widget::Logon_Robot_App()
{
	if (ui->lineEdit_Login->text() == "root" && ui->lineEdit_Pwd->text() == "123456"){
		this->hide();
		emit Show_Robot_App();
	}
	else if (ui->lineEdit_Login->text() == "" || ui->lineEdit_Pwd->text() == ""){
		QMessageBox::warning(this, tr("提示"), tr("用户名或密码不能为空"), QMessageBox::Yes);
		ui->lineEdit_Login->setFocus();
	}
	else if (ui->lineEdit_Login->text() != "root"){
		QMessageBox::warning(this, tr("提示"), tr("用户名不存在，请重新输入"), QMessageBox::Yes);
		ui->lineEdit_Login->clear();
		ui->lineEdit_Login->setFocus();
	}
	else if (ui->lineEdit_Login->text() == "root" && ui->lineEdit_Pwd->text() != "123456"){
		QMessageBox::warning(this, tr("提示"), tr("用户名密码不匹配，请重新输入"), QMessageBox::Yes);
		ui->lineEdit_Login->setFocus();
	}
}
//回车触发
void c_Robot_Logon_Widget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
		emit ui->pB_OK->click();
	}
}
