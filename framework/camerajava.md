# framework 中get number of camera
app get 到的 num 一直为一个确定的值比如说2, 很有可能是因为framework中的代码写死了，比如说在frameworks/base/core/java/android/hardware/Camera.java中的getNumberOfCameras()函数中，直接返回了一个值, 所以需要enable exposeAuxCamera 让其高于2的时候不止返回2


