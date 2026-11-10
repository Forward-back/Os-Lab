# GDB 调试 C 代码规范与要点

本文档约定在 WSL/Ubuntu 环境下使用 gdb（TUI 模式）调试 C 程序的流程与规范。

> 注：本文原为调试非递归汉诺塔示例（`hanoi_nr.c`）时整理，该示例文件不在本工作区中，文中与它相关的内容仅作流程参考。Lab 调试以各自项目的构建产物为目标程序即可。

## 1. 编译规范

调试版本必须关闭优化并保留调试信息：

```bash
gcc -g -O0 -Wall -Wextra -o <program> <source>.c
```

- `-g`：生成 DWARF 调试信息，缺少它 gdb 无法把指令映射回源码行。
- `-O0`：禁止优化。`-O1` 及以上会重排/合并/内联指令，单步顺序与源码不一致，变量可能被优化进寄存器甚至消失。
- `-Wall -Wextra`：编译期警告是成本最低的 bug 来源，不要忽略。

> 不要为了调试去改源码（例如插 printf）。先用断点、单步、观察点定位，确认问题后再考虑临时插桩，且插桩代码不入库。

Nju-os 的 lab 已自带 `-ggdb -O1`（`oslabs.mk` 中），调试体验不佳时可临时加 `CFLAGS += -O0` 覆盖；xv6 自带 `-g`。

## 2. 启动方式

```bash
gdb -tui ./<program>            # 直接进入 TUI 模式
gdb ./<program>                 # 普通模式，之后 Ctrl+X A 随时切换 TUI
gdb --args ./<program> 3        # 程序带命令行参数
gdb -tui ./<program> -ex "run 3"  # 启动即运行（慎用，一般先设断点）
```

程序参数也可在 gdb 内设置：

```
(gdb) set args 3
(gdb) show args
(gdb) run
```

## 3. TUI 布局与按键

| 命令 / 按键 | 作用 |
|---|---|
| `layout src` | 源码窗口 |
| `layout asm` | 汇编窗口 |
| `layout split` | 源码 + 汇编双窗口 |
| `layout regs` | 寄存器 + 汇编窗口（汇编级调试首选） |
| `Ctrl+X 2` | 循环切换布局 |
| `Ctrl+X A` | 进入 / 退出 TUI |
| `Ctrl+L` | 花屏时重绘屏幕 |
| `focus next` / `focus prev` | 在 TUI 窗格间切换焦点，方向键随之滚动对应窗格 |
| `tui reg general` | 在 regs 布局中显示通用寄存器组 |

汇编级调试建议先设置 Intel 语法（默认为 AT&T）：

```
(gdb) set disassembly-flavor intel
```

该设置可写入 `~/.gdbinit` 持久化。

## 4. 断点规范

| 命令 | 说明 |
|---|---|
| `break main` / `b main` | 在函数入口下断点 |
| `b file.c:33` | 在指定文件行下断点（**推荐**：明确文件，多文件工程不歧义） |
| `b *0x4011a0` | 在地址下断点（无调试信息时的兜底手段） |
| `tbreak ...` | 临时断点，命中一次后自动删除 |
| `condition 1 m == 10` | 给 1 号断点加条件 |
| `ignore 1 100` | 跳过 1 号断点前 100 次命中 |
| `info breakpoints` / `i b` | 查看所有断点 |
| `delete 1` / `disable 1` / `enable 1` | 删除 / 禁用 / 启用断点 |
| `commands 1` … `end` | 断点命中时自动执行一组命令（可加 `silent` 避免刷屏） |

规范要点：

- 优先用 `文件:行号` 形式，不用裸行号。
- 循环体内下断点务必加条件，否则反复手动 `continue` 效率极低。
- 断点命中后的自动化命令（`commands`）适合做日志型跟踪，代替插 printf。

## 5. 单步命令

| 命令 | 粒度 | 函数调用行为 |
|---|---|---|
| `next` / `n` | 源码行 | 跳过（step over） |
| `step` / `s` | 源码行 | 进入（step into） |
| `finish` | 源码 | 运行至当前函数返回 |
| `until <行号>` | 源码 | 运行至指定行（常用于跳出循环） |
| `nexti` / `ni` | 单条指令 | 跳过 |
| `stepi` / `si` | 单条指令 | 进入 |
| `continue` / `c` | 运行至下一断点或结束 | — |

规范：默认用源码级 `next`/`step`；只有需要观察寄存器、调用约定、指令级行为时才切到指令级 `ni`/`si`。

## 6. 观察数据

### 变量与表达式

```
(gdb) print m                 # 或 p m
(gdb) p/x from                # 十六进制
(gdb) p pos[1]@5             # 打印 pos[1..5]，数组常用语法
(gdb) display m               # 每停下来自动显示 m
(gdb) undisplay 1
```

### 寄存器

```
(gdb) info registers          # 全部寄存器
(gdb) p/x $rax                # 单个寄存器
(gdb) display/i $pc           # 每步自动显示当前指令
```

### 内存

```
(gdb) x/4i $pc                # 当前指令起反汇编 4 条
(gdb) x/8gx &pos              # 以 8 字节十六进制查看数组内存
(gdb) x/s NAME                # 按字符串查看
```

### 调用栈

```
(gdb) backtrace / bt          # 调用栈
(gdb) frame 1                 # 切换到 1 号栈帧
(gdb) info frame              # 当前帧信息（参数、局部变量、返回地址）
(gdb) info locals             # 当前帧所有局部变量
```

### 观察点（watchpoint）

变量被意外修改时定位写入点：

```
(gdb) watch pos[k]            # 写观察点
(gdb) rwatch ...              # 读观察点
(gdb) awatch ...              # 读写观察点
(gdb) info watchpoints
```

注意：观察点依赖硬件/软件实现，变量须在作用域内且已在内存中（被优化进寄存器时可能失效——这也是必须 `-O0` 的原因之一）。

## 7. 运行时控制

```
(gdb) run / r                 # 重新开始
(gdb) kill                    # 杀死被调试进程
(gdb) signal SIGINT           # 给程序发信号
(gdb) set var m = 5          # 运行时修改变量值，用于验证假设
(gdb) jump 41                 # 跳到指定行（会跳过中间副作用，谨慎）
```

## 8. 调试流程要点

1. **先编译干净**：`-Wall -Wextra` 无警告，再开始调试。
2. **先粗后细**：`b main` → `run` → 用 `next` 走到疑似问题区域 → 再 `b 文件:行` 精确设点。
3. **一次只验证一个假设**：每轮要么看变量、要么看寄存器、要么看栈，避免同时引入多种操作干扰判断。
4. **循环用条件断点或 `until`**：不要手动连按 `c`。
5. **状态被莫名改写时上 `watch`**：直接抓写入时刻。
6. **怀疑优化问题时对比 `-O0` 与 `-O2` 行为**：优化暴露出的未定义行为（越界、有符号溢出、未初始化读取）是最常见的隐蔽 bug。
7. **段错误先看现场**：crash 后 `bt` 看调用栈，`frame N` + `info locals` 看局部变量，`x/i $pc` 看崩溃指令。
8. **调试结论落到回归测试**：修完 bug 后补一个能复现的最小用例，防止回归。

## 9. 远程调试 QEMU（本工作区常用）

裸机/内核代码运行在 QEMU 里，用 gdb 远程连接：

```bash
# MIT xv6：Makefile 自带 qemu-gdb 目标（端口 26000），另开终端
cd mit-xv6-riscv && make qemu-gdb
gdb-multiarch -ex 'target remote localhost:26000'

# Nju-os AM (x86_64-qemu)：手动带 gdbstub 启动（端口 1234），另开终端
cd Nju-os/kernel && make
qemu-system-x86_64 -s -S -serial mon:stdio -machine accel=tcg -smp 1 \
  -drive format=raw,file=build/kernel-x86_64-qemu
gdb build/kernel-x86_64-qemu.elf -ex 'target remote localhost:1234'
```

要点：RISC-V 用 `gdb-multiarch`（普通 gdb 不一定认识 riscv 目标）；`-S` 让 QEMU 等待 gdb 连接后再开始执行；`-s` 等价于 `-gdb tcp::1234`。

## 10. 常见坑

| 现象 | 原因 / 解决 |
|---|---|
| 汇编地址与源码行对不上 | 漏 `-g`，或编译用了优化（`-O2`）；重新 `-g -O0` 编译 |
| 变量显示 `<optimized out>` | 优化导致；必须 `-O0` 调试 |
| TUI 花屏、窗口错位 | `Ctrl+L` 重绘；终端窗口过窄会截断，拉宽终端 |
| 按键无响应 | 焦点不在命令窗格：`Ctrl+X O` 切换焦点，或独立 Windows Terminal 中运行 |
| `run` 后程序立即结束 | 没设断点；`b main` 后 `run` |
| 中文/符号乱码 | 检查终端编码与字体；该问题与 gdb 本身无关 |
| 断点"不生效" | 位置在未加载的共享库，或行号因优化漂移；用 `i b` 查看是否 pending |
| gdb 连不上 QEMU | 端口被占用或 QEMU 未带 `-s`；检查另一终端 `ss -tlnp | grep 26000` |

## 11. 参考配置（可选）

`~/.gdbinit` 常用设置：

```
set disassembly-flavor intel
set pagination off
set confirm off
set print pretty on
```

- `pagination off`：长输出不分页直接滚屏。
- `confirm off`：省掉退出等二次确认。
- `print pretty on`：结构体更易读。
