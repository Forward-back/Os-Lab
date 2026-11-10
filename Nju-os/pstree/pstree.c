#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// M1 任务：实现 pstree。
// - 扫描 /proc/<pid>/stat 与 /proc/<pid>/task 构建进程树
// - 支持选项：-p、--show-pids；-n、--numeric-sort；-V、--version
// 设计要点与验证方法见 notes/M1-pstree.md。

static const struct option table[] = {
    {"show-pids",    no_argument, NULL, 'p'},
    {"numeric-sort", no_argument, NULL, 'n'},
    {"version",      no_argument, NULL, 'V'},
    {0, 0, 0, 0},
};

int main(int argc, char *argv[]) {
  int o;
  while ((o = getopt_long(argc, argv, "-pnV", table, NULL)) != -1) {
    switch (o) {
      case 'p':
        // TODO: 输出中显示 pid
        break;
      case 'n':
        // TODO: 兄弟进程按 pid 数值排序
        break;
      case 'V':
        printf("pstree: own implementation, version 0.0\n");
        return 0;
      default:
        fprintf(stderr, "Usage: %s [-pnV]\n", argv[0]);
        return 1;
    }
  }

  // TODO: 构建进程树并打印
  return 0;
}
