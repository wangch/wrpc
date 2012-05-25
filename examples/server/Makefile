lib_dir = ../../lib
inc_dir = ../../include
incs = -I $(inc_dir)
libs = -L $(lib_dir) -lpthread -lboost_system -lprotobuf -lwrpc

srcs = $(wildcard *.cc)
objs = $(srcs:.cc=.o)

server: $(objs)
	g++ -o $@ $^ $(libs)

%.o:%.cc
	g++ -c -fPIC -std=c++0x $(incs) -o $@ $< 

clean:
	 -rm *.o server
