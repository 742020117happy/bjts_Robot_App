#pragma execution_character_set("utf-8")
#include "RGV_Client.h"
/*************************************************************************************************************************************************
**Function:    构造函数
**Description: 初始化成员变量
**Input:       无输入
**Output:      无输出
**Return:      无返回
**Others:      构造函数初始化在主线程实例化中完成，故在子线程中运行时，成员函数的初始化不能在构造函数中完成
*************************************************************************************************************************************************/
c_RGV_Client::c_RGV_Client(QObject *parent) : QObject(parent)
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
c_RGV_Client::~c_RGV_Client()
{
	m_ModbusDevice->deleteLater();
}
/*************************************************************************************************************************************************
**Function:    初始化函数
**Description: 线程的构造函数
**Input:       无输入
**Output:      无输出
**Return:      无返回
**Others:
*************************************************************************************************************************************************/
void c_RGV_Client::Init()
{
    //对象实列化
    m_ModbusDevice = new QModbusTcpClient;
    //状态改变
    QObject::connect(m_ModbusDevice, &QModbusTcpClient::stateChanged, this, &c_RGV_Client::State_Changed);
    //错误诊断
    QObject::connect(m_ModbusDevice, &QModbusTcpClient::errorOccurred, this, &c_RGV_Client::Status);
}
/*************************************************************************************************************************************************
**Function:    Connect_Device()
**Description: 连接modbustcp服务器
**Input:       -> ip
               -> port
**Output:      onStateChanged -> connectDone    连接成功
                              -> disconnectDone 断开成功
               status -> 0 已连接
                      -> 1 读取操作期间发生错误。
                      -> 2 写入操作期间发生错误。
                      -> 3 尝试打开后端时出错。
                      -> 4 尝试设置配置参数时出错。
                      -> 5 I/O期间发生超时。I/O操作未在给定的时间范围内完成。
                      -> 6 发生Modbus特定协议错误。
                      -> 7 由于设备断开连接，回复被中止。
                      -> 8 发生未知错误
                      -> 9 设备已断开连接。
                      ->10 正在连接设备。
                      ->11 设备正在关闭
**Return:      无返回
**Others:      二次调用时，如果已连接，则会先断开原来的连接
*************************************************************************************************************************************************/
void c_RGV_Client::Connect_Device(QString ip, int port)
{
    //如果已连接，则返回
    if (m_ModbusDevice->state() == QModbusDevice::ConnectedState)
    {
        return;
    }
    //配置modbus tcp的连接参数 IP + Port
    m_ModbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip);
    m_ModbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    //再设置从机无响应时的动作
    m_ModbusDevice->setTimeout(1000);//从设备回复信息的超时时间
    m_ModbusDevice->setNumberOfRetries(2);//重复发送次数
    m_ModbusDevice->connectDevice();
}
/*************************************************************************************************************************************************
**Function:    Disconnect_Device()
**Description: 断开modbustcp服务器
**Input:       无输入
**Output:      onStateChanged、status
**Return:      无返回
**Others:      如果未连接，则会直接返回
*************************************************************************************************************************************************/
void c_RGV_Client::Disconnect_Device()
{
    if ((!m_ModbusDevice) || (m_ModbusDevice->state() != QModbusDevice::ConnectedState))
    {
        return;//如果RGV没有连接则提前退出
    }
    m_ModbusDevice->disconnectDevice();
}
/*************************************************************************************************************************************************
**Function:    readCoils(ModbusTcpDataBlock dataBlock)
**Description: 读线圈寄存器（读写，一般为继电器的控制）
**Input:       QJsonObject dataBlock -> readCoilsAddr
                                     -> readCoilsSize
                                     -> id
**Output:      status、readCoilsError、readReadyCoils
**Return:      无返回
**Others:      采用的是基于ModbusReply的Finished信号，做异步处理
*************************************************************************************************************************************************/
void c_RGV_Client::Read_Coils(int addr, int size)
{
    if ((!m_ModbusDevice) || (m_ModbusDevice->state() != QModbusDevice::ConnectedState))
    {
        return;//如果RGV没有连接则提前退出
    }
    if(size == 0)
    {
        emit Read_Coils_Error();
        return;
    }
    QModbusDataUnit ReadUnit(QModbusDataUnit::Coils, addr, size);
    auto *reply = m_ModbusDevice->sendReadRequest(ReadUnit, 1);
    if(reply)
    {
        if (!reply->isFinished())
        {
          QObject::connect(reply, &QModbusReply::finished, this, &c_RGV_Client::Read_Ready_Coils);
        }
        else
        {
            reply->deleteLater();
            emit Read_Coils_Error();
            return;
        }
    }
    else
    {
        reply->deleteLater();
        emit Read_Coils_Error();;
        return;
    }
}
/*************************************************************************************************************************************************
**Function:    readDiscreteInputs(QJsonObject dataBlock)
**Description: 读离散输入寄存器（只读，通常为开关量输入）
**Input:       QJsonObject dataBlock -> readDiscreteInputsAddr
                                     -> readDiscreteInputsSize
                                     -> id
**Output:      status、readDiscreteInputsError、readReadyDiscreteInputs
**Return:      无返回
**Others:      采用的是基于ModbusReply的Finished信号，做异步处理
*************************************************************************************************************************************************/
void c_RGV_Client::Read_DiscreteInputs(int addr, int size)
{
    if ((!m_ModbusDevice) || (m_ModbusDevice->state() != QModbusDevice::ConnectedState))
    {
        return;//如果RGV没有连接则提前退出
    }
    if(size == 0)
    {
        emit Read_DiscreteInputs_Error();
        return;
    }
    QModbusDataUnit ReadUnit(QModbusDataUnit::DiscreteInputs, addr, size);
    auto *reply = m_ModbusDevice->sendReadRequest(ReadUnit, 1);
    if(reply)
    {
        if (!reply->isFinished())
        {
          QObject::connect(reply, &QModbusReply::finished, this, &c_RGV_Client::Read_Ready_DiscreteInputs);
        }
        else
        {
            reply->deleteLater();
            emit Read_DiscreteInputs_Error();
            return;
        }
    }
    else
    {
        reply->deleteLater();
        emit Read_DiscreteInputs_Error();
        return;
    }
}
/*************************************************************************************************************************************************
**Function:    readInputRegisters(ModbusTcpDataBlock dataBlock)
**Description: 读输入寄存器（只读，一般为模拟量输入）
**Input:       QJsonObject dataBlock -> readInputRegistersAddr
                                     -> readInputRegistersSize
                                     -> id
**Output:      status、readInputRegistersError、readReadyInputRegisters
**Return:      无返回
**Others:      采用的是基于ModbusReply的Finished信号，做异步处理
*************************************************************************************************************************************************/
void c_RGV_Client::Read_InputRegisters(int addr, int size)
{
    if ((!m_ModbusDevice) || (m_ModbusDevice->state() != QModbusDevice::ConnectedState))
    {
        return;//如果RGV没有连接则提前退出
    }
    if(size == 0)
    {
        emit Read_InputRegisters_Error();
        return;
    }
    QModbusDataUnit ReadUnit(QModbusDataUnit::InputRegisters, addr, size);
    auto *reply = m_ModbusDevice->sendReadRequest(ReadUnit, 1);
    if(reply)
    {
        if (!reply->isFinished())
        {
          QObject::connect(reply, &QModbusReply::finished, this, &c_RGV_Client::Read_Ready_InputRegisters);
        }
        else
        {
            reply->deleteLater();
            emit Read_InputRegisters_Error();
            return;
        }
    }
    else
    {
        reply->deleteLater();
        emit Read_InputRegisters_Error();
        return;
    }
}
/*************************************************************************************************************************************************
**Function:    readHoldingRegisters(ModbusTcpDataBlock dataBlock)
**Description: 读保持寄存器（读写，一般状态参数控制）
**Input:       QJsonObject dataBlock -> readHoldingRegistersAddr
                                     -> readHoldingRegistersSize
                                     -> id
**Output:      status、readHoldingRegistersError、readReadyHoldingRegisters
**Return:      无返回
**Others:      采用的是基于ModbusReply的Finished信号，做异步处理
*************************************************************************************************************************************************/
void c_RGV_Client::Read_HoldingRegisters(int addr, int size)
{
    if ((!m_ModbusDevice) || (m_ModbusDevice->state() != QModbusDevice::ConnectedState))
    {
        return;//如果RGV没有连接则提前退出
    }
    if(size == 0)
    {
        emit Read_HoldingRegisters_Error();
        return;
    }
    QModbusDataUnit ReadUnit(QModbusDataUnit::HoldingRegisters, addr, size);
    auto *reply = m_ModbusDevice->sendReadRequest(ReadUnit, 1);
    if(reply)
    {
        if (!reply->isFinished())
        {
          QObject::connect(reply, &QModbusReply::finished, this, &c_RGV_Client::Read_Ready_HoldingRegisters);
        }
        else
        {
            reply->deleteLater();
            emit Read_HoldingRegisters_Error();
            return;
        }
    }
    else
    {
        reply->deleteLater();
        emit Read_HoldingRegisters_Error();
        return;
    }
}
/*************************************************************************************************************************************************
**Function:    writeCoils(int addr, QJsonObject value, quint16 size)
**Description: 写线圈寄存器（读写，一般为继电器的控制）
**Input:       ->addr 地址
               ->value 值
               ->size 个数
**Output:      status、writeCoilsError、writeCoilsDone
**Return:      无返回
**Others:      采用的是基于ModbusReply的Finished信号，做异步处理
*************************************************************************************************************************************************/
void c_RGV_Client::Write_Coils(int addr, QJsonObject value, int size)
{
    if ((!m_ModbusDevice) || (m_ModbusDevice->state() != QModbusDevice::ConnectedState))
    {
        return;//如果RGV没有连接则提前退出
    }
    if(size <= 0)
    {
        emit Write_HoldingRegisters_Error();
        return;
    }
    //写，地址，写多少位
    QModbusDataUnit writeUnit(QModbusDataUnit::Coils, addr, size);
    //该位置，数据
    for(int i=0; i<size; i++)
    {
        QString address = QString::number(addr + i);//当前数据地址
        quint16 data = quint16(value.value(address).toInt());
        writeUnit.setValue(i, data);
    }
    //发送校验 1 代表设备地址
    if(auto *reply = m_ModbusDevice->sendWriteRequest(writeUnit, 1))
    {
        if (!reply->isFinished())
        {
            //如果接收到响应信息
            QObject::connect(reply, &QModbusReply::finished, this, [&, reply]()
            {
                if (reply->error() == QModbusDevice::NoError)
                {
                    //接收到的响应信无错误，发送完成信号
                    emit Write_Coils_Done();
                }
                else
                {
                    reply->deleteLater();
                    emit Write_Coils_Error();
                    return;
                }
                reply->deleteLater();
            });
        }
        else
        {
            reply->deleteLater();
            emit Write_Coils_Error();
            return;
        }
    }
}
/*************************************************************************************************************************************************
**Function:    writeHoldingRegisters(int addr, QJsonObject value, quint16 size)
**Description: 写保持寄存器（读写，一般状态参数控制）
**Input:       ->addr 地址
               ->value 值
               ->size 个数
**Output:      status、writeHoldingRegistersError、writeHoldingRegistersDone
**Return:      无返回
**Others:      采用的是基于ModbusReply的Finished信号，做异步处理
*************************************************************************************************************************************************/
void c_RGV_Client::Write_HoldingRegisters(int addr, QJsonObject value, int size)
{
    if ((!m_ModbusDevice) || (m_ModbusDevice->state() != QModbusDevice::ConnectedState))
    {
        return;//如果RGV没有连接则提前退出
    }
    if(size <= 0)
    {
        emit Write_HoldingRegisters_Error();
        return;
    }

    //写，地址，写多少位
    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, addr, size);
    //该位置，数据
    for(int i=0; i<size; i++)
    {
        QString address = QString::number(addr + i);//当前数据地址
        quint16 data = quint16(value.value(address).toInt());
        writeUnit.setValue(i, data);
    }
    //发送校验 1 代表设备地址
    if(auto *reply = m_ModbusDevice->sendWriteRequest(writeUnit, 1))
    {
        if (!reply->isFinished())
        {
            //如果接收到响应信息
            QObject::connect(reply, &QModbusReply::finished, this, [&, reply]()
            {
                if (reply->error() == QModbusDevice::NoError)
                {
                    //接收到的响应信无错误，发送完成信号
                    emit Write_HoldingRegisters_Done();
                }
                else
                {
                    reply->deleteLater();
                    emit Write_HoldingRegisters_Error();
                    return;
                }
                reply->deleteLater();
            });
        }
        else
        {
            reply->deleteLater();
            emit Write_HoldingRegisters_Error();
            return;
        }
    }
}
/*************************************************************************************************************************************************
**Function:    onStateChanged()
**Description: modbustcp连接状态改变
**Input:       无输入
**Output:      connectDone、disconnectDone、status
**Return:      无返回
**Others:      已连接、连接中、已断开、断开中
*************************************************************************************************************************************************/
void c_RGV_Client::State_Changed()
{
    if(m_ModbusDevice->state() == QModbusDevice::ConnectedState)
    {
        emit Status(0);
        emit Connect_Done();
    }
    if(m_ModbusDevice->state() == QModbusDevice::UnconnectedState)
    {
        emit Status(9);
        emit Disconnect_Done();
    }
    if(m_ModbusDevice->state() == QModbusDevice::ConnectingState)
    {
        emit Status(10);
    }
    if(m_ModbusDevice->state() == QModbusDevice::ClosingState)
    {
        emit Status(11);
    }
}
/*************************************************************************************************************************************************
**Function:    readReadyCoils()
**Description: 读线圈寄存器（读写，一般为继电器的控制）
**Input:       readCoils
**Output:      QJsonObject value -> {"地址"：值}
               status、readCoilsError、readCoilsDone
**Return:      无返回
**Others:      采用的是基于ModbusReply的Finished信号，做异步处理
*************************************************************************************************************************************************/
void c_RGV_Client::Read_Ready_Coils()
{
    if ((!m_ModbusDevice) || (m_ModbusDevice->state() != QModbusDevice::ConnectedState))
    {
        return;//如果RGV没有连接则提前退出
    }
    auto *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        reply->deleteLater();
        emit Read_Coils_Error();
        return;
    }
    //如果校验无误
    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit writeUnit = reply->result();//获取结果
        int size = int(writeUnit.valueCount());//数据长度
        int addr = writeUnit.startAddress();//数据起始地址
        for(int i=0; i<size; i++)
        {
            QString add = QString::number(addr + i);//当前数据地址
            int data = int(writeUnit.value(i));//当前数据值
            m_Coils.insert(add, data);//记录数据
        }
    }
    //如果校验有误
    else
    {
        reply->deleteLater();
        emit Read_Coils_Error();
        return;
    }
    reply->deleteLater(); //删除答复
    //发送信号
    emit Read_Coils_Done(m_Coils);//输出数据
}
/*************************************************************************************************************************************************
**Function:    readReadyDiscreteInputs()
**Description: 读离散输入寄存器（只读，通常为开关量输入）
**Input:       readDiscreteInputs
**Output:      QJsonObject value -> {"地址"：值}
               status、readDiscreteInputsError、readDiscreteInputsDone
**Return:      无返回
**Others:      采用的是基于ModbusReply的Finished信号，做异步处理
*************************************************************************************************************************************************/
void c_RGV_Client::Read_Ready_DiscreteInputs()
{
    if ((!m_ModbusDevice) || (m_ModbusDevice->state() != QModbusDevice::ConnectedState))
    {
        return;//如果RGV没有连接则提前退出
    }
    auto *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        reply->deleteLater();
        emit Read_DiscreteInputs_Error();
        return;
    }
    //如果校验无误
    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit readUnit = reply->result();//获取结果
        int size = readUnit.valueCount();//数据长度
        int addr = readUnit.startAddress();//数据起始地址
        for(int i=0; i<size; i++)
        {
            QString add = QString::number(addr + i);//当前数据地址
            int data = int(readUnit.value(i));//当前数据值
            m_DiscreteInputs.insert(add, data);//记录数据
        }
    }
    //如果校验有误
    else
    {
        reply->deleteLater();
        emit Read_DiscreteInputs_Error();
        return;
    }
    reply->deleteLater(); //删除答复
    //发送信号
    emit Read_DiscreteInputs_Done(m_DiscreteInputs);//输出数据
}
/*************************************************************************************************************************************************
**Function:    readReadyInputRegisters()
**Description: 读输入寄存器（只读，一般为模拟量输入）
**Input:       readInputRegisters
**Output:      QJsonObject value -> {"地址"：值}
               status、readInputRegistersError、readInputRegistersDone
**Return:      无返回
**Others:      采用的是基于ModbusReply的Finished信号，做异步处理
*************************************************************************************************************************************************/
void c_RGV_Client::Read_Ready_InputRegisters()
{
    if ((!m_ModbusDevice) || (m_ModbusDevice->state() != QModbusDevice::ConnectedState))
    {
        return;//如果RGV没有连接则提前退出
    }
    auto *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        reply->deleteLater();
        emit Read_InputRegisters_Error();
        return;
    }
    //如果校验无误
    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit readUnit = reply->result();//获取结果
        int size = readUnit.valueCount();//数据长度
        int addr = readUnit.startAddress();//数据起始地址
        for(int i=0; i<size; i++)
        {
            QString add = QString::number(addr + i);//当前数据地址
            int data = int(readUnit.value(i));//当前数据值
            m_InputRegisters.insert(add, data);//记录数据
        }
    }
    //如果校验有误
    else
    {
        reply->deleteLater();
        emit Read_InputRegisters_Error();
        return;
    }
    reply->deleteLater(); //删除答复
    //发送信号
    emit Read_InputRegisters_Done(m_InputRegisters);//输出数据
}
/*************************************************************************************************************************************************
**Function:    readReadyHoldingRegisters()
**Description: 读保持寄存器（读写，一般状态参数控制）
**Input:       readHoldingRegisters
**Output:      QJsonObject value -> {"地址"：值}
               status、readHoldingRegistersError、readHoldingRegistersDone
**Return:      无返回
**Others:      采用的是基于ModbusReply的Finished信号，做异步处理
*************************************************************************************************************************************************/
void c_RGV_Client::Read_Ready_HoldingRegisters()
{
    if ((!m_ModbusDevice) || (m_ModbusDevice->state() != QModbusDevice::ConnectedState))
    {
        return;//如果RGV没有连接则提前退出
    }
    auto *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        reply->deleteLater();
        emit Read_HoldingRegisters_Error();
        return;
    }
    //如果校验无误
    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit readUnit = reply->result();//获取结果
        int size = readUnit.valueCount();//数据长度
        int addr = readUnit.startAddress();//数据起始地址
        for(int i=0; i<size; i++)
        {
            QString add = QString::number(addr + i);//当前数据地址
            int data = int(readUnit.value(i));//当前数据值
            m_HoldingRegisters.insert(add, data);//记录数据
        }
    }
    //如果校验有误
    else
    {
        reply->deleteLater();
        emit Read_HoldingRegisters_Error();
        return;
    }
    reply->deleteLater(); //删除答复
    //发送信号
    emit Read_HoldingRegisters_Done(m_HoldingRegisters);//输出数据
}
