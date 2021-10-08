#include <iostream>
#include <fstream>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>

const char* ofname1="CHILD1.txt";
const char* ofname2="CHILD2.txt";
FILE* ifile = fopen("TEXT.txt","r");

//передача данных по каналу
int main()
{
	int fildes[2];
	pid_t pid1,pid2;
	char buf[64];
	
	
	//добавление в маску проца необходимых сигналов синхронизации
	sigset_t mask;
	sigaddset(&mask,SIGQUIT);
	sigaddset(&mask,SIGUSR1);
	sigaddset(&mask,SIGUSR2);
	sigprocmask(SIG_BLOCK,&mask,NULL);
	
	
	//создание канала
	if(pipe2(fildes,O_NONBLOCK)==-1)
	{// массив fildes, [0] для чтения и [1] для записи
		exit(001);
	}
	else
	{
		if(!(pid1=fork()))
		{//создание потомка 1
			std::cout<<"Start child 1"<<std::endl;
			close(fildes[1]);
			execl("child1","child1",&fildes[0],ofname1,NULL);
		}
		else
		{//создание потомка 2
			if(!(pid2=fork()))
			{
				std::cout<<"Start child 2"<<std::endl;
				close(fildes[1]);
				execl("child2","child2",&fildes[0],ofname2,NULL);
			}
		}
		
		
		//запись данных из файла в канал
		while(fgets(buf,64,ifile))
		{
			write(fildes[1],buf,strlen(buf));
		}
		
		
		//после записи данных передача сигнала об окончании записи
		std::cout<<"Parent was end write all data"<<std::endl;
		kill(0,SIGQUIT);
		
		
		//ожидание завершения потомков
		waitpid(pid1,NULL,0);
		std::cout<<"End child 1"<<std::endl;
		waitpid(pid2,NULL,0);
		std::cout<<"End child 2"<<std::endl;
		
		
		//закрытие канала
		sigprocmask(SIG_UNBLOCK,&mask,NULL);
		close(fildes[0]);
		close(fildes[1]);
	}
	
	return 0;
}
