# 将 can-idps 推送到 GitHub

项目已准备就绪，按以下步骤推送到你的 GitHub 账户（right_c@hotmail.com）：

## 步骤 1：在 GitHub 创建新仓库

1. 打开 https://github.com/new
2. 登录你的 GitHub 账户
3. 填写：
   - **Repository name**：`can-idps`
   - **Description**：CAN bus IDPS for Linux SocketCAN
   - 选择 **Public**
   - **不要**勾选 "Add a README file"（本地已有）
4. 点击 **Create repository**

## 步骤 2：推送代码

在终端执行（将 `YOUR_USERNAME` 替换为你的 GitHub 用户名）：

```bash
cd /home/right-c/workspace/can-idps

# 添加远程仓库（替换 YOUR_USERNAME 为你的 GitHub 用户名）
git remote add origin https://github.com/YOUR_USERNAME/can-idps.git

# 推送到 GitHub
git push -u origin main
```

若使用 SSH：

```bash
git remote add origin git@github.com:YOUR_USERNAME/can-idps.git
git push -u origin main
```

## 步骤 3：验证

推送成功后，访问 https://github.com/YOUR_USERNAME/can-idps 查看项目。
