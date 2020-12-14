@echo off
@gcc -c pluglib.c
ar -q pluglib.lib pluglib.o
del pluglib.o
echo Done!
pause
