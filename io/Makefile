MACRO=DEBUGALL
CFLAGS+=-g -w -D$(MACRO) -lpthread
SRCPATH=.
INCLUDEPATH=.
INCLUDEPATH+=../util ../task ../thread ../config
TARGET=event
SOURCES=$(wildcard $(SRCPATH)/*.cpp)

#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)
ALIAS_OBJS=../task/task_mgr.o \
		   ../config/config.o \
		   ../task/req_task.o \
		   ../thread/thread_pool.o \
		   ../thread/thread.o

CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))

all: build
	@echo "Build Done!"
clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES} 
	g++ ${CFLAGS} -o ${TARGET} ${OBJS} ${ALIAS_OBJS}
