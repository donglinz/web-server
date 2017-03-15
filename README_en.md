# Multithreading Web Server Based on Boost Asio. 

# Description
Multithreading Web Server Based on Boost Asio.   

> The server is composed of Web service module, log module, cache module and XML configuration module.
> Support HTTP, HTTPS protocol. In HTTPS protocol data stream was encrypted by asio::ssl::context object.
> Support static HTML page, GET/POST method. Server can be easily extended horizontally to support the new request method.
> The server cache uses the LRU algorithm for page replacement to reduce the frequency of IO operation, cache size is configurable. 
> Log system supports log rotation, set the maximum log space, timing rotation, filters and other functions. 

# OS

Ubuntu 14.04 or higher

# Complier

G++5 or higher
# Library dependencies   

All the lib below should be added to CMakeLists.txt
> boost_system 
> ssl 
> crypto 
> pthread 
> boost_log 
> boost_thread 
> boost_log_setup 

My boost lib version is 1.63.0, so boost 1.63.0 or higher is recommended.
For more infromation, See [Boost 1_63_0 doc](http://www.boost.org/doc/).

It is strongly recommended to compile the program and your Boost library with the same compiler on your computer to prevent errors when linking the Dynamic-Link librarys.
# Make

```
cd web-server
cmake CMakeLists.txt
make
```
  

# Configurations

All the configuration is written in the file 'WebConfig.xml' and placed in the same directory as the build file. 'WebConfig.xml.sample' is a sample configuration, please follow the comments to complete your config.

IP black list  will be supported soon.

# Run

Run the executable file 'web-server'

./web-server is your executable file.  ./web/ is your default web root directory(you can set it in the config file). You must ensure that there is an index.html file as your home page in the Web root directory, otherwise the server will return a 404 page

Run as a HTTP/HTTPS server:<br>
```
nohup ./web_server &
```


# DEMO

[http://118.190.23.140:1234/index.html](http://118.190.23.140:1234/index.html)<br>

# Contact me
392183501@outlook.com<br>
