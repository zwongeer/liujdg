#  刘稼级
`liujdg` is the abbreviation for `Liu Laoye's judge`.

` 刘稼级`是[大一年度项目](https://aioj.ac/) 的一部分。可以运行一系列子程序，并且记录子程序的`stdout`。

` 刘稼级`被用来运行`裁判`和`玩家`，关于`裁判`和`玩家`，参见[aioj.ac 帮助](https://aioj.ac/help) 

## 构建
```bash
git clone https://github.com/zwongeer/liujdg.git && cd liujdg
```
#### 安装依赖与构建

- 沙盒支持（仅Linux）

如果不想使用沙盒，可以将`include/LGame.hpp`中的`#define LIUJDG_USESANDBOX`改为false

```bash
sudo apt install firejail
sudo pacman -S firejail
```


- Debian/Ubuntu
```bash
sudo apt install cmake build-essential python3 libfmt-dev librestinio-dev libboost-thread-dev libboost-chrono-dev libsoci-dev libsoci-sqlite3-4.0 libsoci-core4.0 rapidjson-dev nlohmann-json3-dev libasio-dev libsqlite3-dev -y
```
- Arch Linux
```bash
sudo pacman -S boost cmake nlohmann-json rapidjson restinio
yay -S soci
```

- Windows (msys)
```bash
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-python mingw-w64-x86_64-fmt mingw-w64-x86_64-boost mingw-w64-x86_64-nlohmann-json mingw-w64-x86_64-rapidjson restinio mingw-w64-x86_64-soci
vcpkg install restinio:x64-mingw-static # 在普通的控制台
```

考录到github在某片神奇的土地上访问较慢，如果你是在刘某人的VPS上构建，可以使用下面的命令，这样应该会快很多
_(该仓库地址只能通过刘某人的VPS访问)_

```bash
git clone https://cdn.aioj.ac/zwongeer/liujdg.git && cd liujdg
```
- Linux下构建

```
./configure
./mbuild.sh
```

`./mbuild.sh`在构建完成时会提示是否安装

- Windows(msys)下构建

```
scripts/msys_configure.sh [vcpkg toolchain 路径] [vcpkg triplet]
scripts/msys_build.sh
```

## 安装

_需要root权限_

```bash
cd build
./liujdg.install
```

## 运行

```
liujdg.run <json配置文件路径> [错误日志路径]
```

## 卸载

_需要root权限_

```
liujdg.uninstall
```



[详细说明](doc/)

[参考样例](examples/)

[更新日志](CHANGELOG.md)
