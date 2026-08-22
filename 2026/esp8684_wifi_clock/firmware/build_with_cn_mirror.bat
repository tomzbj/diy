@echo off
REM Espressif China mirror for GitHub release assets used by idf_tools.py
REM Bypass local proxy (v2ray etc.) for domestic CDN — otherwise downloads stall/crawl.
set "IDF_GITHUB_ASSETS=dl.espressif.cn/github_assets"
set "NO_PROXY=dl.espressif.cn,dl.espressif.com,.espressif.cn,.espressif.com,ghproxy.net,localhost,127.0.0.1"
set "no_proxy=%NO_PROXY%"
set "UV_INDEX_URL=https://mirrors.aliyun.com/pypi/simple"
set "UV_DEFAULT_INDEX=https://mirrors.aliyun.com/pypi/simple"
set "PIP_INDEX_URL=https://mirrors.aliyun.com/pypi/simple"

echo IDF_GITHUB_ASSETS=%IDF_GITHUB_ASSETS%
echo NO_PROXY=%NO_PROXY%
platformio -f -c eclipse run %*
