#pragma execution_character_set("utf-8")
#include "Variable.h"
/*************************************************************************************************************************************************
**Function:全局变量声明
*************************************************************************************************************************************************/
QJsonObject c_Variable::g_Communicate_DB;
HWND c_Variable::g_Hikvision_20_winId;
HWND c_Variable::g_Hikvision_21_winId;
QKeyEvent  *c_Variable::Key_F6;
/*************************************************************************************************************************************************
**Function:高低位转浮点型
*************************************************************************************************************************************************/
float c_Variable::Short_To_Float(quint16 high, quint16 low)
{
	if (high == 0 && low == 0) {
		return 0;
	}
	int intSign, intSignRest, intExponent, intExponentRest;
	float faResult, faDigit;
	intSign = high / 32768;  //1000 0000 0000 0000  求第16位的值
	intSignRest = high % 32768;//保留第1至15位的值
	intExponent = intSignRest / 128; //1000 0000 求15至9位的值
	intExponentRest = intSignRest % 128;//保留第8至1位的值
	faDigit = (float)(intExponentRest * 65536 + low) / 8388608;//合成24位后 1000 0000 0000 0000 0000 0000 求第24位的值
	faResult = (float)pow(-1, intSign) *  (faDigit + 1) * (float)pow(2, intExponent - 127);//pow(x, y)计算x的y次幂
	return faResult;
}
/*************************************************************************************************************************************************
**Function:通讯错误信息
*************************************************************************************************************************************************/
QString c_Variable::TCP_Status(int State)
{
	switch (State)
	{
	case 0:return "连接被对等方拒绝（或超时）";
	case 1:return "远程主机关闭了连接。请注意，客户端套接字（即此套接字）将在发送远程关闭通知后关闭";
	case 2:return "找不到主机地址";
	case 3:return "套接字操作失败，因为应用程序缺少所需的权限";
	case 4:return "本地系统资源不足（例如，套接字过多）";
	case 5:return "套接字操作超时";
	case 6:return "数据报大于操作系统的限制（可低至8192字节）";
	case 7:return "网络发生错误（例如，网络电缆意外插拔）";
	case 8:return "为QAbstractSocket:：bind（）指定的地址已在使用中，并被设置为独占";
	case 9:return "为QAbstractSocket:：bind（）指定的地址不属于主机";
	case 10:return "本地操作系统不支持请求的套接字操作（例如，缺少IPv6支持）";
	case 11:return "仅由QAbstractSocketEngine使用，上次尝试的操作尚未完成（仍在后台进行）";
	case 12:return "套接字正在使用代理，代理需要身份验证";
	case 13:return "SSL/TLS握手失败，因此连接已关闭（仅在QSslSocket中使用）";
	case 14:return "无法联系代理服务器，因为与该服务器的连接被拒绝";
	case 15:return "与代理服务器的连接意外关闭（在与最终对等方建立连接之前）";
	case 16:return "与代理服务器的连接超时，或者代理服务器在身份验证阶段停止响应。";
	case 17:return "找不到使用setProxy（）设置的代理地址（或应用程序代理）";
	case 18:return "与代理服务器的连接协商失败，因为无法理解来自代理服务器的响应";
	case 19:return "当套接字处于不允许的状态时，尝试了一个操作";
	case 20:return "正在使用的SSL库报告了一个内部错误。这可能是由于库的安装错误或配置错误造成的";
	case 21:return "提供了无效数据（证书、密钥、密码等），其使用导致SSL库中出现错误";
	case 22:return "发生临时错误（例如，操作将被阻塞，套接字为非阻塞）";
	case 23:return "套接字未连接";
	case 24:return "套接字正在执行主机名查找";
	case 25:return "套接字已开始建立连接";
	case 26:return "建立了一个连接";
	case 27:return "套接字绑定到一个地址和端口";
	case 28:return "套接字即将关闭（数据可能仍在等待写入）";
	case 29:return "套接字监听状态（仅供内部使用）";
	case -1:return "发生了一个无法识别的错误";
	default:return "无状态";
	}
}
QString c_Variable::Modbus_Status(int State)
{
	switch (State)
	{
	case 0:return "已连接";
	case 1:return "读取操作期间发生错误";
	case 2:return "写入操作期间发生错误";
	case 3:return "尝试打开后端时出错";
	case 4:return "尝试设置配置参数时出错";
	case 5:return "I/O期间发生超时。I/O操作未在给定的时间范围内完成";
	case 6:return "发生Modbus特定协议错误";
	case 7:return "由于设备断开连接，回复被中止";
	case 8:return "发生未知错误";
	case 9:return "设备已断开连接";
	case 10:return "正在连接设备";
	case 11:return "设备正在关闭";
	default:return "无状态";
	}
}
/*************************************************************************************************************************************************
**Function:非阻塞延时
*************************************************************************************************************************************************/
bool c_Variable::msleep(const int mSec)
{
	QEventLoop *loop = new QEventLoop;
	QTimer::singleShot(mSec, loop, &QEventLoop::quit);
	loop->exec();
	return true;
}
/*************************************************************************************************************************************************
**Function:构造函数/读取通讯参数
*************************************************************************************************************************************************/
c_Variable::c_Variable(QObject *parent) : QObject(parent)
{
	exist = System_Dir.exists(QDir::currentPath() + "/System_DB");
	if (!exist) {
		System_Dir.mkdir(QDir::currentPath() + "/System_DB");
	}
	exist = System_Dir.exists(QDir::currentPath() + "/Robot_App");
	if (!exist) {
		System_Dir.mkdir(QDir::currentPath() + "/Robot_App");
	}
	Communicate_DB.setFileName(QDir::currentPath() + "/System_DB/Communicate_DB.json");
	Communicate_DB.open(QFile::ReadOnly | QIODevice::Text);
	QByteArray Data = Communicate_DB.readAll();
	Communicate_DB.close();
	QJsonDocument DB_Doc(QJsonDocument::fromJson(Data));
	g_Communicate_DB = DB_Doc.object();
	Key_F6 = new QKeyEvent(QEvent::KeyPress, Qt::Key_F6, Qt::NoModifier);
}
/*************************************************************************************************************************************************
**Function:析构函数(修改通讯参数本地文件)
*************************************************************************************************************************************************/
c_Variable::~c_Variable()
{
	Communicate_DB.open(QFile::WriteOnly | QIODevice::Text);
	QJsonDocument DB_Doc;
	DB_Doc.setObject(g_Communicate_DB);
	Communicate_DB.write(DB_Doc.toJson());
	Communicate_DB.close();
	delete Key_F6;
}
