#include <iostream>
#include <sys/shm.h>
#include <algorithm>
#define FNAME "TEXT.txt"
#define KEY 777
#define NUM_PROCESS 3
#define FIRST 0
#define SECOND 1
#define THIRD 2

struct shared
{
	int choosing[NUM_PROCESS];//флаг ожидания выдачи номерка
	int number[NUM_PROCESS];//номерок в очереди
	int stop;
};

int SID=-1;

void getSegment();//запрос на сегмент
void* includeSegment();//включение сегмента в пространство процесса
void excludeSegment(void*);//исключение 
void destructSegment(int);//уничтожение сегмента
void lock(shared*,int);//алгоритм пекарни доступа к сегементу
void unlock(shared*,int);//отклчение инетерса к сегемнту

void lock(shared* share,int proc_i)
{
	//выдача и расчет номерка
	share->choosing[proc_i]=1;
	share->number[proc_i]=1+std::max({share->number[FIRST],share->number[SECOND],share->number[THIRD]});
	share->choosing[proc_i]=0;
	
	for(int i=0;i<NUM_PROCESS;++i)
	{
		if(i!=proc_i)
		{
			//ждем пока процесс i получит номерок
			while(share->choosing[i]);
			//ждем пока все процессы с меньшим номером закончат работу
			while(share->number[i]!=0 && 
					(share->number[proc_i]>share->number[i] ||
					(share->number[proc_i]==share->number[i] &&
					proc_i>i)));
		}
	}
}

void unlock(shared* share,int proc_i)
{
	share->number[proc_i]=0;
}

void getSegment()
{
	SID=shmget(KEY,sizeof(shared),0666|IPC_CREAT);
	if(SID==-1)
	{
		perror("error in function [shmget()]");
	}
}

void* includeSegment()
{
	void* segment_addr=shmat(SID,NULL,0);
	if(*(int*)segment_addr==-1)
	{
		perror("error in function [shmat()]");
	}
	return segment_addr;
}

void excludeSegment(void* segment_addr)
{
	if(shmdt(segment_addr) == -1)
	{
		perror("error in function [shmdt()]");
	}
}

void destructSegment()
{
	if(shmctl(SID,IPC_RMID,NULL)==-1)
	{
		perror("error in function [shmctl()]");
	}
}
