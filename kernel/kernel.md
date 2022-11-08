# 11.7
- kernel 启动顺序
    1.bios
    2.grub
    3.grub建立虚拟系统，挂在kernel
     .开始startkernel的函数调用
    4.kernel自检，加载硬件，driverdengdeng
    5.kernel启动第一个进程，init
    6.kernel加载rc脚本，包括sys脚本(也会初始化一些剩余的驱动进程）(包括mod probe.conf模块,ISA PnP的硬件设备,USB设备,初始化串行端口设备），然后确定运行级别，然后local脚本等等
     .动态库和kernel的模块可能是一样的，都是通过某一个函数，找到加载是动态库中某个函数的地址，从而获取到某个函数，而不是通过加载头文件，然后通过函数调用进行函数的调用



- device driver 创建顺序
    1. 首先分区别，静态模块和动态模块
    2. 静态模块：静态模块从dtsi设备中，解析节点，然后module init，

- device，单片机，嵌入式系统，都有bootloader，运行相应的程序也可以叫小系统，此系统可能只是用来刷机，刷我们的用户程序，那么，类是与鼠标这种外步的io设备，相机，这种android内部的有sensor的设备，这两种相对与linux是设备的东西有没有bootloader呢（烧录程序呢）,相机带sensor，这种设备应该是有自己的bootloader的，鼠标内部的主控芯片也相当与是一个单片机，这样来看，他应该也是有自己的bootloader的
- 由此来看，几乎所有的设备都是有自己的单片机，有自己的bootloader的，
