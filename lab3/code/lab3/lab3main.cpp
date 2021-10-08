#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main()
{
  pid_t pid1,pid2;
  
  std::ofstream out;
  char fname[60];
  int t1,t2,status1,status2;
  char t3[4];
  std::cout<<"Введите название файла: ";
  std::cin>>fname;
  std::cout<<"Введите задержки через пробел предка, потомка1 и потомка2:\n";
  std::cin>>t1>>t2>>t3;
  out.open(fname, out.out | out.app);
  out<<"Задержки: "<<t1<<" "<<t2<<" "<<t3<<"\n";
  out.close();
  
  switch(pid1=fork()) {
  case -1:
          perror("fork"); 
          exit(1);
           
  case 0:
		  sleep(t2);
		 
		  out.open(fname, out.out | out.app);
          out<<"------------------\nCHILD1:"<<
          "\nидентификатор процесса "<<getpid()<<
          "\nидентификатор предка "<<getppid()<<
          "\nидентификатор сессии процесса "<<getsid(getpid())<<
          "\nидентификатор группы процессов "<<getpgid(getpid())<<
          "\nреальный идентификатор пользователя "<<getuid()<<
          "\nэффективный идентификатор пользователя "<<geteuid()<<
          "\nреальный групповой идентификатор "<<getgid()<<
          "\nэффективный групповой идентификатор "<<getegid()<<"\n";
          out.close();
          exit(EXIT_SUCCESS);
          
  default:
          switch(pid2=vfork()) {
		  case -1:
				perror("vfork"); 
				exit(2);
			    
		  case 0:
				execl("child"," ",fname,t3,NULL);
				exit(EXIT_SUCCESS);
				
		  default:
				sleep(t1);
				out.open(fname, out.out | out.app);
				out<<"------------------\nPARENT:"<<
					"\nидентификатор процесса "<<getpid()<<
					"\nидентификатор предка "<<getppid()<<
					"\nидентификатор сессии процесса "<<getsid(getpid())<<
					"\nидентификатор группы процессов "<<getpgid(getpid())<<
					"\nреальный идентификатор пользователя "<<getuid()<<
					"\nэффективный идентификатор пользователя "<<geteuid()<<
					"\nреальный групповой идентификатор "<<getgid()<<
					"\nэффективный групповой идентификатор "<<getegid()<<"\n";
				out.close();
				waitpid(pid1,&status1,0);	
				waitpid(pid2,&status2,0);
          }  
	}
	return 0;
}
