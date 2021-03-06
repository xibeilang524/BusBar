/*
 * rtu_sent.cpp
 * 把接收到的数据还原成结构体
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "rtu_recv.h"


/**
  * 功　能：长度 校验
  * 入口参数：buf -> 缓冲区  len -> 长度
  * 返回值：0 正确
  */
static int rtu_recv_len(uchar *buf, int len)
{
    int ret = 0;
    int rtn = RTU_SENT_LEN+6;

    if(len < rtn) {
        ret = -1;
        qDebug() << "rtu recv Err: len too short!!";
    } else if(len > rtn) {
        ret = -2;
        qDebug() << "rtu recv Err: len too long!!" << len << rtn ;
    } else {
        len = buf[2]*256 + buf[3];
        if(len != RTU_SENT_LEN) {
            ret = -3;
            qDebug() << "rtu recv len Err!!"<< len << rtn ;
        }
    }

    return ret;
}


/**
  * 功　能：读取数据包头部
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：0 正确
  */
static int rtu_recv_head(uchar *ptr,  Rtu_recv *pkt)
{
    pkt->addr = *(ptr++);// 从机地址码
    pkt->fn = *(ptr++);  /*功能码*/
    pkt->len = (*ptr) * 256 + *(ptr+1); /*数据长度*/

    return 4;
}

/**
  * 功　能：读取电参数 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
static int rtu_recv_data(uchar *ptr, RtuRecvLine *msg)
{
    msg->vol =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电压
    msg->cur =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电流
    msg->pow =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取功率
    msg->ele =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能高8位
    msg->ele <<= 8; // 左移8位
    msg->ele +=  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能底8位
    msg->curAlarm =  (*ptr) * 256 + *(ptr+1);  ptr += 2;// 上限电流报警值
    msg->pf =  *(ptr++);// 功率因素
    msg->sw =  *(ptr++);// 开关状态
    msg->wave =  (*ptr) * 256 + *(ptr+1);  ptr += 2;    // 谐波值

    msg->apPow = msg->vol * msg->cur / 10; // 视在功率   

    return 16;   ////============ 加上开关，功率因素之后，是为14
}

/**
  * 功　能：读取环境 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
static int rtu_recv_env(uchar *ptr, RtuRecvEnv *msg)
{
    msg->tem = *(ptr++); /*温度*/
//    msg->hum = *(ptr++); // 湿度
    return 1;
}

/**
  * 功　能：检验码 数据
  * 入口参数：buf -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：true
  */
static bool rtu_recv_crc(uchar *buf, int len, Rtu_recv *msg)
{
    bool ret = true;
    int rtn = len-2; uchar *ptr = buf+rtn;

    msg->crc = (ptr[1]*256) + ptr[0]; // 获取校验码
    ushort crc = rtu_crc(buf, rtn);
    if(crc != msg->crc) {
        ret = false;
        qDebug() << "rtu recv crc Err!";
    }

    return ret;
}



/**
  * 功　能：还原数据包
  * 入口参数：buf -> 缓冲区   len -> 数据长度
  * 出口参数：pkt -> 结构体
  * 返回值：true
  */
bool rtu_recv_packet(uchar *buf, int len, Rtu_recv *pkt)
{
    bool ret = false;

    int rtn = rtu_recv_len(buf, len);
    if(rtn == 0) {
        uchar *ptr=buf;
        ptr += rtu_recv_head(ptr, pkt);

        for(int i=0; i<RTU_LINE_NUM; ++i) // 读取电参数
            ptr += rtu_recv_data(ptr, &(pkt->data[i]));

        pkt->rate = *(ptr++);
        for(int i=0; i<RTU_TH_NUM; ++i) // 读取环境 数据
            ptr += rtu_recv_env(ptr, &(pkt->env[i]));
        pkt->lineNum = *ptr;

        ret = rtu_recv_crc(buf, len, pkt);
    }
    return ret;
}
