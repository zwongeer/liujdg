#  刘稼级

` 刘稼级`是[大一年度项目](https://aioj.ac/) 的一部分。可以运行一系列子程序，并且记录子程序的`stdout`。

` 刘稼级`被用来运行`裁判`和`玩家`，关于`裁判`和`玩家`，参见[aioj.ac 帮助](https://aioj.ac/help) 



## 构建

```bash
$ git clone https://github.com/zwongeer/liujdg.git && cd liujdg
```

考录到github在某片神奇的土地上访问较慢，如果你是在刘某人的VPS上构建，可以使用下面的命令，这样应该会快很多
_(该仓库地址只能通过刘某人的VPS访问)_
```bash
$ git clone https://cdn.aioj.ac/zwongeer/liujdg.git && cd liujdg
```

```bash
$ mkdir build && cd build
$ cmake .. && make
```

也可以

```
$ ./build.sh
```

`./build.sh`在构建完成时会提示是否安装

#### 系统

在以下系统编译通过：
- `Arch Linux`
- `CentOS 8`
- `Debian 10`
- `Ubuntu 20.04`

## 安装

_需要root权限_

```bash
$ cd build
# ./liujdg.install
```

## 运行

```
$ liujdg.run <json配置文件路径> [错误日志路径]
```

## 卸载

_需要root权限_

```
# liujdg.uninstall
```



[详细说明](doc/)

[参考样例](examples/)

[更新日志](CHANGELOG.md)
