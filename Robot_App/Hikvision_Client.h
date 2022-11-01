#include "Public_Header.h"
#include "HCNetSDK.h"
#include "PlayM4.h"
class c_Hikvision_CallBack : public QObject
{
	Q_OBJECT

public:
	c_Hikvision_CallBack(QObject * parent = nullptr);
	virtual ~c_Hikvision_CallBack();
	//操作对象
	static c_Hikvision_CallBack *g_Hikvision_CallBack;
	//设置异常消息回调函数
	static void CALLBACK ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
signals:
	void Status(QString status);//监视器状态
};

class c_Hikvision_Client : public QObject
{
	Q_OBJECT
public:
	explicit c_Hikvision_Client(QObject *parent = nullptr);
	virtual ~c_Hikvision_Client();
	LONG lUserID;
	LONG lRealPlayHandle;
	NET_DVR_USER_LOGIN_INFO struLoginInfo;
	NET_DVR_CLIENTINFO ClientInfo;
	NET_DVR_DEVICEINFO_V30 struDeviceInfoV30;

	public slots:
	void Connect_Device(QString ip, int port, QString name, QString key, HWND hWnd);
	void Disconnect_Device();

signals:
	void Connect_Done();//工作状态
	void Disconnect_Done();//非工作状态
	void Status(QString status);//监视器状态
};
