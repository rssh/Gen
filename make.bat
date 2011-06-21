@echo off
if %1A == with-corbaA nmake /F Makefile.nt WITH_CORBA="1" %2 & goto end
if %2A == with-corbaA nmake /F Makefile.nt WITH_CORBA="1" %1 & goto end
nmake /F Makefile.nt %1 
:end

