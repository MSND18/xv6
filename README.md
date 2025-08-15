下面是一份通用、开箱即用的 **xv6-riscv** 项目 `README.md` 模板（中文友好，适合课堂/自学/个人 fork）。你可以直接复制到仓库根目录，根据你的实际环境微调即可。

---

# xv6-riscv

受 Unix 第六版（V6）启发的教学用 RISC-V 操作系统。该仓库包含内核、用户态程序、简易文件系统与一套用于实验/评测的 Makefile 与脚本。目标读者为操作系统课程学习者与对内核实现感兴趣的开发者。

> 提示：本 README 以 **RISC-V 64 位（rv64）** 版本为准。

## 特性一览

* 进程与调度（多 CPU 内核线程 / 进程切换）
* 虚拟内存与页表（sv39）
* 简易 ELF 装载与系统调用
* 设备驱动（UART、磁盘仿真、控制台）
* 日志式文件系统与基础 shell
* QEMU 支持、GDB 远程调试

---

## 环境准备

### 依赖（通用概念）

* **RISC-V 交叉工具链**：`riscv64-unknown-elf-gcc`、`riscv64-unknown-elf-ld`、`riscv64-unknown-elf-gdb`（或 `gdb-multiarch`）
* **QEMU（system 模式）**：`qemu-system-riscv64`
* `make`、`gcc`、`clang`（任选其一，用于构建工具/生成器）
* Python（用于部分辅助脚本，若有）

### 在 Linux (Debian/Ubuntu 示例)

```bash
# 工具链（推荐使用发行版或官方工具链，也可用 riscv-gnu-toolchain 自行编译）
sudo apt update
sudo apt install -y build-essential gdb-multiarch qemu-system-misc git

# （可选）如果你没有 elf 交叉工具链，可安装预编译包或自编译：
# sudo apt install -y gcc-riscv64-unknown-elf gdb-multiarch
```

### 在 macOS (Homebrew)

```bash
# 安装 QEMU 与交叉工具链
brew update
brew install qemu riscv-gnu-toolchain
# 或者：brew install gdb (注意 macOS 下签名/权限)
```

> 如果你的发行版不提供 `riscv64-unknown-elf-*` 前缀，也可使用 `riscv64-elf-*`，并在 `Makefile` 中调整工具链前缀变量 `TOOLPREFIX`。

---

## 快速开始

```bash
# 1) 克隆代码
git clone https://example.com/your/xv6-riscv.git
cd xv6-riscv

# 2) 构建
make            # 生成内核镜像 kernel，以及文件系统镜像 fs.img

# 3) 运行（启动 QEMU）
make qemu       # 启动后进入 xv6 shell

# 4) 退出 QEMU
# 在 QEMU 窗口中：Ctrl+A 再按 X（某些环境是 Ctrl+A, X）
# 或在 shell 中运行 poweroff（若实现了）
```

常用目标：

```bash
make clean          # 清理生成文件
make qemu CPUS=2    # 以 2 个虚拟 CPU 运行
make qemu-gdb       # 以调试模式启动（等待 GDB 连接）
make grade          # 运行内置测试/评分脚本（若提供）
```

---

## 目录结构（典型）

```
.
├─ Makefile           # 构建与运行规则
├─ README.md
├─ kernel/            # 内核源码（启动、陷入、调度、内存、fs、驱动）
├─ user/              # 用户态程序与库
├─ fs/                # 简易文件系统工具/脚本
├─ include/           # 公共头文件（若有拆分）
├─ mkfs/              # 文件系统镜像生成工具（若独立）
├─ .gdbinit           # 预置 GDB 脚本（target remote :PORT 等）
└─ grade/             # 实验/评测脚本（若有）
```

> 实际目录可能略有差异，请以你的仓库为准。

---

## 调试指南（GDB + QEMU）

1. 启动等待调试的 QEMU：

   ```bash
   make qemu-gdb
   # 该命令不会自动运行内核，而是监听 GDB 远程连接（常用端口 26000）
   ```
2. 在另一个终端连接 GDB：

   ```bash
   riscv64-unknown-elf-gdb kernel/kernel
   # 或 gdb-multiarch kernel/kernel
   (gdb) target remote :26000
   (gdb) layout asm      # （可选）查看汇编
   (gdb) layout regs     # （可选）查看寄存器
   (gdb) b main          # 断点
   (gdb) c               # 继续执行
   ```

> `.gdbinit` 中通常已包含 `target remote :26000`、`symbol-file`、便捷宏等；你也可以按照需要添加 `set disassemble-next-line on` 等个人偏好。

---

## 常见任务

* **添加/修改系统调用**

  1. 在内核 syscall 表里注册号与处理函数
  2. 在用户态头文件声明封装
  3. 重新编译并在用户程序中调用测试

* **实现/扩展调度或同步**
  修改 `proc`/`sched`/`trap` 相关文件；可用 `CPUS=N` 增加并发压力。

* **文件系统实验**
  调整 `fs`/`log` 模块；使用 `mkfs` 重新打包 `fs.img`；编写用户程序回归测试。

* **用户程序**
  在 `user/` 下新增 `.c`，在相应 `UPROGS` 或构建列表添加条目，然后 `make qemu` 运行测试。

---

## 测试与评分（如果提供）

```bash
make grade     # 运行全部测试
make xcheck    # 运行指定/扩展测试（若有）
```

输出会给出通过/失败用例与提示。若你的课程仓库不同步，请按授课方脚本为准。

---

## 已知问题与排查

* **`qemu-system-riscv64` 找不到**
  检查 QEMU 是否安装在 PATH；macOS 用 `brew info qemu` 确认安装路径。

* **交叉工具链前缀不匹配**
  修改 `Makefile` 中：

  ```makefile
  TOOLPREFIX = riscv64-unknown-elf-
  # 若你的工具链是 riscv64-elf-，就改成：
  # TOOLPREFIX = riscv64-elf-
  ```

* **GDB 无法连接**
  确认使用了 `make qemu-gdb` 启动；核对端口（常见为 26000）；防火墙/占用问题会导致连接失败。

* **启动卡住/黑屏**
  打开 QEMU 串口日志、在 GDB 中单步至 `start`/`main`；确认链接脚本与入口符号一致。

---

## 贡献指南

欢迎提交 PR 与 Issue：

* 提交前请运行 `make clean && make && make grade`
* 保持代码风格与现有一致（缩进、命名、注释）
* 在 PR 说明中简要描述问题、变更点与测试结果

---

## 参考材料

* xv6 教材与源码（MIT 6.S081/6.S082 等课程）
* RISC-V 特权架构手册（Sv39 内存管理）
* QEMU 与 GDB 官方文档

> 本仓库通常采用 MIT 许可（或上游一致的宽松许可）。请在根目录附上相应 `LICENSE` 文件并声明版权归属与来源（例如 “Copyright © The Regents of the University of California, MIT, and contributors.”）。

---

## 版权与许可

除非你另行声明，建议沿用 **MIT** 或与上游一致的许可，并在此处说明与上游 xv6-riscv 的关系与变更摘要。

---

### 快速清单（TL;DR）

```bash
# 安装依赖（示例）
# Ubuntu: sudo apt install qemu-system-misc gdb-multiarch build-essential
# macOS:  brew install qemu riscv-gnu-toolchain

# 构建 & 运行
make
make qemu
# 调试
make qemu-gdb &  riscv64-unknown-elf-gdb kernel/kernel
# 清理
make clean
```

---
