# git
1. git pull githuborigin main  
把githuborigin 远端仓库的main分支拉取到执行命令的分支
2. git push githuborigin localmain:main
把localmain分支推送到远端仓库的main分支
3. git remote -v查看远端分支地址和在本地的名字
4. git remote rename 可以改变远端分支在本地的名字

1. 查看当前远程仓库
```
git remote -v
```
2. 使用 git remote rename 命令更改远程仓库的名字
```
git remote rename origin new-name
```

3. git 设置默认编辑器为vim
```
git config --global core.editor "vim"
```

# 连接问题
1. 当git push clone的命令忽然连接不上远程服务器的时候，需要在~/.ssh/config
   文件中写入
```
Host github.com
  Hostname ssh.github.com
  Port 443
```
