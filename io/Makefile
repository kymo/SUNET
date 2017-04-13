MACRO=DEBUGALL
CFLAGS+=-g -w -D$(MACRO) -lpthread -O2
SRCPATH=.
INCLUDEPATH=.
INCLUDEPATH+=../util ../task ../thread ../config ../http
TARGET=event
SOURCES=$(wildcard $(SRCPATH)/*.cpp)

#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)
ALIAS_OBJS=../task/task_mgr.o \
		   ../config/config.o \
		   ../task/req_task.o \
		   ../thread/thread_pool.o \
		   ../thread/thread.o \
		   ../http/http_parse.o

CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))

all: build
	@echo "Build Done!"
clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES} 
	g++ ${CFLAGS} -o ${TARGET} ${OBJS} ${ALIAS_OBJS}
