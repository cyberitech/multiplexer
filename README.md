compiling this library requires these C++ libs: boost::asio, boost::algorithm and libc-dev (for pthreads). to compile manually you could use: `g++ -o multiplexer.o multiplexer.cpp -lboost_system -lpthread`