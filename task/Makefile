MACRO=DEBUGALL
CFLAGS+=-g -w -D$(MACRO) -pg
SRCPATH=.
INCLUDEPATH=.
INCLUDEPATH+=../config ../thread ../http ../io ../util ../alias/jsoncpp/include ../plugin
TARGET=sub
SOURCES=$(wildcard $(SRCPATH)/*.cpp)

#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)
OBJS+=../config/config.o ../http/http_parse.o
CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))

all: build
	@echo "Build Done!"
	@echo $(CFLAGS)
clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES} ../alias/jsoncpp/lib/libjson_linux-gcc-4.4.7_libmt.a 
	#g++ ${CFLAGS} -o ${TARGET} ${OBJS}

