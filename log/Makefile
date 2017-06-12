MACRO=DEBUGALL
CFLAGS+=-pg -w -D$(MACRO) -lpthread
SRCPATH=.
INCLUDEPATH=. ../config ../util ../alias/gtest/include
ALIAS_O=../config/config.o
LIB=../alias/gtest
TARGET=test_log
SOURCES=$(wildcard $(SRCPATH)/*.cpp)

#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)
CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))

all: build
	@echo "Build Done!"
	@echo $(CFLAGS)
clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES} ${ALIAS_O}
	g++ ${CFLAGS} -o ${TARGET} ${OBJS} ${ALIAS_O} -L${LIB} -lgtest

