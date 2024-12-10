    asmlinkage void __init start_kernel(void)  
    {  
        char  command_line;  
        extern const struct kernel_param __start___param[], __stop___param[];  
        /这两个变量为地址指针，指向内核启动参数处理相关结构体段在内存中的位置（虚拟地址）。 
        声明传入参数的外部参数对于ARM平台，位于 include\asm-generic\vmlinux.lds.h*/  
        / 
          Need to run as early as possible, to initialize the 
          lockdep hash: 
         lockdep是一个内核调试模块，用来检查内核互斥机制（尤其是自旋锁）潜在的死锁问题。 
         /  
        lockdep_init();//初始化内核依赖的关系表，初始化hash表  
        smp_setup_processor_id();//获取当前CPU,单处理器为空  
        debug_objects_early_init();//对调试对象进行早期的初始化,其实就是HASH锁和静态对象池进行初始化  
      
        / 
          Set up the the initial canary ASAP: 
         初始化栈canary值 
         canary值的是用于防止栈溢出攻击的堆栈的保护字 。 
         /  
        boot_init_stack_canary();  
        /*1.cgroup: 它的全称为control group.即一组进程的行为控制.  
        2.该函数主要是做数据结构和其中链表的初始化  
        3.参考资料： Linux cgroup机制分析之框架分析 
        /  
        cgroup_init_early();  
      
        local_irq_disable();//关闭系统总中断（底层调用汇编指令）  
        early_boot_irqs_disabled = true;  
      
    / 
      Interrupts are still disabled. Do necessary setups, then 
      enable them 
     /  
        tick_init();//1.初始化内核时钟系统  
        boot_cpu_init();//1.激活当前CPU（在内核全局变量中将当前CPU的状态设为激活状态）  
        page_address_init();//高端内存相关，未定义高端内存的话为空函数  
        printk(KERN_NOTICE ”%s”, linux_banner);  
        /1.内核构架相关初始化函数,可以说是非常重要的一个初始化步骤。 
        其中包含了处理器相关参数的初始化、内核启动参数（tagged list）的获取和前期处理、 
        内存子系统的早期的初始化（bootmem分配器）。 主要完成了4个方面的工作，一个就是取得MACHINE和PROCESSOR的信息然或将他们赋值 
        给kernel相应的全局变量，然后呢是对boot_command_line和tags接行解析，再然后呢就是 
        memory、cach的初始化，最后是为kernel的后续运行请求资源″*/  
        setup_arch(&command_line);  
        /1.初始化代表内核本身内 
        存使用的管理结构体init_mm。  
        2.ps：每一个任务都有一个mm_struct结构以管理内存空间，init_mm是内核的mm_struct，其中：  
        3.设置成员变量 mmap指向自己，意味着内核只有一个内存管理结构;  
        4.设置 pgd=swapper_pg_dir，swapper_pg_dir是内核的页目录(在arm体系结构有16k， 所以init_mm定义了整个kernel的内存空间)。  
        5.这些内容涉及到内存管理子系统/  
        mm_init_owner(&init_mm, &init_task);  
        mm_init_cpumask(&init_mm);//初始化CPU屏蔽字  
        /1.对cmdline进行备份和保存：保存未改变的comand_line到字符数组static_command_line［］ 中。保存  boot_command_line到字符数组saved_command_line［］中 
        /  
        setup_command_line(command_line);  
      
        /如果没有定义CONFIG_SMP宏，则这个函数为空函数。如果定义了CONFIG_SMP宏，则这个setup_per_cpu_areas()函数给每个CPU分配内存，并拷贝.data.percpu段的数据。为系统中的每个CPU的per_cpu变量申请空间。 
        /  
        /下面三段1.针对SMP处理器的内存初始化函数，如果不是SMP系统则都为空函数。 (arm为空)  
        2.他们的目的是给每个CPU分配内存，并拷贝.data.percpu段的数据。为系统中的每个CPU的per_cpu变量申请空间并为boot CPU设置一些数据。  
        3.在SMP系统中，在引导过程中使用的CPU称为boot CPU/  
        setup_nr_cpu_ids();  
        setup_per_cpu_areas();  
        smp_prepare_boot_cpu(); / arch-specific boot-cpu hooks /  
      
      
        build_all_zonelists(NULL, NULL);//  建立系统内存页区(zone)链表  
      
        page_alloc_init();//内存页初始化  
      
      
        printk(KERN_NOTICE ”Kernel command line: %s\n”, boot_command_line);  
        parse_early_param();//  解析早期格式的内核参数  
        /函数对Linux启动命令行参数进行在分析和处理, 
        当不能够识别前面的命令时，所调用的函数。/  
        parse_args(”Booting kernel”, static_command_line, __start___param,  
               __stop___param - __start___param,  
               -1, -1, &unknown_bootoption);  
        jump_label_init();  
        / 
          These use large bootmem allocations and must precede 
          kmem_cache_init() 
         /  
        setup_log_buf(0);  
        /初始化hash表，以便于从进程的PID获得对应的进程描述指针，按照开发办上的物理内存初始化pid hash表 
        /  
        pidhash_init();  
        vfs_caches_init_early();//建立节点哈希表和数据缓冲哈希表  
        sort_main_extable();//对异常处理函数进行排序  
        trap_init();//初始化硬件中断  
        mm_init();//    Set up kernel memory allocators     建立了内核的内存分配器  
        / 
          Set up the scheduler prior starting any interrupts (such as the 
          timer interrupt). Full topology setup happens at smp_init() 
          time - but meanwhile we still have a functioning scheduler. 
         /  
        sched_init();  
        / 
          Disable preemption - early bootup scheduling is extremely 
          fragile until we cpu_idle() for the first time. 
         /  
        preempt_disable();//禁止调度  
        //  先检查中断是否已经打开，若打开，输出信息后则关闭中断。  
        if (!irqs_disabled()) {  
            printk(KERN_WARNING ”start_kernel(): bug: interrupts were ”  
                    ”enabled *very early, fixing it\n”);  
            local_irq_disable();  
        }  
        idr_init_cache();//创建錳dr缓冲区  
        perf_event_init();  
        rcu_init();//互斥访问机制  
        radix_tree_init();  
        / init some links before init_ISA_irqs() /  
        early_irq_init();  
        init_IRQ();//中断向量初始化  
        prio_tree_init();//初始化优先级数组  
        init_timers();//定时器初始化  
        hrtimers_init();//高精度时钟初始化  
        softirq_init();//软中断初始化  
        timekeeping_init();//   初始化资源和普通计时器  
        time_init();  
        profile_init();//   对内核的一个性能测试工具profile进行初始化。  
        call_function_init();  
        if (!irqs_disabled())  
            printk(KERN_CRIT ”start_kernel(): bug: interrupts were ”  
                     ”enabled early\n”);  
        early_boot_irqs_disabled = false;  
        local_irq_enable();//使能中断  
        kmem_cache_init_late();//kmem_cache_init_late的目的就在于完善slab分配器的缓存机制.  
      
        / 
          HACK ALERT! This is early. We’re enabling the console before 
          we’ve done PCI setups etc, and console_init() must be aware of 
          this. But we do want output early, in case something goes wrong. 
         /  
        console_init();//初始化控制台以显示printk的内容  
        if (panic_later)  
            panic(panic_later, panic_param);  
      
        lockdep_info();//   如果定义了CONFIG_LOCKDEP宏，那么就打印锁依赖信息，否则什么也不做  
      
        / 
          Need to run this when irqs are enabled, because it wants 
          to self-test [hard/soft]-irqs on/off lock inversion bugs 
          too: 
         /  
        locking_selftest();  
    #ifdef CONFIG_BLK_DEV_INITRD  
        if (initrd_start && !initrd_below_start_ok &&  
            page_to_pfn(virt_to_page((void )initrd_start)) < min_low_pfn) {  
            printk(KERN_CRIT ”initrd overwritten (0x%08lx < 0x%08lx) - ”  
                ”disabling it.\n”,  
                page_to_pfn(virt_to_page((void )initrd_start)),  
                min_low_pfn);  
            initrd_start = 0;  
        }  
    #endif  
        page_cgroup_init();  
        debug_objects_mem_init();  
        kmemleak_init();  
        setup_per_cpu_pageset();  
        numa_policy_init();  
        if (late_time_init)  
            late_time_init();  
        sched_clock_init();  
        calibrate_delay();//校准延时函数的精确度  
        pidmap_init();//进程号位图初始化，一般用一个錺age来表示所有进程的錺id占用情况  
        anon_vma_init();    //  匿名虚拟内存域（ anonymous VMA）初始化  
    #ifdef CONFIG_X86  
        if (efi_enabled)  
            efi_enter_virtual_mode();  
    #endif  
        thread_info_cache_init();//获取thread_info缓存空间，大部分构架为空函数（包括ARM  
        cred_init();    //任务信用系统初始化。详见：Documentation/credentials.txt  
        fork_init(totalram_pages);  //进程创建机制初始化。为内核”task_struct”分配空间，计算最大任务数。  
        proc_caches_init(); //初始化进程创建机制所需的其他数据结构，为其申请空间。  
        buffer_init();  //缓存系统初始化，创建缓存头空间，并检查其大小限时。  
        key_init(); //内核密钥管理系统初始化  
        security_init();    //内核安全框架初始?  
        dbg_late_init();  
        vfs_caches_init(totalram_pages);vfs_caches_init(totalram_pages);//虚拟文件系统（VFS）缓存初始化  
        signals_init();//信号管理系统初始化  
        / rootfs populating might need page-writeback /  
        page_writeback_init();//页写回机制初始化  
    #ifdef CONFIG_PROC_FS  
        proc_root_init();//proc文件系统初始化  
    #endif  
        cgroup_init();//control group正式初始化  
        cpuset_init();//CPUSET初始化。 参考资料：《多核心計算環境—NUMA與CPUSET簡介》  
        taskstats_init_early(); //任务状态早期初始化函数：为结构体获取高速缓存，并初始化互斥机制。  
        delayacct_init();   //任务延迟初始化  
                  
        check_bugs();//检查CPU BUG的函数，通过软件规避BUG  
        acpi_early_init(); / before LAPIC and SMP initACPI早期初始化函数。 ACPI - Advanced Configuration and Power Interface高级配置及电源接口 /  
        sfi_init_late();//功能跟踪调试机制初始化，ftrace 是 function trace 的简称  
      
        if (efi_enabled)  
            efi_free_boot_services();  
        ftrace_init();  
        / Do the rest non-__init’ed, we’re now alive /  
        rest_init();// 虽然从名字上来说是剩余的初始化。但是这个函数中的初始化包含了很多的内容  
    }  



 1.内核启动参数的获取和处理

 2.setup_arch(&command_line);函数

 3.内存管理的初始化（从bootmem到slab）

 4.各种内核体系的初始化

 5.rest_init();函数




























 	lockdep_init() //lockdep 是死锁检测模块，此函数会初始化两个 hash 表
	set_task_stack_end_magic() //设置任务栈结束魔术数，用于栈溢出检测
	smp_setup_processor_id() //跟 SMP 有关(多核处理器)，设置处理器 ID
	debug_objects_early_init() //做一些和 debug 有关的初始化
	boot_init_stack_canary() //栈溢出检测初始化
	cgroup_init_early() //cgroup 初始化，cgroup 用于控制 Linux 系统资源
	local_irq_disable() //关闭当前 CPU 中断, 中断关闭期间做一些重要的操作，然后打开中断
	boot_cpu_init() //跟 CPU 有关的初始化
	page_address_init() //页地址相关的初始化
	pr_notice("%s", linux_banner);/* 打印 Linux 版本号、编译时间等信息 */
	setup_arch(&command_line); //架构相关的初始化，此函数会解析传递进来的ATAGS 或者设备树(DTB)文件。会根据设备树里面的 model 和 compatible 这两个属性值来查找Linux 是否支持这个单板。此函数也会获取设备树中 chosen 节点下的 bootargs 属性值来得到命令行参数，也就是 uboot 中的 bootargs 环境变量的值，获取到的命令行参数会保存到command_line 中
	mm_init_cpumask(&init_mm) //内存有关的初始化
	setup_command_line(command_line) //存储命令行参数
	setup_nr_cpu_ids() //如果只是 SMP(多核 CPU)的话，此函数用于获取CPU 核心数量，CPU 数量保存在变量nr_cpu_ids 中
	setup_per_cpu_areas() //在 SMP 系统中有用，设置每个 CPU 的 per-cpu 数据
	smp_prepare_boot_cpu()
	build_all_zonelists(NULL, NULL); /* 建立系统内存页区(zone)链表 */
	page_alloc_init(); /* 处理用于热插拔 CPU 的页 */
	pr_notice("Kernel command line: %s\n", boot_command_line); //打印命令行信息
	parse_early_param(); /* 解析命令行中的 console 参数 */
	after_dashes = parse_args("Booting kernel",
			static_command_line, __start___param,
			__stop___param - __start___param,
			-1, -1, &unknown_bootoption);
	setup_log_buf(0); /* 设置 log 使用的缓冲区*/
	pidhash_init(); // 构建 PID 哈希表，Linux 中每个进程都有一个 ID,这个 ID 叫做 PID。通过构建哈希表可以快速搜索进程信息结构体
	idr_init_cache(); // IDR 初始化，IDR 是 Linux 内核的整数管理机制，也就是将一个整数 ID 与一个指针关联起来
	rcu_init(); // 初始化 RCU，RCU 全称为 Read Copy Update(读-拷贝修改)
	trace_init(); // 跟踪调试相关初始化 
	radix_tree_init(); //基数树相关数据结构初始化
	early_irq_init(); //初始中断相关初始化,主要是注册 irq_desc 结构体变量，因为 Linux 内核使用 irq_desc 来描述一个中断
	init_IRQ(); /* 中断初始化 */
	tick_init(); /* tick 初始化 */
	init_timers(); /* 初始化定时器 */
	hrtimers_init(); /* 初始化高精度定时器 */
	softirq_init(); /* 软中断初始化 */
	timekeeping_init(); 
	time_init(); /* 初始化系统时间 */
	WARN(!irqs_disabled(), "Interrupts were enabled early\n");
	early_boot_irqs_disabled = false;
	local_irq_enable(); /* 使能中断 */
	kmem_cache_init_late(); /* slab 初始化，slab 是 Linux 内存分配器 */
	console_init(); //初始化控制台，之前 printk 打印的信息都存放缓冲区中，并没有打印出来。只有调用此函数初始化控制台以后才能在控制台上打印信息
	locking_selftest() /* 锁自测 */
	kmemleak_init(); /* kmemleak 初始化，kmemleak 用于检查内存泄漏 */
	calibrate_delay(); /* 测定 BogoMIPS 值，可以通过 BogoMIPS 来判断 CPU 的性能BogoMIPS 设置越大，说明 CPU 性能越好*/
	pidmap_init(); /* PID 位图初始化 */
	anon_vma_init(); /* 生成 anon_vma slab 缓存 */
	cred_init(); /* 为对象的每个用于赋予资格(凭证) */
	fork_init(); /* 初始化一些结构体以使用 fork 函数 */
	proc_caches_init(); /* 给各种资源管理结构分配缓存 */
	buffer_init(); /* 初始化缓冲缓存 */
	key_init(); /* 初始化密钥 */
	security_init(); /* 安全相关初始化 */
	vfs_caches_init(totalram_pages); /* 为 VFS 创建缓存 */
	signals_init(); /* 初始化信号 */
	page_writeback_init(); /* 页回写初始化 */
	proc_root_init(); /* 注册并挂载 proc 文件系统 */
	cpuset_init(); /* 初始化 cpuset，cpuset 是将 CPU 和内存资源以逻辑性和层次性集成的一种机制，是 cgroup 使用的子系统之一*/
	cgroup_init(); /* 初始化 cgroup */
	taskstats_init_early(); /* 进程状态初始化 */
	check_bugs(); /* 检查写缓冲一致性 */
	rest_init(); /* rest_init 函数 */
