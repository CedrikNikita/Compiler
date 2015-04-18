@ECHO OFF
FOR /L %%f IN (1,1,39) DO Compiler -s test"%%f".c -out output"%%f".out
g++ validator.cpp
a.exe
g++ onefile.cpp
a.exe