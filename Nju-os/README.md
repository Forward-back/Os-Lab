# Nju-os — 南大「操作系统：设计与实现」2024 实验

本目录为个人学习版：无 Online Judge，仅本地测试；`oslabs.mk` 中的 git 提交环节已置空（非南大学生）。课程资料：

- 课程主页：https://jyywiki.cn/OS/2024/
- 课程视频：https://www.bilibili.com/video/BV1Xm411f7CM

## 实验列表

| Lab | 目录 | 内容 | 状态 | 笔记 |
|---|---|---|---|---|
| L0 | `kernel/` | 绘制图片：AM 图形栈、BMP 解析、缩放 | 未开始 | `notes/L0.md` |
| M1 | `pstree/` | 进程树：/proc 解析、树构建、pstree 风格打印 | 未开始 | `notes/M1-pstree.md` |
| M2 | `libco/` | 用户态协程库：状态机、独立栈、上下文切换 | 未开始 | `notes/M2-libco.md` |

三个实验目录当前均为**可编译的空白基线**：`make` 通过，功能待实现（代码内有 TODO 指引）。

## 开始一个 lab

1. 读课程对应实验说明与代码内 TODO（`kernel/kernel.c`、`pstree/pstree.c`、`libco/co.c`）。
2. 复制根目录 [docs/notes-template.md](../docs/notes-template.md) 到 `notes/<lab>.md`，状态改「进行中」。
3. 实现与调试（gdb 规范见 [docs/gdb-debugging.md](../docs/gdb-debugging.md)）。
4. 测试通过 + 笔记写完 + git 提交 → 进度表改「已完成」。

## 构建与运行（WSL 内）

L0（AM + QEMU x86_64）：

```bash
cd kernel
make                 # 产出 build/kernel-x86_64-qemu
make run             # 启动 QEMU（WSLg 有图形窗口）
make run mainargs=hi # 传参
make clean
```

M1（原生可执行，Linux /proc）：

```bash
cd pstree
make                 # 产出 pstree-64 与 pstree-32（-m32 需 gcc-multilib）
./pstree-64 --version
```

M2（协程库 + 自带的测试程序）：

```bash
cd libco
make                 # 产出 libco-64.so / libco-32.so
cd tests
make test            # 运行 64/32 位协程测试
```

## 目录说明

| 路径 | 说明 |
|---|---|
| `abstract-machine/`（AM） | 课程框架：裸机抽象层、klib、构建脚本。**实验不要求改它，不要改** |
| `kernel/` | L0 的工作目录（`make` 走 AM 构建） |
| `pstree/`、`libco/` | M1/M2 工作目录（顶层 Makefile 统一编译，支持 32/64 位） |
| `L0/image.h` | L0 用的 BMP 图像数据 |
| `notes/` | 每个 lab 的笔记（模板见 docs/notes-template.md） |
| `oslabs.mk` | 课程公共构建片段；`submit` 目标指向南大 OJ（本学习版不用） |

## 注意

- 顶层 `Makefile` 与 `oslabs.mk` 标注 DO NOT MODIFY，个人化改动仅限：git 目标置空（已做）、笔记/文档。
- 所有编译均使用 `-Wall -Werror`，带警告即失败。
