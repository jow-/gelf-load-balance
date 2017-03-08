gelf-load-balance
===============

gelf 协议 UDP负载均衡


依赖 boost 1.46.0 以上
cmake 2.8 以上


```
brew install boost
brew install cmake

```

编译
```
mkdir build
cd build
cmake ../
make

```

运行
```
./gelf-load-balance -f pathto/gelf-load-balance.json
```

目前支持 gelf UDP格式协议的分发

TODO: Chunked Message