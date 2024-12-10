    void __init setup_arch(char **cmdline_p)  
    {  
        /*内核通过machine_desc结构体来控制系统体系架构相关部分的初始化。 
        machine_desc结构体通过MACHINE_START宏来初始化，在代码中， 
        通过在start_kernel->setup_arch中调用setup_machine_fdt来获取。*/  
        struct machine_desc *mdesc;  
        /*首先从arm寄存器里面取得cpu ID，然后调用lookup_processor_type来取得proc_info_list这个结构体。 
        取得proc_info_list以后，将里面的内容一个个赋值给相应的全局变量， 
        然后将CPU的信息打印出来。然后它会从arm寄存器里面获得cache的信息， 
        并将cache的信息打印出来（CPU: ARMv7 Processor [410fc051] revision 1 (ARMv7), cr=10c53c7d   
        CPU: PIPT / VIPT nonaliasing data cache, VIPT aliasing instruction cache）。*/  
        setup_processor();  
        /*machine_desc结构体通过MACHINE_START宏来初始化，在代码中，调用setup_machine_fdt来获取。 
        对于samad5，它通过在arch/arm/mach-at91/board_dt.c中的DT_MACHINE_START宏初始化 
        输出CPU型号Machine: Atmel SAMA5 (Device Tree), model: Atmel SAMA5D34-EK*/  
        mdesc = setup_machine_fdt(__atags_pointer);  
        if (!mdesc)  
            mdesc = setup_machine_tags(machine_arch_type);  
        machine_desc = mdesc;  
        machine_name = mdesc->name;  
      
        setup_dma_zone(mdesc);  
      
        //通过struct machine_desc 中的soft_reboot数据来设置重启类型：  
        if (mdesc->restart_mode)  
            reboot_setup(&mdesc->restart_mode);  
          
        /*这里通过连接脚本中得到的Linux代码位置数据来初始化一个mm_struct结构体init_mm中的部分数据 
        ps：每一个任务都有一个mm_struct结构以管理内存空间，init_mm是内核自身的mm_struct 
        */  
        init_mm.start_code = (unsigned long) _text;  
        init_mm.end_code   = (unsigned long) _etext;  
        init_mm.end_data   = (unsigned long) _edata;  
        init_mm.brk    = (unsigned long) _end;  
      
        /* 同时填充cmd_line以备后用, 保护boot_command_line数据 populate cmd_line too for later use, preserving boot_command_line */  
        strlcpy(cmd_line, boot_command_line, COMMAND_LINE_SIZE);  
        *cmdline_p = cmd_line;  
        /* 
        处理在 struct obs_kernel_param 中定义为early的启动参数（主要是内存配置部分的参数） 
             
        其中就分析了mem=size@start参数初始化了struct meminfo meminfo; 
        同时如果有vmalloc=size参数也会初始化 vmalloc_min 
        参考：《Linux内核高-低端内存设置代码跟踪（ARM构架）》 
             
        这里需要注意的是内核的cmdline中的参数按照其被需要的先后，分为early和非early的。 
        include/linux/init.h： 
         
        struct obs_kernel_param { 
        const char *str;              //在cmdline中相应参数名。 
        int (*setup_func)(char *);  //对于此参数的专用处理函数 
        int early;                //是否为早期需要处理的参数 
        }; 
         
        两种不同的参数在内核中用了不同的宏来定义： 
        early： #define early_param(str, fn) \ 
               __setup_param(str, fn, fn, 1) 
         
        非early： #define __setup(str, fn) \ 
                __setup_param(str, fn, fn, 0) 
         
        使用这两个宏定义的参数和构架相关，一些构架或者板子可以定义自己特定的参数和处理函数。对于比较重要的“men”参数就是early参数。 
        */    
        parse_early_param();  
      
        sort(&meminfo.bank, meminfo.nr_banks, sizeof(meminfo.bank[0]), meminfo_cmp, NULL);  
        sanity_check_meminfo();//在此处设置struct meminfo meminfo中每个bank中的highmem变量2.通过vmalloc_min确定每个bank中的内存是否属于高端内存  
        arm_memblock_init(&meminfo, mdesc);//按地址数据从小到大排序meminfo中的数据，并初始化全局的memblock数据。  
      
        /* 
        设置内核的参考页表。 
        此页表不仅用于物理内存映射，还用于管理vmalloc区。 
        此函数中非常重要的一点就是初始化了bootmem分配器！ 
        */  
        paging_init(mdesc);  
        //通过获取设备描述结构体（struct machine_desc）中的数据和编译时产生的地址数据，初始化内存相关的全局结构体变量。  
        request_standard_resources(mdesc);  
      
        if (mdesc->restart)  
            arm_pm_restart = mdesc->restart;  
        //通过启动参数中的“非平坦设备树”信息(如果有),获取内存相关信息  
        unflatten_device_tree();  
      
        /*针对SMP处理器，初始化可能存在的CPU映射 - 这描述了可能存在的CPU*/  
    #ifdef CONFIG_SMP  
        if (is_smp())  
            smp_init_cpus();  
    #endif  
        /*1.用于内核崩溃时的保留内核此功能通过内核command line参数中的”crashkernel=”保留下内存用于主内核崩溃时获取内核信息的导出*/  
        reserve_crashkernel();  
        /* 
        初始化ARM内部的TCM（紧耦合内存）。 
        参考资料：《对ARM紧致内存TCM的理解》 
        */  tcm_init();  
      
    #ifdef CONFIG_MULTI_IRQ_HANDLER  
        handle_arch_irq = mdesc->handle_irq;  
    #endif  
      
    #ifdef CONFIG_VT  
    #if defined(CONFIG_VGA_CONSOLE)  
        conswitchp = &vga_con;  
    #elif defined(CONFIG_DUMMY_CONSOLE)  
        conswitchp = &dummy_con;  
    #endif  
    #endif  
        /* 
        1.如果设备描述结构体定义了init_early函数（应该是早期初始化之意），则在这里调用。 
        */  if (mdesc->init_early)  
            mdesc->init_early();  
    }  
