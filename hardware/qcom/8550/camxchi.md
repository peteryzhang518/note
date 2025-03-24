为什么有camxhal 首先是因为linux开源协议，很多厂商不想开源，所以在android中抽象出来一层,本质是驱动层的补充
既然，是驱动层，所以有硬件，包括ipe ife这些isp硬件等,同时因为高通芯片基本上面向android系统来设计,虽然可以用其他系统来调用，但比较少，所以我们在camera上需要使用高通架构，也叫做基线,也就是camx|chi结构


技术分享从源头开始，数据，地址
先铺垫下基础数据结构
然后讲下高通相机基本框架
放到camx，chi中就是数据结构   把控制数据地址的代码，也就是函数
所以camx和chi就是这样的，imagebuffer, metadata,  and  usecase feature2Graph feature2 pipeline session node

camx和chi的关系是camx是基本框架和基本操作等如操作硬件，基础数据结构等等，chi是可自定义框架,新建一些node，chi等
树形接口数据流通树型结构后依次向上传递回到app 

那这些内容有什么种类呢
usecase 种类base advanceusecase multicusecase，feature2种类，zsl, realtime等
pipeline  很多各种自定义pipelinee，realtime ， reprocess，sat，等。 node
有硬件node 也有软件node 如ife图像裁减降噪格式转换等，一般挂载到realtime pipeline上面
ipe 高级的图像处理颜色矫正，阴影，畸变，降噪美化,亮度
gpu 裁减，重复计算
awb ae af node
fd 人脸识别
eva运动补偿，滤波，降噪提升信噪比
gme运动估计算法等
eis防抖


如何出图，点相机按钮，屏幕上显示
很好对上层（用户，输入点击：输出：图像）
可能点击很多次出一个图像

那数据在流通这些结构时具体的流程
首先来说，这个是个驱动，系统库，所以在手机启动的时候就会进行一些初始化的操作
app2接口
输入:
输出:
启动  从app角度 open ， configure ， request， result   。   flush  recovery等




service会创建session，device等
hal3.接口
调用接口即继承一个hal3类，然后类里面有一些函数，即可以通过这些函数调用到camx hal3
本质的结构是camera_module_t


枚举相机，几个能提供什么size，提供什么帧率
camx和chi进行初始化也就是provider等
camx 的哪个函数emulate再做这种函数
创建externmodule，chiops等函数接口提供对方使用
1. 获取所有硬件信息，ipe sensor cameraid 
2. camera会寻找so库
3. 与chi会加载camx camx会加载chi的库
Environment构造方法中会调用其Initialize方法，在该方法中实例化了CSLModeManager对象，并通过CSLModeManager提供的接口，获取了所有底层支持的硬件设备信息，其中包括了Camera Request Manager、CAPS模块（该驱动模块主要用于CSL获取Camera平台驱动信息，以及IPE/BPS模块的电源控制）以及Sensor/IPE/Flash等硬件模块，并且通过调用CSLHwInternalProbeSensorHW方法获取了当前设备安装的Sensor模组信息，并且将获取的信息暂存起来，等待后续阶段使用，总得来说在HwEnvironment初始化的过程中,通过探测方法获取了所有底层的硬件驱动模块，并将其信息存储下来供后续阶段使用。
之后通过调用HwEnvironment对象中的ProbeChiCompoents方法在/vendor/lib64/camera/components路径下找寻各个Node生成的so库，并获取Node提供的标准对外接口，这些Node不但包括CHI部分用户自定义的模块，还包括了CamX部分实现的硬件模块，并最后都将其都存入ExternalComponentInfo对象中，等待后续阶段使用。
同样地，CHI中的ExtensionModule在初始化的时候，其构造方法中也会通过调用dlopen方法加载camera.qcom.so库，并将其入口方法ChiEntry通过dlsym映射出来，之后调用该方法，将g_chiContextOps（ChiContextOps，该结构体中定义了很多指针函数）作为参数传入CamX中，一旦进入CamX中，便会将本地的操作方法地址依次赋值给g_chiContextOps中的每一个函数指针，这样CHI之后就可以通过g_chiContextOps访问到CamX方法。



opencamera
open 的时候打开了相机，
上文提到severice主要是继承一个类，然后调用这个类的函数，那hal层所作的内容在open的时候创建一个hal3device，把这个devices返回给service去调用
然后get session





request
usecase ,execute request
acttived pipeline
pipeline streamon

usecase submit request
pipeline request
node request
drq


drp


result
fenceCallBack
notifyResult


一旦底层有事件上传就会走到SyncManagerPollMethod中：
file: vendor/qcom/proprietary/camx/src/csl/hw/camxsyncmanager.cpp
--> VOID* SyncManager::SyncManagerPollMethod(VOID* pPollData)
   |--> rc = poll(fds, 2, -1)   //监听的syncFd有事件上传
   |   |--> VOID* pData[] = {pEv, NULL};    //这里的pEv是包含回调方法的，根据setRepeatingRequest中的分析，这里的回调就是Node::CSLFenceCallback
   |   |--> ioctl(pCtrl->syncFd, VIDIOC_DQEVENT, pEv);  //取出事件
   |   |--> result = pCtrl->pThreadManager->PostJob(pCtrl->hJob, SyncManager::StoppedCbDispatchJob, pData, FALSE, FALSE);   //这里就会调到之前注册的线程并回调SyncManager::CbDispatchJob方法
       file: vendor/qcom/proprietary/camx/src/csl/hw/camxsyncmanager.cpp
   |   |--> VOID* SyncManager::CbDispatchJob(VOID* pData)
   |   |   |--> Utils::Memcpy(&ev, reinterpret_cast<struct v4l2_event*> (pData), sizeof(ev)); 
   |   |   |--> pPayloadData = CAM_SYNC_GET_PAYLOAD_PTR(ev, uint64_t);
   |   |   |--> reinterpret_cast<CSLFenceHandler>(pPayloadData[0]))(reinterpret_cast<VOID* >(pPayloadData[1]), pEvHeader->sync_obj, fenceResult);   //回调Node::CSLFenceCallback
           file: vendor/qcom/proprietary/camx/src/core/camxnode.cpp
   |   |   |--> VOID Node::CSLFenceCallback(...)
   |   |   |   |--> result = pNode->GetThreadManager()->PostJob(pNode->GetJobFamilyHandle(), NULL, &pData[0], FALSE, FALSE) //将工作放到JobFamilyHandle线程去做,回调Node::NodeThreadJobFamilyCb方法
   |   |   |   |   |--> VOID* Node::NodeThreadJobFamilyCb(...)  //因为是另一个线程处理，所以这个以缩进代表异步关系
   |   |   |   |   |   |--> FenceCallbackData* pFenceCallbackData = static_cast<FenceCallbackData*>(pCbData);  
   |   |   |   |   |   |--> NodeFenceHandlerData* pNodeFenceHandlerData = static_cast<NodeFenceHandlerData*>(pFenceCallbackData->pNodePrivateData);
   |   |   |   |   |   |--> pFenceCallbackData->pNode->ProcessFenceCallback(pNodeFenceHandlerData);
   |   |   |   |   |   |   |--> OutputPort* pOutputPort    = pFenceHandlerData->pOutputPort;
   |   |   |   |   |   |   |--> UINT64      requestId      = pFenceHandlerData->requestId;
   |   |   |   |   |   |   |--> UINT        requestIdIndex = requestId % MaxRequestQueueDepth;
   |   |   |   |   |   |   |--> m_pPipeline->NonSinkPortFenceSignaled(&pFenceHandlerData->hFence, pFenceHandlerData->requestId);    //如果是no sink port
                           file: vendor/qcom/proprietary/camx/src/core/camxpipeline.cpp
   |   |   |   |   |   |   |--> VOID Pipeline::NonSinkPortFenceSignaled(...)
   |   |   |   |   |   |   |   |--> m_pDeferredRequestQueue->FenceSignaledCallback(phFence, requestId);
                               file: vendor/qcom/proprietary/camx/src/core/camxdeferredrequestqueue.cpp
   |   |   |   |   |   |   |   |--> VOID DeferredRequestQueue::FenceSignaledCallback(...)
   |   |   |   |   |   |   |   |   |--> UpdateDependency(PropertyIDInvalid, phFence, NULL, requestId, 0, TRUE);
   |   |   |   |   |   |   |   |   |--> DispatchReadyNodes();
   |   |   |   |   |   |   |   |   |   |--> CamxResult result = m_pThreadManager->PostJob(m_hDeferredWorker, NULL, &pData[0], FALSE, FALSE);    //针对所有ready Nodes 循环处理,m_hDefferredWorker 对应的回调是DeferredWorkerWrapper
   |   |   |   |   |   |   |   |   |   |   |--> VOID* DeferredRequestQueue::DeferredWorkerWrapper(VOID* pData)  //异步调用,以缩进表示
   |   |   |   |   |   |   |   |   |   |   |   |--> Dependency* pDependency = reinterpret_cast<Dependency*>(pData);
   |   |   |   |   |   |   |   |   |   |   |   |--> DeferredRequestQueue* pDeferredQueue = pDependency->pInstance;
   |   |   |   |   |   |   |   |   |   |   |   |--> result = pDeferredQueue->DeferredWorkerCore(pDependency);
   |   |   |   |   |   |   |   |   |   |   |   |   |--> pNode->ProcessRequest(&processRequest, pDependency->requestId)
                                                   file: vendor/qcom/proprietary/camx/src/core/camxnode.cpp
   |   |   |   |   |   |   |   |   |   |   |   |   |--> CamxResult Node::ProcessRequest(NodeProcessRequestData* pNodeRequestData, UINT64 requestId)
   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> result = ExecuteProcessRequest(&executeProcessData); //这里暂时以ipe node为例进行分析
                                                       file: vendor/qcom/proprietary/camx/src/hwl/ipe/camxipenode.cpp
   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> CamxResult IPENode::ExecuteProcessRequest(ExecuteProcessRequestData* pExecuteProcessRequestData)
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> GetHwContext()->Submit(GetCSLSession(), m_hDevice, pIQPacket);  //发送设置命令
                                                           file: vendor/qcom/proprietary/camx/src/core/camxhwcontext.cpp
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> CamxResult HwContext::Submit(CSLHandle hCSLSession, CSLDeviceHandle hDevice, Packet* pPacket)
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> result = CSLSubmit(hCSLSession, hDevice, pPacket->GetMemHandle(), pPacket->GetOffset());
                                                               file: vendor/qcom/proprietary/camx/src/csl/camxcsl.cpp
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> CamxResult CSLSubmit(...)
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> return pJumpTable->CSLSubmit(hCSL, hDevice, hPacket, offset);  //跳转方法见func_list_camx_chi ⑤ ,接下来我就不分析了
   |   |   |   |   |   |   |--> m_pPipeline->SinkPortFenceSignaled(pOutputPort->sinkTargetStreamId, ...)    //如果是sink port
                           file: vendor/qcom/proprietary/camx/src/core/camxpipeline.cpp
   |   |   |   |   |   |   |--> VOID Pipeline::SinkPortFenceSignaled(...)
   |   |   |   |   |   |   |   |--> ResultsData resultsData = {};
   |   |   |   |   |   |   |   |--> resultsData.pPrivData = pPerRequestInfo->request.pPrivData;
   |   |   |   |   |   |   |   |--> resultsData.type      = CbType::Buffer;
   |   |   |   |   |   |   |   |--> m_pSession->NotifyResult(&resultsData);
                               file: vendor/qcom/proprietary/camx/src/core/camxsession.cpp
   |   |   |   |   |   |   |   |--> VOID Session::NotifyResult(ResultsData* pResultsData)
   |   |   |   |   |   |   |   |   |--> switch (pResultsData->type)
   |   |   |   |   |   |   |   |   |--> case CbType::Buffer:
   |   |   |   |   |   |   |   |   |--> HandleBufferCb(&pResultsData->cbPayload.buffer, pResultsData->pipelineIndex, pResultsData->pPrivData);
   |   |   |   |   |   |   |   |   |   |--> InjectResult(ResultType::BufferOK, &outBuffer, pPayload->sequenceId, pPrivData, pipelineIndex);
   |   |   |   |   |   |   |   |   |   |   |--> result = m_pThreadManager->PostJob(m_hJobFamilyHandle, NULL, &pData[0], FALSE, FALSE); //这里的线程cb是CHISession::ThreadJobCallback,为什么呢？
                                           file: vendor/qcom/proprietary/camx/src/core/chi/camxchisession.cpp
   |   |   |   |   |   |   |   |   |   |   |--> VOID* CHISession::ThreadJobCallback(VOID* pData)
   |   |   |   |   |   |   |   |   |   |   |   |--> result = pSession->ThreadJobExecute();
   |   |   |   |   |   |   |   |   |   |   |   |   |--> result = ProcessResults();
                                                   file: vendor/qcom/proprietary/camx/src/core/camxsession.cpp
   |   |   |   |   |   |   |   |   |   |   |   |   |--> CamxResult Session::ProcessResults()
   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> LightweightDoublyLinkedListNode* pNode = m_resultHolderList.Head();
   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> bufferReady = ProcessResultBuffers(pResultHolder, metadataReady, &numResults);  //针对每一个node都调用这个方法，感觉这里就是全部node处理buffer的一个过程，后期需要确认下
   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> pNode = m_resultHolderList.NextNode(pNode); //取出下一个node进行操作
   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> DispatchResults(&m_pCaptureResult[0], numResults);  //发送results
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> m_chiCallBacks.ChiProcessCaptureResult(&pCaptureResults[index], m_pPrivateCbData);  //这里的m_chiCallBacks的介绍见func_list_camx_chi ⑦
                                                           file: vendor/qcom/proprietary/chi-cdk/vendor/chioverride/default/chxadvancedcamerausecase.h
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |-->  static VOID      ProcessResultCb(...)
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> static_cast<AdvancedCameraUsecase*>(pCbData->pUsecase)->ProcessResult(pResult, pPrivateCallbackData);
                                                               file: vendor/qcom/proprietary/chi-cdk/vendor/chioverride/default/chxadvancedcamerausecase.cpp
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> VOID AdvancedCameraUsecase::ProcessResult(...)
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |-->  pFeature->ProcessResult(pResult, pPrivateCallbackData);    //如果是AdvancedFeatureEnabled,这里我们考虑的是预览，所以不是这分支
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> CameraUsecaseBase::SessionCbCaptureResult(pResult, pPrivateCallbackData);
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> pCameraUsecase->SessionProcessResult(pCaptureResult, pSessionPrivateData);
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> camera3_capture_result_t* pUsecaseResult     = GetCaptureResult(resultFrameIndex);
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> ChxUtils::PopulateChiToHALStreamBuffer(&pResult->pOutputBuffers[i], pResultBuffer);
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> ProcessAndReturnFinishedResults();
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> Usecase::ReturnFrameworkResult(&result, m_cameraId);
                                                                               file: vendor/qcom/proprietary/chi-cdk/vendor/chioverride/default/chxusecase.cpp
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> VOID Usecase::ReturnFrameworkResult(...)
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> ExtensionModule::GetInstance()->ReturnFrameworkResult(pResult, cameraID);
                                                                                   file: vendor/qcom/proprietary/chi-cdk/vendor/chioverride/default/chxextensionmodule.cpp
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> VOID ExtensionModule::ReturnFrameworkResult(const camera3_capture_result_t* pResult, UINT32 cameraID)
   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |--> m_pHALOps->process_capture_result(m_logicalCameraInfo[cameraID].m_pCamera3Device, pResult); //这里就将result给到上面了





# 编译logcial camera
在oem multi camera 的config里面
编译时使用vendor/qcom/proprietary/chi-cdk/tools/logicalcameragenerator/logicalcamcompiler.py
