MACRO=DEBUGALL
CFLAGS+=-g -w -D$(MACRO) 
SRCPATH=.
INCLUDEPATH=.
INCLUDEPATH+=../util
INCLUDEPATH+=../io
INCLUDEPATH+=../thread

SOURCES=$(wildcard $(SRCPATH)/*.cpp)

#OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
OBJS=$(SOURCES:.cpp=.o)

CFLAGS+=$(foreach dir, $(INCLUDEPATH), -I$(dir))
ALIAS_OBJS=../io/select.o
all: build
	@cd ../task
	@echo "Build Done!"
	@echo $(CFLAGS)
clean:
	$(RM) $(OBJS) $(TARGET)

build:
	g++ -c ${CFLAGS} ${SOURCES} 
	#g++ ${CFLAGS} -o ${TARGET} ${OBJS} ${ALIAS_OBJS}
