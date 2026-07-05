# TCP 通信项目

基于 Qt 框架的 TCP 网络通信示例项目，包含客户端和服务端程序。

## 项目结构

```
TCP/
├── TCP_Cilent/    # TCP 客户端程序
└── TCP_Server/    # TCP 服务端程序
```

## 功能说明

### TCP_Client（客户端）

客户端程序主要功能：
- 自动获取并显示本机 IP 地址
- 连接/断开服务器
- 发送和接收文本消息
- 实时显示 Socket 连接状态
- 状态栏显示当前连接状态

**主要类和功能**：
- `MainWindow`：主窗口类
- `m_socket`：QTcpSocket 对象，用于 TCP 通信
- `getLocalIP()`：获取本机 IPv4 地址

**信号槽连接**：
- `connected`：连接成功时触发
- `disconnected`：断开连接时触发
- `stateChanged`：Socket 状态变化时触发
- `readyRead`：有数据可读时触发

### TCP_Server（服务端）

服务端程序主要功能：
- 自动获取并显示本机 IP 地址
- 开始/停止监听指定端口
- 接受客户端连接
- 发送和接收文本消息
- 实时显示 Socket 状态
- 状态栏显示监听状态和连接状态

**主要类和功能**：
- `MainWindow`：主窗口类
- `m_server`：QTcpServer 对象，用于监听客户端连接
- `m_socket`：QTcpSocket 对象，用于与客户端通信
- `getLocalIP()`：获取本机 IPv4 地址

**信号槽连接**：
- `newConnection`：有新客户端连接时触发
- `connected`：客户端连接成功时触发
- `disconnected`：客户端断开连接时触发
- `stateChanged`：Socket 状态变化时触发
- `readyRead`：有数据可读时触发

## 使用方法

### 服务端

1. 运行 TCP_Server 程序
2. 窗口标题会显示本机 IP 地址
3. 选择监听地址和端口（默认已填入本机 IP）
4. 点击"开始监听"按钮
5. 等待客户端连接
6. 连接成功后，可以在文本框中输入消息并发送

### 客户端

1. 运行 TCP_Client 程序
2. 窗口标题会显示本机 IP 地址
3. 在地址栏输入服务器 IP 地址
4. 设置服务器端口（需与服务端一致）
5. 点击"连接"按钮
6. 连接成功后，可以在文本框中输入消息并发送

## 技术栈

- Qt 6.9.0
- MinGW 64-bit
- Qt Network 模块（QTcpSocket、QTcpServer）

## 编译说明

使用 Qt Creator 打开对应的 `.pro` 文件即可编译运行：
- `TCP_Cilent/TCP_Cilent.pro`
- `TCP_Server/TCP_Server.pro`

## 注意事项

1. 确保防火墙允许程序的网络通信
2. 客户端和服务端需在同一网络或可达网络中
3. 服务端需要先启动并开始监听，客户端才能连接
4. 消息发送采用 UTF-8 编码，以换行符结尾

## 功能特性

- 实时显示连接状态
- 自动获取本机 IP 地址
- 支持文本消息的发送和接收
- 状态栏显示详细的 Socket 状态信息
