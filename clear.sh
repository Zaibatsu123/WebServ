#!/bin/bash
echo 111 > ./root/YoupiBanane/youpi.bad_extension
echo 222 > ./root/YoupiBanane/youpi.bla
echo 333 > ./root/YoupiBanane/nop/other.pouic
echo 444 > ./root/YoupiBanane/nop/youpi.bad_extension
echo 555 > ./root/YoupiBanane/Yeah/not_happy.bad_extension
mkdir ./root/put_test
mkdir ./root/StartPage/del_dir/
rm -rf ./root/put_test/*
rm ./root/YoupiBanane/youpla.bla