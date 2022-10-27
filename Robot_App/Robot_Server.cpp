#pragma execution_character_set("utf-8")
#include "Robot_Server.h"
/*************************************************************************************************************************************************
**Function:    构造函数
**Description: 初始化成员变量
**Input:       无输入
**Output:      无输出
**Return:      无返回
**Others:
*************************************************************************************************************************************************/
c_Robot_Server::c_Robot_Server(QObject *parent) : QObject(parent)
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
c_Robot_Server::~c_Robot_Server()
{
    m_Socket->deleteLater();
	m_Server->deleteLater();
}
/*************************************************************************************************************************************************
**Function:    初始化函数
**Description: 线程的构造函数
**Input:       无输入
**Output:      无输出
**Return:      无返回
**Others:
*************************************************************************************************************************************************/
void c_Robot_Server::Init()
{
    m_Server = new QTcpServer;//实例化服务器
	m_Socket = new QTcpSocket;//实例化客户端
    //错误报告
    QObject::connect(m_Server, &QTcpServer::acceptError, this, &c_Robot_Server::Status);
	QObject::connect(m_Server, &QTcpServer::newConnection, this, &c_Robot_Server::New_Connection);
	emit Init_Done();
}
/*************************************************************************************************************************************************
**Function:    Connect_Device()
**Description: 连接tcpsocket服务器
**Input:       -> ip
               -> port
**Output:      New_Connection
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
void c_Robot_Server::Connect_Device(QString ip, int port)
{    
	//侦听一个端口，使得客户端可以使用这个端口访问服务器
	m_Server->listen(QHostAddress(ip), port);
}
/*************************************************************************************************************************************************
**Function:    Disconnect_Device()
**Description: 断开tcpSocket服务器
**Input:       无输入
**Output:      onStateChanged、status
**Return:      无返回
**Others:      服务器主动断开
*************************************************************************************************************************************************/
void c_Robot_Server::Disconnect_Device()
{
    if((!m_Socket) || (m_Socket->state() != QTcpSocket::ConnectedState))
    {
        return;
    }
    m_Socket->close();
}
/*************************************************************************************************************************************************
**Function:    New_Connection()
**Description: 建立新的连接
**Input:       无输入
**Output:      onStateChanged、status
**Return:      无返回
**Others:      * nextPendingConnection
               * 将下一个挂起连接作为已连接的QTcpSocket对象返回。
               * 套接字是作为服务器的子对象创建的，这意味着当QTcpServer对象被销毁时，
               * 它将被自动删除。在处理完对象后，最好显式删除该对象，以避免浪费内存。
               * 将下一个挂起的连接作为已连接的QTcpSocket对象返回。
               * 套接字是作为服务器的子对象创建的，这意味着当QTcpServer对象被销毁时，
               * 它将被自动删除。在处理完对象后显式删除它仍然是一个好主意，以避免浪费内存。
               * 如果在没有挂起的连接时调用此函数，则返回0。
               * 注意：返回的QTcpSocket对象不能从其他线程使用。如果要使用来自另一个线程的传入连接，则需要重写incomingConnection（）。
*************************************************************************************************************************************************/
void c_Robot_Server::New_Connection()
{
    //获取客户端连接
    m_Socket = m_Server->nextPendingConnection();
	emit Connect_Done();
	emit Status(26);
    //监控连接状态，如果断开，则需要析构旧的m_Socket
    QObject::connect(m_Socket, &QTcpSocket::stateChanged, this, &c_Robot_Server::State_Changed);
    QObject::connect(m_Socket, &QTcpSocket::readyRead, this, &c_Robot_Server::Read_Json);
}
/*************************************************************************************************************************************************
**Function:    Write_Json(QJsonObject value)
**Description: 写消息
**Input:       QJsonObject value -> {"RGV":{},"Jaka_120":{},"Jaka_121":{},"Meijidenki_20":{},"Meijidenki_21":{},"Hypersen_30":{},"Hypersen_31":{}}
**Output:      status、writeError、writeDone
**Return:      无返回
**Others:      无
*************************************************************************************************************************************************/
void c_Robot_Server::Write_Json(QJsonObject json)
{
	if ((!m_Socket) || (m_Socket->state() != QTcpSocket::ConnectedState))
	{
		return;
	}
	//将QJsonObject转为QByteArray
	QByteArray sendMessage = QJsonDocument(json).toJson();
	//写入设备
	m_Socket->write(sendMessage);
	//写入套接字
	if (m_Socket->flush())
	{
		emit Write_Json_Done();
	}
	else
	{
		emit Write_Json_Error();
	}
}
void c_Robot_Server::Write_String(QString str)
{
	if ((!m_Socket) || (m_Socket->state() != QTcpSocket::ConnectedState))
	{
		return;
	}
	//写入设备
	m_Socket->write(str.toUtf8());
	if (m_Socket->flush())
	{
		emit Write_String_Done();
	}
	else
	{
		emit Write_String_Error();
	}
}
/*************************************************************************************************************************************************
**Function:    Read_Json()
**Description: 读消息
**Input:       QJsonObject value -> {"cmdName":"cmd"}
**Output:      status、readError、readDone
**Return:      无返回
**Others:      只适用于JSON格式的传输
*************************************************************************************************************************************************/
void c_Robot_Server::Read_Json()
{
    if((!m_Socket) || (m_Socket->state() != QTcpSocket::ConnectedState)){
        return;
    }
    QByteArray buffer = m_Socket->readAll();
	if(buffer.isEmpty()){
		emit Read_String_Error();
		emit Read_Json_Error();
		return;
	}
	QJsonDocument doc = QJsonDocument::fromJson(buffer);
	QJsonObject object = doc.object();
	//如果接收到的数据为空
	if (object.isEmpty()) {
		emit Read_String_Done(QString(buffer));
		emit Read_Json_Error();
		return;
	}
    //将接收到的数据传递出去
    emit Read_Json_Done(object);
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
void c_Robot_Server::State_Changed()
{
    if(m_Socket->state() == QAbstractSocket::UnconnectedState)
    {
        m_Socket->abort();
        emit Disconnect_Done();
        emit Status(23);
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
		emit Connect_Done();
		emit Status(26);
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
