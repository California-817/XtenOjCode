#!/bin/sh
#move脚本的作用就是将最新生成的动态库及服务器框架移动到bin可执行文件路径下
if [ ! -d bin/module ]
then
    mkdir bin/module
else
    unlink bin/XtenOjCode
    unlink bin/module/libXtenOjCode.so
    unlink bin/module/libCompileRun.so
fi

cp Xten/bin/XtenFrame bin/XtenOjCode
cp lib/libXtenOjCode.so bin/module/
cp lib/libCompileRun.so bin/module/
