# clash
1. 首先需要安装clash版本
2. 需要取aur下载yay
3. git clone https://aur.archlinux.org/yay.git
   cd yay
   makepkg -si
4. 安装时如果go
   downloading出现问题，那么可以改变bashrc中goproxy的值，改变go的下载源
5. config.yam 可以从机场文件里获取，有的机场不支持可以生成后就不好使
6. 

# git
1. git pull githuborigin main  
把githuborigin 远端仓库的main分支拉取到执行命令的分支
2. git push githuborigin localmain:main
把localmain分支推送到远端仓库的main分支
3. git remote -v查看远端分支地址和在本地的名字
4. git remote rename 可以改变远端分支在本地的名字

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

