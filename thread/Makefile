MACRO=DEBUGALL
CFLAGS+=-g -w -D$(MACRO) -lpthread
SRCPATH=.
INCLUDEPATH=.
INCLUDEPATH+=../config
INCLUDEPATH+=../task
TARGET=sub
SOURCES=$(wildcard $(SRCPATH)/*.cpp)

#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)
ALIAS_OBJS=../task/task_mgr.o \
		   ../config/config.o \
		   ../task/req_task.o

CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))

all: build
	@cd ../task
	@echo "Build Done!"
	@echo $(CFLAGS)
clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES} 
	g++ ${CFLAGS} -o ${TARGET} ${OBJS} ${ALIAS_OBJS}
