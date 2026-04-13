@echo off
echo 正在设置 UTF-8 环境变量...
chcp 65001 >nul
set PYTHONUTF8=1

echo 正在提取代码文件到 touhou_engine.txt...
C:\Users\web\AppData\Roaming\Python\Python313\Scripts\files-to-prompt.exe . -e cpp -e c -e h -e hpp -e lua -e json > touhou_engine.txt

echo 提取完成！
pause