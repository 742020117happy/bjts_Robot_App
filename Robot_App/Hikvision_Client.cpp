#pragma execution_character_set("utf-8")
#include "Hikvision_Client.h"

/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_Hikvision_CallBack::c_Hikvision_CallBack(QObject * parent) : QObject(parent)
{
	// 初始化
	NET_DVR_Init();
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);
	//设置异常消息回调函数
	NET_DVR_SetExceptionCallBack_V30(0, NULL, ExceptionCallBack, NULL);
}
/*************************************************************************************************************************************************
**Function:析构指针
*************************************************************************************************************************************************/
c_Hikvision_CallBack::~c_Hikvision_CallBack()
{
	//释放 SDK 资源
	NET_DVR_Cleanup();
}
/*************************************************************************************************************************************************
**Function:状态回调
*************************************************************************************************************************************************/
void CALLBACK c_Hikvision_CallBack::ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	char tempbuf[256] = { 0 };
	switch (dwType)
	{
	case EXCEPTION_RECONNECT: //预览时重连
		emit g_Hikvision_CallBack->Status("预览时重连");
		break;
	default:
		break;
	}
}
/*************************************************************************************************************************************************
**Function:构造函数(状态监视)
*************************************************************************************************************************************************/
c_Hikvision_Client::c_Hikvision_Client(QObject *parent) : QObject(parent)
{
	QObject::connect(c_Hikvision_CallBack::g_Hikvision_CallBack, &c_Hikvision_CallBack::Status, this, &c_Hikvision_Client::Status);
}
/*************************************************************************************************************************************************
**Function:析构
*************************************************************************************************************************************************/
c_Hikvision_Client::~c_Hikvision_Client()
{
	//关闭预览
	NET_DVR_StopRealPlay(lRealPlayHandle);
	//注销用户
	NET_DVR_Logout(lUserID);
}
/*************************************************************************************************************************************************
**Function:连接设备
*************************************************************************************************************************************************/
void c_Hikvision_Client::Connect_Device(QString ip, int port, QString name, QString key, HWND hWnd)
{
	// 注册设备登录参数，包括设备地址、登录用户、密码等
	struLoginInfo.bUseAsynLogin = 0; //同步登录方式
	strcpy(struLoginInfo.sDeviceAddress, ip.toLatin1().data()); //设备IP地址
	struLoginInfo.wPort = port; //设备服务端口
	strcpy(struLoginInfo.sUserName, name.toLatin1().data()); //设备登录用户名
	strcpy(struLoginInfo.sPassword, key.toLatin1().data()); //设备登录密码																			  //设备信息, 输出参数
	lUserID = NET_DVR_Login_V30(
		struLoginInfo.sDeviceAddress,
		struLoginInfo.wPort,
		struLoginInfo.sUserName,
		struLoginInfo.sPassword,
		&struDeviceInfoV30);
	if (lUserID < 0) {
		emit Status("登陆错误代码:" + QString::number(NET_DVR_GetLastError()));
	}
	else {
		ClientInfo.lChannel = 1; //Channel number 设备通道号
		ClientInfo.hPlayWnd = hWnd;  //窗口为空，设备SDK不解码只取流
		ClientInfo.lLinkMode = 0;    //Main Stream
		ClientInfo.sMultiCastIP = NULL;
		lRealPlayHandle = NET_DVR_RealPlay_V30(lUserID, &ClientInfo, NULL, NULL, 0);
		if (lRealPlayHandle < 0)
		{
			emit Status("实时预览错误代码:" + QString::number(NET_DVR_GetLastError()));
			NET_DVR_Logout(lUserID);
			return;
		}
		emit Connect_Done();
		emit Status("连接成功");
	}
}
/*************************************************************************************************************************************************
**Function:断开设备
*************************************************************************************************************************************************/
void c_Hikvision_Client::Disconnect_Device()
{
	//关闭预览
	NET_DVR_StopRealPlay(lRealPlayHandle);
	//注销用户
	NET_DVR_Logout(lUserID);
	emit Status("断开连接成功");
}