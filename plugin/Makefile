MACRO=DEBUG
CFLAGS+=-g -w -D$(MACRO) -pg -lpthread -O2 
SRCPATH=.
INCLUDEPATH=../util \
		../alias/gtest/include \
		../http \
		../config/ \
		../alias/jsoncpp/include \
		../alias/crf/CRF++-0.53 \
		../alias/wordseg/ \
		../alias/dat/ \
		../log/ \

LIB=-L../alias/gtest
SOURCES=$(wildcard $(SRCPATH)/*.cpp)
TARGET=test_strategy
#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)
CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))
ALIAS_OBJS=../config/strategy_conf.o \
		   ../config/config.o \
		   ../http/http_parse.o \
		   ../alias/wordseg/wordseg.o \
		   ../log/sub_log.o
all: build
	@echo "Build Done!"

clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES}
	g++ ${CFLAGS} -o ${TARGET} ${OBJS} ${ALIAS_OBJS} ${LIB} -lgtest ../alias/jsoncpp/lib/libjson_linux-gcc-3.4.5_libmt.a ../alias/crf/CRF++-0.53/libs/libcrfpp.a -lpthread
