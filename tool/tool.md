# clash
1. 首先需要安装clash版本
2. 需要取aur下载yay
3. git clone https://aur.archlinux.org/yay.git
   cd yay
   makepkg -si
4. 安装时如果go
   downloading出现问题，那么可以改变bashrc中goproxy的值，改变go的下载源
5. config.yam 可以从机场文件里获取，有的机场不支持可以生成后就不好使

# v2ray
1. v2ray 是一种内核，通过内核做出来的产品才是真正可以用的东西
2. ubuntu
   系统可以通过github直接使用v2rayN这个产品，然后把飞机上的v2ray连接连接上就可以直接使用
3. 使用时点击+号，然后把链接填写上
4. 然后更新service 不通过代理更新
5. 最后点击下面的通过系统代理即可以（有的时候需要重启）

# grub
1. 首先来说grub是放在一个efi文件系统中的引导软件
2. 所以首先要在硬盘开辟一块空间并且配置称efi系统
3. 然后在创建系统的时候把所有的系统，比如arch挂在到mnt上，然后把启动盘挂在mnt/boot上，然后把window的c盘挂载到mnt/随便一个目录
4. 然后启动os-什么check软件扫描所有系统
5. 然后grub config
6. 然后就可以看到gurb的配置文件有两个系统了

如果为EFI/GPT引导方式：
安装grub与efibootmgr两个包：
pacman -S grub efibootmgr
部署grub：
grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=grub
生成配置文件：
grub-mkconfig -o /boot/grub/grub.cfg
*提示信息应与上面的图类似，如果你发现错误，请仔细检查是否正确完成上面的过程。**
如果报warning failed to connect to lvmetad，falling back to device
scanning.错误。参照这篇文章，简单的方法是编辑/etc/lvm/lvm.conf这个文件，找到use_lvmetad
= 1将1修改为0，保存，重新配置grub。
如果报grub-probe: error: cannot find a GRUB drive for /dev/sdb1, check your
device.map类似错误，并且sdb1这个地方是你的u盘，这是u盘uefi分区造成的错误，对我们的正常安装没有影响，可以不用理会这条错误。
安装后检查
如果你是多系统，请注意上面一节中对os-prober这个包的安装。
强烈建议使用如下命令检查是否成功生成各系统的入口，如果没有正常生成会出现开机没有系统入口的情况：
vim /boot/grub/grub.cfg
检查接近末尾的menuentry部分是否有windows或其他系统名入口。下图例子中是Arch
Linux入口与检测到的windows10入口（安装在/dev/sda1），具体情况可能有不同：
如果你已经安装os-prober包并生成配置文件后还是没有生成其他系统的入口：
你目前处的U盘安装环境下有可能无法检测到其他系统的入口，请在下一步中重启登陆之后重新运行：

grub-mkconfig -o /boot/grub/grub.cfg
如果还没有生成其他系统的入口，请参照：

https://wiki.archlinux.org/index.php/GRUB/Tips_and_tricks#Combining_the_use_of_UUIDs_and_basic_scripting

编辑配置文件手动添加引导的分区入口。


# fzf
如果发现xception not caught: Failed to run "/home/p/.fzf/bin/fzf --version" 这个错误
1. 检查shell,vim使用shell
2. 检查fzf install脚本的执行
3. 最有可能是fzf的配置文件出错，删掉fzf对于窗口登配置项目的配置

# solarized
如果发现一直颜色异常，可能是因为solarized的配置没有配置bkg和fkg等背景色和前景色，可以使用solarized去配置
使用solarized8可以解决这个问题

# docker
1. run 命令是从一个已经有的镜像中创建一个容器
2. start是从创建好的容器中打开一个容器
3. exec是进入某一个已经打开的容器

docker pull
docker image
docker run
docker ps -a
docker start 
docker ps -a
docker exec

# run

sudo docker run --name pz8750 -it -d \
    -v /home/p/code/8750:/home/p/docker8750 \
    -v /lib/modules:/lib/modules \
    --privileged \
    -v /dev/:/dev \
    -v /run/udev:/run/udev \
    192.168.6.131:5000/ubuntu_22.04_bsp:v1.0 /bin/bash


docker run --name ubuntu_20.04 -it -d -v {/home/#####}:/home/bsp/workspace -v /lib/modules:/lib/modules --privileged -v /dev/:/dev -v /run/udev:/run/udev 192.168.6.131:5000/ubuntu_20.04_bsp:v1.0 /bin/bash


sudo docker run：运行一个新的容器。
--name pz8750：为容器指定名称 pz8750，方便管理。
-it：交互模式：
-i：保持输入通道打开（适用于交互式 shell）。
-t：分配一个伪终端（TTY）。
-d：让容器在后台运行（daemon 模式）。
-v /home/p/code/8750:/home/p/docker8750：
将宿主机目录 /home/p/code/8750 挂载到容器的 /home/p/docker8750，这样可以在宿主机和容器之间共享文件。
-v /lib/modules:/lib/modules：
将宿主机的 /lib/modules 挂载到容器内，通常用于加载宿主机的内核模块（例如驱动程序）。
--privileged：
给予容器更高的权限，允许访问宿主机的设备文件、管理网络等。
-v /dev/:/dev：
将宿主机的 /dev/ 设备文件挂载到容器，使容器能够访问硬件设备。
-v /run/udev:/run/udev：
挂载 udev 设备管理系统相关的目录，通常用于支持热插拔设备。
192.168.6.131:5000/ubuntu_22.04_bsp:v1.0：
使用镜像 ubuntu_22.04_bsp:v1.0（来自 192.168.6.131:5000 的私有 Docker 仓库） 作为容器的基础环境。
/bin/bash：
指定启动 bash 进程，确保容器启动时有一个 shell 运行。


docker start ubuntu_20.04
docker exec -it -u 1000:1000 -w /home/bsp ubuntu_20.04  /bin/bash


# fd
1. fd查找精准名字，全称匹配用-g


# 环境代理命令
 sudo apt install *** -o Acquire::http::Proxy="http://192.168.6.202:8080"
 curl -k -O ***

# linux设置app
1. export PATH=$PATH:/home/user/myapp
2. sudo cp /home/user/myapp/myprogram /usr/local/bin/
sudo chmod +x /usr/local/bin/myprogram
3. sudo ln -s /home/user/myapp/myprogram /usr/local/bin/myprogram
4. echo 'alias myapp="/home/user/myapp/myprogram"' >> ~/.bashrc


# addr2line
1. 编译时需要有调试选项
 - ninja 修改 CMake 构建
  1. set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
  2. cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
     ninja
 - 直接修改 build.ninja
  1. sed -i 's/-O2/-O0 -g/g' build.ninja
  2. ninja
 - mmm修改 Android.mk
  1. LOCAL_CFLAGS += -g -O0
     LOCAL_CPPFLAGS += -g -O0
  2. mmm path/to/module
 - mmm 修改 Android.bp
  1. cc_library_shared {
     name: "mylib",
     srcs: ["mylib.cpp"],
     cflags: ["-g", "-O0"],
     }
  2. mmm path/to/module

2. 验证是否生效
readelf -S out/target/product/sun/symbols/vendor/lib64/hw/camera.qcom.so
 1. 生效o
  [29] .debug_loclists   PROGBITS         0000000000000000  0083d898
  [30] .debug_abbrev     PROGBITS         0000000000000000  00ceb156
  [31] .debug_info       PROGBITS         0000000000000000  00e3e557
  [32] .debug_rnglists   PROGBITS         0000000000000000  02caa9bb
  [33] .debug_str_o[...] PROGBITS         0000000000000000  02e5843c
  [34] .debug_str        PROGBITS         0000000000000000  0346fc10
  [35] .debug_addr       PROGBITS         0000000000000000  03cca70a
  [37] .debug_line       PROGBITS         0000000000000000  03d9e181
  [38] .debug_line_str   PROGBITS         0000000000000000  0426b500
  [39] .debug_ranges     PROGBITS         0000000000000000  04284c39
 2. 未生效
  [30] .gnu_debugdata    PROGBITS         0000000000000000  0083d9db
3. 确定是否需要基地址去计算偏移地址
readelf -h  out/target/product/sun/symbols/vendor/lib64/hw/camera.qcom.so | rg type
 1. 不需要
  Type:                              DYN (Shared object file)
 2. 需要
未遇到这种情况

4. 解析
-e 指定要解析的可执行文件或共享库（通常是未剥离符号的 .so 或 .elf 文件）。
-f 显示函数名（默认情况下只显示文件名和行号，-f 让其额外输出对应的函数名）。
-C 解码 C++ 符号，将 _Z 这种被 C++ 编译器修饰的符号（mangled name）转换回可读的 C++ 函数名。
-i 显示内联函数信息，如果该地址命中的是内联函数，会显示完整的内联展开路径。



