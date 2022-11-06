#pragma execution_character_set("utf-8")
#include "Jaka_Client.h"

/*************************************************************************************************************************************************
**Function:    构造函数
**Description: 初始化成员变量
**Input:       无输入
**Output:      无输出
**Return:      无返回
**Others:      构造函数初始化在主线程实例化中完成，故在子线程中运行时，成员函数的初始化不能在构造函数中完成
*************************************************************************************************************************************************/
c_Jaka_Client::c_Jaka_Client(QObject *parent) : QObject(parent)
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
c_Jaka_Client::~c_Jaka_Client()
{
	m_Stop_Connect = true;
	if (m_Socket->state() == QAbstractSocket::ConnectedState) { m_Socket->close();}
	m_Socket->deleteLater();
}
/*************************************************************************************************************************************************
**Function:    初始化函数
**Description: 线程的构造函数
**Input:       无输入
**Output:      无输出
**Return:      无返回
**Others:
*************************************************************************************************************************************************/
void c_Jaka_Client::Init()
{
    //实例化
    m_Socket = new QTcpSocket;
    //如果，客户端，状态改变，发送，本线程，状态信息
    QObject::connect(m_Socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, [=](QAbstractSocket::SocketError socketError)
    {
       emit Status(socketError);
    });
    //如果，客户端，状态改变，执行，本线程，状态改变函数
    QObject::connect(m_Socket, &QTcpSocket::stateChanged, this, &c_Jaka_Client::State_Changed);
	//开启监听模式{机器人，有可读取通道，对象，读取信号}
	QObject::connect(m_Socket, &QTcpSocket::readyRead, this, &c_Jaka_Client::Read_Json);
}
/*************************************************************************************************************************************************
**Function:    Connect_Device(QJsonObject dataBlock)
**Description: 连接tcpsocket服务器
**Input:       -> ip
               -> port
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
*************************************************************************************************************************************************/
void c_Jaka_Client::Connect_Device(QString ip, int port)
{
    //如果已连接则返回
    if(!m_Socket || m_Socket->state() != QAbstractSocket::UnconnectedState) {return;}
	//如果发出断开连接请求则终止循环连接，并复位请求标志
	if (m_Stop_Connect) {
		m_Stop_Connect = false;
		return;
	}
    //建立新的连接
	m_Ip = ip;
	m_Port = port;
	m_Socket->connectToHost(ip, port);
	//等待连接3秒
	if(!m_Socket->waitForConnected(3000)){
		emit Connect_Loop(m_Ip, m_Port);
	}
}
/*************************************************************************************************************************************************
**Function:    Disconnect_Device()
**Description: 断开tcpsocket服务器
**Input:       无输入
**Output:      onStateChanged、status
**Return:      无返回
**Others:      如果未连接，则会直接返回
*************************************************************************************************************************************************/
void c_Jaka_Client::Disconnect_Device()
{
	m_Stop_Connect = true;
	if(!m_Socket || m_Socket->state() != QAbstractSocket::ConnectedState){return;}
    m_Socket->close();
}
/*************************************************************************************************************************************************
**Function:    Read()
**Description: 读消息
**Input:       QJsonObject value -> {"cmdName":"cmd","errorCode":"0","errorMsg":"unknown","parameter1":"parameter1","parameter2":"parameter2",...}
**Output:      status、readError、readDone
**Return:      无返回
**Others:
*************************************************************************************************************************************************/
void c_Jaka_Client::Read_Json()
{
	if (!m_Read_Ready && m_Port == c_Variable::g_Communicate_DB.value("Jaka_Monitor_Port").toInt()) {
		return;
	}
	m_Read_Ready = false;
	QByteArray buffer = m_Socket->readAll();
    //如果接收到的数据为空
    if(buffer.isEmpty())
    {
        emit Read_Json_Error();
		m_Read_Ready = true;
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(buffer);
    QJsonObject object = doc.object();
    if(object.isEmpty())
    {
        emit Read_Json_Error();
		m_Read_Ready = true;
        return;
    }
    emit Read_Json_Done(object);	
}
/*************************************************************************************************************************************************
**Function:    Read_Ready()
**Description: 读消息准备
**Input:       
**Output:      m_Read_Ready
**Return:      无返回
**Others:
*************************************************************************************************************************************************/
void c_Jaka_Client::Read_Ready()
{
	m_Read_Ready = true;
}
/*************************************************************************************************************************************************
**Function:    Write(QJsonObject value)
**Description: 写消息
**Input:       QJsonObject value -> {"cmdName":"cmd","parameter1":"parameter1","parameter2":"parameter2",...}
**Output:      status、writeError、writeDone
**Return:      无返回
**Others:      无
*************************************************************************************************************************************************/
void c_Jaka_Client::Write_Json(QJsonObject value)
{
    //如果设备未打开
    if((!m_Socket) || (m_Socket->state() != QAbstractSocket::ConnectedState) || (value.isEmpty()))
    {
        return;
    }
    //将QJsonObject转为QByteArray
    QByteArray sendMessage = QJsonDocument(value).toJson();
    //写入设备
    m_Socket->write(sendMessage);
    //写入套接字
    if(m_Socket->flush())
    {
        emit Write_Json_Done();
    }
    else
    {
        emit Write_Json_Error();
    }
}
/*************************************************************************************************************************************************
**Function:    State_Changed()
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
void c_Jaka_Client::State_Changed()
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
