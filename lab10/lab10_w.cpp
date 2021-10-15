#include "fun.h"
#include <unistd.h>
#include <fstream>

int main()
{
	struct sembuf sb;
	
	
	getSemafor();
	
	for(int i=0;i<WRITE_COUNT;++i)
	{
		sleep(1);
		
		
		//Увеличение количества активных писателей
		sb.sem_num=1; sb.sem_flg=0; sb.sem_op=1;
		if(semop(SID,&sb,1)<0)
		{
			perror("Error in function semop(add active writer)");
			exit(2);
		}
		sb.sem_num=2; sb.sem_flg=0; sb.sem_op=0;
		semop(SID,&sb,1);//ожидание окончания работы активных читателей
		sb.sem_num=0; sb.sem_flg=0; sb.sem_op=-1;
		if(semop(SID,&sb,1)<0)//блокировка ресурса на запись
		{
			perror("Error in function semop(block writer)");
			exit(2);
		}
		
		
		
		std::ofstream fout(FNAME,std::ios_base::app);
		fout<<"Писатель PID: "<<getpid()<<" Запись номер "<<i<<std::endl;
		std::cout<<"Писатель PID: "<<getpid()<<" Запись номер "<<i<<std::endl;
		fout.close();
		
		
		
		sb.sem_num=0; sb.sem_flg=0; sb.sem_op=1;
		if(semop(SID,&sb,1)<0)//разблокировка ресурса на запись
		{
			perror("Error in function semop(unblock writer)");
			exit(2);
		}
		sb.sem_num=1; sb.sem_flg=0; sb.sem_op=-1;
		if(semop(SID,&sb,1)<0)//отпустить флаг активного писателя
		{
			perror("Error in function semop(add active writer)");
			exit(2);
		}
	}
	
	destructSemafor();
	return 0;
}

