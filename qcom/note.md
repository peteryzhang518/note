# 上报给app的size
camx/src/core/camxhwenvironment.cpp    :    SetAvailableStreamConfig
上报能力的位置camx/src/hwl/titan17x/camxtitan17xcontext.cpp

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
