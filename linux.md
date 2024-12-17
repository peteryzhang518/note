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
