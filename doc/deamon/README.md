## liujdg.deamon

### 运行

```
liujdg.deamon <json配置文件路径>
```
- 根据json文件中的配置，启动`http`服务器（默认1984端口），提够调用`liujdg`的`api`
- 第二个参数为`-`时，会从`stdin`读取json配置
- 接受的json与`liujdg.run`不同
- 请提前建立文件夹并且正确配置权限
- `port`：http监听端口（Linux中小于等于1024端口需要特殊权限）
- `path`：数据保存路径（不是对局结果保存路径）
- `https`：启用https（默认false）（~~可以做但是现在懒得做~~）
- `cert,key`：证书和密钥路径(默认留空)
### 配置文件样例
```json
{
    "port": 1984,
    "path": "/home/attack204/.liujdg/deamon",
    "https": false
}
```

### API
实现见tools/deamon/L$(API_NAME).cxx

- /new
	- 方法：POST
	- 参数：一个JSON，与`liujdg.run`接受的JSON相同
	- 返回一个JSON，返回例如
		- 出错：`{"status": "错误类型", "message": "错误信息"}`
		  
		- 正常：`{"status": "received", "id": "对局的唯一id"}`

- /get
	- 方法：POST
	- 参数：一个JSON，包含对局的id，例如`{"id": "对局id"}`
	- 返回一个JSON，返回例如
		- 出错：`{"status": "错误类型", "message": "错误信息"}`
		- 正常：`{"status": "对局状态", "logpath": "对局的保存路径"}`
		- 注意:编译错误会保存在`liujdg.deamon`存数据的位置，而不是`logpath`中的路径
- /
	- 方法：任意
	- 参数：任意
	- 返回一串文本
- /version
	- 方法：任意
	
	- 参数：任意
	
	- 返回`刘稼级`版本
- /test
	- 保留
	
### 状态类型
```c++
"error"; // error request
"building"; // building
"buildError"; // compile errror
"runtimeError"; // runtime error
"received"; // received json
"running"; // game is running
"fulfilled"; // game is funished normally
"unknown"; //unknown error
```

