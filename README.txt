compiling this library requires these C++ libs: boost::asio, boost::algorithm and libc-dev (for pthreads). to compile manually you could use: 

    g++ -o multiplexer.o multiplexer.cpp -lboost_system -lpthread

$ for i in {1..25}; do echo "echo $i" >> commands.txt; done && cat commands.txt | ./multiplexer -n 5
[+] maximum procs set to 5
[+] Reading from stdin to learn what programs to run (hint: you should pipe commands to this program)...
[+] initializing a thread pool of 5 workers
[+] 5 programs are queued.  Processing now

1
5
2
6
7
8
9
10
4
3
11
12
13
14
15
16
18
19
17
20
21
22
23
24
25


[*] Program done.  Exiting.

