#include "serialportwriteread.h"

SerialPortWriteRead::SerialPortWriteRead() : m_serial_port(new SerialPort), m_encrypt(new Encrypt)
{

}

SerialPortWriteRead::~SerialPortWriteRead()
{
    if(this->m_serial_port != NULL)
    {
        delete this->m_serial_port;
        this->m_serial_port = NULL;
    }
    if(this->m_encrypt != NULL)
    {
        delete this->m_encrypt;
        this->m_encrypt = NULL;
    }
}

//进入配置函数
int SerialPortWriteRead::EnterAppConfig(QString com_name)
{
    QByteArray to_data = com_name.toLatin1();
    char* cp_com = to_data.data();
    bool ok = this->m_serial_port->initSerilPort(cp_com, 230400, 'N', 8, 1);
    if(ok == false)
    {
        qDebug() << "Init protoserialport Fail" << com_name;
        return -1;
    }

    QByteArray pre_array;
    quint8 pre_write = 0xFF;
    quint8 cnt = 15;
    while(cnt--)
    {
        pre_array.append(pre_write);
    }
    SerialPort::Data data;
    data.buffer = pre_array;
    data.length = pre_array.length();
    int ret = this->m_serial_port->writeData(data);
    if(ret == -1)
    {
       qDebug() << QString::fromLocal8Bit("串口号")+ com_name + QString::fromLocal8Bit("写入预配置失败");
       return -1;
    }

    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_MODULE_CUSTOMISE);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    data.buffer = send_cus_pkg.data();
    data.length =send_cus_pkg.length();
    ret = this->m_serial_port->writeData(data);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号")+ com_name + QString::fromLocal8Bit("写入配置信息失败");
    }
    char config_buf[6];
    ret = this->m_serial_port->readData(config_buf, 6);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号")+ com_name + QString::fromLocal8Bit("读配置信息失败");
        return -1;
    }
    QByteArray byteArray = QByteArray(config_buf);
    protoserialport::REP_TYPE type = static_cast<protoserialport::REP_TYPE>(byteArray.at(2));
    protoserialport::ResPkg<protoserialport::ResResult> res(byteArray.data(), byteArray.length());
    switch (type)
    {
        case protoserialport::REP_ERR:
        {
            if(protoserialport::ERR_CRC == res.getData().getRes())
            {
                QString bad_info = QString::fromLocal8Bit("CRC出错");
                qDebug() << bad_info;
            }
            else if (protoserialport::ERR_FRM == res.getData().getRes())
            {
                QString bad_info = QString::fromLocal8Bit("帧类型出错");
                qDebug() << bad_info;
            }
            else
            {
                QString bad_info = QString::fromLocal8Bit("不知类型错误");
                qDebug() << bad_info;
            }
        }break;

        case protoserialport::REP_WR_RES:
        {
            if (protoserialport::RES_SUCCESS == res.getData().getRes())
            {
                QString suc_str =  QString::fromLocal8Bit("配置成功");
                qDebug() << suc_str;
            }
            else if (protoserialport::RES_FAILURE == res.getData().getRes())
            {
                QString bad_info = QString::fromLocal8Bit("配置失败");
                qDebug() << bad_info;
            }
            else
            {
                QString bad_info = QString::fromLocal8Bit("不知类型错误");
                qDebug() << bad_info;
            }
        } break;
    }
    return 0;
}

//退出配置函数
int SerialPortWriteRead::ExitAppConfig()
{
    SerialPort::Data data;
    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_MODULE_APPLICATE);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    data.buffer = send_cus_pkg.data();
    data.length =send_cus_pkg.length();
    int ret = this->m_serial_port->writeData(data);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入退出配置模式指令失败");
        return -1;
    }
    qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入退出配置模式指令成功");

    char end_config_buf[6];
    ret = this->m_serial_port->readData(end_config_buf, 6);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读退出配置失败");
        return -1;
    }
    qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读退出配置成功");
    return 0;
}

//录入标签函数
int SerialPortWriteRead::SetIspdTagId(QString record_id)
{
    SerialPort::Data data;
    std::string current_sign_id = record_id.toStdString();
    qint32 ispd_id_dencrypt = this->m_encrypt->dencrypt_ispd_id(current_sign_id);
    protoserialport::IspdID ispd_id(ispd_id_dencrypt, protoserialport::REQ_TYPE::ISPD_WR_ID);
    protoserialport::ReqMsg<protoserialport::IspdID> res_msg(ispd_id);
    QByteArray send_id = res_msg.toBinary();
    data.buffer = send_id.data();
    data.length =  send_id.length();
    int ret = this->m_serial_port->writeData(data);
    if(ret == -1)
    {
        qDebug() << "write 2 fail !";
    }
    qDebug() << "write 2 succes !";

    return 0;
}

int SerialPortWriteRead::GetIspdTagId()
{
    SerialPort::Data data;
    char tag_id_msg[9];
    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_RD_ID);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    data.buffer = send_cus_pkg.data();
    data.length =send_cus_pkg.length();

    int ret = this->m_serial_port->writeData(data);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入读ID信息失败");
    }
    qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入读ID信息成功");

    ret = this->m_serial_port->readData(tag_id_msg, 9);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读ID失败");
    }
    qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读ID成功");

    QByteArray byteArray = QByteArray(tag_id_msg);
    protoserialport::REP_TYPE type = static_cast<protoserialport::REP_TYPE>(byteArray.at(2));
    protoserialport::ResPkg<protoserialport::ResID> resid(tag_id_msg, strlen(tag_id_msg));
    if(type == protoserialport::REP_ISPD_ID)
    {
        qint32 ispd_encryt_id = resid.getData().getID();
        QString ispd_encryt_strid = m_encrypt->encrypt_ispd_id(ispd_encryt_id);
        qDebug() << "ispd_encryt_strid = " << ispd_encryt_strid;
    }
    else
    {
        qDebug() << "类型不匹配";
    }
    return 0;
}

//读采样率函数
int SerialPortWriteRead::GetHz()
{
    SerialPort::Data data;
    char hz_msg[6];
    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_RD_HZ);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    data.buffer = send_cus_pkg.data();
    data.length =send_cus_pkg.length();
    int ret = this->m_serial_port->writeData(data);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入采样率失败");
        return -1;
    }
    qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入采样率成功");

    ret = this->m_serial_port->readData(hz_msg, 6);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读ID失败");
        return -1;
    }
    qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读ID成功");

    QByteArray byteArray = QByteArray(hz_msg);
    protoserialport::ResPkg<protoserialport::ResHZ> reshz(byteArray.data(), byteArray.length());
    qDebug() << QString::fromLocal8Bit("采样率 = ") << reshz.getData().getHZ();

    return 0;
}

//读版本号函数
int SerialPortWriteRead::GetVersion()
{
    SerialPort::Data data;
    char version_msg[8];
    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_RD_VERSION);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    data.buffer = send_cus_pkg.data();
    data.length =send_cus_pkg.length();
    int ret = this->m_serial_port->writeData(data);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入版本号指令失败");
        return -1;
    }
    qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入版本号指令成功");
    ret = this->m_serial_port->readData(version_msg, 8);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读版本失败");
        return -1;
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读版本成功");
        protoserialport::ResPkg<protoserialport::ResVersion> resversion(version_msg, strlen(version_msg));
        QString ispd_version =  QString::fromLocal8Bit("固件") +  QString("%1").arg(resversion.getData().get_ispd_version(),3,10,QChar('0'));
        QString uwb_version =  QString::fromLocal8Bit("透传") + QString("%1").arg(resversion.getData().get_uwb_version(),3,10,QChar('0'));
        QString sp_version =  QString::fromLocal8Bit("协议") + QString("%1").arg(resversion.getData().get_sp_version(),3,10,QChar('0'));
        qDebug() << QString::fromLocal8Bit("固件 = ") << ispd_version << uwb_version << sp_version;
    }
    return 0;
}

//读心率并处理数据函数
int SerialPortWriteRead::GetHeartRate()
{
    SerialPort::Data data;
    char heart_rate_msg[8];
    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_RD_HR);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    data.buffer = send_cus_pkg.data();
    data.length =send_cus_pkg.length();
    int ret = this->m_serial_port->writeData(data);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入心率指令失败");
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入心率指令成功");
    }

    ret = this->m_serial_port->readData(heart_rate_msg, 6);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读心率失败");
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读心率成功");
        protoserialport::ResPkg<protoserialport::ResHR2> reshr(heart_rate_msg, strlen(heart_rate_msg));
        quint8 hr = reshr.getData().get_hr();
        qDebug() << QString::fromLocal8Bit("心率 = ") << hr;
    }
    return 0;
}

//读传感器数据函数
int SerialPortWriteRead::GetSensor()
{
    SerialPort::Data data;
    char sensor_msg[20];
    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_RD_SENSOR);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    data.buffer = send_cus_pkg.data();
    data.length =send_cus_pkg.length();
    int ret = this->m_serial_port->writeData(data);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入获取传感器数据失败");
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入获取传感器数据成功");
    }

    ret = this->m_serial_port->readData(sensor_msg, 20);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号")+ QString::fromLocal8Bit("读传感器数据失败");
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("串口号")+ QString::fromLocal8Bit("读传感器数据成功");
        protoserialport::ResPkg<protoserialport::Sensor> sensor(sensor_msg, strlen(sensor_msg));
        qint16 ac_x = sensor.getData().get_ac_x();
        qint16 ac_y = sensor.getData().get_ac_y();
        qint16 ac_z = sensor.getData().get_ac_z();

        qint16 gy_x = sensor.getData().get_gy_x();
        qint16 gy_y = sensor.getData().get_gy_y();
        qint16 gy_z = sensor.getData().get_gy_z();

        quint16 hb = sensor.getData().get_hb();

        quint8 pw = sensor.getData().get_pw() & 0x7F;

        quint8 is_charge = sensor.getData().get_pw() & 0x80;

        QString is_charge_str = is_charge == 1 ?  QString::fromLocal8Bit("正在充电") :  QString::fromLocal8Bit("未充电");

        QString ac_pt = "(" + QString::number(ac_x) + ","
                + QString::number(ac_y) + ","
                + QString::number(ac_z) + ")";

        QString gy_pt = "(" + QString::number(gy_x) + ","
                + QString::number(gy_y) + ","
                + QString::number(gy_z) + ")";

        qDebug() << QString::fromLocal8Bit("传感器 = ") << hb << pw << is_charge_str << ac_pt << gy_pt;
    }
    return 0;
}

//一键获取
int SerialPortWriteRead::GetHzVersionTagId()
{
    SerialPort::Data data;
    char all_msg[13];
    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_RD_ALL);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    data.buffer = send_cus_pkg.data();
    data.length =send_cus_pkg.length();
    int ret = this->m_serial_port->writeData(data);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入一键获取指令失败");
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("写入一键获取指令成功");
    }

    ret = this->m_serial_port->readData(all_msg, 13);
    if(ret == -1)
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读一键获取失败");
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("串口号") + QString::fromLocal8Bit("读一键获取成功");
        protoserialport::ResPkg<protoserialport::ResAll> resall(all_msg, strlen(all_msg));
        qint32 ispd_encryt_id = resall.getData().getID();
        QString ispd_encryt_strid = m_encrypt->encrypt_ispd_id(ispd_encryt_id);
        QString hz12 = QString::number(resall.getData().getHZ());
        QString ispd_version =  QString::fromLocal8Bit("固件") +  QString("%1").arg(resall.getData().get_ispd_version(),3,10,QChar('0'));
        QString uwb_version =  QString::fromLocal8Bit("透传") + QString("%1").arg(resall.getData().get_uwb_version(),3,10,QChar('0'));
        QString sp_version =  QString::fromLocal8Bit("协议") + QString("%1").arg(resall.getData().get_sp_version(),3,10,QChar('0'));
        qDebug() << "all = " << ispd_encryt_strid << hz12 << ispd_version << uwb_version << sp_version;
    }
    return 0;
}


