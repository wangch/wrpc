lib_dir = ../lib
inc_dir = ../include
incs = -I $(inc_dir)
libs = -L $(lib_dir) -lpthread -lboost_system -lprotobuf

srcs = $(wildcard *.cc)
objs = $(srcs:.cc=.o)

libwrpc.so: $(objs)
	g++ -shared -o $@ $^ $(libs)
	cp $@ $(lib_dir)

%.o:%.cc
	g++ -c -fPIC -std=c++0x $(incs) -o $@ $< 

clean:
	-rm *.o libwrpc.so $(lib_dir)/libwrpc.so
install:
	cp libwrpc.so /usr/local/lib/
	cp -r $(inc_dir)/wrpc /usr/local/include/

