# Git 提交流程与规范

本工作区为**根目录单 git 仓库**，两个课程实验、文档与笔记共用一条历史。远端：`https://github.com/Forward-back/Os-Lab`（`main` 已设 upstream）。

## 1. 提交时机

- **一个逻辑单元一次提交**：一次只做一件事（实现某功能、修某 bug、写下某篇文档、更新进度表）。不要混合无关改动。
- **lab 完成时**：代码 + 笔记 + 根 README 进度表更新，放在**同一次提交**里。这正是「完成判定 = 测试通过 + 笔记写完 + git 提交」的落地方式。
- 过程可以多次小提交（WIP 修复、调试结论），完成时保证历史能读懂即可。

## 2. 提交前检查（每次必做）

```bash
git status                    # 确认改动范围符合预期
git diff                      # 过一遍未暂存改动
git diff --cached             # 过一遍已暂存改动
git add <具体文件>            # 优先按文件名添加，不用 git add -A
```

自查清单：

- [ ] 没有把构建产物带进来（`build/`、`*.o`、`*.d`、`*.img`、生成的二进制/`.so`/`.asm`/`.sym` 都已被 `.gitignore` 排除，`git status` 里不应出现）
- [ ] 没有机密（token、密码、私钥）；`oslabs.mk` 里的 `TOKEN := ???` 是占位符，不要填真值
- [ ] 笔记与进度表是否需要随本次改动一起更新

## 3. 提交消息格式

类型前缀 + 中文简述，空行后写清「为什么」：

```
feat: L0 完成 BMP 图像绘制

支持 32 位 BMP 解码与最近邻缩放，居中绘制到 640x480 屏幕。
```

| 前缀 | 用途 |
|---|---|
| `feat` | 新功能 / lab 实现推进 |
| `fix` | 修 bug |
| `docs` | 文档、笔记、进度表 |
| `chore` | 构建、配置、脚本、清理 |
| `refactor` | 不改变行为的结构调整 |
| `test` | 测试相关 |

要求：标题不超过 ~50 字、不加句号；正文说明动机与影响。示例：

```bash
git commit -m "$(cat <<'EOF'
feat: M1 pstree 实现 /proc 扫描与树状打印

支持 -p/-n/-V 选项；孤儿进程挂到 1 号进程下。
EOF
)"
```

## 4. 分支策略

- 个人独作，**直接提 `main`**，不强制 PR。
- 大改/试验性方案（例如重写 libco 上下文切换）可开临时分支，验证通过后合并：

```bash
git switch -c exp/libco-ucontext
# ... 开发、提交 ...
git switch main && git merge --no-ff exp/libco-ucontext
git branch -d exp/libco-ucontext
```

## 5. 推送

```bash
git push                # 首推已完成：git push -u origin main
```

- 推送前确认远端无他人新提交；出现分叉时用 `git pull --rebase` 后重推，**不要强推 `main`**。
- 已推送的提交**不改写**（不 `--amend`、不 `rebase -i`）；有错就新提一个修复提交。

## 6. 回滚与纠错

| 场景 | 做法 |
|---|---|
| 丢弃工作区未暂存改动 | `git restore <file>` |
| 取消暂存 | `git restore --staged <file>` |
| 撤销某次已提交改动（已推送） | `git revert <hash>` |
| 回到历史版本查看 | `git switch --detach <hash>`（看完 `git switch main`） |
| 误删文件恢复 | `git restore <file>`；已提交过的都能找回 |

## 7. 与本工作区约定的联动

- 开始 lab：README 状态改「进行中」→ 提交（可并入后续提交）。
- lab 完成：测试通过 + `notes/<lab>.md` 写完 + README 状态改「已完成」→ 一次提交。
- 笔记模板：`docs/notes-template.md`。
- 完成判定细则：根 [README.md](../README.md)。
