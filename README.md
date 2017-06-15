### SUNET
------------------
SUNET is a raw light strategy server framework written in C++.

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

sub.conf
> crf_model:/*/SUNET/alias/wordseg/model </br>
port:9000 </br>
thread_cnt:10 </br>
log_level:15 </br>
log_path:./log/ </br>

You can start the server just by run [./sub port]. then you can visit the defined uri in you strategy.conf. for exammple.

> curl -s http://ip:port/?q=南京市长江大桥 

the response data should be:

> {  </br>
   "rewrite" : "yes",  </br>
   "seg" : "南京市_长江_大桥_",  </br>
   "text" : "yes"   </br>
}

### Make

cd io
sh make.sh



