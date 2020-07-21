@echo off
rem seila so queria escrever isso
if [%1] == [] goto final
gcc -shared  -o %1.so -fPIC %1.c
:final