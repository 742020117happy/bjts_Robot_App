/*************************************************************************************************************************************************
**Copyright(C), 2020-2025, bjtds.
**Version:2.0
**Author: dz
**Date:2022.2.1
**Description:
**Others:                              "TcpServer"
                              ____________________________
               connectDevice->|REQ          |        DONE|->Done
            disconnectDevice->|DISCONNECT   |       ERROR|->Error
                              |read/write   |      STATUS|->status
                     ip,port->|CONNECT      |            |
                      value ->|DATA         |            |
                              ----------------------------
*************************************************************************************************************************************************/
#pragma once
#include "Variable.h"
#include "Public_Header.h"

class c_Robot_Server : public QObject
{
    Q_OBJECT
public:
    explicit c_Robot_Server(QObject *parent = nullptr);
	virtual ~c_Robot_Server();

	public slots:
	void Init();//子线程初始化
	void Connect_Device(QString ip, int port);//连接到服务器
	void Disconnect_Device();//断开连接
	void Write_Json(QJsonObject json); //写Json数据
	void Write_String(QString str);//写STring数据

signals:
	void Init_Done();//完成初始化
    void Connect_Done();//连接到服务器完成
    void Disconnect_Done();//断开连接完成
	void Write_Json_Done(); //写完成
	void Write_Json_Error(); //写错误
	void Write_String_Done();//写完成
	void Write_String_Error();//写错误
    void Read_Json_Done(QJsonObject buffer);//读完成
    void Read_Json_Error();//读错误
	void Read_String_Done(QString buffer);//读完成
	void Read_String_Error();//读错误
    void Status(int state);//通讯状态

private slots:
    void New_Connection();//新的连接被建立
    void Read_Json();//读Json数据
    void State_Changed();//状态改变

private:
    QTcpServer *m_Server;//服务端
    QTcpSocket *m_Socket;//客户端、套接字
};

