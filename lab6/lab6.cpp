#include <iostream>
#include <ctime>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <chrono>

int i=0;

//ф-ия обработки сигнала таймера
void periodicFunction(int sig)
{
	pid_t pid = fork();//создать потомка
	if(pid==0)
	{//код потомка
		time_t s = time(NULL);
		tm* timeinfo = localtime(&s);
		std::cout<<"PID потомка: "<<getpid()<<std::endl;
		std::cout<<"Дата и время: "<<asctime(timeinfo)<<std::endl;
		exit(0);
	}
	wait(&pid);
	++i;
}

//1 - период | 2 - количество  
int main(int argc, char **argv)
{
	unsigned int start_time = clock();
	unsigned int end_time;
	
	
	//переопределение обработки сигнала
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,SIGTSTP);//сигнала приостановки
	sigprocmask(SIG_SETMASK,&mask,NULL);
	signal(SIGALRM,periodicFunction);//сигнал таймера
	
		
	int period,count_period;
	period=atoi(argv[1]);
	count_period=atoi(argv[2]);
	
	
	//инициализация структуры параметров таймера
	itimerval value, ovalue;
	value.it_interval.tv_sec = period;
	value.it_interval.tv_usec = 0;
	value.it_value.tv_sec = period;
	value.it_value.tv_usec = 0;
	
	
	//работа таймера
	setitimer(ITIMER_REAL,&value,&ovalue);
	for(;;)
		pause();
	
	
	end_time = clock();
	std::cout<<"Затраченное время: "<<(float)(end_time-start_time)/CLOCKS_PER_SEC<<" секунд."<<std::endl;
	return 0;
}
