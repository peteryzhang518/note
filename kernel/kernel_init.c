    static int __init kernel_init(void * unused)  
    {  
        /*Wait until kthreadd is all set-up.*/  
        wait_for_completion(&kthreadd_done);  
        /* Now the scheduler is fully set up and can do blocking allocations */  
        gfp_allowed_mask = __GFP_BITS_MASK;  
      
        /* init can allocate pages on any node */  
        set_mems_allowed(node_states[N_HIGH_MEMORY]);  
        /* init can run on any cpu. */  
        set_cpus_allowed_ptr(current, cpu_all_mask);  
        cad_pid = task_pid(current);  
        smp_prepare_cpus(setup_max_cpus);  
        do_pre_smp_initcalls();  
        lockup_detector_init();  
        smp_init();  
        sched_init_smp();  
        //以上代码是在SMP系统做准备，激活所有CPU，并开始SMP系统的调度  
      
        /*do_basic_setup函数主要是初始化设备驱动，完成其他驱动程序（直接编译进内核的模块）的初始化。内核中大部分的启动数据输出（都是各设备的驱动模块输出）都是这里产生的*/  
        do_basic_setup();  
        /* Open the /dev/console on the rootfs, this should never fail */  
        if (sys_open((const char __user *) “/dev/console”, O_RDWR, 0) < 0)  
            printk(KERN_WARNING ”Warning: unable to open an initial console.\n”);  
        (void) sys_dup(0);  
        (void) sys_dup(0);  
        /*   
         复制两次标准输入（0）的文件描述符（它是上面打开的/dev/console，也就是系统控制台）： 
         一个作为标准输出（1） 
         一个作为标准出错（2） 
         * check if there is an early userspace init.  If yes, let it do all 
         * the work 
         */  
          
         /*检查是否有早期用户空间的init程序。如果有，让其执行*/  
        if (!ramdisk_execute_command)  
            ramdisk_execute_command = ”/init”;  
      
        if (sys_access((const char __user *) ramdisk_execute_command, 0) != 0) {  
            ramdisk_execute_command = NULL;  
            prepare_namespace();  
        }  
      
        /* 
         * Ok, we have completed the initial bootup, and 
         * we’re essentially up and running. Get rid of the 
         * initmem segments and start the user-mode stuff.. 
         */  
            printk(KERN_NOTICE ”47—”);  
      
        init_post();//在内核init线程的最后执行了init_post函数，在这个函数中真正启动了用户空间进程init  
        return 0;  
    }  

//上面调用函数
    static noinline int init_post(void)  
    {  
        /* need to finish all async __init code before freeing the memory */      
        /* 在释放内存前，必须完成所有的异步 __init 代码 */  
        async_synchronize_full();  
        free_initmem();//释放所有init.* 段中的内存。  
          
        mark_rodata_ro();//通过修改页表，保证只读数据段为只读属性。大部分构架为空函数  
        system_state = SYSTEM_RUNNING;//设置系统状态为运行状态  
          
        numa_default_policy();//设定NUMA系统的内存访问策略为默认  
      
        current->signal->flags |= SIGNAL_UNKILLABLE;//设置当前进程（init）为不可以杀进程（忽略致命的信号）  
        flush_delayed_fput();  
      
        if (ramdisk_execute_command) {//如果ramdisk_execute_command有指定的init程序，就执行它  
            run_init_process(ramdisk_execute_command);  
            printk(KERN_WARNING ”Failed to execute %s\n”,  
                    ramdisk_execute_command);  
        }  
      
        /* 
         * We try each of these until one succeeds. 
         * The Bourne shell can be used instead of init if we are 
         * trying to recover a really broken machine. 
         我们尝试以下的每个函数，直到函数成功执行. 
        如果我们试图修复一个真正有问题的设备， 
        Bourne shell 可以替代init进程。 
        */  
          
        if (execute_command) {  
            run_init_process(execute_command);  
            printk(KERN_WARNING ”Failed to execute %s.  Attempting ”  
                        ”defaults…\n”, execute_command);  
        }  
        run_init_process(”/sbin/init”);  
        run_init_process(”/etc/init”);  
        run_init_process(”/bin/init”);  
        run_init_process(”/bin/sh”);  
      
        panic(”No init found.  Try passing init= option to kernel. ”  
              ”See Linux Documentation/init.txt for guidance.”);  
    }  


