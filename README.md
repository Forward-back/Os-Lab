# OS 实验工作区

两个互补的操作系统课程实验，目标是**自己动手把每个 lab 做完并讲清楚**。

| 项目 | 来源 | 实验形态 |
|---|---|---|
| [Nju-os](Nju-os/) | 南京大学「操作系统：设计与实现」2024（[课程主页](https://jyywiki.cn/OS/2024/)、[视频](https://www.bilibili.com/video/BV1Xm411f7CM)） | Mini Labs + OS Labs，本地测试 |
| [mit-xv6-riscv](mit-xv6-riscv/) | MIT 6.1810（[课程主页](https://pdos.csail.mit.edu/6.1810/)） | 基于 xv6 的 10 个 lab |

## 目录

```
os_lab/
├─ README.md               # 本文件：总导航与进度总览
├─ CLAUDE.md               # 给 AI 会话的构建/约定速查
├─ docs/
│  ├─ gdb-debugging.md     # GDB 调试规范与要点
│  ├─ wsl-env.md           # WSL 环境搭建与开发形态
│  └─ notes-template.md    # lab 笔记模板
├─ scripts/setup-wsl.sh    # WSL 工具链一键安装
├─ Nju-os/                 # 南大实验：README.md + notes/
└─ mit-xv6-riscv/          # MIT 实验：README.md（个人指引）+ README（上游原文，Makefile 依赖）
```

## 进度总览

状态定义：**未开始 / 进行中 / 已完成**。「已完成」= 测试通过 + 笔记写完 + git 提交，三者缺一不可。

### Nju-os

| Lab | 目录 | 内容 | 状态 | 笔记 |
|---|---|---|---|---|
| L0 | kernel/ | 绘制图片（AM 图形栈、BMP 解析、缩放） | 未开始 | notes/L0.md |
| M1 | pstree/ | 进程树（/proc 解析、树构建与打印） | 未开始 | notes/M1-pstree.md |
| M2 | libco/ | 用户态协程库（上下文切换） | 未开始 | notes/M2-libco.md |

### mit-xv6-riscv

| Lab | 内容 | 状态 | 笔记 |
|---|---|---|---|
| util | 系统调用与用户程序热身 | 未开始 | notes/lab-util.md |
| syscall | 系统调用机制 | 未开始 | notes/lab-syscall.md |
| pgtbl | 页表 | 未开始 | notes/lab-pgtbl.md |
| traps | 陷阱与中断 | 未开始 | notes/lab-traps.md |
| cow | 写时复制 fork | 未开始 | notes/lab-cow.md |
| thread | 线程切换 | 未开始 | notes/lab-thread.md |
| net | 网络驱动 | 未开始 | notes/lab-net.md |
| lock | 并发与锁 | 未开始 | notes/lab-lock.md |
| fs | 文件系统 | 未开始 | notes/lab-fs.md |
| mmap | 内存映射 | 未开始 | notes/lab-mmap.md |

## 快速开始

所有构建与运行都在 WSL2 (Ubuntu 24.04) 内进行：

```bash
# 一次性安装工具链（需 sudo 密码）
bash scripts/setup-wsl.sh

# Nju-os：在具体 lab 目录内构建/运行
cd Nju-os/kernel  && make && make run      # L0，QEMU x86_64
cd Nju-os/pstree  && make && ./pstree-64   # M1，原生可执行
cd Nju-os/libco/tests && make test         # M2，运行协程测试

# MIT xv6
cd mit-xv6-riscv && make && make qemu      # 退出：Ctrl-A 然后 X
```

环境细节、VSCode 远程开发配置见 [docs/wsl-env.md](docs/wsl-env.md)。

## 实验流程

1. 领取 lab：把根 README 与项目 README 中该 lab 的状态改为「进行中」。
2. 复制 [docs/notes-template.md](docs/notes-template.md) 为 `项目/notes/<lab>.md`，随做随记。
3. 实现、测试、调试（规范见 [docs/gdb-debugging.md](docs/gdb-debugging.md)）。
4. 完成判定三项全过 → 状态改「已完成」→ 提交 git。

约定：不改上游框架代码（`abstract-machine/`、xv6 核心）除非实验本身要求；个人改动与理由记进笔记。
