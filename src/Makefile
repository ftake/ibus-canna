program = ibus-engine-canna
CXX = g++
objs = engine.o main.o cannaui.o
libs = -lcanna $(shell icu-config --ldflags) $(shell pkg-config --libs ibus-1.0)
CFLAGS += -Wall -g $(shell pkg-config --cflags ibus-1.0)

.SUFFIX: .cpp .o

$(program): $(objs)
	$(CXX) -o $(program) $(libs) $^

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

.PHONY: all
all: $(program)

.PHONY: clean
clean:
	$(RM) $(program) $(objs)

.PHONY: install 
install:

.PHONY: install
uninstall: 

cannaui.o: cannaui.h
engine.o: engine.h cannaui.h
main.o: engine.h