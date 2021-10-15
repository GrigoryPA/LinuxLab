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
		
		
		std::cout<<"Sem_w = "<<semctl(SID,0,GETVAL)<<std::endl;
		std::cout<<"Sem_w_count = "<<semctl(SID,1,GETVAL)<<std::endl;
		std::cout<<"Sem_r_count = "<<semctl(SID,2,GETVAL)<<std::endl;
		
		//Уставнока флага о появлении писателя
		sb.sem_num=1; sb.sem_flg=0; sb.sem_op=1;
		if(semop(SID,&sb,1)<0)
		{
			perror("Error in function semop(add active writer)");
			exit(2);
		}
		std::cout<<"добавление активного писателя"<<std::endl;
		
		sb.sem_num=2; sb.sem_flg=0; sb.sem_op=0;
		semop(SID,&sb,1);//ожидание окончания работы активных читателей
		std::cout<<"Активных читателей нет"<<std::endl;
		
		sb.sem_num=0; sb.sem_flg=0; sb.sem_op=-1;
		if(semop(SID,&sb,1)<0)//блокировка ресурса на запись
		{
			perror("Error in function semop(block writer)");
			exit(2);
		}
		std::cout<<"блок семафора писателя"<<std::endl;

		std::cout<<"Sem_w = "<<semctl(SID,0,GETVAL)<<std::endl;
		std::cout<<"Sem_w_count = "<<semctl(SID,1,GETVAL)<<std::endl;
		std::cout<<"Sem_r_count = "<<semctl(SID,2,GETVAL)<<std::endl;	
		
		
		
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
		std::cout<<"разблок семафора писателя"<<std::endl;
		sb.sem_num=1; sb.sem_flg=0; sb.sem_op=-1;
		if(semop(SID,&sb,1)<0)//отпустить флаг активного писателя
		{
			perror("Error in function semop(add active writer)");
			exit(2);
		}
		std::cout<<"удаление активного писателя"<<std::endl;
	}
	
	destructSemafor();
	return 0;
}

