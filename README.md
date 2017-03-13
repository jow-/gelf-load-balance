gelf-load-balance
===============

gelf 协议 UDP负载均衡


依赖 boost 1.46.0 以上
cmake 2.8 以上

Mac
```
brew install boost
brew install cmake
```

CentOS
```
yum install boost-devel
yum install cmake
```

编译
```
mkdir build
cd build
cmake ../
make
make install

```

运行
```
gelf-load-balance -f pathto/gelf-load-balance.json
```
or
```
cd pathto/gelf-load-balance
gelf-load-balance-server.sh start
#gelf-load-balance-server.sh restart

```

目前支持 gelf UDP格式协议的分发

```
{
    "host": "127.0.0.1",
    "port": 12200,
    "upstream": [
        {
            "host": "127.0.0.1",
            "port": 12201,
            "weight": 100
        },
        {
            "host": "127.0.0.1",
            "port": 12202,
            "weight": 100
        }
    ]
}

```


gelf UDP 协议格式
```
message:
00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16
1e 0f e2 05 bc 19 7a 90 b9 eb 00 04 78 9c ed 98 c7 ce a8 48 92 85 df ...
1e 0f e2 05 bc 19 7a 90 b9 eb 01 04 c8 f0 84 31 9a a3 f8 f5 aa 17 10 ...
1e 0f e2 05 bc 19 7a 90 b9 eb 02 04 57 9d 03 91 da 1c 36 20 72 4a 75 ...
1e 0f e2 05 bc 19 7a 90 b9 eb 03 04 2b d8 c0 af a5 4d 20 b5 cb f4 af ...


00 type (zlib, gzip, chunked)
  0x78 zlib
  0x1f gzip
  0x1e chunked


chunked message: 

0a index
0b total
02-08 id

```

