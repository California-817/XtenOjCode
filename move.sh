#!/bin/sh

if [ ! -d bin/module ]
then
    mkdir bin/module
else
    unlink bin/XtenOjCode
    unlink bin/module/libXtenOjCode.so
fi

cp Xten/bin/XtenFrame bin/XtenOjCode
cp lib/libXtenOjCode.so bin/module/
