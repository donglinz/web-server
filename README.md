<h1>WebServer based on c++ and boost asio</h1>

基于Boost Asio的多线程Web服务器。
服务器由Web服务模块，日志模块，Cache模块，XML配置模块构成
支持HTTP，HTTPS协议。HTTPS协议用asio::ssl::context对象对socket数据流进行加密。
支持静态HTML页面，GET/POST方法，可以方便的进行横向扩展以支持新的请求方法。
服务器Cache利用LRU算法进行页面替换，减少磁盘IO次数，Cache大小可以在XML配置 文件中指定。Log系统支持日志轮转，设置最大日志占用空间，定时轮转，过滤器等功能。   

<h1>OS</h1>

建议ubuntu14及以上<br>

<h1>Complier</h1>

G++5及以上
#Library dependencies   

以下库都在CMakeLists.txt中包含
> boost_system 
> ssl 
> crypto 
> pthread 
> boost_log 
> boost_thread 
> boost_log_setup 

开发环境Boost版本为1.63.0
详情见[Boost 1_63_0官方文档](http://www.boost.org/doc/)

强烈建议在本机用相同的编译器编译本程序以及Boost库,防止链接动态库时出错。
#Make   

```
cd web-server
cmake CMakeLists.txt
make
```
  

<h1>Configurations</h1>   

所有的配置都要写在WebConfig.xml文件内，并放在和编译生成文件相同目录下。WebConfig.xml.sample是一份样例配置，请按文件里的注释进行配置。   
IP黑名单暂时只能配置，并不会生效，但是很快会在后续版本里支持。

<h1>Run</h1>

运行'web-server'文件来开始服务器。

./web-server 是服务器可执行文件。 ./web/是默认的web根目录(可以在WebConfig.xml中修改). 必需保证Web根目录下有index.html文件. 否则服务器将会返回一个404页面。

Run as a HTTP/HTTPS server:<br>
```
nohup ./web_server &
```


<h1>DEMO</h1>
[http://118.190.23.140:1234/index.html](http://118.190.23.140:1234/index.html)<br>

<h1>Contact me</h1>
392183501@outlook.com<br>
