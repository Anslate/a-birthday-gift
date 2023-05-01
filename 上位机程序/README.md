# 上位机程序
由于我不太会写软件，所以本程序可能会引起部分人不适（就是屎山啦）  

上位机程序由两部分组成:
+ 硬件交互由名为`.\uart.py`的Python脚本完成，主要用于串口通信，再用PyInstaller编译为exe后被用户交互程序调用
+ 用户交互由`.\Set clock\`目录下的Godot工程完成，其中`uart.exe`为`.\uart.py`的编译结果  
~~因为我不会写普通的gui所以才用游戏引擎写gui的~~

`.\Set clock\Set clock.exe`为上位机程序最终的编译结果  
将`.\Set clock\Set clock.exe`与`.\Set clock\uart.exe`放在同一目录下运行即可