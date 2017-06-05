MACRO=DEBUGALL
CFLAGS+=-g -w -D$(MACRO) -lpthread -O2 -pg
SRCPATH=.
INCLUDEPATH=.
INCLUDEPATH+=../util ../task ../thread ../config ../http ../plugin ../alias/jsoncpp/include
TARGET=sub
SOURCES=$(wildcard $(SRCPATH)/*.cpp)

#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)
ALIAS_OBJS=../task/task_mgr.o \
		   ../config/config.o \
		   ../task/req_task.o \
		   ../thread/thread_pool.o \
		   ../thread/thread.o \
		   ../http/http_parse.o \
		   ../plugin/strategy_mgr.o \
		   ../plugin/text_strategy.o \
		   ../plugin/rewrite_strategy.o \
		   ../config/strategy_conf.o

CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))

all: build
	@echo "Build Done!"
clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES} ../alias/jsoncpp/lib/libjson_linux-gcc-4.4.7_libmt.a  
	g++ ${CFLAGS} -o ${TARGET} ${OBJS} ${ALIAS_OBJS} ../alias/jsoncpp/lib/libjson_linux-gcc-4.4.7_libmt.a 
	#g++ ${CFLAGS} -o -pg ${TARGET} ${OBJS} ${ALIAS_OBJS}
