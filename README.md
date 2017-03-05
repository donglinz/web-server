#WebServer based on c++ and boost asio
基于Boost Asio的多线程Web服务器。
服务器由Web服务模块，日志模块，Cache模块，XML配置模块构成
支持HTTP，HTTPS协议。HTTPS协议用asio::ssl::context对象对socket数据流进行加密， 通过async_handshake方法进行SSL握手。
支持静态HTML页面，GET/POST方法，可以方便的进行横向扩展以支持新的请求方法。
服务器Cache利用LRU算法进行页面替换，减少磁盘IO次数，Cache大小可以在XML配置 文件中指定。   
Log系统基于Boost Log，封装了Boost Log的部分功能。支持日志轮转，设置最大日志占用空间，定时轮转，过滤器等功能。
#OS
建议ubuntu14及以上<br>

#Complier
G++5及以上
#Library dependencies

> boost_system 
> ssl 
> crypto 
> pthread 
> boost_log 
> boost_thread 
> boost_log_setup 

(见 CMakeLists.txt).   

强烈建议在本机用相同的编译器编译本程序以及Boost库,防止链接动态库时出错。
#Make
```
cd web-server
cmake CMakeLists.txt
make
```
  

#Configurations

所有的配置都要写在WebConfig.xml文件内，并放在和编译生成文件相同目录下。WebConfig.xml.sample是一份样例配置，请按文件里的注释进行配置。
#Run
运行'web-server'文件来开始服务器。

./web-server 是服务器可执行文件。 ./web/是默认的web根目录(可以在WebConfig.xml中修改). 必需保证Web根目录下有index.html文件. 否则服务器将会返回一个404页面。

Run as a HTTP server:<br>
```
nohup ./web_server &
```


#DEMO
[http://118.190.23.140:1234/index.html](http://118.190.23.140:1234/index.html)<br>

#Contact me
392183501@outlook.com<br>
