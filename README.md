# 这是操作系统课程设计代码
## 题目：分别使用 System V 共享内存与 System V 消息队列实现数据的发送和接收程序

## 本项目使用教程：
### 首先将代码拉取到本地
```shell
$ git clone https://github.com/foxzt/os.git
```
### 然后进入到项目目录，并创建build文件夹，使用cmake工具构建项目，使用make编译
```shell
$ cd os
$ mkdir build
$ cd build && cmake ..
$ make
```
### 项目目录下的bin文件夹就是可执行程序
```shell
$ cd ../bin
$ #测试使用共享内存实现的消息发送和接收程序
$ ./svshm_sender < infile &
$ svshm_receiver > out_file
$ #可以使用diff命令检测两个文件的内容是否一样，如果完全一样，则程序运行成功
```
