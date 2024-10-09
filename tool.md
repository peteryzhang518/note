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

