#WebServer based on c++ and boost asio

A light weight web server supporting static HTML page, get/post method.<br>
Support HTTP/HTTPS Protocol.<br>

#OS
Linux(ubunt is recommended).<br>

#Complier
Minimun require : g++ 4.8 support c++ 11 stander.<br>

#Library dependencies
boost_system ssl crypto pthread (See CMakeLists.txt).<br>

#Make
```bash
cd web-server 
cmake CMakeLists.txt 
make 
```

#Run
Run file 'web-server' to start web server.   

./web-server is your executable web server file and ./web/ is your website root directory. You must promise ./web/index.html is available. otherwise a 404 code will be responded.   

Preceding file and directory can be saved in any place in your computer.   

Run as a HTTP server:<br>
```
./web_server ServerHttp <port number> <thread num> 
```
eg.<br>
```
./web_server ServerHttp 12345 4
```
Run as a HTTPS server:<br>
```
./web_server ServerHttps <port number> <thread num> <public key filename> <private key filename>
```
eg.<br>
```
./web_server ServerHttps 12345 4 server.crt server.key 
```
If you want run server at port 1~1023, please run as a superuser or add sudo to you command.<br>

#DEMO
[http://118.190.23.140:1234/index.html](http://118.190.23.140:1234/index.html)<br>

#Contact me
392183501@outlook.com<br>
