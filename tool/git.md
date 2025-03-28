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

# 回退 git commit --amend
1. git reset HEAD@{1}

# git 命令详解
 git branch -a
本地分支    ：* localconfig
远端head    ：  remotes/githubconfig/HEAD -> githubconfig/main
远端分支    ：  remotes/githubconfig/main


# git 不会在github上显示的
如果用的名字不是同一个电子邮件地址，是不会在github上显示的
1. 在github上添加新的邮件地址
2. 提交的时候用相同的地址去提交
GIT_COMMITTER_EMAIL="peteryzhang518@gmail.com" git push
git commit --author="peteryzhang518@gmail.com"
git commit --amend --author="peterZ <peteryzhang518@gmail.com>"
git commit --amend --author="peteryzhang518 <peteryzhang518@gmail.com>"


用的邮箱地址没有注册账号的话，提交会被标记为未有用户地址,但不影响提交

# git commit 缺少sign off 邮件
使用git  commit  -m

# git rebase 
1. 切换到origin分支
2. 把origin分支拉到最新
3. 切换到使用的功能分支
4. git rebase origin 把功能分支变基到origin分支

当gitrebase起冲突时，优先使用主分支内容
git rebase -Xtheirs origin/main

当gitrebase起冲突时，优先使用当前功能分支内容
git rebase -Xours origin/main


git rebase -i HEAD~3
