# 二维自由度舵机云台

## 介绍
主要使用两种舵机，分别型号为SG90舵机云台和型号为DS3218 20KG的舵机云台

都是二维自由度的

实现了：

1、使用MPU6050读取翻滚角Roll和横向角Yaw，控制二维舵机，根据MPU6050姿态的改变而改变云台姿态

2、使用ADC采样摇杆模块的X轴和Y轴，控制云台的运动

3、扩展，根据OpenMV识别物品(如色块、物品、数字等)传回的数据，控制二维舵机的运动，激光笔接于舵机上，准确打到物品上,这里采用的是一个绿色的小球，舵机云台根据小球的运动而运动，使小球每时每刻准确无误的在摄像头的正中心

4、分别在X轴的舵机和Y轴的舵机，根据OpenMV传回的坐标数据，传入PID进行计算，从而控制舵机进行运动，但是这种云台的PID和普通常见的PID计算有所不同，需要理解清楚，然后就是愉快的调参过程啦

## 软件架构

- 二维舵机云台随MPU6050姿态运动

- 摇杆模块控制二维舵机云台

- 二维舵机云台协同OpenMV追踪小球



#### 安装教程

1.  xxxx
2.  xxxx
3.  xxxx

#### 使用说明

1.  xxxx
2.  xxxx
3.  xxxx

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
