@echo off
del test.exe
cls
g++ -s cri_source.cpp cri_test.cpp -o test.exe
test.exe
pause
