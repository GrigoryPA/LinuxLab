#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int main()
{
	int SocID, ServerID;
	struct sockaddr_in addr;
	struct timeval tv;
	fd_set fds;
	int cv;
	pid_t pid;
	char SocID_char[255];
	
	//создание сокета
	ServerID=socket(AF_INET,SOCK_STREAM,0);//создание принимающего сокета
	if(ServerID<0)
	{
		perror("Error in socket()");
		exit(1);
	}
	
	addr.sin_family=AF_INET;
	addr.sin_port=htons(7777);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	//привязка к сетевому адресу
	if(bind(ServerID,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Error in bind()");
		exit(2);
	}
	
	listen(ServerID,1);
	
	while(true)
	{
		//ожидание клиента
		FD_ZERO(&fds);
		FD_SET(ServerID,&fds);
		tv.tv_sec=5;
		tv.tv_usec=0;
		cv=select(FD_SETSIZE,&fds,NULL,NULL,&tv);
		if(cv==0)
		{
			std::cout<<"Истечение времени ожидания запроса от клиента!"<<std::endl;
			break;
		}
		else
		{
			SocID=accept(ServerID,NULL,NULL);
			if(SocID<0)
			{
				perror("Error in accpt()");
				exit(3);
			}
			
			//получение сообщения
			FD_ZERO(&fds);
			FD_SET(SocID,&fds);
			tv.tv_sec=5;
			tv.tv_usec=0;
			cv=select(FD_SETSIZE,&fds,NULL,NULL,&tv);
			if(cv==0)
			{
				std::cout<<"Истечение времени ожидания сообщения от клиента!"<<std::endl;
				close(SocID);
				break;
			}
			else
			{
				pid=fork();
				std::sprintf(SocID_char,"%d",SocID);
				if(pid==0)
					execl("process"," ",SocID_char,NULL);
				close(SocID);
			}
		}
	}
	
	close(ServerID);
	return 0;
}
