#include <stc12.h>
#include <BIN2HEX.h>

#define uchar unsigned char 
#define _nop_() __asm nop __endasm
uchar BCD[10]={0x3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};
void Delay5us(void)		//@35MHz
{
	unsigned char j;

	_nop_();
	j = 41;
	while (--j);
}

void Delay1s(void)		//@35MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 133;
	j = 255;
	k = 40;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void RST(){//对RST进行调整(将RST先拉底再拉高)
	P4_0=0;//SCLK
	P4_2=0;//RST
	Delay5us();
	P4_2=1;//RST
	Delay5us();
}
uchar M2L(uchar in){//端序转换
	uchar i = 0x00;
	uchar out = 0x00;
	for(i=0;i<8;i++){
		out = out<<1;
		out = out|((in>>i)&0x01);
	}
	return out;
}
void readRTC(uchar *dat){//多字节读取时钟数据
	uchar j,i;
	RST();
	P4_0=0;//SCLK
	P4_2=0;//RST
	Delay5us();
	P4_2=1;//RST
	Delay5us();
	
	for(i=0;i<8;i++){
		P4_1=((0xBF>>i)&0x01);
		Delay5us();
		P4_0=1;
		Delay5us();
		P4_0=0;
		Delay5us();
	}
	P4M1|=b00000010;//IO高阻输入
	P4M0&=b11111101;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			*(dat+i)=*(dat+i)<<1;
			*(dat+i)|=P4_1;
			Delay5us();
			P4_0=1;
			Delay5us();
			P4_0=0;
			Delay5us();
		}
	}
	P4M1&=b11111101;//恢复推挽
	P4M0|=b00000010;
}
void readRAM(uchar *dat){//多字节读取RAM数据(实际8字节)
	uchar j,i;
	RST();
	P4_0=0;//SCLK
	P4_2=0;//RST
	Delay5us();
	P4_2=1;//RST
	Delay5us();
	
	for(i=0;i<8;i++){
		P4_1=((0xFF>>i)&0x01);
		Delay5us();
		P4_0=1;
		Delay5us();
		P4_0=0;
		Delay5us();
	}
	P4M1|=b00000010;//IO高阻输入
	P4M0&=b11111101;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			*(dat+i)=*(dat+i)<<1;
			*(dat+i)|=P4_1;
			Delay5us();
			P4_0=1;
			Delay5us();
			P4_0=0;
			Delay5us();
		}
	}
	P4M1&=b11111101;//恢复推挽
	P4M0|=b00000010;
}
uchar BH(uchar dat){//优化1，将BCD存储的时间数据转为普通的计数数据
	return (dat>>4)*10+(dat&0x0F);
}
unsigned long timestamp23(uchar *dat){//优化3，转化为23年时间戳
	const unsigned long a=2678400;//优化2，大月一整个月的秒数
	const unsigned long b=86400;//一天的秒数
	unsigned long secs=0;
	uchar i;
	for(i=0x17;i<BH(*(dat+6));i++){//年转秒
		if(i%4==0){//闰年
			secs+=31622400;
		}
		else{//平年
			secs+=31536000;
		}
	}
	for(i=1;i<BH(*(dat+4));i++){//月转秒
		switch(i){
			case 1:
				secs+=a;
				break;
			case 2:
				if(*(dat+6)%4==0){//闰年
					secs+=(a-b*2);
				}
				else{//平年
					secs+=(a-b*3);
				}
				break;
			case 3:
				secs+=a;
				break;
			case 4:
				secs+=(a-b);
				break;
			case 5:
				secs+=a;
				break;
			case 6:
				secs+=(a-b);
				break;
			case 7:
				secs+=a;
				break;
			case 8:
				secs+=a;
				break;
			case 9:
				secs+=(a-b);
				break;
			case 10:
				secs+=a;
				break;
			case 11:
				secs+=(a-b);
				break;
			case 12:
				secs+=a;
				break;
		}
	}
	secs+=(*(dat+3)-1)*b;//日转秒
	secs+=BH(*(dat+2))*3600;//时转秒
	secs+=BH(*(dat+1))*60;//分转秒
	secs+=BH(*dat);//秒加秒
	return secs;
}
unsigned long sec(){//计算剩余秒数
	uchar d_n[8];
	uchar d_t[8];
	uchar i;
	unsigned long secs_n = 0;
	unsigned long secs_t = 0;
	readRTC(&d_n[0]);//现在(now)时间
	readRAM(&d_t[0]);//目标(target)时间
	//端序转换
	for(i=0;i<8;i++){
		d_n[i]=M2L(d_n[i]);
		d_t[i]=M2L(d_t[i]);
	}
	//转化为23年时间戳
	secs_t = timestamp23(&d_t[0]);
	secs_n = timestamp23(&d_n[0]);
	return secs_t-secs_n;
}
void encode(unsigned long Msecs,unsigned long *secs){
	*secs=Msecs/10000000;
	*secs=*secs<<4;
	*secs|=(Msecs%10000000)/1000000;
	*secs=*secs<<4;
	*secs|=(Msecs%1000000)/100000;
	*secs=*secs<<4;
	*secs|=(Msecs%100000)/10000;
	*secs=*secs<<4;
	*secs|=(Msecs%10000)/1000;
	*secs=*secs<<4;
	*secs|=(Msecs%1000)/100;
	*secs=*secs<<4;
	*secs|=(Msecs%100)/10;
	*secs=*secs<<4;
	*secs|=Msecs%10;
}
void shift(unsigned long dat){//移位寄存器(数码管高4位)
	uchar i;
	P4_6=0;//复位
	P4_4=0;
	P4_7=0;
	Delay5us();
	P4_6=1;
	Delay5us();
	for(i=0;i<32;i++){
		P4_3=((dat>>(31-i))&0x01);
		Delay5us();
		P4_5=1;
		Delay5us();
		P4_5=0;
		Delay5us();
	}
	P4_4=1;
}
unsigned long BCD2PINh(unsigned int hours){//BCD码转换为硬件针脚电平(小时)
	unsigned long dat=0x00000000;
	uchar i;
	for(i=0;i<16;i+=4){
		dat=dat<<8;
		dat|=BCD[(hours>>(12-i))&0x0F];
	}
	return dat;
}

void main(){
	unsigned long secs;//BCD码，剩余的秒数
	unsigned long Msecs;//总相差秒数
	P0M1=0x00;//P0口强推挽
	P0M0=0xFF;
	P1M1=0x00;//P1口强推挽
	P1M0=0xFF;
	P2M1=0x00;//P2口强推挽
	P2M0=0xFF;
	P3M1=0x00;//P3口强推挽
	P3M0=0xFF;
	P4M1=0x00;//P4口强推挽
	P4M0=0xFF;
	Msecs=sec();
	uchar count=0;
	while(1){//主循环
		if(count==0xFF){
			count=0;
			Msecs=sec();
		}
		encode(Msecs,&secs);
		shift(BCD2PINh(secs>>16));
		P0=BCD[secs&0x0F];
		P1=BCD[(secs>>4)&0x0F];
		P2=BCD[(secs>>8)&0x0F];
		P3=BCD[(secs>>12)&0x0F];
		Delay1s();
		Msecs-=1;
		count+=1;
	}
}
/*コードが完成しました！*/