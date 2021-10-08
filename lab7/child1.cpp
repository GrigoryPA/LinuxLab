#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <string.h>

//потомок общения через канал
int main(int argc, char* argv[])
{
	//получение имени файла записи и канала для чтения
	std::ofstream fout(argv[2]);
	int read_pipe = *argv[1];
	char ch;
	int sig;
	
	
	//добавление сигнала в маску проца
	sigset_t mask;
	sigaddset(&mask,SIGUSR1);
	
	
	//считывать посимвольно из канала пока есть что
	while(read(read_pipe,&ch,1)>0)
	{
		fout<<ch;//запись в файл
		//std::cout<<"Р1: "<<ch<<std::endl;
		kill(0,SIGUSR2);//передача разрешения 2 потомку
		sigwait(&mask,&sig);//ожидание разрешения от 2 потомка
	}
	kill(0,SIGUSR2);
	
	
	
	fout.close();
	exit(0);
}
