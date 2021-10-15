#include "fun.h"
#include <unistd.h>
#include <fstream>

int main()
{
	struct sembuf sb;
	getSemafor();
	
	
	std::string str;
	
	
	
	//Начало работы только если нет активного писателя или
	//писателей ожидающих окончания активных читателей
	sb.sem_num=1; sb.sem_flg=0; sb.sem_op=0;
	semop(SID,&sb,1);
	sb.sem_num=2; sb.sem_flg=0; sb.sem_op=1;
	if(semop(SID,&sb,1)<0)//добавление активного читателя
	{
		perror("Error in function semop(add activ reader)");
		exit(2);
	}
					
	
	
	sleep(1);
	std::ifstream fin(FNAME);
	std::cout<<"------------------------"<<std::endl;
	while(getline(fin,str))
		std::cout<<str<<std::endl;
	std::cout<<"------------------------"<<std::endl;
	fin.close();
	
	
		
	sb.sem_num=2; sb.sem_flg=0; sb.sem_op=-1;
	if(semop(SID,&sb,1)<0)//уменьшение числа активных читателей
	{
		perror("Error in function semop(del activ reader)");
		exit(2);
	}	


	
	destructSemafor();
	return 0;
}
