# 1. 需求分析
在windows操作系统上启动一个服务，该服务具有以下功能:
* 按照配置获取采集卡的模拟量数据，并根据动作周期，将数据存入数据库。
* 提供外部接口，使得外部程序可以控制本服务采集功能的开启及停止。
* 通过外部接口，可以将实时数据发送给外部程序，用于数据的实时显示。


# 2. 技术方案
## 2.1 对外接口
本服务对外提供TCP Server端口，使得外部程序可以通过网络连接本服务，从而获得控制采集功能开始/停止，以及获得实时数据的功能
## 2.2 配置文件功能设计
配置文件需要满足以下功能点：
* 设置选择的板卡
* 设置板卡采样通道数量
* 设置板卡采样频率
* 设置485通道周期采样开关及采样周期
本程序通过yaml文件的方式进行数据采集的配置，配置文件设计如下：
```
devices:
  -
    description: 'DemoDevice,BID#0'                 #设备描述符
    profilePath: '../../profile/DemoDevice.xml'     #设备预配置文件路径
    startChannel: 0                                 #起始通道
    channelCount: 2                                 #通道数量
    clockRate: 1000                                 #通道时钟频率
    sectionLength: 1024                             #通道采样缓存，即每次采样个数
    groupStartChannel: 0                            #每个动作周期起始触发通道
    groupStopChannel: 0                             #每个动作周期终止触发通道
    thresholds:                                     #每个通道阈值，个数与channelCount相同
      - 0.1
      - 0.1
  - 
    description: 'DemoDevice,BID#1'
    profilePath: '../../profile/DemoDevice.xml'
    startChannel: 0
    channelCount: 2
    clockRate: 1000
    sectionLength: 1024
    groupStartChannel: 0
    groupStopChannel: 0
    thresholds:
      - 0.1
      - 0.1
serialPort:
  -
    portName: "com1"                                  #485端口名称
    period: 30s                                       #采样周期
  -
    portName: "com2"
    period: 1m
```