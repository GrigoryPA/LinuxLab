#include "fun.h"
#include <unistd.h>
#include <fstream>

int main()
{
	struct sembuf sb;
	getSemafor();
	
	
	std::string str;
	
	
	
	std::cout<<"Sem_w = "<<semctl(SID,0,GETVAL)<<std::endl;
	std::cout<<"Sem_w_count = "<<semctl(SID,1,GETVAL)<<std::endl;
	std::cout<<"Sem_r_count = "<<semctl(SID,2,GETVAL)<<std::endl;	
	
	
	//Начало работы только если нет активного писателя или
	//писателей ожидающих окончания активных читателей
	sb.sem_num=1; sb.sem_flg=0; sb.sem_op=0;
	semop(SID,&sb,1);
	std::cout<<"Активных и ожидающих питателей нет"<<std::endl;
	
	sb.sem_num=2; sb.sem_flg=0; sb.sem_op=1;
	if(semop(SID,&sb,1)<0)//добавление активного читателя
	{
		perror("Error in function semop(add activ reader)");
		exit(2);
	}
	std::cout<<"добавление активного читателя"<<std::endl;
	
	semctl(SID,0,SETVAL,0);//запрет записи писателям
	std::cout<<"блок семафора писателя"<<std::endl;	
		
	std::cout<<"Sem_w = "<<semctl(SID,0,GETVAL)<<std::endl;
	std::cout<<"Sem_w_count = "<<semctl(SID,1,GETVAL)<<std::endl;
	std::cout<<"Sem_r_count = "<<semctl(SID,2,GETVAL)<<std::endl;				
	
	
	sleep(1);
	std::ifstream fin(FNAME);
	std::cout<<"------------------------"<<std::endl;
	while(getline(fin,str))
		std::cout<<str<<std::endl;
	std::cout<<"------------------------"<<std::endl;
	fin.close();
	
	
	
	semctl(SID,0,SETVAL,1);//разрешение записи писателям
	std::cout<<"разблок семафора писателя"<<std::endl;
		
	sb.sem_num=2; sb.sem_flg=0; sb.sem_op=-1;
	if(semop(SID,&sb,1)<0)//уменьшение числа активных читателей
	{
		perror("Error in function semop(del activ reader)");
		exit(2);
	}
	std::cout<<"удаление активного читателя"<<std::endl;	
		
	std::cout<<"Sem_w = "<<semctl(SID,0,GETVAL)<<std::endl;
	std::cout<<"Sem_w_count = "<<semctl(SID,1,GETVAL)<<std::endl;
	std::cout<<"Sem_r_count = "<<semctl(SID,2,GETVAL)<<std::endl;	


	
	destructSemafor();
	return 0;
}
