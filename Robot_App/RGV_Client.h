/*************************************************************************************************************************************************
**Copyright(C), 2020-2025, bjtds.
**Version:2.0
**Author: dz
**Date:2021.12.27
**Description:
**Others:                          "CModbusTcpClient"
                              ____________________________
              Connect_Device->|REQ          |        DONE|->Done
           Disconnect_Device->|DISCONNECT   |       ERROR|->Error
                  Read/Write->|MB_MODE      |      STATUS|->status
                        addr->|MB_DATA_ADDR |            |
                        size->|MB_DATA_LEN  |            |
                      value ->|MB_DATA_PTR  |            |
                  ip,port,id->|CONNECT      |            |
                              ----------------------------
*************************************************************************************************************************************************/
#pragma once
#include "Variable.h"
#include "Public_Header.h"

class c_RGV_Client : public QObject
{
    Q_OBJECT
public:
    explicit c_RGV_Client(QObject *parent = nullptr);
	virtual ~c_RGV_Client();

signals:
    void Connect_Done();//连接到服务器
    void Disconnect_Done();//断开连接
    void Read_Coils_Done(QJsonObject value);//读线圈完成
    void Read_DiscreteInputs_Done(QJsonObject value);//读离散输入完成
    void Read_InputRegisters_Done(QJsonObject value);//读输入寄存器完成
    void Read_HoldingRegisters_Done(QJsonObject value);//读保持寄存器完成
    void Write_Coils_Done(); //写线圈完成
    void Write_HoldingRegisters_Done();//写保持寄存器完成
    void Read_Coils_Error();//读线圈错误
    void Read_DiscreteInputs_Error();//读离散输入错误
    void Read_InputRegisters_Error();//读输入寄存器错误
    void Read_HoldingRegisters_Error();//读保持寄存器错误
    void Write_Coils_Error(); //写线圈完成
    void Write_HoldingRegisters_Error();//写保持寄存器错误
    void Status(int state);//通讯状态

public slots:
    void Init();//子线程初始化
    void Connect_Device(QString ip, int port);//连接到服务器
    void Disconnect_Device();//断开连接
    void Read_Coils(int addr, int size);//读线圈
    void Read_DiscreteInputs(int addr, int size);//读离散输入
    void Read_InputRegisters(int addr, int size);//读输入寄存器
    void Read_HoldingRegisters(int addr, int size);//读保持寄存器
    void Write_Coils(int addr, QJsonObject value, int size); //写线圈
    void Write_HoldingRegisters(int addr, QJsonObject value, int size);//写保持寄存器

private slots:
    void State_Changed();//状态改变
    void Read_Ready_Coils();//准备读线圈
    void Read_Ready_DiscreteInputs();//准备散输入完成
    void Read_Ready_InputRegisters();//准备输入寄存器错误
    void Read_Ready_HoldingRegisters();//准备寄存器错误

private:
    QModbusTcpClient *m_ModbusDevice;
	QJsonObject m_Coils;//数据缓存区间
	QJsonObject m_DiscreteInputs;//数据缓存区间
	QJsonObject m_InputRegisters;//数据缓存区间
	QJsonObject m_HoldingRegisters;//数据缓存区间
};

