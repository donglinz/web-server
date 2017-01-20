#WebServer based on c++ and boost asio

A light web server support static HTML page, get/post method. 
support HTTP/HTTPS Protocol. 

#OS
linux(ubunt is recommended)

#Complier
Minimun : g++ 4.8 support c++ 11 stander 

#Library
boost_system ssl crypto pthread (See CMakeLists.txt) 

#Make
cd web-server 
cmake CMakeLists.txt 
make 

#Run
web/ is your website root directory. You must promise web/index.html is available. 

run as a HTTP server 
'''
./web_server ServerHttp <port number> <thread num> 
eg. web_server ServerHttp 12345 4 
'''

run as a HTTPS server 
'''
./web_server ServerHttps <port number> <thread num> <public key filename> <private key filename> 
eg. web_server ServerHttps 12345 4 server.crt server.key 
'''

if you want run server at port 1~1023, please run as a superuser or add sudo to you command. 

#DEMO
IN COMMING 

#Reference
https://www.shiyanlou.com/courses/568 

#Contact me
392183501@outlook.com 
