#pragma execution_character_set("utf-8")
#include "Scan_Client.h"

/*************************************************************************************************************************************************
**Function:    构造函数
**Description: 初始化成员变量
**Input:       无输入
**Output:      无输出
**Return:      无返回
**Others:      构造函数初始化在主线程实例化中完成，故在子线程中运行时，成员函数的初始化不能在构造函数中完成
*************************************************************************************************************************************************/
c_Scan_Client::c_Scan_Client(QObject *parent) : QObject(parent)
{

}
/*************************************************************************************************************************************************
**Function:    析构函数
**Description: 析构成员变量
**Input:       无输入
**Output:      无输出
**Return:      无返回
**Others:      析构成员变量后，再次操作时需要重新实例化
*************************************************************************************************************************************************/
c_Scan_Client::~c_Scan_Client()
{
	delete m_Socket;
}
/*************************************************************************************************************************************************
**Function:    初始化函数
**Description: 线程的构造函数
**Input:       无输入
**Output:      无输出
**Return:      无返回
**Others:
*************************************************************************************************************************************************/
void c_Scan_Client::Init()
{
    //实列化
    m_Socket = new QUdpSocket;
    //如果，UDP，状态改变，执行，本线程，状态改变函数
    QObject::connect(m_Socket, &QUdpSocket::stateChanged, this, &c_Scan_Client::State_Changed);
    //错误诊断
    QObject::connect(m_Socket, QOverload<QAbstractSocket::SocketError>::of(&QUdpSocket::error), this, [=](QAbstractSocket::SocketError socketError){
        emit Status(socketError);
    });
	//绑定读函数
	QObject::connect(m_Socket, &QUdpSocket::readyRead, this, &c_Scan_Client::Read_String);
}
/*************************************************************************************************************************************************
**Function:    Connect_Device(QJsonObject dataBlock)
**Description: 绑定udpsocket
**Input:       m_localIp;
               m_remoteIp;
               m_localPort;
               m_remotePort;
**Output:      onStateChanged -> connectDone    连接成功
                              -> disconnectDone 断开成功
               status -> 0 连接被对等方拒绝（或超时）。
                      -> 1 远程主机关闭了连接。请注意，客户端套接字（即此套接字）将在发送远程关闭通知后关闭。
                      -> 2 找不到主机地址。
                      -> 3 套接字操作失败，因为应用程序缺少所需的权限。
                      -> 4 本地系统资源不足（例如，套接字过多）。
                      -> 5 套接字操作超时。
                      -> 6 数据报大于操作系统的限制（可低至8192字节）。
                      -> 7 网络发生错误（例如，网络电缆意外插拔）。
                      -> 8 为QAbstractSocket:：bind（）指定的地址已在使用中，并被设置为独占。
                      -> 9 为QAbstractSocket:：bind（）指定的地址不属于主机。
                      -> 10 本地操作系统不支持请求的套接字操作（例如，缺少IPv6支持）。
                      -> 11 仅由QAbstractSocketEngine使用，上次尝试的操作尚未完成（仍在后台进行）。
                      -> 12 套接字正在使用代理，代理需要身份验证。
                      -> 13 SSL/TLS握手失败，因此连接已关闭（仅在QSslSocket中使用）
                      -> 14 无法联系代理服务器，因为与该服务器的连接被拒绝
                      -> 15 与代理服务器的连接意外关闭（在与最终对等方建立连接之前）
                      -> 16 与代理服务器的连接超时，或者代理服务器在身份验证阶段停止响应。
                      -> 17 找不到使用setProxy（）设置的代理地址（或应用程序代理）。
                      -> 18 与代理服务器的连接协商失败，因为无法理解来自代理服务器的响应。
                      -> 19 当套接字处于不允许的状态时，尝试了一个操作。
                      -> 20 正在使用的SSL库报告了一个内部错误。这可能是由于库的安装错误或配置错误造成的。
                      -> 21 提供了无效数据（证书、密钥、密码等），其使用导致SSL库中出现错误。
                      -> 22 发生临时错误（例如，操作将被阻塞，套接字为非阻塞）。
                      -> 23 套接字未连接
                      -> 24 套接字正在执行主机名查找
                      -> 25 套接字已开始建立连接
                      -> 26 建立了一个连接
                      -> 27 套接字绑定到一个地址和端口
                      -> 28 套接字即将关闭（数据可能仍在等待写入）
                      -> 29 套接字监听状态（仅供内部使用）
                      -> -1 发生了一个无法识别的错误。
**Return:      无返回
**Others:      二次调用时，如果已连接，则会先断开原来的连接
               bind -> 使用BindMode模式绑定到端口上的地址。
                       对于UDP套接字，绑定后，只要UDP数据报到达指定的地址和端口，就会发出信号QUdpSocket:：readyRead（）。因此，此函数对于编写UDP服务器非常有用。
                       对于TCP套接字，此函数可用于指定用于传出连接的接口，这在多个网络接口的情况下非常有用。
                       默认情况下，使用DefaultForPlatform绑定模式绑定套接字。如果未指定端口，则选择随机端口。
                       成功后，函数返回true，套接字进入BoundState；否则返回false。
                joinMulticastGroup -> 使用BindMode模式绑定到端口上的地址。
                       对于UDP套接字，绑定后，只要UDP数据报到达指定的地址和端口，就会发出信号QUdpSocket:：readyRead（）。因此，此函数对于编写UDP服务器非常有用。
                       对于TCP套接字，此函数可用于指定用于传出连接的接口，这在多个网络接口的情况下非常有用。
                       默认情况下，使用DefaultForPlatform绑定模式绑定套接字。如果未指定端口，则选择随机端口。
                       成功后，函数返回true，套接字进入BoundState；否则返回false。
*************************************************************************************************************************************************/
void c_Scan_Client::Connect_Device(QString ip, int port)
{
    //如果已连接则返回
	qDebug() << "c_Scan_Client::Connect_Device";
    if(m_Socket->state() == QAbstractSocket::BoundState)
    {
		qDebug() << "c_Scan_Client::Connect_Device_return";
		return;
    }
    //绑定主机、监听信号
	m_Socket->bind(QHostAddress(ip), port);
}
/*************************************************************************************************************************************************
**Function:    Disconnect_Device()
**Description: 断开tcpsocket服务器
**Input:       无输入
**Output:      onStateChanged、status
**Return:      无返回
**Others:      如果未连接，则会直接返回
*************************************************************************************************************************************************/
void c_Scan_Client::Disconnect_Device()
{
	qDebug() << "c_Scan_Client::Disconnect_Device";
	if((!m_Socket) || (m_Socket->state() != QAbstractSocket::BoundState)){
		qDebug() << "c_Scan_Client::Disconnect_Device_return";
        return;
    }
    m_Socket->close();
}
/*************************************************************************************************************************************************
**Function:    Read()
**Description: 读消息
**Input:       任何类型
**Output:      status、readError、readDone
**Return:      无返回
**Others:      hasPendingDatagrams() -> 如果至少有一个数据报正在等待读取，则返回true；否则返回false。
**             pendingDatagramSize() -> 返回第一个挂起的UDP数据报的大小。如果没有可用的数据报，此函数将返回-1。
*************************************************************************************************************************************************/
void c_Scan_Client::Read_String()
{
    if(m_Socket->hasPendingDatagrams())
    {
        QByteArray dataGram;
        dataGram.resize(static_cast<int>(m_Socket->pendingDatagramSize()));
        //接受信息
        qint64 len = m_Socket->readDatagram(dataGram.data(),dataGram.size());
        //如果消息不为空则发送消息到主线程
        if(len > 0)
        {
			QString value = QString(dataGram);
			emit Read_String_Done(value);
        }
    }
}
/*************************************************************************************************************************************************
**Function:    Write(QString ip, int port, QString value)
**Description: 写消息
**Input:       QString value -> "   &   "
               QString ip
			   int port
**Output:      status、writeError、writeDone
**Return:      无返回
**Others:      无
*************************************************************************************************************************************************/
void c_Scan_Client::Write_String(QString ip, int port, QString value)
{
   if((!m_Socket) || (m_Socket->state() != QAbstractSocket::BoundState) || (value.isEmpty()))
   {
       return;
   }
   //发送消息
   if(m_Socket->writeDatagram(value.toUtf8(), QHostAddress(ip), port) != -1)
   {
       emit Write_String_Done();
   }
}
/*************************************************************************************************************************************************
**Function:    On_State_Changed()
**Description: tcpsocket连接状态改变
**Input:       无输入
**Output:      connectDone、disconnectDone、status
**Return:      无返回
**Others:      -> 23 套接字未连接
               -> 24 套接字正在执行主机名查找
               -> 25 套接字已开始建立连接
               -> 26 建立了一个连接
               -> 27 套接字绑定到一个地址和端口
               -> 28 套接字即将关闭（数据可能仍在等待写入）
               -> 29 套接字监听状态（仅供内部使用）
*************************************************************************************************************************************************/
void c_Scan_Client::State_Changed()
{
    if(m_Socket->state() == QAbstractSocket::UnconnectedState)
    {
        emit Status(23);
        emit Disconnect_Done();
    }
    if(m_Socket->state() == QAbstractSocket::HostLookupState)
    {
        emit Status(24);
    }
    if(m_Socket->state() == QAbstractSocket::ConnectingState)
    {
        emit Status(25);
    }
    if(m_Socket->state() == QAbstractSocket::ConnectedState)
    {
        emit Status(26);
        emit Connect_Done();
    }
    if(m_Socket->state() == QAbstractSocket::BoundState)
    {
        emit Status(27);
        emit Connect_Done();
    }
    if(m_Socket->state() == QAbstractSocket::ClosingState)
    {
        emit Status(28);
    }
    if(m_Socket->state() == QAbstractSocket::ListeningState)
    {
        emit Status(29);
    }
}
