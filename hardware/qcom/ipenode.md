ipenode.cpp会包含iqmodule
iqmodule会调用iqinterface.h这里面会创建所有的硬件相关的功能比如tf
iqmodule的实例化其中有一个camxtf.cpp
调用interface就会调用到camxtf.cpp
tf.cpp会调用实例化和比源的titantf和tfsetting去做真正接触到硬件的内容,使用的参数是input,最后的input也是通过ipenode.cpp传送过来的
camxtf.cpp是以camxiqmodule的形式存在的



ipenode.cpp的postpipelineCreate会creategenericpassObj,同时把m_pEnabledIPEIQModule的module参数设置到obj里面去
这个obj在ipeGenericPass.cpp文件里面可能使用设置的param创建iqmodule的cmdBuffer等 
然后ipenode在postpipelineCreate中会CreateIPEIQModules,m_pGenericPassObjects[index]->CreateIQModules





在ipenode.h中ISPIQModule*              m_pEnabledIPEIQModule
camx-api/camx-lib/system/isphwsetting/pipeline/camxisppipeline.h:    CAMX_INLINE std::list<ISPIQModule*>& GetActiveIQModulesList()
camx-api/camx-lib/system/isphwsetting/pipeline/camxisppipeline.h:    std::list<ISPIQModule*>    m_activeModuleList;        ///< Active IQ Moduele List
camx/src/hwl/ipe/camxipenode.h:    // ISPIQModule*              m_pEnabledIPEIQModule[MaxIPEIQModule];      ///< List of IQ Modules
camx/src/hwl/ipe/camxipegenericpass.h:    CAMX_INLINE ISPIQModule** GetEnabledIQModuleList()
camx/src/hwl/ipe/camxipegenericpass.h:        ISPIQModule*        pCurrentIQModule);
camx/src/hwl/ipe/camxipegenericpass.h:    ISPIQModule*            m_pEnabledIPEIQModule[MaxIPEIQModule];              ///< List of enabled IQ modules
