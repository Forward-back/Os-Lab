# CLAUDE.md

## 项目结构

`d:\os_lab` 是个人 OS 实验工作区，根目录单 git 仓库，含两个互补课程实验：

- `Nju-os/` — 南大 2024 OS 实验个人学习版。可用 lab：`kernel/`(L0 绘图)、`pstree/`(M1)、`libco/`(M2)。框架为 `abstract-machine/`（不要改）。`oslabs.mk` 的 git 目标已置空（非南大学生，不用 OJ）。
- `mit-xv6-riscv/` — MIT 6.1810 xv6 骨架，按官方顺序做 lab。个人指引在 `README.md`，上游原文在 `README`（`fs.img` 目标依赖，勿改名）。

## 环境

所有构建/测试在 WSL2 Ubuntu 24.04 内执行（Windows 侧只编辑）。安装：`bash scripts/setup-wsl.sh`（需 sudo）。

## 构建与测试

```
cd Nju-os/kernel && make && make run          # AM x86_64-qemu；make clean
cd Nju-os/pstree && make                      # 产出 pstree-64/-32；原生运行
cd Nju-os/libco && make                       # 产出 libco-64.so/-32.so
cd Nju-os/libco/tests && make test            # 协程测试
cd mit-xv6-riscv && make && make qemu         # qemu 退出：Ctrl-A 再 X
cd mit-xv6-riscv && ./test-xv6.py usertests   # 自动化测试
cd mit-xv6-riscv && make qemu-gdb             # gdb 端口 26000
```

## 约定

- 进度与「完成」定义见根 README：测试通过 + 笔记写完 + git 提交。
- 每个 lab 的笔记放 `项目/notes/<lab>.md`，模板 `docs/notes-template.md`。
- 不改 `abstract-machine/` 与 xv6 核心，除非实验本身要求；改动写进笔记。
- 状态标记（README 表格）：未开始 / 进行中 / 已完成。
