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
  //создание процесса потомка1 и предка
  switch(pid1=fork()) {
  case -1://проверка ошибок при создании процесса
          perror("fork"); 
          exit(1);
           
  case 0://код выполнения процессом потомком1
		  sleep(t2);//регулируемая задержка потомка1
		  //вывод в файл информации о потомке1
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
          exit(EXIT_SUCCESS);//успешное завершение потомка1
          
  default://ко двыполнения предка
          switch(pid2=vfork()) {//создание потомка2
		  case -1://обработка ошибки создания потомка2
				perror("vfork"); 
				exit(2);
			    
		  case 0:
		        //переход к выполнению кода другой программы с передачей параметров
				execl("child"," ",fname,t3,NULL);
				exit(EXIT_SUCCESS);//успешное завершение потомка2
				
		  default://код программы предка
				sleep(t1);//задержка предка
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
				waitpid(pid1,&status1,0);//ожидание завершения потомка1	
				waitpid(pid2,&status2,0);//ожидание завершения потомка2
          }  
	}
	return 0;
}
