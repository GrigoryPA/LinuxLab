#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	std::ofstream out;
    out.open(argv[1], out.out | out.app);
	sleep(atoi(argv[2]));
	out<<"------------------\nCHILD2:"<<
          "\nидентификатор процесса "<<getpid()<<
          "\nидентификатор предка "<<getppid()<<
          "\nидентификатор сессии процесса "<<getsid(getpid())<<
          "\nидентификатор группы процессов "<<getpgid(getpid())<<
          "\nреальный идентификатор пользователя "<<getuid()<<
          "\nэффективный идентификатор пользователя "<<geteuid()<<
          "\nреальный групповой идентификатор "<<getgid()<<
          "\nэффективный групповой идентификатор "<<getegid()<<"\n";
    out.close();			  
	return 0;
}
