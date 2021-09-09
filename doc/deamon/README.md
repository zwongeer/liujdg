## liujdg.deamon

### 运行

```
$ liujdg.deamon <json配置文件路径>
```

- 根据json文件中读取配置，并启动`http`服务器（默认1984端口），用于用`api`调用`liujdg`
- 第二个参数为`-`时，会从`stdin`读取json配置
- 接受的json与`liujdg.run`不同
- 暂未实现全部功能