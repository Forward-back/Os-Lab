# WSL 环境与开发形态

本工作区统一在 **WSL2 (Ubuntu 24.04)** 内构建、运行与调试，Windows 侧只做编辑。VSCode 通过 Remote - WSL 连接。

## 1. 一次性安装

```bash
bash scripts/setup-wsl.sh   # 需要输入 sudo 密码
```

安装的包及用途：

| 包 | 用途 |
|---|---|
| `build-essential` | gcc/make 等基础构建链 |
| `gcc-multilib` | Nju-os 的 32 位构建（`-m32`） |
| `gcc-riscv64-linux-gnu` | xv6 交叉编译（Makefile 自动识别 `riscv64-linux-gnu-` 前缀） |
| `qemu-system-riscv64`（`qemu-system-misc`） | 运行 xv6（要求 ≥ 7.2） |
| `qemu-system-x86_64`（`qemu-system-x86`） | 运行 Nju-os AM（`ARCH=x86_64-qemu`） |
| `gdb` / `gdb-multiarch` | 本机调试 / RISC-V 远程调试 |
| `python3` | `mit-xv6-riscv/test-xv6.py` 自动化测试 |

## 2. 验证

```bash
qemu-system-riscv64 --version     # ≥ 7.2
riscv64-linux-gnu-gcc --version
qemu-system-x86_64 --version
```

冒烟测试：

```bash
cd mit-xv6-riscv && make -j && echo $?     # 编译通过
cd Nju-os/kernel && make && echo $?        # 编译通过
```

## 3. VSCode 远程开发

1. 安装扩展 **WSL**（ms-vscode-remote.remote-wsl）。
2. 在 WSL 终端执行 `code /mnt/d/os_lab`（或 VSCode 命令面板：`WSL: Open Folder in WSL`）。
3. 工作区 `.vscode/` 已配置好 Include 路径与两个 QEMU 调试配置：
   - **xv6 gdb**：先 `make qemu-gdb`，再启动调试（attach localhost:26000）
   - **AM x86_64 qemu gdb**：先用 `-s -S` 参数启动 QEMU，再 attach localhost:1234
4. 远程会话里的集成终端默认就是 WSL bash，直接 `make` 即可。

## 4. 注意事项

- 项目在 `/mnt/d`（Windows 盘），跨文件系统 IO 比 WSL 原生目录慢。构建慢时可以把仓库放到 WSL 内（`~/os_lab`），但注意两边不要同时编辑。
- 构建产物（`build/`、`*.o`、生成的可执行文件）已在 `.gitignore` 中排除。
- QEMU 退出方式：
  - xv6（`-nographic` console）：`Ctrl-A` 再按 `X`
  - AM x86_64：关闭 QEMU 窗口，或终端前台时 `Ctrl-C`（AM 用 `-serial mon:stdio` 时可 `Ctrl-A X`）
- 没有图形界面时 AM 的窗口无法显示（WSLg 正常时可以直接弹出 QEMU 窗口）。

## 5. 已知环境事实（2026-11 记录）

- WSL 发行版：Ubuntu 24.04.4 LTS（WSL2）
- 宿主 Windows 11；VSCode 原生 Windows 版 + WSL 扩展
- Windows 侧另有 mingw gcc（`E:/mingw`），仅用于语法检查，不作为实验构建工具
