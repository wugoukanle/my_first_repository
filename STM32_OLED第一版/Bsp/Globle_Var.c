#define  uchar            unsigned char
#define  uint             unsigned int
#define  ulong            unsigned long

unsigned char time_buff[6];

uchar end_flag=0;

uchar USB_Cheak_init_flag=1;

uchar YH_BH_H=0x00;
uchar YH_BH_L=0x00;

uchar shouquan_jieguo=0;		//0表示初始状态，1表示授权，2表示未授权

uchar guanjiflag=0;//关机标志

uchar key_code=0;		//要钥匙编号
