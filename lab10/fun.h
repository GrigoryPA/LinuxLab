#include <iostream>
#include <sys/sem.h>
#include <algorithm>
#include <semaphore.h>
#define FNAME "TEXT.txt"
#define KEY 999
#define WRITE_COUNT 5


int SID=-1;

void getSemafor();//открыть\создать семафор
void destructSemafor();//уничтожение семафора


void getSemafor()
{
	
	std::cout<<"Подготовка к Семафор открыть"<<std::endl;
	SID=semget(KEY,4,0666);
	if(SID<0)
	{
		std::cout<<"Подготовка к Семафор создать"<<std::endl;
		SID=semget(KEY,4,0666|IPC_CREAT);
		if(SID<0)
		{
			perror("error in function [semget()]");
			exit(1);
		}
		std::cout<<"Семафор создан"<<std::endl;
		//первый семафор в 1 - ресурс свободен на запись, в 0 - ресурс занят
		//второй семафор - счетчик активных писателей
		//третий семафор - счетчик активных читателей
		//четвертый семафор - счетчик процессов работающих с множ-ным семафором 
		short val[4]={1,0,0,0};
		semctl(SID,4,SETALL,val);
		std::cout<<"Семафор инициализирован"<<std::endl;
	}
	std::cout<<"Семафор открыт"<<std::endl;
	
	struct sembuf sb;//увеличить количество работающих процессов
	sb.sem_num=3; sb.sem_flg=0; sb.sem_op=1;
	if(semop(SID,&sb,1)<0)
	{
		perror("Error in function semop(add activ proc)");
		exit(2);
	}
	std::cout<<"Семафор используют "<<semctl(SID,3,GETVAL)<<" процессов"<<std::endl;
}


void destructSemafor()//уничтожение семафора
{
	struct sembuf sb;
	sb.sem_num=3; sb.sem_flg=0; sb.sem_op=-1;
	if(semop(SID,&sb,1)<0)//минус 1 работающий процесс
	{
		perror("Error in function semop(minus activ proc)");
		exit(2);
	}
	if(semctl(SID,3,GETVAL)==0)//это последний процесс
	{
		semctl(SID,IPC_RMID,0);//уничтожение множ-го семафора
		std::cout<<"Семафор уничтожен!"<<std::endl;
	}
}
