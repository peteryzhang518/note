# sensor
sensor 选择的时候，选完成会去链接pipeline， 判断这个pipeline是否是直连sensor，然后会把sensor的输出口输出到pipeline上
# node创建

camx/src/core/camxnodelegacy.cpp 文件中定义了nodeImpl
目前来调用node的creat之后node会获取factory的单例，然后factory通过warpper去创建node
node.h中的class node 有frinde nodeImpL和私有变量impl
m_pNodeImp = pCreateInputData->pNodeImplemenation;


所以目前来看node.cpp也是个壳子，其内部是nodeimpl,在impl:create的时候赋值给createinfo中的impl，方便node.cpp从creatinfo中获取
然后impl:create 会调用node:create一个wrapper通过factory:create出来node转成wrapper,然后也赋值给impl

原因可能是node是一个控制类，并不负责实现，但是负责impl,factory,wrapper等传递信息,然后最后是impl里面有个wrapper

然后调用一般node公共函数的顺序就是调用impl然后getwrapper,因为wrapper是通过node:create再通过factory:creater创建出来的，并且这个公共函数是虚函数的话，就调用到了各自子类的公共函数中去了

# graph , feature, pipeline
graph 以xml的形式存在，比如mcxsuper.xml，里面有着link各种各样的feature的link，和pipelinelink node的方式很像
feature 是以descriptor xml和cpp的形式存在，里面列举了很多包含的pipeline
pipeline又回到以xml形式的存在，然后pipeline里面会link很多node

# pipeline
看了pipeline的createNode之后，发现刚才理解错了，node.cpp不是个壳子，而是nodeimpl是真正所谓的create等代码调用的地方，而node.cpp只是给众多子node的一个模板，然后pipeline调用impl中的函数，然后impl调用node.cpp然后有些是虚函数，再调用真正的子node的函数进行创建
目前来看的是camx的配置在pipeline和usecase中间有断层，这部分断层就是chi,也就是一般情况下，app调用到usecase之后就会去chi中创建feature等内容，然后通过接口函数返回camx中去接着为基础创建pipeline等内容



?chifeature2featurepool.cpp会ProbeChiFeature2Features，然后创建m_featureVersionToOpsMap中专属于每个Feature的ops

ChiFeaturePoolManager::CreateFeatureInstances 中会创建所有的createinfo，这个info在下面所有的创建中，会贯穿始终
    if ((m_featureVersionToOpsMap.end() != itr) && (NULL != itr->second.pCreate))
        pFeature2Instance = m_pFeature2FeaturePoolCommon->CreateFeature(m_pClientId, itr->second.pCreate, pCreateInputInfo, instanceId);
featureCreate中，并没有直接create，而是通过m_featureVersionToOpsMap中的ops，second.pCreate来进行创建的，这个op需要把createinfo传进去





FinalizeFeatureInstances
查看feature2创建时使用的feature  关键字chifeature2base.cpp:4303 ConfigureFeature
这个时候就会对xml中配置各个pipeline节点进行验证和检查等

feature2base  ，ConfigureFeature的 PrepareFeatureData

关于pipeline中的profileID是从feature2base中从sessionData来的，使用一个for循环，从iter中去sessiondata的pipelinedata

configureFeature有createpipeline，参数是cameraid 等
但是需要通过chicontext转化下到camx中的pipeline create
feature2base 会调用到chipipeline中的createpipeline
在createPipeline的过程中就会先SetPipelineNodePorts，把node profile等信息先设置进去然后create的时候就会发送到camx进行下一步
紧接着feature2base 会调用到刚刚创建的chipipeline.cpp的CreateDescriptor
chi-cdk/core/chiframework/chxpipeline.cpp 的ExtensionModule::GetInstance()->CreatePipelineDescriptor(&pipelineCreateData);
chi-cdk/core/chiframework/chxextensionmodule.cpp  的chi-cdk/core/chiframework/chxextensionmodule.cpp
chi-cdk/api/common/chi.h定义了这个接口 pCreatePipelineDescriptor
camx/src/core/chi/camxchi.cpp 的PipelineDescriptor = pChiContext->CreatePipelineDescriptor(pPipelineName
camxchicontext.cpp中的camx/src/core/chi/camxchicontext.cpp
最后回到camxpipeline.cpp的pipeline::create
camxpipeline在create的时候，会把之前backstep或者forwordstep走到的每个node去create,create包括buffer,pipeline inputInfo inputOption outputOption等内容(不是link即使link正确，在node的backstep的没有走正确，链接正确也相当于没有链接上，然后会在后面sessioncreate的时候，去把pipeline有的input option和buffer option去匹配，如果在backstep的时候没成功，会缺少pipelineDescript中的inputOptinoInfo，和拥有的buffer 的instanceId 不匹配，导致createSession失败



////?这里面会创建chi 的pipeline但是真正的在后面的finalize中创建session时session会创建pipeline    CreatePipelineFromDesc    Pipeline::Create.这个不确定什么时候会走，先走上面的




configure feature 之后
ChiFeaturePoolManager::FinalizeFeatureInstances这个函数最终会通过 feature2base 在调用finalizeInitialize 的时候会创建session
然后通过extensionmodule和camxchi.cpp调用到camx的session中，去createsession
然后camx session 会再通过。camxchi.cpp调用setpipeline等调用会，chi设置pipeline 的port口，等这个port 口是在feature2base  createpipeline的时候创建和初始化的
///?然后调用到camx/src/core/chi/camxchicontext.cpp中的pipeline:create











从log整体上看上面流程，再看一遍

创建feature
找到srouce 和sink 同时add descrp
判断source 和 sink类型，match portId和cameraid和stream id,每个物理相机都有hal内部的stream
把portId 的类型链接到这个内部stream上
OnPipelineSelect
PrepareFeatureData
addDescriptor
link node camxpipeline.cpp:2685 CreateNodes() Topology: Pipeline[MultiCameraSATWithBoe0_0_cam_1] Link: Node::MultiCameraSATWithBoe0_IPE0_cam1(outPort 8) --> (inPort 0) (1 / 3) Node::MultiCameraSATWithBoe0_com.qti.node.boeproc0_cam1 using format 3
link node 的时候会处理buffer  init buffer 包括target buffer等等

CreatePipelineDescriptor() GetFormat: pixelFormat 34, outputFormat 0, rawformat 0, selectedFormat 3 usage 0x20900
 OnInitializeStream
updatetPhysicalCameraConfiguration() GlobalMeta MultiCameraSATWithBoe0: camera idx  2, camera Id 2,w 4000, h 3000
findbestSensorMode
camxnode.cpp:605 Initialize()CreateNodes
 chxsensorselectmode.cpp:843 FindBestSensorMode() 
 done
然后创建graph link 前一个feature
然后chifeature2base.cpp:4479 FinalizeInitialization   FinalizeInitialization

camxsession.cpp:2149 Initialize()
 camxsession.cpp:2937 Initialize() Pipeline(MultiCameraSATWithBoe0) maxLivePendingRequests is 6 enablePerReqSync 1
 chifeature2base.cpp:12613 CreateTargetBufferManagers




















12-31 09:05:41:734327986  12126  12130 I CHIUSECASE: [CORE_CFG ] chifeature2graphmanager.cpp:347 Initialize() graph:MultiCameraVirtualSuperGraph,physicalCamID:4,isMultiCamera:1,flags:0x0, m_enableXCFAOptimization:0
12-31 09:05:41:734348090  12126  12130 I CHIUSECASE: [CORE_CFG ] chifeature2featurepool.cpp:452 CreateFeatureInstances() :MultiCameraVirtualSuperGraph, numFeatureInstances:2, logicalCameraId:4

12-31 09:05:41:734741059  12126  12130 I CHIUSECASE: [INFO   ] chifeature2featurepool.cpp:213 DoStreamNegotiation() feature:RealTimeMCX, pStreamNegotiation:0x6fd3072090K
12-31 09:05:41:734758038  12126  12130 V CHIUSECASE: [VERBOSE] chifeature2realtimemcx.cpp:7815 DoStreamNegotiation() DoStreamNegotiation not supported for realtime MCX
12-31 09:05:41:734778090  12126  12130 V CHIUSECASE: [VERBOSE] chifeature2featurepool.cpp:291 UpdateInstancePropsInGraphDesc() MultiCameraVirtualSuperGraph,RealTimeMCX,featureInstance {17,0,0,0},pFeatureGraphDesc=0x6ff79dd1d0

12-31 09:05:41:735027621  12126  12130 V CHIUSECASE: [VERBOSE] chifeature2base.cpp:5403 Initialize() pOwnedStreams is NULL or size is 0
12-31 09:05:41:735041111  12126  12130 V CHIUSECASE: [VERBOSE] chxutils.cpp:2327 GetMultiCameraControlZSLMode() MCControlZSLMode 0 
12-31 09:05:41:735053767  12126  12130 I CHIUSECASE: [INFO   ] chifeature2realtimemcx.cpp:285 Create() rCreateInputInfo.pFeatureDescriptor->pFeatureName RealTimeMCX numStreams 0 BokehMode 0 multiCameraControlZSLMode 0 swMasterCameraIndex 0
12-31 09:05:41:735087309  12126  12130 I CHIUSECASE: [INFO   ] chifeature2featurepoolcommon.cpp:91 CreateFeature() Added feature instance to map with featureId 17 instanceID 0 featureInstance 0xb400007118e3d9a0
12-31 09:05:41:735100486  12126  12130 V CHIUSECASE: [VERBOSE] chifeature2featurepool.cpp:187 DoFeatureCreate() feature:RealTimeMCX, pCreate:0x6fd3071fe0K
12-31 09:05:41:735112882  12126  12130 I CHIUSECASE: [CORE_CFG ] chifeature2featurepool.cpp:677 CreateFeatureInstances() RealTimeMCX(0xb400007118e3d9a0) {featureId, instanceId, cameraId, flags}: {17, 0, 0, 0}
12-31 09:05:41:735132882  12126  12130 V CHIUSECASE: [VERBOSE] chifeature2utils.h:5349 GetCameraIdFromFeatureToCamIdMap()  for feature =MC_REALTIME_REPROCESS instance=0 cameraid=0 featuredesc=0xb400007018f635d0
12-31 09:05:41:735146423  12126  12130 I CHIUSECASE: [INFO   ] chifeature2featurepool.cpp:213 DoStreamNegotiation() feature:MultiCameraReprocessRealtime, pStreamNegotiation:0x6fd315cde0K
12-31 09:05:41:735586319  12126  12130 V CHIUSECASE: [VERBOSE] chifeature2featurepool.cpp:187 DoFeatureCreate() feature:MultiCameraReprocessRealtime, pCreate:0x6fd315d210K
12-31 09:05:41:735597882  12126  12130 I CHIUSECASE: [CORE_CFG ] chifeature2featurepool.cpp:677 CreateFeatureInstances() MultiCameraReprocessRealtime(0xb400007118e42b80) {featureId, instanceId, cameraId, flags}: {19, 0, 0, 0}
12-31 09:05:41:735615538  12126  12130 I CHIUSECASE: [CORE_CFG ] chifeature2featurepool.cpp:991 FinalizeFeatureInstances() MultiCameraReprocessRealtime_0: Backward walk...

_0, matchInputPortId:0
8025:12-31 09:05:41:741224600  12126  12130 V CHIUSECASE: [VERBOSE] chifeature2base.cpp:11846 CreatePipeline() srcTargetIdx:TARGET_BUFFER_SAT_INPUT_FULL_0, matchInputPortId:0
8050:12-31 09:05:41:741534600  12126  12130 V CHIUSECASE: [VERBOSE] chifeature2base.cpp:11846 CreatePipeline() srcTargetIdx:TARGET_BUFFER_SAT_INPUT_FULL_1, matchInputPortId:22
8097:12-31 09:05:41:742067777  12126  12130 V CHIUSECASE: [VERBOSE] chifeature2base.cpp:11846 CreatePipeline() srcTargetIdx:TARGET_BUFFER_SAT_INPUT_FULL_2, matchInputPortId:44
8234:12-31 09:05:41:743885902  12126  12130 I CHIUSECASE: [INFO   ] chifeature2base.cpp:12047 CreatePipeline() MultiCameraReprocessRealtime Creating pipeline MultiCameraSATWithBoe0 for cameraId 1, pointer 0xb4000071b8e1d8e0K
8687:12-31 09:05:41:778896007  12126  12130 I CHIUSECASE: [INFO   ] chifeature2base.cpp:12115 CreatePipeline() pPipeline: 0xb4000071b8e1d8e0 MultiCameraSATWit

