#pragma execution_character_set("utf-8") 
#include "Public_Header.h"
#include "Robot_Logon_Widget.h"

int main(int argc, char *argv[])
{
	QApplication Application(argc, argv);

	c_Variable Variable_DB;
	c_Robot_Logon_Widget Robot_Logon_Widget;

	Robot_Logon_Widget.show();

	return Application.exec();

}
