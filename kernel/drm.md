 DRM 的主要驱动模型
a. 驱动类型：

DRM 框架下的驱动通常分为两类：

    通用驱动：例如 fbdev 的兼容实现。
    硬件特定驱动：如 Intel i915、AMD amdgpu、高通 msm 驱动。

b. 驱动注册机制：

    DRM 框架提供了通用的驱动注册接口（drm_dev_register）。
    厂商的驱动会实现硬件特定的 probe 函数，在设备树或总线发现对应硬件时加载驱动。

c. Mode Setting 例子：

当用户空间请求设置显示模式时：

    用户空间调用 DRM 的 Mode Setting 接口。
    DRM 框架通过厂商驱动提供的接口，将分辨率和刷新率等参数设置到硬件中。
    厂商驱动执行寄存器操作，完成模式切换。



# 总结理解
drm是一个linux内核子系统框架，上层应用使用这个接口去调用显示相关的内容和设备，本质上相当于内核的unix
的api，所以下面还需要定义给厂商平台的接口用来给上层app或者进程统一调用

drm 的主要分为几个部分
1.  驱动需要实现的核心回调函数
```
load()
    功能: 初始化驱动，设置硬件状态。
    绑定位置: 注册到 struct drm_driver 的 load 字段。
unload()
    功能: 卸载驱动，释放资源。
    绑定位置: 注册到 struct drm_driver 的 unload 字段。
open()
    功能: 打开设备文件时被调用，设置用户上下文。
    绑定位置: struct drm_driver 的 open 字段。
release()
    功能: 释放设备文件，清理用户上下文。
    绑定位置: struct drm_driver 的 release 字段。
ioctl()
    功能: 用户空间通过 ioctl 调用驱动的接口。
    绑定位置: struct drm_driver 的 ioctl 字段。
mode_valid()
    功能: 验证显示模式是否支持。
    绑定位置: struct drm_mode_config_funcs 的 mode_valid 字段。
fb_create()
```
2. 显示回调

这些回调由驱动提供，用于实现显示硬件的具体功能。

    encoder_init()
        功能: 初始化显示编码器。

    connector_init()
        功能: 初始化显示连接器。

    crtc_helper_funcs
        功能: 设置、清空和控制显示控制器 (CRTC)。
        结构: 驱动实现 struct drm_crtc_helper_funcs。

    plane_helper_funcs
        功能: 管理显示平面（primary、cursor）。
        结构: 驱动实现 struct drm_plane_helper_funcs。

3. 显存回调
    gem_init_object()
        功能: 初始化一个 GEM 对象。
        绑定位置: struct drm_driver 的 gem_init_object 字段。

    dumb_create()
        功能: 创建一个 Dumb Buffer（简单缓冲区）。
        绑定位置: struct drm_driver 的 dumb_create 字段。

    prime_handle_to_fd()
        功能: 将 GEM 对象的句柄转为文件描述符。
        绑定位置: struct drm_driver 的 prime_handle_to_fd 字段。

    prime_fd_to_handle()
        功能: 将文件描述符转为 GEM 对象的句柄。
        绑定位置: struct drm_driver 的 prime_fd_to_handle 字段。



驱动需要调用的接口函数
1. 设备管理
    drm_dev_alloc()
        分配并初始化一个 DRM 设备结构。
    drm_dev_register()
        注册 DRM 设备，使其可被用户空间访问。
    drm_dev_unplug()
        卸载 DRM 设备，通常在设备被移除时使用。
    drm_irq_install()
        安装中断处理程序。
    drm_irq_uninstall()
        卸载中断处理程序。
    drm_kms_helper_poll_init()
        初始化 KMS（Kernel Mode Setting）设备的热插拔轮询。
    drm_connector_attach_encoder()
        将编码器与连接器关联。
2. 以下是 DRM（Direct Rendering Manager） 框架为驱动提供的接口，分为 驱动需要调用的接口 和 驱动需要实现的回调函数。这些接口使得驱动能够实现与硬件的交互，同时遵循 DRM 框架的标准化流程。
1. 驱动需要实现的回调函数

这些函数由驱动开发者提供，实现与硬件相关的逻辑。它们通常注册到 DRM Core 的相关结构体中。
核心回调

    load()
        功能: 初始化驱动，设置硬件状态。
        绑定位置: 注册到 struct drm_driver 的 load 字段。

    unload()
        功能: 卸载驱动，释放资源。
        绑定位置: 注册到 struct drm_driver 的 unload 字段。

    open()
        功能: 打开设备文件时被调用，设置用户上下文。
        绑定位置: struct drm_driver 的 open 字段。

    release()
        功能: 释放设备文件，清理用户上下文。
        绑定位置: struct drm_driver 的 release 字段。

    ioctl()
        功能: 用户空间通过 ioctl 调用驱动的接口。
        绑定位置: struct drm_driver 的 ioctl 字段。

    mode_valid()
        功能: 验证显示模式是否支持。
        绑定位置: struct drm_mode_config_funcs 的 mode_valid 字段。

    fb_create()
        功能: 创建帧缓冲对象。
        绑定位置: struct drm_mode_config_funcs 的 fb_create 字段。

显示回调

这些回调由驱动提供，用于实现显示硬件的具体功能。

    encoder_init()
        功能: 初始化显示编码器。

    connector_init()
        功能: 初始化显示连接器。

    crtc_helper_funcs
        功能: 设置、清空和控制显示控制器 (CRTC)。
        结构: 驱动实现 struct drm_crtc_helper_funcs。

    plane_helper_funcs
        功能: 管理显示平面（primary、cursor）。
        结构: 驱动实现 struct drm_plane_helper_funcs。

显存回调

    gem_init_object()
        功能: 初始化一个 GEM 对象。
        绑定位置: struct drm_driver 的 gem_init_object 字段。

    dumb_create()
        功能: 创建一个 Dumb Buffer（简单缓冲区）。
        绑定位置: struct drm_driver 的 dumb_create 字段。

    prime_handle_to_fd()
        功能: 将 GEM 对象的句柄转为文件描述符。
        绑定位置: struct drm_driver 的 prime_handle_to_fd 字段。

    prime_fd_to_handle()
        功能: 将文件描述符转为 GEM 对象的句柄。
        绑定位置: struct drm_driver 的 prime_fd_to_handle 字段。

2. 驱动需要调用的接口函数

这些接口由 DRM Core 提供，驱动通过调用它们完成各种操作。
设备管理

    drm_dev_alloc()
        分配并初始化一个 DRM 设备结构。
    drm_dev_register()
        注册 DRM 设备，使其可被用户空间访问。
    drm_dev_unplug()
        卸载 DRM 设备，通常在设备被移除时使用。
    drm_irq_install()
        安装中断处理程序。
    drm_irq_uninstall()
        卸载中断处理程序。
    drm_kms_helper_poll_init()
        初始化 KMS（Kernel Mode Setting）设备的热插拔轮询。
    drm_connector_attach_encoder()
        将编码器与连接器关联。

2. 显存管理
    drm_gem_object_alloc()
        分配 GEM 对象，用于显存管理。
    drm_gem_object_put()
        释放 GEM 对象的引用。
    drm_mode_create_blob()
        创建显示模式或属性的二进制对象。
    drm_fbdev_generic_setup()
        初始化帧缓冲设备（用于与 fbdev 兼容）。

3. 显示管理
    drm_crtc_helper_set_config()
        配置 CRTC（显示控制器）。
    drm_plane_init()
        初始化一个显示平面。
    drm_encoder_init()
        初始化一个显示编码器。
    drm_connector_init()
        初始化一个显示连接器


sde 就是高通的驱动，使用module
init，然后在dtsi解析完成，调用到驱动函数的时候使用sde进程注册drm，实现drm，然后初始化硬件和显示设备等等
