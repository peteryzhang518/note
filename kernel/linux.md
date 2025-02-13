#
- 如何安装软件的patch

patch -p1 < xxx.patch

# 如何在代码里面写一个进程并且书写编译文件到生成的系统指定目录
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := boe-ai-director-service


code souce
include source
.....

config ai_director_service.rc  vendor/etc/init
LOCAL_INIT_RC := boe_ai_director_service.rc
LOCAL_CPPFLAGS := -std=c++17
LOCAL_LDLIBS := -llog -lcutils -ldmabufheap



可以在代码的任何位置创建一个目录，只需要在其编译mk文件中写明其生成位置
$(shell mkdir -p $(TARGET_OUT_VENDOR)/boe/weights)
include $(CLEAR_VARS)
LOCAL_MODULE       := xxxxxxx.dlc
LOCAL_MODULE_TAGS   := optional
LOCAL_MODULE_CLASS  := ETC
LOCAL_SRC_FILES     := res/weights/xxxxxxx.dlc
LOCAL_MODULE_PATH   := $(TARGET_OUT_VENDOR)/xxxxxx/xxxxx //此处为代码在编译成系统的生成位置
include $(BUILD_PREBUILT)


#to run:
#export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/system/apex/com.android.i18n/lib64/



#现有的库copy 到 TARGET_COPY_OUT_VENDOR：out/target/product/kalama/vendor
#mkdir 
$(shell mkdir -p $(TARGET_OUT_VENDOR)/boe/weights)
$(shell mkdir -p $(TARGET_OUT_VENDOR)/boe/adsplib)
$(shell mkdir -p $(TARGET_OUT_VENDOR)/boe/lib)
PRODUCT_COPY_FILES += \
    vendor/boe/ai-director-service/res/weights/yolov8_head_1280_736.dlc:$(TARGET_COPY_OUT_VENDOR)/boe/weights/yolov8_head_1280_736.dlc \

PRODUCT_PACKAGES += boe-ai-director-service
PRODUCT_PACKAGES += boe-ai-director-service.rc

# 书写一段启动这个进程的脚本或者服务
service
#定义服务
service boe-ai-director-service /vendor/bin/boe-ai-director-service
    class main
    user root
    group root
    seclabel u:r:boe-ai-director-service:s0
	# 设置环境变量作为服务的一部分
    setenv  LD_LIBRARY_PATH /vendor/boe/lib/:/vendor/lib64/
    setenv  ADSP_LIBRARY_PATH /vendor/boe/adsplib/:/system/vendor/lib/rfsa/adsp
	
on property:sys.boot_completed=1
    # 启动 vndservice
    start boe-ai-director-service


# 把写好的sh或者service编译到指定位置
如/vendor/init/
PRODUCT_COPY_FILES += \
vendor/boe/ai-director-service/boe_vendor_product.mk
	vendor/boe/ai-director-service/boe_ai_director_service.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/boe_ai_director_service.rc \

PRODUCT_PACKAGES += boe-ai-director-service
PRODUCT_PACKAGES += boe-ai-director-service.rc


# 如果是高通的文件或者android文件需要把上面的mk文件链接到指定位置，让其编译
vendor/boe/ai-director-service/boe_vendor_product.mk::vendor/qcom/defs/product-defs/vendor/boe_vendor_product.mk
vendor/boe/ai-director-service/boe-vendor-board.mk::vendor/qcom/defs/product-defs/vendor/boe-vendor-board.mk


# 然后配置系统rc脚本，把编译到系统位置的rc文件加上
system/core/rootdir/init.rc
13:import /vendor/etc/init/boe_ai_director_service.rc



# 编译完成的文件是ai_director_service.rc


# 最后可以用脚本调试是否成功
设置环境变量
export LD_LIBRARY_PATH=/vendor/boe/lib/:/vendor/lib64/
export ADSP_LIBRARY_PATH="/vendor/boe/adsplib/;/system/vendor/lib/rfsa/adsp"
mkdir -p /data/vendor/camera/
cd /data/vendor/camera/
killall boe-ai-director-service
sleep 2
nohup /vendor/bin/boe-ai-director-service > /data/vendor/camera/boeservice.log &


# 进程在系统中已经有了编译位置自启动权限之后
进程需要主动去调用或者访问其他的文件和进程，这个时候需要使用linux自定义的selinux系统





# selinux子模块
SELinux 判断流程
步骤 1: 检查请求主体和目标的上下文
    用户执行命令时，shell 的上下文是 u1_u:user_r:user_t:s0。
    用户启动进程后，进程的上下文被转换为 system_u:system_r:boe_ai_service_t:s0（因为进程执行文件的上下文被 SELinux 策略定义为 boe_ai_service_t 类型）。
    被访问的目标文件 /data/vendor/camera/boe.log 的上下文是 system_u:object_r:boe_ai_data_t:s0。


步骤 2: 加载 SELinux 策略
SELinux 加载已编译的策略规则，并在策略数据库中查找是否有允许规则。


步骤 3: 匹配策略规则
    主体：boe-ai-director-service 的上下文是 system_u:system_r:boe_ai_service_t:s0。
    目标：文件 /data/vendor/camera/boe.log 的上下文是 system_u:object_r:boe_ai_data_t:s0。
    动作：read 和 open。
    策略匹配：
        策略规则1 匹配到 allow boe_ai_service_t boe_ai_data_t:file { read open getattr };，允许访问。
        即：进程可以读取文件。


上下文（Context）
1. 定义
上下文是 SELinux 中对象的安全标识，描述了进程、文件或资源的安全属性。它决定了对象可以如何与其他对象交互。上下文包括以下部分：

    SELinux 用户（User）：如 system_u、unconfined_u。
    角色（Role）：如 object_r、system_r。
    类型（Type）：如 httpd_t、httpd_sys_content_t，主要用于定义访问规则。
    级别（Sensitivity/Category）：如 s0 或 s0:c0.c1023，用于多级安全模型（MLS）

规则
2.  策略是 SELinux 的核心规则集合，规定了哪些上下文之间的操作是被允许或禁止的。策略控制了系统中所有对象之间的交互，包括：

    哪些类型的进程可以访问哪些类型的文件。
    哪些网络端口可以被监听。
    哪些服务可以启动。



# 配置selinux
1. 创建新类型
type boe_ai_service, domain;       # 创建一个新的类型 boe_ai_service，并且它属于域（进程）
type boe_ai_service_exec, exec_type; # 创建一个用于执行文件的类型 boe_ai_service_exec

2. 创建新规则
为 boe_ai_service 类型的进程定义规则
allow boe_ai_service boe_ai_service_exec:file { getattr open read execute };

3. 定义
定义 boe_ai_service 类型的进程可以访问的资源
allow boe_ai_service self:process { sigchld };


# 编译进程的时候需要修改mk文件中的initrc文件
编译进程的时候需要修改mk文件中的initrc文件,让init进程或者服务的时候，以自定义的selinux type 运行进程，以便修改进程上下文

# Board specific SELinux policy variable definitions
定义完成文件后，需要把他放在相对系统编译的地方，编译到系统中，或者新建个mk文件，把自定义文件编译到系统中
device/qcom/sepolicy_vndr
ifeq ($(call is-vendor-board-platform,QCOM),true)
SEPOLICY_PATH:= device/qcom/sepolicy_vndr
QSSI_SEPOLICY_PATH:= device/qcom/sepolicy
BOARD_SYSTEM_EXT_PREBUILT_DIR := device/qcom/sepolicy/generic
BOARD_PRODUCT_PREBUILT_DIR := device/qcom/sepolicy/generic/product
SYS_ATTR_PROJECT_PATH := $(TOP)/device/qcom/sepolicy/generic/public/attribute
SYSTEM_EXT_PUBLIC_SEPOLICY_DIRS:= \
    $(SYSTEM_EXT_PUBLIC_SEPOLICY_DIRS) \
    $(QSSI_SEPOLICY_PATH)/generic/public \
    $(QSSI_SEPOLICY_PATH)/generic/public/attribute

    ifeq ($(TARGET_SEPOLICY_DIR),)
      BOARD_SEPOLICY_DIRS += $(SEPOLICY_PATH)/generic/vendor/$(TARGET_BOARD_PLATFORM)
      BOARD_SEPOLICY_DIRS += $(SEPOLICY_PATH)/qva/vendor/$(TARGET_BOARD_PLATFORM)
    else
      BOARD_SEPOLICY_DIRS += $(SEPOLICY_PATH)/generic/vendor/$(TARGET_SEPOLICY_DIR)
      BOARD_SEPOLICY_DIRS += $(SEPOLICY_PATH)/qva/vendor/$(TARGET_SEPOLICY_DIR)
    endif
