#include <stc12.h>
#include <BIN2HEX.h>

#define uchar unsigned char 
#define _nop_() __asm nop __endasm
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
void writeRTC(uchar *dat){//多字节写时钟数据
	uchar i,j;
	RST();
	for(i=0;i<8;i++){
		P4_1=((0xBE>>i)&0x01);
		Delay5us();
		P4_0=1;
		Delay5us();
		P4_0=0;
		Delay5us();
	}
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			P4_1=((*(dat+i)>>j)&0x01);
			Delay5us();
			P4_0=1;
			Delay5us();
			P4_0=0;
			Delay5us();
		}
	}
}
void UartInit(void){		//38400bps@35MHz

	SCON = 0x50;		//8 bits and variable baudrate
	AUXR |= 0x40;		//imer clock is 1T mode
	AUXR &= 0xFE;		//UART 1 use Timer1 as baudrate generator
	TMOD &= 0x0F;		//Set timer work mode
	TL1 = 0x1C;		//Initial timer value
	TH1 = 0xFF;		//Initial timer value
	ET1 = 0;		//Disable Timer%d interrupt
	TR1 = 1;		//Timer1 start run
}
void writeRAM(uchar *dat){//多字节写RAM数据
	uchar i,j;
	RST();
	for(i=0;i<8;i++){
		P4_1=((0xFE>>i)&0x01);
		Delay5us();
		P4_0=1;
		Delay5us();
		P4_0=0;
		Delay5us();
	}
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			P4_1=((*(dat+i)>>j)&0x01);
			Delay5us();
			P4_0=1;
			Delay5us();
			P4_0=0;
			Delay5us();
		}
	}
}
void UART_TX(uchar dat){
	SBUF=dat;
	while(!TI);
	TI=0;
}

uchar UART_RX(){//阻塞式读取串口
	uchar dat;
	while(!RI);
	dat=SBUF;
	RI=0;
	return dat;
}

void main(){
	UartInit();
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
	P0=0x00;
	P1=0x00;
	P2=0x00;
	P3=0xFF;
	P4=0x00;
	while(1){//主循环
		if(RI){
			uchar dat;
			dat=SBUF;
			RI=0;
			switch(dat){
				uchar i,d[8];
				case 0x00://检测
					for(i=0;i<8;i++){
						UART_TX(0x39);
					}
					break;
				case 0x01://读取RTC,并串口输出
					readRTC(&d[0]);
					for(i=0;i<8;i++){
						UART_TX(M2L(d[i]));
					}
					break;
				case 0x02://串口输入，并写入RTC
					for(i=0;i<8;i++){
						d[i]=UART_RX();
					}
					writeRTC(&d[0]);
					break;
				case 0x03://读取RAM,并串口输出
					readRAM(&d[0]);
					for(i=0;i<8;i++){
						UART_TX(M2L(d[i]));
					}
					break;
				case 0x04://串口输入，并写入RAM
					for(i=0;i<8;i++){
						d[i]=UART_RX();
					}
					writeRAM(&d[0]);
			}
		}
	}
}
