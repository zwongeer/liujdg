# 样例

该文件夹下有一些样例：

`sample-log`为样例运行结果

_建议先看前两个_ (~~因为最后一个懒得写说明~~)

1. [最普通的样例(c++)](sample1/)
2. [最普通的样例(python3)](sample1-python3/)
3. [三子棋(c++)](ticTacToe/)
4. [五子棋(c++)](gomoku/)
5. [编译异常](bad_build_sample/)



#### 如何运行样例

1. ~~首先需要手动编译`judger`、`p1`、`p2`，直接执行`./build.sh` ，懒得写`liujdg.compile`了~~
2. 现在用`liujdg.build config.json`即可
3. 注意需要在`judger`、`p1`、`p2`文件夹下建立一个`data`文件夹，还要有一个`log`文件夹（已经在`liujdg.build`中创建了），因为`刘稼级`不会自动建立文件夹，之后将当前目录重定向到一个样例文件夹
4. 虽然说`python`不需要编译，但是还是应该在运行`python`样例前先使用一次`liujdg.build`，以创建必要的文件夹
5. 如果已经安装了`刘稼级`，可以执行`liujdg.run config.json [错误日志路径]`
6. 运行完后可以使用`./clean.sh`清除可执行文件与记录

