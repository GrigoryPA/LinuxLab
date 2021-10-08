#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
	srand(time(0));
	int SocID;
	struct sockaddr_in addr;
	struct timeval tv;
	fd_set writefds;
	int wv;
	char message[3],buf[3];
	
	//создание сокета
	SocID=socket(AF_INET,SOCK_STREAM,0);
	if(SocID<0)
	{
		perror("Error in socket()");
		exit(1);
	}
	
	addr.sin_family=AF_INET;
	addr.sin_port=htons(7777);
	addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
	
	//подключение к серверу
	if(connect(SocID,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Error in connect()");
		exit(2);
	}
	else
	{//ожидание отправки сообщения
		FD_ZERO(&writefds);
		FD_SET(SocID,&writefds);
		tv.tv_sec=10;
		tv.tv_usec=0;
		wv=select(FD_SETSIZE,NULL,&writefds,NULL,&tv);
		if(wv==0)
		{
			std::cout<<"Истечение времени ожидания отправки сообщения!"<<std::endl;
		}
		else
		{
			//отправка
			std::sprintf(message,"%d",rand()%899+100);
			send(SocID,message,sizeof(message),0);
			std::cout<<"Отправлено: "<<message<<std::endl;
			
			//получение
			recv(SocID,buf,sizeof(message),0);
			std::cout<<"Получено: "<<buf<<std::endl;
		}
	}
	
	close(SocID);
	return 0;
}
