以下对此线程池进行技术分析

特性
	1. 多个test实例，例子使用shell脚本调用，便于多次的多角度地测试同一应用程序
	2. 仅支持Linux POSIX平台开发
	3. 仅支持一个进程内，创建唯一的、独立的线程库
	4. 不支持动态调节线程池大小，静态创建线程数目

顺序
	1. 功能实现
	2. 数据结构
	3. 调用关系
	4. 性能分析

1. 功能实现
	具体直接看API头文件即可。

​```sequence
Andrew->China: Says Hello
Note right of China: China thinks\nabout it
China-->Andrew: How are you?
Andrew->>China: I am good thanks!
```