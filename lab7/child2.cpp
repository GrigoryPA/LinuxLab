#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <string.h>

//потомок 2 чтения из канала
int main(int argc, char* argv[])
{
	//имя файла и канал чтения
	std::ofstream fout(argv[2]);
	int read_pipe = *argv[1];
	char ch;
	int sig;
	
	
	//добавление сиганла потомку
	sigset_t mask;
	sigaddset(&mask,SIGUSR2);
	
	
	//потомок 2 читает ждет потомка 1
	sigwait(&mask,&sig);
	while(read(read_pipe,&ch,1)>0)
	{//читать из канала пока есть что
		fout<<ch;//запись в файл
		//std::cout<<"Р2: "<<ch<<std::endl;
		kill(0,SIGUSR1);//передача разрешения потомку 1
		sigwait(&mask,&sig);//ожидания разрешения от потомка 1
	}
	
	
	
	fout.close();
	exit(0);
}

