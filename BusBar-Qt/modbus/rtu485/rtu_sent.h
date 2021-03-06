#ifndef RTU_SENT_H
#define RTU_SENT_H

#include <QtCore>

#define RTU_SENT_LEN (16*3+5)  ////=============  长度需要改变 ////============ 加上开关，功率因素之后，是为14

struct Rtu_Sent {
    Rtu_Sent():fn(3),reg(0),len(RTU_SENT_LEN){}
    uchar addr; // 表示从机地址码
    uchar fn;  // 表示功能码
    ushort reg; // 表示寄存器首地址
    ushort len; // 表示数据长度
    ushort crc; // 表示CRC校验
};

ushort rtu_crc(uchar *buf, int len);
int rtu_sent_buff(uchar addr, uchar *buf);

#endif // RTU_SENT_H
