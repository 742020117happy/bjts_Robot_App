#pragma execution_character_set("utf-8")
#include "Meijidenki_Client.h"

c_Meijidenki_CallBack *c_Meijidenki_CallBack::g_Meijidenki_CallBack = new c_Meijidenki_CallBack;
/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_Meijidenki_CallBack::c_Meijidenki_CallBack(QObject * parent) : QObject(parent)
{
	EquipmentCommInit(NULL, EqCommDataCallBack, EqCommStateCallBack);
}
/*************************************************************************************************************************************************
**Function:析构指针
*************************************************************************************************************************************************/
c_Meijidenki_CallBack::~c_Meijidenki_CallBack()
{
	g_Meijidenki_CallBack = NULL;
	g_Meijidenki_CallBack->deleteLater();
}
/*************************************************************************************************************************************************
**Function:状态回调
*************************************************************************************************************************************************/
void CALLBACK c_Meijidenki_CallBack::EqCommStateCallBack(int _cid, unsigned int _state_code, char* _ip, int _port, int _paddr)
{
	emit g_Meijidenki_CallBack->State_Changed(_cid, _state_code);
}
/*************************************************************************************************************************************************
**Function:数据回调
*************************************************************************************************************************************************/
void CALLBACK c_Meijidenki_CallBack::EqCommDataCallBack(int _cid, unsigned int _lim_code, void* _lim, int _lim_len, int _paddr)
{
	if (_cid == g_Meijidenki_CallBack->m_Meijidenki_20_id && !g_Meijidenki_CallBack->m_Meijidenki_20_Read_Ready) {
		return;
	}
	if (_cid == g_Meijidenki_CallBack->m_Meijidenki_20_id && g_Meijidenki_CallBack->m_Meijidenki_20_Read_Ready) {
		g_Meijidenki_CallBack->m_Meijidenki_20_Read_Ready = false;
	}
	if (_cid == g_Meijidenki_CallBack->m_Meijidenki_21_id && !g_Meijidenki_CallBack->m_Meijidenki_21_Read_Ready) {
		return;
	}
	if (_cid == g_Meijidenki_CallBack->m_Meijidenki_21_id && g_Meijidenki_CallBack->m_Meijidenki_21_Read_Ready) {
		g_Meijidenki_CallBack->m_Meijidenki_21_Read_Ready = false;
	}
	g_Meijidenki_CallBack->m_Meijidenki_DB.cid = _cid;
	g_Meijidenki_CallBack->m_Meijidenki_DB.lim = (LIM_HEAD*)_lim;
	unsigned int checksum = LIM_CheckSum(g_Meijidenki_CallBack->m_Meijidenki_DB.lim);
	if (checksum != g_Meijidenki_CallBack->m_Meijidenki_DB.lim->CheckSum) {
		return;
	}
	QVariant varData_DB;
	varData_DB.setValue(g_Meijidenki_CallBack->m_Meijidenki_DB);
	emit g_Meijidenki_CallBack->ReadReady(varData_DB);
}
/*************************************************************************************************************************************************
**Function:读完成同步
*************************************************************************************************************************************************/
void c_Meijidenki_CallBack::Meijidenki_20_Read_Ready(qint32 id)
{
	m_Meijidenki_20_Read_Ready = true;
	m_Meijidenki_20_id = id;
}
void c_Meijidenki_CallBack::Meijidenki_21_Read_Ready(qint32 id)
{
	m_Meijidenki_21_Read_Ready = true;
	m_Meijidenki_21_id = id;
}
/*************************************************************************************************************************************************
**Function:    构造函数
**Description: 初始化成员变量
**Input:       无输入
**Output:      无输出
**Return:      无返回
**Others:      构造函数初始化在主线程实例化中完成，故在子线程中运行时，成员函数的初始化不能在构造函数中完成
*************************************************************************************************************************************************/
c_Meijidenki_Client::c_Meijidenki_Client(QObject *parent) : QObject(parent)
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
c_Meijidenki_Client::~c_Meijidenki_Client()
{
	m_Stop_Connect = true;
	CloseEquipmentComm(m_device_id); // 关闭当前设备
	EquipmentCommDestory(); //关闭通讯库
}
/*************************************************************************************************************************************************
**Function:    初始化函数
**Description: 线程的构造函数
**Input:       无输入
**Output:      无输出
**Return:      无返回
**Others:
*************************************************************************************************************************************************/
void c_Meijidenki_Client::Init()
{
	//如果，客户端，状态改变，执行，本线程，状态改变函数
	QObject::connect(c_Meijidenki_CallBack::g_Meijidenki_CallBack, &c_Meijidenki_CallBack::State_Changed, this, &c_Meijidenki_Client::State_Changed);
	//开启监听模式{机器人，有可读取通道，对象，读取信号}
	QObject::connect(c_Meijidenki_CallBack::g_Meijidenki_CallBack, &c_Meijidenki_CallBack::ReadReady, this, &c_Meijidenki_Client::Read_Json);
}
/*************************************************************************************************************************************************
**Function:    Connect_Device(QString ip, int port)
**Description: 连接tcpsocket服务器
**Input:       -> ip
			   -> port
**Output:      ->connectDone    连接成功
			   ->disconnectDone 断开成功
**Return:      无返回
**Others:      分二个步骤
*************************************************************************************************************************************************/
void c_Meijidenki_Client::Connect_Device(int id, QString ip, int port)
{
	//如果处于连接状态立即返回
	if (m_State) { return; }
	m_State = true;
	//如果发出断开请求，立即返回停止循环连接，复位请求标志
	if (m_Stop_Connect) {
		m_Stop_Connect = false;
		return;
	}
	//通讯库初始化
	m_device_id = id;
	m_ip = ip;
	m_port = port;
	OpenEquipmentComm(id, ip.toLatin1().data(), port);
	emit Status("正在尝试连接中");
}
/*************************************************************************************************************************************************
**Function:    Disconnect_Device()
**Description: 断开tcpsocket服务器
**Input:       无输入
**Output:      status
**Return:      无返回
**Others:      如果未连接，则会直接返回
*************************************************************************************************************************************************/
void c_Meijidenki_Client::Disconnect_Device()
{
	m_Stop_Connect = true;
	if (!m_State) { return; }//如果处于未连接状态立即返回
	m_State = false;
	if (!CloseEquipmentComm(m_device_id)) {
		m_State = true;
		emit Disconnect_Error();
		emit Status("断开设备连接:..........失败!");
	}
	else {
		emit Status("断开设备连接:..........成功!");
	}
}
/*************************************************************************************************************************************************
**Function:    Read_Json()
**Description: 读消息
**Input:
**Output:      status、readError、readDone
**Return:      无返回
**Others:
*************************************************************************************************************************************************/
void c_Meijidenki_Client::Read_Json(QVariant db)
{
	s_Meijidenki_DB Meijidenki_DB = db.value<s_Meijidenki_DB>();
	if (Meijidenki_DB.cid != m_device_id ) {
		return;
	}
	quint32 type = Meijidenki_DB.lim->nCode;
	switch (type)
	{
	case LIM_CODE_LDBCONFIG:
		m_Value.insert("LIM_CODE_LDBCONFIG", Meijidenki_LDBCONFIG_Decoding(Meijidenki_DB.lim));
		break;
	case LIM_CODE_LMD:
		m_Value.insert("LIM_CODE_LMD", Meijidenki_LMD_Decoding(Meijidenki_DB.lim));
		break;
	case LIM_CODE_LMD_RSSI:
		m_Value.insert("LIM_CODE_LMD_RSSI", Meijidenki_LMD_RSSI_Decoding(Meijidenki_DB.lim));
		break;
	case LIM_CODE_FMSIG:
		m_Value.insert("LIM_CODE_FMSIG", Meijidenki_FMSIG_Decoding(Meijidenki_DB.lim));
		break;
	case LIM_CODE_IOSTATUS:
		m_Value.insert("LIM_CODE_IOSTATUS", Meijidenki_IOSTATUS_Decoding(Meijidenki_DB.lim));
		break;
	case LIM_CODE_ALARM:
		emit Status(Meijidenki_ALARM_Decoding(Meijidenki_DB.lim));
		break;
	case LIM_CODE_DISALARM:
		emit Status(Meijidenki_DISALARM_Decoding(Meijidenki_DB.lim));
		break;
	default:
		emit Read_Json_Error();
		break;
	}
	emit Read_Json_Done(m_Value);
}
/*************************************************************************************************************************************************
**Function:    Write(quint32 code)
**Description: 写消息
**Input:       quint32 code
**Output:      status、writeError、writeDone
**Return:      无返回
**Others:      无
*************************************************************************************************************************************************/
void c_Meijidenki_Client::Write(quint32 code)
{
	if (!m_State) { return; }//如果处于未连接状态立即返回
	LIM_HEAD* lim = NULL;
	//编辑报文
	LIM_Pack(lim, m_device_id, code, NULL);
	//发送报文
	if (SendLIM(m_device_id, lim, lim->nLIMLen)) {
		emit Write_Done();
	}
	else {
		emit Write_Error();
	}
	LIM_Release(lim);
}
/*************************************************************************************************************************************************
**Function:    State_Changed()
**Description: 连接状态改变
**Input:       无输入
**Output:      connectDone、disconnectDone、status
**Return:      无返回
**Others:
*************************************************************************************************************************************************/
void c_Meijidenki_Client::State_Changed(qint32 _cid, quint32 _state_code)
{
	if (_cid == m_device_id && _state_code == EQCOMM_STATE_OK) {
		Write(LIM_CODE_GET_LDBCONFIG);
		m_State = true;
		emit Connect_Done();
		emit Status("连接设备.................成功");
	}
	if (_cid == m_device_id && _state_code == EQCOMM_STATE_ERR)
	{
		m_State = false;
		CloseEquipmentComm(m_device_id);
		emit Connect_Error();
		emit Status("连接设备.................失败");
		//如果发出断开请求，立即返回停止循环连接，复位请求标志
		if (m_Stop_Connect) {
			m_Stop_Connect = false;
			return;
		}
		emit Connect_Loop();
	}
	if (_cid == m_device_id && _state_code == EQCOMM_STATE_LOST) {
		m_State = false;
		emit  Disconnect_Done();	
	}
}
/*************************************************************************************************************************************************
**Function:   Decoding(LIM_HEAD *lim)
**Description: 解析消息
**Input:       LIM_HEAD *lim
**Output:      QJsonObject
**Return:      无返回
**Others:      无
*************************************************************************************************************************************************/
// LIM_CODE_LDBCONFIG LIM包解码
QJsonObject c_Meijidenki_Client::Meijidenki_LDBCONFIG_Decoding(LIM_HEAD *lim)
{
	QJsonObject object;
	if (lim->nCode != LIM_CODE_LDBCONFIG) {
		return object;
	}
	ULDINI_Type *ldtype = (ULDINI_Type*)LIM_ExData(lim);
	object.insert("Type", QString(ldtype->szType));
	object.insert("Manufacturer", QString(ldtype->szManufacturer));
	object.insert("ReleaseDate", QString(ldtype->szReleaseDate));
	object.insert("SerialNo", QString(ldtype->szSerialNo));
	object.insert("szMAC", QString(ldtype->szMAC));
	object.insert("szIP", QString(ldtype->szIP));
	object.insert("szMask", QString(ldtype->szMask));
	object.insert("szGate", QString(ldtype->szGate));
	object.insert("szDNS", QString(ldtype->szDNS));
	object.insert("MR", ldtype->nMR);
	object.insert("ESAR", ldtype->nESAR);
	object.insert("ESA0", ldtype->nESA[0] / 1000);
	object.insert("ESA1", ldtype->nESA[1] / 1000);
	object.insert("SAR", ldtype->nSAR);
	object.insert("SA0", ldtype->nSA[0]);
	object.insert("SA1", ldtype->nSA[1]);
	object.insert("SAV", ldtype->nSAV);
	object.insert("SAP", ldtype->nSAP / 1000.);
	object.insert("PF", ldtype->nPF / 100);
	return object;
}
// LIM_CODE_LMD LIM包解码
QJsonObject c_Meijidenki_Client::Meijidenki_LMD_Decoding(LIM_HEAD*lim)
{
	QJsonObject object;
	if (lim->nCode != LIM_CODE_LMD){
		return object;
	}	
	LMD_INFO* lmd_info = LMD_Info(lim);  // 获取LMD_INFO指针
	LMD_D_Type* lmd = LMD_D(lim);        // 获取LMD数据指针										
	object.insert("nRange", lmd_info->nRange / 100.);
	object.insert("nBAngle", lmd_info->nBAngle / 1000.);
	object.insert("nEAngle", lmd_info->nEAngle / 1000.);
	object.insert("nAnglePrecision", lmd_info->nAnglePrecision / 1000.);
	object.insert("nRPM", lmd_info->nRPM / 1.);
	object.insert("nMDataNum", lmd_info->nMDataNum / 1.);
	QJsonArray distance;
	for (int i = 0; i < 540; i++)
	{
		quint16 date = lmd[i];
		distance.append(date);
	}
	object.insert("LMD_D", distance);
	return object;
}
// LIM_CODE_LMD_RSSI LIM包解码
QJsonObject c_Meijidenki_Client::Meijidenki_LMD_RSSI_Decoding(LIM_HEAD*lim)
{
	QJsonObject object;
	if (lim->nCode != LIM_CODE_LMD_RSSI) {
		return object;
	}
	LMD_INFO* lmd_info = LMD_Info(lim);  //获取LMD_INFO指针
	LMD_D_Type* lmd = LMD_D(lim);        //获取LMD数据指针
	LMD_D_RSSI_Type *lmdrssi = LMD_D_RSSI(lim);//获取RSSI数据指针
	//打印LMD_INFO
	object.insert("nRange", lmd_info->nRange / 100.);
	object.insert("nBAngle", lmd_info->nBAngle / 1000.);
	object.insert("nEAngle", lmd_info->nEAngle / 1000.);
	object.insert("nAnglePrecision", lmd_info->nAnglePrecision / 1000.);
	object.insert("nRPM", lmd_info->nRPM / 1.);
	object.insert("nMDataNum", lmd_info->nMDataNum / 1.);
	return object;
}
// LIM_CODE_FMSIG LIM包解码
QJsonObject c_Meijidenki_Client::Meijidenki_FMSIG_Decoding(LIM_HEAD*lim)
{
	QJsonObject object;
	if (lim->nCode != LIM_CODE_FMSIG) {
		return object;
	}
	int fieldIdx;
	int alm;
	int attentionW, attentionA;
	int alertW, alertA;
	int alarmW, alarmA;

	fieldIdx = lim->Data[0];
	alm = lim->Data[1];

	alarmA = (alm & 0x01);
	alarmW = (alm & 0x02) >> 1;
	alertA = (alm & 0x04) >> 2;
	alertW = (alm & 0x08) >> 3;
	attentionA = (alm & 0x10) >> 4;
	attentionW = (alm & 0x20) >> 5;

	object.insert("fieldIdx", fieldIdx);
	object.insert("alarmA", alarmA);
	object.insert("alertA", alertA);
	object.insert("attentionA", attentionA);
	object.insert("alarmW", alarmW);
	object.insert("alertW", alertW);
	object.insert("attentionW", attentionW);
	return object;
}
// LIM_CODE_IOSTATUS LIM包解码
QJsonObject c_Meijidenki_Client::Meijidenki_IOSTATUS_Decoding(LIM_HEAD*lim)
{
	QJsonObject object;
	if (lim->nCode != LIM_CODE_IOSTATUS) {
		return object;
	}
	int iosts = lim->Data[0];

	QJsonArray IO_OutSts;
	QJsonArray IO_InSts;

	for (int i = 0; i < IO_OUTNUM; i++)
	{
		int arry = ((iosts & (1 << i)) >> i);
		IO_OutSts.append(arry);
	}

	for (int i = IO_OUTNUM; i < IO_OUTNUM + IO_INNUM; i++)
	{
		int arry = ((iosts & (1 << i)) >> i);
		IO_InSts.append(arry);
	}

	object.insert("IO_OutSts", IO_OutSts);
	object.insert("IO_InSts", IO_InSts);

	return object;
}
// LIM_CODE_ALARM LIM包解码
QString c_Meijidenki_Client::Meijidenki_ALARM_Decoding(LIM_HEAD*lim)
{
	QString str;
	if (lim->nCode != LIM_CODE_ALARM) {
		return str;
	}
	switch (lim->Data[0]) {
	case LIM_DATA_ALARMCODE_INTERNAL:
		str = "内部错误";
		break;
	case LIM_DATA_ALARMCODE_Occluded:
		str = "设备出光面被遮挡或者透过罩太脏";
		break;
	case LIM_DATA_ALARMCODE_High_Temperature:
		str = "高温报警";
		break;
	case LIM_DATA_ALARMCODE_Low_Temperature:
		str = "低温报警";
		break;
	default:
		str = "未知错误";
		break;
	}
	return str;
}
// LIM_CODE_DISALARM LIM包解码
QString c_Meijidenki_Client::Meijidenki_DISALARM_Decoding(LIM_HEAD*lim)
{
	QString str;
	if (lim->nCode != LIM_CODE_DISALARM) {
		return str;
	}
	switch (lim->Data[0]) {
	case LIM_DATA_ALARMCODE_INTERNAL:
		str = "内部错误解除";
		break;
	case LIM_DATA_ALARMCODE_Occluded:
		str = "设备出光面被遮挡或者透过罩太脏解除";
		break;
	case LIM_DATA_ALARMCODE_High_Temperature:
		str = "高温报警解除";
		break;
	case LIM_DATA_ALARMCODE_Low_Temperature:
		str = "低温报警解除";
		break;
	default:
		str = "未知错误解除";
		break;
	}
	return str;
}



