#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>

int main(int argc,char* argv[])
{
	char bufc[3];
	int size_buf;
	int SocID=atoi(argv[1]);//сокет
	
	size_buf=recv(SocID,bufc,3,0);//получени
	std::cout<<"Получено: "<<bufc<<std::endl;
	std::sort(std::begin(bufc),std::end(bufc));//сортировка
	
	send(SocID,bufc,size_buf,0);//передача
	std::cout<<"Отправлено: "<<bufc<<std::endl;
	
	exit(0);
}

