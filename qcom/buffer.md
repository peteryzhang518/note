buffer笔记

# buffer种类
从grep的搜索结果来看，大概可能有几类，cmdbuffer， imgbuffer， 还有什么来这
# buffer大概结构
目前来看，buffer的架构有点类似于feature，
有manager，pool，和imgbuffer这三个基础类，可能也有一些其他。
然后通用的是pool会create manager，manager去creater img buffer

mempoolmgr 去注册mempoolgroup
imagemanager 去注册 mempoolmgr
nodelegacy 去注册 imagemanager

# 创建buffer的过程
nodelegacy中会有部分函数在执行某些过程中需要buffer
目前来看到的方式是node一开始会创建imagemanager然后在需要的时候，直接去调用imagemanager中的getBuffer函数
比如setRequestOutputImageBuffer等等其他函数

# Buffer 的流转顺序
目前的想法是先看看流程上的buffer是如何处理的

