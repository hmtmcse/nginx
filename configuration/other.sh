#!/bin/sh
cd /usr/local/nginx/sbin/
./nginx -s stop
mv  /usr/local/nginx/conf/nginx.conf /tmp/
cd /root/nginx
rm -rf nginx-1.6.2
tar -xzf nginx-1.6.2.tar.gz
cd /root/nginx/nginx-1.6.2
./configure --with-debug   --add-module=/root/nginx/example/autobill --add-module=/root/nginx/example/echo-nginx-module
make
rm -rf /usr/local/nginx/*
make install
mv /tmp/nginx.conf /usr/local/nginx/conf/
cd /usr/local/nginx/sbin/
./nginx





