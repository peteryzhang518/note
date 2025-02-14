ip是基础的网络配置管理工具

ip配置过的文件中，需要经常切换网络连接设置
所以出现了
netctl  networkmanager systemd-networkd等

在ip route 配置的路由表中，有三个默认路由表
是从小到大排序的
先看0：回环地址
然后是32766:本地
最后是32767:默认

如果需要加自己的配置，比如所有来自192.168.6.131的流量从1走，其他的从2走
那就需要加两个路由表，一个配置1,一个配置2



nmcli device show
nmcli device wifi list


$nmcli device wifi connect ZTE_BSP password bsp123456 ifname wlx0013ef6f4922
Device 'wlx0013ef6f4922' successfully activated with '086fa89f-52b1-4c2c-a5db-a5a71b72b559'.
$nmcli connection show
NAME                UUID                                  TYPE      DEVICE  
$nmcli connection delete ZTE_BSP

