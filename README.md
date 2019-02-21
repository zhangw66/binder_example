# binder_example
将目录放置android目录下,任意目录,执行mm -B,会得到四个可执行:
Install: out/target/product/falcon/system/bin/test_server_c
Install: out/target/product/falcon/system/bin/test_client_c
Install: out/target/product/falcon/system/bin/testClient
Install: out/target/product/falcon/system/bin/testServer
c实现更底层些.是直接与binder驱动打交道.
c++的则继承了BBinder,Ibinder等类,也是比较标准的写法.
