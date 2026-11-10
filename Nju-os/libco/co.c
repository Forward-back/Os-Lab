#include "co.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// M2 任务：实现用户态协程库。接口见 co.h，测试见 tests/。
// 设计要点：协程状态机（NEW/RUNNING/WAITING/DEAD）、独立栈、
// 上下文切换（setjmp/longjmp、ucontext 或内联汇编）。
// 设计决策与踩坑记录见 notes/M2-libco.md。

struct co {
  const char *name;
};

static __attribute__((noreturn)) void not_implemented(const char *fn) {
  fprintf(stderr, "%s: not implemented yet\n", fn);
  abort();
}

struct co *co_start(const char *name, void (*func)(void *), void *arg) {
  (void)name;
  (void)func;
  (void)arg;
  not_implemented("co_start");
}

void co_yield() {
  not_implemented("co_yield");
}

void co_wait(struct co *co) {
  (void)co;
  not_implemented("co_wait");
}
