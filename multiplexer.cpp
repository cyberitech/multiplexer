#ifndef MULTIPLEXER_CPP
#define MULTIPLEXER_CPP
#include <cstring>
#include <string>
#include <stdio.h>
#include <vector>
#include <boost/asio/thread_pool.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/io_service.hpp>


inline bool isNumber(const char*  s)
{
    size_t l = std::strlen(s);
    for (int i = 0; i < l; i++)
        if (isdigit(s[i]) == false)
            return false;

    return true;
}

inline void RunProg(const char* prog){
    FILE *fp;
    char* line = nullptr;
    size_t str_len=0;
    /* Open the command for reading. */
    fp = popen(prog, "r");
    if (fp == nullptr) {
        printf("Failed to run command: %s\n",prog);
        return;
    }
    /* Read the output a line at a time - output it. */
    while ((getline(&line, &str_len, fp)) != -1) {
        printf("%s", line);
    }
    /* close */
    pclose(fp);
}


void DoThreadPool(uint32_t max_workers,std::vector<std::string> progs)
{
    printf("[+] initializing a thread pool of %d workers\n",max_workers);
    boost::asio::thread_pool threadpool(max_workers);
    for (auto iter=progs.begin(); iter!=progs.end(); ++iter)
    {
        const char* arg = iter->c_str();
        boost::asio::post(threadpool,[arg] { return RunProg(arg); });
    }
    printf("[+] %d programs are queued.  Processing now\n\n",max_workers);
    threadpool.join();  //block until complete
}


inline bool TestArgs( int argc,  char **argv){ return (argc==3 && 0==std::strcmp(argv[1],"-n") && isNumber(argv[2]) && 0<std::stoi(argv[2]));}
inline int Fail( char **argv)
{
    printf("\n\nUsage:\n\n\tsomeprogram.o | %s -n [threads_max]\n\n"
                 "This program will ony read from a pipe.  "
                 "It expects to have a list of commands be piped in from another program.\n"
                 "It will read these commands line by line and queue them in a thread pool of size [max_threads]\n"
                 "Thus, a [max_threads] of size 10 will execute at most 10 processes concurrently\n"
                 "As processes die the next process will be queued.\n"
                 "This process repeats until all processes are dead\n\n",argv[0]);
    return 1;
}

int main(int argc, char **argv)
{
    std::vector<std::string> progs;
    uint32_t max_workers;
    size_t str_len=0;
    char* line = nullptr;

    if (!TestArgs(argc,argv))
        return Fail(argv);

    max_workers=std::stoi(argv[2]);
    printf("[+] maximum procs set to %d\n",max_workers);
    printf("[+] Reading from stdin to learn what programs to run (hint: you should pipe commands to this program)...\n");
    while (getline(&line, &str_len, stdin) != -1) {
        progs.emplace_back(line);
    }
    if (!progs.empty())
        DoThreadPool(max_workers,progs);
    else
        printf("[-] Nothing to do here.  nothing was read from the pipe to execute.\n");
    printf("\n\n[*] Program done.  Exiting.\n\n");
    return 0;
}
#endif