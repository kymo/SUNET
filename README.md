### SUNET
------------------
SUNET is a light strategy server framework written in C++. 

- Non-blocking IO model
- multi-thread task queue
- algorithm modules configurable


- [Non-blocking IO model](#non-blocking-io-model)
- [Multi-thread task queue](#multi-thread-task-queue)
- [Algorithm modules Configurable](#algorithm-modules-configurable)

| System | 2.7 | 3.5 |
| --- | --- | --- |
| Linux CPU | [![Build Status](https://travis-ci.org/pytorch/pytorch.svg?branch=master)](https://travis-ci.org/pytorch/pytorch) | [![Build Status](https://travis-ci.org/pytorch/pytorch.svg?branch=master)](https://travis-ci.org/pytorch/pytorch) |
| Linux GPU | [![Build Status](http://build.pytorch.org:8080/buildStatus/icon?job=pytorch-master-py2-linux)](https://build.pytorch.org/job/pytorch-master-py2-linux) | [![Build Status](http://build.pytorch.org:8080/buildStatus/icon?job=pytorch-master-py3-linux)](https://build.pytorch.org/job/pytorch-master-py3-linux) |
| macOS CPU | [![Build Status](http://build.pytorch.org:8080/buildStatus/icon?job=pytorch-master-py2-osx-cpu)](https://build.pytorch.org/job/pytorch-master-py2-osx-cpu) | [![Build Status](http://build.pytorch.org:8080/buildStatus/icon?job=pytorch-master-py3-osx-cpu)](https://build.pytorch.org/job/pytorch-master-py3-osx-cpu) |



## Non-blocking IO Model

you can define epoll or select model as you want. the sockets are non-blocking and the epoll works under ET mode.

## Multi-thread task queue

## Algorithm modules Configurable

you can just add you own algorithm models by inherting  the abstact class IStrategy and implement the virtual function defined in it. and defined the uri that your algorithm modules work on.
