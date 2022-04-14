@echo off
setlocal
set /p flags=<%~dp0flags
if not defined CC set CC=clang
if not defined LD set LD=lld
if not defined ARCH set ARCH=x86_64
%CC% %flags% -target %ARCH%-unknown-windows-coff -fuse-ld="%LD%" -Wl,-opt:ref -Wl,-entry:main -L"%~dp0src\windows\lib" %*
endlocal