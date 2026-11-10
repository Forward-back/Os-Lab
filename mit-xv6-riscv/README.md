# mit-xv6-riscv — MIT 6.1810 xv6 实验

xv6 是 MIT 6.1810 的教学用操作系统（RISC-V 版）。上游原文保存在 `README.upstream`。

- 课程主页：https://pdos.csail.mit.edu/6.1810/
- 实验说明（按学期日程逐 lab 发布）：https://pdos.csail.mit.edu/6.1810/2024/schedule.html
- 官方实验骨架仓库：`git://g.csail.mit.edu/xv6-labs-2024`（每个 lab 一个分支）

## 实验列表

当前仓库是**未改动的基线快照**，所有 lab 未开始：

| Lab | 内容 | 状态 | 笔记 |
|---|---|---|---|
| util | 系统调用与用户程序热身 | 未开始 | `notes/lab-util.md` |
| syscall | 系统调用机制 | 未开始 | `notes/lab-syscall.md` |
| pgtbl | 页表 | 未开始 | `notes/lab-pgtbl.md` |
| traps | 陷阱与中断 | 未开始 | `notes/lab-traps.md` |
| cow | 写时复制 fork | 未开始 | `notes/lab-cow.md` |
| thread | 线程切换 | 未开始 | `notes/lab-thread.md` |
| net | 网络驱动 | 未开始 | `notes/lab-net.md` |
| lock | 并发与锁 | 未开始 | `notes/lab-lock.md` |
| fs | 文件系统 | 未开始 | `notes/lab-fs.md` |
| mmap | 内存映射 | 未开始 | `notes/lab-mmap.md` |

## 开始一个 lab

1. 按课程日程页找到对应 lab handout，确定验收测试（多数为 `usertests` 的指定子集）。
2. 实现路径二选一（在笔记里记录选择）：
   - 直接在本基线快照上按 handout 实现；或
   - 以官方 lab 分支为参考起步（`git clone git://g.csail.mit.edu/xv6-labs-2024` 后 checkout `util`/`syscall`/…），再把本目录作为对照。
3. 复制根目录 [docs/notes-template.md](../docs/notes-template.md) 到 `notes/lab-<name>.md`，状态改「进行中」。
4. 测试通过 + 笔记写完 + git 提交 → 进度表改「已完成」。

## 构建与运行（WSL 内）

工具链：`riscv64-linux-gnu-gcc`、`qemu-system-riscv64`（≥ 7.2），安装见 [docs/wsl-env.md](../docs/wsl-env.md)。

```bash
make                 # 编译 kernel 与用户程序
make qemu            # 启动 xv6（-nographic console）；退出：Ctrl-A 再按 X
make qemu CPUS=1     # 单核（调试竞态问题时更方便对照）
make clean
```

自动化测试（无需手动敲 shell）：

```bash
./test-xv6.py usertests          # 跑完整 usertests
./test-xv6.py -q usertests       # 只跑 quick 子集（日常回归）
./test-xv6.py crash              # 崩溃一致性测试
./test-xv6.py log                # 日志崩溃测试
```

## 调试

```bash
# 终端 1：启动带 gdbstub 的 QEMU（端口 26000）
make qemu-gdb

# 终端 2
gdb-multiarch kernel/kernel
(gdb) target remote localhost:26000
```

VSCode 配置见工作区 `.vscode/launch.json`（xv6 gdb attach）。通用 gdb 技巧见 [docs/gdb-debugging.md](../docs/gdb-debugging.md)。

## 目录说明

| 路径 | 说明 |
|---|---|
| `kernel/` | 内核源码（进程、内存、文件系统、陷阱、锁等） |
| `user/` | 用户态程序与系统调用桩（`usys.pl` 生成 `usys.S`） |
| `mkfs/` | 生成初始文件系统镜像 |
| `test-xv6.py` | 无交互自动化测试脚本 |
| `notes/` | 每个 lab 的笔记（模板见 docs/notes-template.md） |

## 注意

- 内核代码风格要求见课程 handout；保持 `make` 无警告。
- xv6 是教学代码，优先求简单清晰，不加不必要的功能。
