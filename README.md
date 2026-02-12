# can-idps

针对 **CAN 总线**的轻量入侵检测系统（IDS），基于 Linux SocketCAN。

## 功能

- **CAN ID 检测**：按仲裁 ID、掩码匹配
- **数据内容匹配**：payload 字节模式
- **速率限制**：单 ID 每秒帧数超阈值告警（Flood 检测）
- **错误帧检测**：CAN 错误帧告警

## 环境要求

- Linux 内核支持 SocketCAN
- CAN 接口已配置（如 `can0`、`vcan0`）

```bash
# 虚拟 CAN 测试
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

## 编译与运行

```bash
make
sudo ./can-idps -i can0
```

### 参数

- `-i iface`：CAN 接口（默认 can0）
- `-r rules`：规则文件路径

### 示例

```bash
# 监听 can0
sudo ./can-idps -i can0

# 指定规则
sudo ./can-idps -i vcan0 -r rules/can.rules
```

## 规则格式

```
alert 0x<can_id> [0x<mask>] msg:"..." [rate:N]
```

- `can_id`：CAN 仲裁 ID（十六进制）
- `mask`：ID 掩码，0 或省略时使用 0x7FF（标准帧）
- `msg`：告警消息
- `rate`：可选，该 ID 每秒最大帧数，超限告警

### 示例

```
alert 0x7DF 0x7FF msg:"OBD-II request"
alert 0x100 0x7FF msg:"Suspicious ID" rate:200
```

## 目录结构

```
can-idps/
├── include/      头文件
├── src/          源码（capture, decoder, detect, rate, output, main）
├── rules/        规则
└── Makefile
```

## 典型攻击检测

| 攻击类型 | 检测方式 |
|----------|----------|
| CAN Flood | 速率限制（rate） |
| 非法 ID 注入 | CAN ID 白名单/规则 |
| 错误帧 | 错误帧规则 |
| 重放攻击 | 需结合时序/序列分析（可扩展） |

## 参考资料

- ISO 11898: CAN 总线标准
- Linux SocketCAN: `man 7 socket`
# can-idps
