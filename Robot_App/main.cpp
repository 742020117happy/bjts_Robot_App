#pragma execution_character_set("utf-8") 
#include "Robot_Logon_Widget.h"

c_Hypersen_CallBack *c_Hypersen_CallBack::g_Hypersen_CallBack = new c_Hypersen_CallBack;
c_Meijidenki_CallBack *c_Meijidenki_CallBack::g_Meijidenki_CallBack = new c_Meijidenki_CallBack;
c_Hikvision_CallBack *c_Hikvision_CallBack::g_Hikvision_CallBack = new c_Hikvision_CallBack;

int main(int argc, char *argv[])
{
	QApplication Application(argc, argv);

	c_Variable Variable_DB;
	c_Robot_Logon_Widget Robot_Logon_Widget;

	return Application.exec();

	c_Hypersen_CallBack::g_Hypersen_CallBack->deleteLater();
	c_Meijidenki_CallBack::g_Meijidenki_CallBack->deleteLater();
	c_Hikvision_CallBack::g_Hikvision_CallBack->deleteLater();
}
