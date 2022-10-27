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
	void Init();//���̳߳�ʼ��
	void Connect_Device(QString ip, int port);//���ӵ�������
	void Disconnect_Device();//�Ͽ�����
	void Write_Json(QJsonObject json); //дJson����
	void Write_String(QString str);//дSTring����

signals:
	void Init_Done();//��ɳ�ʼ��
    void Connect_Done();//���ӵ����������
    void Disconnect_Done();//�Ͽ��������
	void Write_Json_Done(); //д���
	void Write_Json_Error(); //д����
	void Write_String_Done();//д���
	void Write_String_Error();//д����
    void Read_Json_Done(QJsonObject buffer);//�����
    void Read_Json_Error();//������
	void Read_String_Done(QString buffer);//�����
	void Read_String_Error();//������
    void Status(int state);//ͨѶ״̬

private slots:
    void New_Connection();//�µ����ӱ�����
    void Read_Json();//��Json����
    void State_Changed();//״̬�ı�

private:
    QTcpServer *m_Server;//�����
    QTcpSocket *m_Socket;//�ͻ��ˡ��׽���
};

