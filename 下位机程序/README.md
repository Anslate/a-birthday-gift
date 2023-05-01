# 下位机程序
各位请使用SDCC编译  

## 食用方法
~~开袋即食~~  

0. 不要焊接RN7电阻（会下拉串口导致串口通信失效）  
1. 将单片机时钟频率调为35MHz
2. 写入`Init.c`程序
3. 连接电脑，启动[上位机程序](../%E4%B8%8A%E4%BD%8D%E6%9C%BA%E7%A8%8B%E5%BA%8F/)中的[Set clock.exe](../%E4%B8%8A%E4%BD%8D%E6%9C%BA%E7%A8%8B%E5%BA%8F/Set%20clock/Set%20clock.exe)，并设置时间
4. 写入`cd only seconds.c`或`cd with hours.c`程序  
5. 焊接RN7电阻
6. 充电，观赏

## `cd only seconds.c`与`cd with hours.c`的差异
+ `cd only seconds.c`就是显示8位秒数，距离目标时间相差的总秒数，~~不怎么准~~
+ `cd with hours.c`中高4位为剩余小时数，中2位为剩余分钟数，末2位为剩余秒数，60进制，~~也不怎么准~~