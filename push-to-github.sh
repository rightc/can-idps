#!/bin/bash
# 将 can-idps 推送到 GitHub（right_c 账户）
# 用法：./push-to-github.sh [github_username]

USER="${1:-right_c}"
REPO="can-idps"
URL="https://github.com/${USER}/${REPO}.git"

echo "准备推送到: $URL"
echo "若用户名不对，请运行: $0 你的用户名"
echo ""

cd "$(dirname "$0")"
git remote remove origin 2>/dev/null
git remote add origin "$URL"
git push -u origin main
