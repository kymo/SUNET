MACRO=DEBUGALL
CFLAGS+=-g -w -D$(MACRO) -lpthread
SRCPATH=.
INCLUDEPATH=.
INCLUDEPATH+=../config
INCLUDEPATH+=../thread
TARGET=sub
SOURCES=$(wildcard $(SRCPATH)/*.cpp)

#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)
OBJS+=../config/config.o
CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))

all: build
	@echo "Build Done!"
	@echo $(CFLAGS)
clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES}
	g++ ${CFLAGS} -o ${TARGET} ${OBJS}

