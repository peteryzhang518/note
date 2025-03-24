# 上报给app的size
camx/src/core/camxhwenvironment.cpp    :    SetAvailableStreamConfig
上报能力的位置camx/src/hwl/titan17x/camxtitan17xcontext.cpp

? camx/src/core/camxstaticcaps.cpp 



# 选择camerainfo所用的xml
这里配置了所谓的一个逻辑相机使用几个物理相机，以及物理相机的性质，什么焦距使用那颗相机
chimcxcameraconfig.cpp:180 PickMatchingCameraXML() Building LogicalCameraInfo from kailua_supersetdc.xml based on override setting

chxlogicalcameras.cpp:513 EnforceCameraIdInvariants() Cam.(frameworkId 1, cameraId = 0, xmlCameraId = 1, fold 0, ppDeviceInfo[0].(cameraId = 0, frameworkId = 1, sensorId = 1, logicalCamId 0, isPrimary = 1))
chxlogicalcameras.cpp:513 EnforceCameraIdInvariants() Cam.(frameworkId 2, cameraId = 1, xmlCameraId = 2, fold 0, ppDeviceInfo[0].(cameraId = 1, frameworkId = 2, sensorId = 2, logicalCamId 1, isPrimary = 1))
chxlogicalcameras.cpp:513 EnforceCameraIdInvariants() Cam.(frameworkId 3, cameraId = 2, xmlCameraId = 3, fold 0, ppDeviceInfo[0].(cameraId = 2, frameworkId = 3, sensorId = 3, logicalCamId 2, isPrimary = 1))
chxlogicalcameras.cpp:513 EnforceCameraIdInvariants() Cam.(frameworkId 0, cameraId = 3, xmlCameraId = 0, fold 0, ppDeviceInfo[0].(cameraId = 3, frameworkId = 0, sensorId = 0, logicalCamId 3, isPrimary = 1))
chxlogicalcameras.cpp:513 EnforceCameraIdInvariants() Cam.(frameworkId 4, cameraId = 4, xmlCameraId = 4, fold 0, ppDeviceInfo[0].(cameraId = 1, frameworkId = 2, sensorId = 2, logicalCamId 1, isPrimary = 1))
chxlogicalcameras.cpp:513 EnforceCameraIdInvariants() Cam.(frameworkId 4, cameraId = 4, xmlCameraId = 4, fold 0, ppDeviceInfo[1].(cameraId = 0, frameworkId = 1, sensorId = 1, logicalCamId 0, isPrimary = 0))
chxlogicalcameras.cpp:513 EnforceCameraIdInvariants() Cam.(frameworkId 4, cameraId = 4, xmlCameraId = 4, fold 0, ppDeviceInfo[2].(cameraId = 2, frameworkId = 3, sensorId = 3, logicalCamId 2, isPrimary = 0))




# link feature 口
grep -inE "MCX_REALTIME.*MC_REALTIME_REPROCESS.*is saved"
ZZ
# camx chi 的链接
camx chi 的链接再chiexternmode 到camxchicontext这两个文件中

# 编译逻辑camera
 chiutilsheaderlib_32 <= vendor/qcom/proprietary/chi-cdk/tools/logicalcameragenerator/logicalcamcompiler.py

# 在feature2base中，会把每条，pipeline传输到camx中，为每一条pipeline，创建个session

# pipelne xml
pipeline 的xml转换成一个.h文件是通过一个perl脚本来转换的
usecaseconverter.pl脚本
g_pipeline.h

# chxusecaseutils.cpp
会根据num_streams 选择对应的useCase
然后在advancecameraUsecase中调用DefaultMatchUsecase


# camx中关于logic从xml中解析完成之后的枚举和创建logical camera
camx/src/core/camxchilogicalcamera.cpp: 
chxexternalmodule.cpp:  getNumCameras
enumeratGeneratedCameras
enforceCameraIdInvariants



# 每个stream 都会分配e一个物理camera id
Update logical streams to physical streams
For single camera, all streams will be updated to physical streams and processed in the MCX use
case



# dumpdebuginfo 中看卡在那里的简单方法
8567:03-10 09:59:33:406604864  13788  13989 I CamX: [ INFO][CORE   ] camxpipeline.cpp:8032 LogPendingNodes() +        Request 1 Nodes: com.qti.node.gme0 IFE0 IPE0 IPE19 EVA0 FDManager0 
8568:03-10 09:59:33:406624707  13788  13989 I CamX: [ INFO][CORE   ] camxpipeline.cpp:8032 LogPendingNodes() +        Request 2 Nodes: com.qti.node.gme0 IFE0 IPE0 IPE19 EVA0 FDManager0


# 关键字
HIUSECASE: [CORE_CFG ] chimcxcameraconfig.cpp:180 PickMatchingCameraXML() Building LogicalCameraInfo from kailua_supersetdc.xml based on override setting

chxusecaseutils.cpp:2071 GetMatchingUsecase() Default usecase selected

2582:03-10 09:20:47:187812470  1274  1332 I CHIUSECASE: [CORE_CFG ] chifeature2featurepool.cpp:452 CreateFeatureInstances() :VirtualSuperGraph, numFeatureInstances:7, logicalCameraId:0

2593:03-10 09:20:47:189566428  1274  1332 I CHIUSECASE: [CORE_CFG ] chifeature2featurepool.cpp:991 FinalizeFeatureInstances() Yuv2Yuv_0: Backward walk...

chxpipeline.cpp:375 CreateDescriptor() Pipeline[RealTimeFeatureZSLPreviewRaw] 

# fence crash 关键字
 camxsession.cpp:11341 HandleSmartErrorCodeCreation() Smart error code : 1075456, Verbose error string : IFE node has 2 unsignaled fences for requestID 1 , nodeReqStatus : SUBMIT 
11861:03-10 09:20:48:803693094  1274  5513 I CamX: [CORE_CFG][CORE   ] camxsession.cpp:7002 NotifyChiRecovery() CHI triggered PCR timeout recovery: ErrorCode:1075456, ErrorString: IFEFenceUnsignaled, ErrorDescription: IFE node has 2 unsignaled fences for requestID 1 , nodeReqStatus : SUBMIT 

38905:03-10 14:15:05.706 17046 17360 I CamX    : [ DUMP][CORE   ] camxsession.cpp:10962 DumpKMDInfo() Session: 0xb40000762e2bca20 pipeline: RealTimeFeatureZSLPreviewRaw_0_cam_0 firstPendingReqId: 1 firstPendingCSLSyncId: 1 lastSubmittedRequestId: 5 lastCSLSyncId: 5


# build id
Pakala.LA.1.0/contents.xml
Pakala.LA.1.0/about.html

# kmd camera driver log
adb shell "echo 1 > /sys/module/camx/parameters/debug_level"
adb shell "echo 1 > /sys/module/camera/parameters/debug_mdl"
 drivers/cam_utils/cam_debug_util.h

# formate 33 
8550/LA.VENDOR.13.2.8/LINUX/android/system$ vim core/libsystem/include/system/graphics-base-v1.0.h
21:    HAL_PIXEL_FORMAT_RGBA_FP16 = 22,
22:    HAL_PIXEL_FORMAT_RAW16 = 32,
23:    HAL_PIXEL_FORMAT_BLOB = 33,
24:    HAL_PIXEL_FORMAT_IMPLEMENTATION_DEFINED = 34,
25:    HAL_PIXEL_FORMAT_YCBCR_420_888 = 35,
26:    HAL_PIXEL_FORMAT_RAW_OPAQUE = 36,
27:    HAL_PIXEL_FORMAT_RAW10 = 37,


# umdcamera uvc camera
LINUX/android/vendor/qcom/opensource/iot-media/umd-daemon/daemon/umd-camera.cpp
LINUX/android/kernel_platform/common/drivers/usb/gadget/function/f_uvc.c
LINUX/android/kernel_platform/msm-kernel/drivers/usb/gadget/function/f_uvc.c

