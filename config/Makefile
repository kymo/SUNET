MACRO=DEBUG
CFLAGS+=-g -w -D$(MACRO) -pg -lpthread -O2 
SRCPATH=.
INCLUDEPATH=../util ../gtest/include
LIB=../gtest
SOURCES=$(wildcard $(SRCPATH)/*.cpp)
TARGET=test_config
#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)
CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))

all: build
	@echo "Build Done!"

clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES} -L${LIB} -lgtest
	g++ ${CFLAGS} -o ${TARGET} ${OBJS} -L${LIB} -lgtest
