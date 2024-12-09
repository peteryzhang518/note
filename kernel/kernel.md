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



2.1 设备树解析和驱动加载
内核启动时，设备树被加载并解析。对于每一个硬件设备节点，内核会根据该设备的 compatible 属性（例如 compatible = "ti,am335x-usb";）查找并加载相应的驱动程序。如果设备树中描述的是一个 USB 设备，内核会加载 USB 驱动程序并初始化设备。
2.2 创建 /dev 设备文件
设备驱动程序加载后，会在 /dev 目录下创建相应的设备文件。这些文件通常由 udev （设备管理器）创建，它根据设备驱动暴露的接口和规则创建对应的设备节点。



是的，您理解的基本正确。在 Linux 内核启动过程中，解析设备树文件（dts 或 dtb）以及根据设备树加载并初始化驱动程序是一个非常重要的步骤，内核通常会在这一阶段集中处理硬件初始化。内核解析设备树并加载驱动程序后，才会继续执行其他的系统初始化工作。
内核在启动过程中处理设备树的顺序
    启动阶段： 在启动过程中，内核首先执行硬件初始化、CPU 初始化、内存初始化等基础操作。此时，内核并没有直接处理设备树，它只是进入初始化阶段，准备好设备树和硬件相关的配置。
    设备树的加载和解析：
        在内核启动过程中，设备树（dtb 文件）通常会被引导加载程序（如 U-Boot）传递给内核。
        内核会通过设备树库（libfdt）将设备树加载到内存中，并进行解析。
    解析的关键内容：
        设备树的根节点：内核首先会检查设备树头部的有效性（如通过 fdt_check_header()），然后获取根节点。
        遍历设备树节点：内核会遍历设备树中的所有节点，查找每个节点的 compatible 属性，根据该属性来查找并匹配设备驱动程序。
    驱动程序的加载与初始化：
        对于每一个匹配的设备，内核会查找相应的驱动程序，并通过调用 probe() 函数来初始化该设备。
        内核根据设备树中的信息（如寄存器地址、资源等）和驱动程序进行硬件初始化，可能会请求中断、映射内存、初始化设备等。
    设备驱动的注册与 /dev 文件创建：
        驱动程序的 probe() 函数执行后，内核会注册设备并可能会创建相应的 /dev 设备文件。
        例如，对于一个 SPI 控制器，内核可能会在 /dev/spidev0.0 下创建一个设备文件，允许用户空间的应用程序与硬件交互。
    设备树处理完成后： 一旦设备树解析完毕，并且所有设备驱动程序都已经加载并初始化，内核就会进入系统的其他初始化阶段。这包括：
        内核调度器的启动：内核开始调度进程，启动用户空间的进程。
        初始化用户空间：内核开始执行 init 进程（通常是 /sbin/init），并开始加载用户空间服务和应用程序。



启动引导加载程序（Bootloader）
通常，启动过程是由引导加载程序（如 U-Boot 或 GRUB）启动的，它会负责加载内核映像（vmlinuz）和设备树二进制文件（dtb）。引导加载程序执行的操作通常是： 加载内核和设备树到内存中。 将控制权交给内核启动。
引导加载程序将内核映像和设备树加载到预定的位置，内核通过这些信息开始自己的初始化过程。
2. 早期硬件初始化（Early Hardware Setup）
内核启动时，首先会执行一系列基础硬件初始化操作。这些操作通常在内核的 启动代码（start_kernel()） 中完成。这部分初始化在内核的最开始阶段非常重要，它会确保处理器、内存、以及其他关键硬件资源已经准备好，供后续的操作使用。
2.1 CPU 初始化
CPU 初始化是内核启动的第一步。具体来说，这一过程包括：
    设置 CPU 内部的相关状态，如 MMU（内存管理单元）、CPU 核心的启动等。
    进行系统的初步检测，确保 CPU 可以正常运行。
    配置处理器的 中断向量表，并初始化基本的 中断管理机制，使得内核能够处理硬件中断。
在内核的早期启动阶段，CPU 的基本初始化通常会在 start_kernel() 函数的开头执行。内核会为每个 CPU 核心执行初始化代码，确保所有核心处于可执行状态。
例如，在 ARM 架构中，CPU 的初始化可能涉及到：
    启动所有 CPU 核心。
    配置和启用 MMU，这对于内存管理至关重要。
    设置 中断控制器（例如 GIC，Generic Interrupt Controller）。
2.2 内存初始化
内存初始化是启动过程中至关重要的一部分。内核需要初始化物理内存管理（PMM）和虚拟内存管理（VMM）系统，使得内存能够被合理地管理和分配。具体来说： 内核会进行 内存检测，确保系统中所有可用的内存都被正确识别。
    配置 页表，启用 MMU（内存管理单元），以便能够管理虚拟内存。
    初始化 分配器，如 Buddy System，以便内核可以开始为应用程序和驱动分配内存。
内存的初始化会在内核的早期代码中完成，确保内核可以使用物理内存和虚拟内存的管理机制。
例如，在 ARM 架构中，内存初始化通常涉及：
    使用设备树中的内存信息配置系统内存。
    设置内存的起始地址和大小。
    配置 MMU，确保内核能够正确地访问和管理内存。
2.3 其他硬件初始化 除了 CPU 和内存之外，内核还会在这一步进行其他一些基础硬件的初始化：
    定时器初始化：内核会初始化系统定时器（如 TSC、PIT、Tick Timer），这些定时器对于内核调度、任务切换等非常重要。
    I/O 设备的初始化：包括基础的控制器和总线（如 UART、SPI、I2C 等）初始化，为后续的设备驱动加载做准备。
3. 设备树的加载和驱动程序加载（设备树相关） 一旦基础硬件初始化（如 CPU、内存、定时器等）完成，内核就会开始加载 设备树 并根据设备树的描述加载硬件驱动程序。这一步的具体操作如下：
3.1 设备树的加载
内核会从引导加载程序传递的地址处加载设备树（二进制格式的 dtb 文件）。加载后，内核会使用 libfdt 库解析设备树。
3.2 驱动程序的加载
内核遍历设备树中的每个硬件节点，读取每个节点的 compatible 属性，根据这个属性来查找并加载相应的驱动程序。驱动程序会根据设备树的信息配置硬件资源并完成设备初始化。
    驱动程序通过 of_match_device() 函数查找与设备树节点匹配的设备驱动，并通过调用 probe() 函数进行设备初始化。
    设备初始化包括映射硬件寄存器、配置中断、分配 DMA 等。
4. 继续执行系统初始化
当设备树解析并且硬件驱动加载完成后，内核会继续执行其他的系统初始化步骤，包括：
    进程初始化：内核会启动第一个进程（init）并开始进程调度。
    文件系统挂载：内核会挂载根文件系统（通常是通过 initramfs 或磁盘上的文件系统）。
    用户空间的初始化：内核会启动用户空间的应用程序和服务。
内核启动流程总结
    引导加载程序（Bootloader） 加载内核映像和设备树，并将控制权交给内核。
    内核初始化：
        CPU 初始化：初始化处理器、MMU 和中断管理。
        内存初始化：检测物理内存并配置页表和内存管理。
        硬件初始化：包括定时器和基础 I/O 控制器等的初始化。
    设备树加载和解析：
        内核加载设备树并解其他硬件初始化析。
        根据设备树信息查找并加载匹配的驱动程序。
    驱动程序初始化：初始化硬件设备，并通过 /dev 文件暴露设备。
    系统初始化：启动调度器，挂载根文件系统，初始化用户空间。


# 为什么有些硬件初始化不通过dtsi
    定时器初始化：内核通常会使用硬件平台上 默认的定时器（如 TSC、PIT 等）进行早期初始化，这些定时器并不依赖设备树。只有在设备树解析后，内核才可能会使用设备树中描述的其他定时器硬件（例如 ARM 平台上的定时器控制器）。
    I/O 设备初始化：一些 基础 I/O 控制器（如 UART、SPI、I2C）在内核早期启动时是通过硬编码的方式初始化的，而不依赖设备树。但对于平台上 可选或附加的 I/O 设备，设备树提供了详细的配置信息，内核在解析设备树后，会根据这些信息进一步初始化设备。
    设备树的作用：设备树提供了平台硬件的描述，帮助内核在后期初始化时识别和配置硬件设备。虽然在启动初期，一些硬件资源的初始化不依赖设备树，但设备树在后期的设备初始化中是非常重要的，尤其是在复杂硬件平台上。
因此，内核在启动时的硬件初始化并不完全依赖设备树，只有在设备树解析后，设备树中的配置信息才会开始影响内核的硬件管理。
# 这些最基本的硬件初始化代码在哪里
动代码位置
最基本的硬件初始化通常发生在内核的早期阶段，在这些初始化操作之后，内核才会继续处理其他的系统功能（如设备树解析、驱动加载等）。不同体系结构的启动代码位置和内容有所不同，但一般来说，启动代码的目录结构和位置如下：
    x86 架构：启动代码通常位于 arch/x86/boot/ 和 arch/x86/kernel/ 目录下。
    ARM 架构：启动代码通常位于 arch/arm/boot/ 和 arch/arm/kernel/ 目录下。
    RISC-V 架构：启动代码通常位于 arch/riscv/boot/ 和 arch/riscv/kernel/ 目录



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

