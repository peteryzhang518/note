framwrok ： app框架层 （Google 维护（AOSP，Android Open Source Project）
system   ： 系统层（service层, google 维护 不包括hardware，防止更新系统困难,  不包括framework,因为system相当于android的system 不和用户混杂一起）
hardware ： 硬件层(ardware/ 目录下的 HAL 代码 部分是 AOSP 维护的，Google 可以规定标准的 API，而具体的实现可以由厂商定制。) 
vendor   ： 供应商层（硬件层）
kernel   ： 内核层
device   ： 设备层（设备定制层,bsp层）


vndk:
	vndk 本质上是一个隔离system 和 vendor的架构，主要用在代码的编译和运行时

原因：system和vendor会共用到system中的lib库，但是这些库是由不同的团队维护的，所以为了避免冲突，所以需要vndk来隔离
形式：在编译的时候，vndk会将system和vendor的lib库分开，分别放到system/lib和vendor/lib中，这样就避免了冲突
结果：vendor可以在system升级时，system中的lib变化时，仍然保持自己需要的system库不发生变化，方便系统升级

结构：
llndk:system 和 vendor 都可以使用

fwk only: 只有system可以使用
vndk: 只有vendor可以使用 

sp-hal: system 反过来调用 vendor 的库时,  vendor 提供system使用的库
vndk-sp: system 反过来调用 vendor 的库时,解耦产生的结构, sp-hal如果要使用system库，不能使用vndk，只能使用vndk-sp

除了sp-hal剩下都是system的库




