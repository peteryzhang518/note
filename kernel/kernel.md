# 11.7
- kernel 启动顺序
    1. bios
    2. grub
    3. grub建立虚拟系统，挂载kernel
        包括parse_dtb,解析设备树（”setup.c” : parse_dtb）,这一过程
        这部分过程就是下面rc文件中设备树转换成凭atform设备的过程，也是device_create的
    4. 开始startkernel的函数调用
        kernel自检，加载硬件，driverdengdeng
        kernel中module的加载在kernel中非常靠后，rest_init->调用kernel_thread->调用init()->do_base_setup()->driver_init()
        do_base_setup()函数中同时也会调用do_initcalls()，这个函数完成了所有需要静态加载模块的初始化
    5. kernel启动第一个进程，init
    6. kernel加载rc脚本，包括sys脚本(也会初始化一些剩余的驱动进程）(包括mod probe.conf模块,ISA PnP的硬件设备,USB设备,初始化串行端口设备），然后确定运行级别，然后local脚本等等


- driver和device在kernel中的源码运行顺序,基于kernel启动顺序
    1. init/main.c      do_base_setup()
    2. init/main.c          driver_init()
    3. include/linux/device/driver.h
                            driver_init()
    4. driver/base/init.c   driver_init()
    5. driver/base/init.c       device_init()
    6. driver/base/base.h       device_init()
    6. driver/base/core.c       device_init()   
- 这个函数过程几乎再这里完成,这个device仅仅是添加了全局变量，并且在dev文件夹下面注册相关文件

- platform_bus在注册完之后，只是注册完成，在initcall中的deviceinitcall中，会调用platform_add_devices，然后会去向总线注册device,然后就回去匹配driver,去调用dirver的probe函数


# 类似的nuttx系统的启动流程
__start--                                 #处理器执行的第一条指令
        |  
        v  
   stm32_clockconfig()------              #初始化时钟
                           |  
                           v  
                 rcc_reset()              #复位rcc
                 stm32_stdclockconfig()   #初始化标准时钟
                 rcc_enableperipherals()  #使能外设时钟
                           |  
        --------------------  
        |  
        v  
   stm32_fpuconfig()                      #配置fpu，shenzhou/nsh未调用
   stm32_lowsetup()                       #基本初始化串口，之后可以使用up_lowputc()
   stm32_gpioinit()                       #初始化gpio，只是调用stm32_gpioremap()设置重映射
   up_earlyserialinit()                   #初始化串口，之后可以使用up_putc()
   stm32_boardinitialize()--              #板级初始化
                           |  
                           v  
                 stm32_spiinitialize()    #初始化spi，只是调用stm32_configgpio()设置gpio
                 stm32_usbinitialize()    #初始化usb，只是调用stm32_configgpio()设置gpio
                 board_led_initialize()   #初始化led，只是调用stm32_configgpio()设置gpio
                           |  
        --------------------  
        |  
        v  
   os_start()---------------              #初始化操作系统
                           |  
                           v  
                 dq_init()                #初始化各种状态的任务列表（置为null）
                 g_pidhash[i]=            #初始化唯一可以确定的元素--进程ID
                 g_pidhash[PIDHASH(0)]=   #分配空闲任务的进程ID为0
                 g_idletcb=               #初始化空闲任务的任务控制块
                 sem_initialize()--       #初始化信号量
                                  |  
                                  v  
                       dq_init()          #将信号量队列置为null
                       sem_initholders()  #初始化持有者结构以支持优先级继承，shenzhou/nsh未调用
                                  |  
                           --------  
                           |  
                           v  
                 up_allocate_heap()       #分配用户模式的堆（设置堆的起点和大小）
                 kumm_initialize()        #初始化用户模式的堆
                 up_allocate_kheap()      #分配内核模式的堆，shenzhou/nsh未调用
                 kmm_initialize()         #初始化内核模式的堆，shenzhou/nsh未调用
                 task_initialize()        #初始化任务数据结构，shenzhou/nsh未调用
                 irq_initialize()         #将所有中断向量都指向同一个异常中断处理程序
                 wd_initialize()          #初始化看门狗数据结构
                 clock_initialize()       #初始化rtc
                 timer_initialize()       #配置POSIX定时器
                 sig_initialize()         #初始化信号
                 mq_initialize()          #初始化命名消息队列
                 pthread_initialize()     #初始化线程特定的数据，空函数
                 fs_initialize()---       #初始化文件系统
                                  |  
                                  v  
                       sem_init()         #初始化节点信号量为1
                       files_initialize() #初始化文件数组，空函数
                                  |  
                           --------  
                           |  
                           v  
                 net_initialize()--       #初始化网络
                                  |  
                                  v  
                       uip_initialize()   #初始化uIP层
                       net_initroute()    #初始化路由表，shenzhou/nsh未调用
                       netdev_seminit()   #初始化网络设备信号量
                       arptimer_init()    #初始化ARP定时器
                                  |  
                           --------  
                           |  
                           v  
                 up_initialize()---       #处理器特定的初始化
                                  |  
                                  v  
                       up_calibratedelay()#校准定时器
                       up_addregion()     #增加额外的内存段
                       up_irqinitialize() #设置中断优先级，关联硬件异常处理函数
                       up_pminitialize()  #初始化电源管理，shenzhou/nsh未调用
                       up_dmainitialize() #初始化DMA，shenzhou/nsh未调用
                       up_timerinit()     #初始化定时器中断
                       devnull_register() #注册/dev/null
                       devzero_register() #注册/dev/zero，shenzhou/nsh未调用
                       up_serialinit()    #注册串口控制台/dev/console和串口/dev/ttyS0
                       up_rnginitialize() #初始化并注册随机数生成器，shenzhou/nsh未调用
                       up_netinitialize() #初始化网络，是arch/arm/src/chip/stm32_eth.c中的
                       up_usbinitialize() #初始化usb驱动，shenzhou/nsh未调用
                       board_led_on()     #打开中断使能led，但很快会被其它地方的led操作改变状态
                                  |  
                           --------  
                           |  
                           v  
                 lib_initialize()         #初始化c库，空函数
                 group_allocate()         #分配空闲组
                 group_setupidlefiles()   #在空闲任务上创建stdout、stderr、stdin
                 group_initialize()       #完全初始化空闲组
                 os_bringup()------       #创建初始任务
                                  |  
                                  v  
                       KEKERNEL_THREAD()  #启动内核工作者线程
                       board_initialize() #最后一刻的板级初始化，shenzhou/nsh未调用
                       TASK_CREATE()      #启动默认应用程序
                                  |  
                           --------  
                           |  
                           v  
for up_idle()            #空闲任务循环
                           |  
        --------------------  
        |  
        v  
for(;;)                                #不应该到达这里















































- 动态库和kernel的模块可能是一样的，都是通过某一个函数，找到加载是动态库中某个函数的地址，从而获取到某个函数，而不是通过加载头文件，然后通过函数调用进行函数的调用

- 各种Initcall就是kernel module的调用

- device driver 创建顺序
    1. 首先分区别，静态模块和动态模块
    2. 静态模块：静态模块从dtsi设备中，解析节点，然后module init，

- device，单片机，嵌入式系统，都有bootloader，运行相应的程序也可以叫小系统，此系统可能只是用来刷机，刷我们的用户程序，那么，类是与鼠标这种外步的io设备，相机，这种android内部的有sensor的设备，这两种相对与linux是设备的东西有没有bootloader呢（烧录程序呢）,相机带sensor，这种设备应该是有自己的bootloader的，鼠标内部的主控芯片也相当与是一个单片机，这样来看，他应该也是有自己的bootloader的
- 由此来看，几乎所有的设备都是有自己的单片机，有自己的bootloader的，
- 比如说你写一个stm32的程序，但是这个程序只包含了你要如何使用这个单片机，但是单片机从上电那一刻起，所需要的硬件，因脚时钟等检查，然后从flush里去执行你存放的单片机程序，流水灯之类的程序，这个程序就是bootloader
- 如果要移植linux系统，如果是没有mmu也就是虚拟内存管理硬件的单片机，那么只能移植uclinux，这个是专门设计用来给没有mmc硬件的微型嵌入式系统，这种情况下，你可以使用默认的也就是上行提到的bootloader，stm32提供的默认bootloader。同时你也可以使用自己的bootloader。
- uboot就是一种专门的bootloader形式，一种实体形式，具体实现，你可以在移植linux的时候自己编写uboot
- bootloader也分等级，上面提到的stm32默认的这种属于一级bootloader。uboot这种形式的属于二级bootloader

