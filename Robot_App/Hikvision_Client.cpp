#pragma execution_character_set("utf-8")
#include "Hikvision_Client.h"
/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_Hikvision_Client::c_Hikvision_Client(QObject *parent) : QObject(parent)
{
	
}
/*************************************************************************************************************************************************
**Function:析构
*************************************************************************************************************************************************/
c_Hikvision_Client::~c_Hikvision_Client()
{
	m_State = false;
	m_Stop_Connect = true;
	//关闭预览
	NET_DVR_StopRealPlay(lRealPlayHandle);
	//注销用户
	NET_DVR_Logout(lUserID);
	//释放 SDK 资源
	NET_DVR_Cleanup();
}
/*************************************************************************************************************************************************
**Function:连接设备
*************************************************************************************************************************************************/
void c_Hikvision_Client::Connect_Device(QVariant Login, QVariant Client)
{
	qDebug() << "c_Hikvision_Client::Connect_Device";
	//如果处于连接状态则立即退出
	if (m_State) { 
		qDebug() << "c_Hikvision_Client::Connect_Device_return";
		return; 
	}
	//初始化
	NET_DVR_Init();
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(10000, 3);
	NET_DVR_SetReconnect(50000, true);
	// 注册设备登录参数，包括设备地址、登录用户、密码等
	NET_DVR_USER_LOGIN_INFO struLoginInfo = Login.value<NET_DVR_USER_LOGIN_INFO>();
	NET_DVR_DEVICEINFO_V30 struDeviceInfoV30 = { 0 };
	//设备信息, 输出参数
	lUserID = NET_DVR_Login_V30(
		struLoginInfo.sDeviceAddress,
		struLoginInfo.wPort,
		struLoginInfo.sUserName,
		struLoginInfo.sPassword,
		&struDeviceInfoV30);
	if (lUserID < 0) {
		emit Status("登陆错误代码:" + QString::number(NET_DVR_GetLastError()));
		if (m_Stop_Connect){
			m_Stop_Connect = false;
			return;
		}
		emit Connect_Loop();
	}
	else {
		NET_DVR_CLIENTINFO ClientInfo = Client.value<NET_DVR_CLIENTINFO>();
		lRealPlayHandle = NET_DVR_RealPlay_V30(lUserID, &ClientInfo, NULL, NULL, 0);
		if (lRealPlayHandle < 0)
		{
			emit Status("实时预览错误代码:" + QString::number(NET_DVR_GetLastError()));
			NET_DVR_Logout(lUserID);
			return;
		}
		m_State = true;
		emit Connect_Done();
		emit Status("连接成功");
	}
}
/*************************************************************************************************************************************************
**Function:断开设备
*************************************************************************************************************************************************/
void c_Hikvision_Client::Disconnect_Device()
{
	qDebug() << "c_Hikvision_Client::Disconnect_Device";
	if (!m_State) { 
		qDebug() << "c_Hikvision_Client::Disconnect_Device_return";
		return;
	}  //如果处于断开状态立即退出
	m_State = false;
	m_Stop_Connect = true;
	//关闭预览
	NET_DVR_StopRealPlay(lRealPlayHandle);	
	//注销用户
	NET_DVR_Logout(lUserID);
	emit Disconnect_Done();
	emit Status("断开连接成功");
}