MACRO=DEBUGALL
CFLAGS+=-g -w -D$(MACRO)
SRCPATH=./src
TESTPATH=./test
INCLUDEPATH=./include
TARGET=sub
SOURCES=$(wildcard $(SRCPATH)/*.cpp)
SOURCES+=$(wildcard $(TESTPATH)/*.cpp)

#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)
CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))

all: build
	@echo ${INCLUDEPATH}
	@echo $(CFLAGS)

clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES}
	g++ ${CFLAGS} -o ${TARGET} ${OBJS}

