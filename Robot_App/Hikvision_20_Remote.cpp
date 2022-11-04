#pragma execution_character_set("utf-8")
#include "Hikvision_20_Remote.h"
/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_Hikvision_20_Remote::c_Hikvision_20_Remote(c_Hikvision_Remote * parent) : c_Hikvision_Remote(parent) {

}
/*************************************************************************************************************************************************
**Function:析构指针
*************************************************************************************************************************************************/
c_Hikvision_20_Remote::~c_Hikvision_20_Remote() {

}
/*************************************************************************************************************************************************
**Function:连接设备
*************************************************************************************************************************************************/
void c_Hikvision_20_Remote::Connect()
{
	c_Variable::msleep(10000);//等待10s

	QString ip = c_Variable::g_Communicate_DB.value("Hikvision_20_Ip").toString();
	int port = c_Variable::g_Communicate_DB.value("Hikvision_20_Port").toInt();
	QString	name = c_Variable::g_Communicate_DB.value("Hikvision_20_User").toString();
	QString	key = c_Variable::g_Communicate_DB.value("Hikvision_20_Pass").toString();
	NET_DVR_USER_LOGIN_INFO struLoginInfo;
	struLoginInfo.bUseAsynLogin = 0; //同步登录方式
	strcpy(struLoginInfo.sDeviceAddress, ip.toLatin1().data()); //设备IP地址
	struLoginInfo.wPort = port; //设备服务端口
	strcpy(struLoginInfo.sUserName, name.toLatin1().data()); //设备登录用户名
	strcpy(struLoginInfo.sPassword, key.toLatin1().data()); //设备登录密码	
	QVariant Login;
	Login.setValue(struLoginInfo);

	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.lChannel = 1; //Channel number 设备通道号
	ClientInfo.hPlayWnd = c_Variable::g_Hikvision_20_winId;  //窗口为空，设备SDK不解码只取流
	ClientInfo.lLinkMode = 0;    //Main Stream
	ClientInfo.sMultiCastIP = NULL;
	QVariant Client;
	Client.setValue(ClientInfo);

	emit Connect_Device(Login, Client);
}