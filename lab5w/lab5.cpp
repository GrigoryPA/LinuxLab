#include <iostream>
#include <signal.h>

//ф-ия обработки сигнала
void mysighandler(int signal)
{
	switch(signal)
	{
	case SIGFPE:
		std::cout<<"Неверная операция (деление на ноль)"<<std::endl;
		exit(1);
		break;
	case SIGSEGV:
		std::cout<<"Нарушение защиты памяти"<<std::endl;
		exit(2);
		break;
	}
}

int main()
{
	//переназначение обработки сигнала на новую функцию 
	signal(SIGFPE, mysighandler);
	signal(SIGSEGV, mysighandler);
	
	int Z;
	int i=0;
	int *p=NULL;
	std::cout<<"Тип ошибки деление на ноль/обращения к памяти (1/2): ";
	std::cin>>Z;
	switch(Z)
	{
	case 1:
		i=0/0;
		break;
	case 2:
		*p=555;
		break;
	}
}
