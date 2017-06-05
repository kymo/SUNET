MACRO=DEBUG
CFLAGS+=-g -w -D$(MACRO) -pg -lpthread -O2 
SRCPATH=.
INCLUDEPATH=../util ../alias/gtest/include ../http ../config/ ../alias/jsoncpp/include
LIB=-L../alias/gtest
SOURCES=$(wildcard $(SRCPATH)/*.cpp)
TARGET=test_strategy
#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)
CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))
ALIAS_OBJS=../config/strategy_conf.o \
		   ../http/http_parse.o

all: build
	@echo "Build Done!"

clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES} ${LIB} -lgtest
	g++ ${CFLAGS} -o ${TARGET} ${OBJS} ${ALIAS_OBJS} ${LIB} -lgtest ../alias/jsoncpp/lib/libjson_linux-gcc-4.4.7_libmt.a
