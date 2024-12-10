    /*好了, 设备现在已经初始化完成。 但是还没有一个设备被初始化过， 
     但是 CPU 的子系统已经启动并运行， 
     且内存和处理器管理系统已经在工作了。 
     现在我们终于可以开始做一些实际的工作了.. 
     */  
    static void __init do_basic_setup(void)  
    {  
        cpuset_init_smp();//针对SMP系统，初始化内核control group的cpuset子系统。如果非SMP，此函数为空。  
        /*创建一个单线程工作队列khelper。运行的系统中只有一个，主要作用是指定用户空间的程序路径和环境变量, 最终运行指定的user space的程序，属于关键线程，不能关闭*/  
        usermodehelper_init();  
          
        shmem_init();  
        driver_init();//初始化驱动模型中的各子系统，可见的现象是在/sys中出现的目录和文件  
        init_irq_proc();//在proc文件系统中创建irq目录，并在其中初始化系统中所有中断对应的目录。  
        do_ctors();//调用链接到内核中的所有构造函数，也就是链接进.ctors段中的所有函数。  
        usermodehelper_enable();  
        do_initcalls();//调用所有编译内核的驱动模块中的初始化函数。  
    }  


上面的函数调用了driver_init函数，作用是驱动模型子系统的初始化，对于内核驱动工程师来说比较重要，详解如下：


    void __init driver_init(void)  
    {  
        /* These are the core pieces */  
        devtmpfs_init();//初始化devtmpfs文件系统，驱动核心设备将在这个文件系统中添加它们的设备节点。  
        /*初始化驱动模型中的部分子系统和kobject： 
        devices 
        dev 
        dev/block 
        dev/char 
        */  
        devices_init();  
        buses_init();//初始化驱动模型中的bus子系统  
        classes_init();//1.初始化驱动模型中的class子系统  
        firmware_init();//1.初始化驱动模型中的firmware子系统  
        hypervisor_init();//1.初始化驱动模型中的hypervisor子系统  
      
        /* These are also core pieces, but must come after the 
         * core core pieces.      
         这些也是核心部件, 但是必须在以上核心中的核心部件之后调用。 
         */  
        platform_bus_init();//1.初始化驱动模型中的bus/platform子系统  
        cpu_dev_init();//1.初始化驱动模型中的devices/system/cpu子系统  
        memory_dev_init();//初始化驱动模型中的devices/system/memory子系统  
    }

而另外一个很主要的函数do_initcalls()调用所有编译内核的驱动模块中的初始化函数。其中按照各个内核模块初始化函数所自定义的启动级别（1～7），按顺序调用器初始化函数。对于同一级别的初始化函数，安装编译是链接的顺序调用，也就是和内核Makefile的编写有关。
