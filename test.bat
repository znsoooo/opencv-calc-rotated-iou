@echo off
del test.exe
cls
g++ -xc cri_source.c -xc++ cri_test.cpp -o test.exe
test.exe
pause
