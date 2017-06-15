### SUNET
------------------
SUNET is a light strategy server framework written in C++. 

- Non-blocking IO model
- multi-thread task queue
- algorithm modules configurable


- [Non-blocking IO model](#non-blocking-io-model)
- [Multi-thread task queue](#multi-thread-task-queue)
- [Algorithm modules Configurable](#algorithm-modules-configurable)


### Non-blocking IO Model

you can define epoll or select model as you want. the sockets are non-blocking and the epoll works under ET mode. Main thread maintains the epoll loop, and if there is a new connection from client, the main thread receives the data and hand it to the work thread queue.

### Multi-thread task queue

task queue are visited by thread pool through a read-write thread lock. the work thread firstly parses the received data into Request format(url, method, params etc.), and then the strategy-mgr calls the method binded to the uri, and write the return data into Json Object.

### Algorithm modules Configurable

algorithms are exists as plugins. you can add you own add you own algorithm modules by inherting  the abstact class IStrategy and implement the virtual function defined in it. and then just defined the uri that your algorithm modules work on. 

### Usage

suppose your strategy.conf and sub.conf are defined as follows:

strategy.conf
> [/]  </br>
TextStrategy:255 </br>
RewriteStrategy:255 </br>
[/search] </br>
TextStrategy:255 </br>
RewriteStrategy:255 </br>

curl -s http://ip:port/q?

### Make

cd io
sh make.sh

## Run
./sub port



