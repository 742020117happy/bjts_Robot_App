#include "HCNetSDK.h"
#include "PlayM4.h"
#include "Variable.h"
#include "Public_Header.h"

Q_DECLARE_METATYPE(NET_DVR_USER_LOGIN_INFO);
Q_DECLARE_METATYPE(NET_DVR_CLIENTINFO);

class c_Hikvision_Client : public QObject
{
	Q_OBJECT
public:
	explicit c_Hikvision_Client(QObject *parent = nullptr);
	virtual ~c_Hikvision_Client();
	LONG lUserID;
	LONG lRealPlayHandle;
	bool m_State = false;

	public slots:
	void Connect_Device(QVariant Login, QVariant Client);
	void Disconnect_Device();

signals:
	void Connect_Done();//工作状态
	void Disconnect_Done();//非工作状态
	void Status(QString status);//监视器状态
};
