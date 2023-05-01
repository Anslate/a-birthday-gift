#8位数据，无校验，1停止位
#-p 获取串口表
#-CTX <COM> <data>输出RTC数据(16进制)
#-CRX <COM>读取RTC数据
#-MTX <COM> <data>输出RAM数据(16进制)
#-MRX <COM> <data>读取RAM数据
import serial,serial.tools.list_ports
import sys
def main():
    if(sys.argv[1]=="-p"):
        port = serial.tools.list_ports.comports()
        for i in port:
            print(i.device)
        return
    elif(sys.argv[1]=="-CTX"):
        s=serial.Serial(sys.argv[2],38400,timeout=4)
        s.write(bytes.fromhex("02"))
        s.write(bytes.fromhex(sys.argv[3]))
        s.close()
    elif(sys.argv[1]=="-CRX"):
        s=serial.Serial(sys.argv[2],38400,timeout=4)
        s.write(bytes.fromhex("01"))
        dat=s.read(8)
        print(dat.hex("-"))
        s.close()
    elif(sys.argv[1]=="-MTX"):
        s=serial.Serial(sys.argv[2],38400,timeout=4)
        s.write(bytes.fromhex("04"))
        s.write(bytes.fromhex(sys.argv[3]))
        s.close()
    elif(sys.argv[1]=="-MRX"):
        s=serial.Serial(sys.argv[2],38400,timeout=4)
        s.write(bytes.fromhex("03"))
        dat=s.read(8)
        print(dat.hex("-"))
    else:
        pass
    
if(__name__=="__main__"):
    main()