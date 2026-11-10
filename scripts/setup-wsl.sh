#!/usr/bin/env bash
# WSL2 Ubuntu 24.04 工具链安装（幂等，可重复执行）
# 用法: bash scripts/setup-wsl.sh
set -euo pipefail

sudo apt-get update
sudo apt-get install -y \
  build-essential gcc-multilib gdb gdb-multiarch \
  gcc-riscv64-linux-gnu \
  qemu-system-x86 qemu-system-misc \
  python3 git curl

echo "--- 验证 ---"
for t in gcc make qemu-system-x86_64 qemu-system-riscv64 \
         riscv64-linux-gnu-gcc gdb gdb-multiarch python3 git; do
  if command -v "$t" >/dev/null 2>&1; then
    printf 'OK   %s\n' "$t"
  else
    printf 'MISS %s\n' "$t"
  fi
done

echo "--- QEMU 版本（xv6 要求 >= 7.2）---"
qemu-system-riscv64 --version | head -1
