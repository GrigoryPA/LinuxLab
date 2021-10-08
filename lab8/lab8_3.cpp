#include <iostream>
#include <fstream>
#include <sys/msg.h>
#define PID 3
#define FNAME "TEXT.txt"

struct ask_msg
{
	long mtype;
	int sender;
	int answer_queue;
	int ask_time;
};

struct answer_msg
{
	long mtype;
	int sender;
};

int main()
{
	bool is_creator=false;
	int proc_end=0;
	int local_queue;
	int joint_queue;
	ask_msg ask;//отправка запроса
	ask_msg ask_buf[2];//полученные запросы
	int buf_i=0;//количество запросов в ожидании
	answer_msg answer;//отправка ответов
	answer_msg access;//получение разрешений
	int num_get_access=0;//количество полученных
	int num_send_access=0;//количество отправленных
	
	
	
	//создаем общую очередь
	joint_queue=msgget(777,0606|IPC_CREAT|IPC_EXCL);
	if(joint_queue!=-1)
	{
		is_creator=true;
		std::cout<<"Создана общая очередь"<<std::endl;
	}
	else
	{//подключаемся если уже создана
		joint_queue=msgget(777,0606|IPC_CREAT);
		std::cout<<"Подключен к общей очереди"<<std::endl;
	}
	
	//создаем локальную очередь
	local_queue=msgget(IPC_PRIVATE,0606|IPC_CREAT);
	std::cout<<"Создана локальная очередь"<<std::endl;
	
	
	
	//инициализация запросов и занесение их в очередь
	ask.ask_time=time(NULL);
	ask.mtype=(PID)%3+1;
	ask.sender=PID;
	ask.answer_queue=local_queue;
	msgsnd(joint_queue,&ask,sizeof(ask_msg),0);
	ask.mtype=(PID+1)%3+1;
	msgsnd(joint_queue,&ask,sizeof(ask_msg),0);
	//инициализация ответа
	answer.mtype=1;
	answer.sender=PID;
	
	
	
	//ожидание двух разрешений на файл (проверка общей очереди)
	while(num_get_access<2)
	{
		if(msgrcv(joint_queue,&ask_buf[buf_i],sizeof(ask_msg),PID,IPC_NOWAIT)!=-1)
		{//проверкa запросов в общей очереди для этой програмы
			std::cout<<"Получен запрос доступа от "<<ask_buf[buf_i].sender<<std::endl;
			std::cout<<"Время запроса: "<<ask_buf[buf_i].ask_time<<std::endl;
			if(ask_buf[buf_i].ask_time<ask.ask_time || (ask_buf[buf_i].ask_time==ask.ask_time && ask_buf[buf_i].sender<PID))
			{//приоритет младшего и меньший индификатор
				msgsnd(ask_buf[buf_i].answer_queue,&answer,sizeof(answer_msg),0);
				++num_send_access;
				std::cout<<"Выдача разрешения процессу "<<ask_buf[buf_i].sender<<std::endl;
			}
			else//запомнить в буфере если старше
				++buf_i;
		}
		
		if(msgrcv(local_queue,&access,sizeof(answer_msg),1,IPC_NOWAIT)!=-1)
		{//проверка разрешений в локальной очереди
			++num_get_access;
			std::cout<<"Получено разрешение на доступ от "<<access.sender<<std::endl;
			std::cout<<"Время получения: "<<time(NULL)<<std::endl;
		}
	}
	
	
	
	//вывод файла
	std::cout<<"Начать вывод файла"<<std::endl;
	std::ifstream fin(FNAME);
	std::string str;
	while(std::getline(fin,str))
		std::cout<<str<<std::endl;
	fin.close();
	std::cout<<"Конец файла. Файл закрыт."<<std::endl;
	
	
	
	//выдача разрешений всем ожидающим
	while(buf_i>0)
	{
		--buf_i;
		msgsnd(ask_buf[buf_i].answer_queue,&answer,sizeof(answer_msg),0);
		++num_send_access;
		std::cout<<"Выдача разрешения процессу "<<ask_buf[buf_i].sender<<std::endl;
	}
	//если еще не все запросили доступ
	while(num_send_access<2)
	{//проверка запросов из общей очереди для этой программы
		if(msgrcv(joint_queue,&ask_buf[buf_i],sizeof(ask_msg),PID,IPC_NOWAIT)!=-1)
		{
			msgsnd(ask_buf[buf_i].answer_queue,&answer,sizeof(answer_msg),0);
			++num_send_access;
			std::cout<<"Выдача разрешения процессу "<<ask_buf[buf_i].sender<<std::endl;
		}
	}
	
	
	
	//отпрвака готовности завершения общей очереди
	ask.mtype=0;
	msgsnd(joint_queue,&ask,sizeof(ask_msg),0);
	//ожиание готовности остальных процессов
	if(is_creator)
	{
		while(proc_end<3)
		{
			if(msgrcv(joint_queue,&ask,sizeof(ask_msg),0,0)!=-1)
				++proc_end;
		}
		msgctl(joint_queue,IPC_RMID,0);
	}
	//удалене локальной очереди
	msgctl(local_queue,IPC_RMID,0);
	
	return 0;
}
