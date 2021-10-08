#include "functions.h"
#include <unistd.h>
#include <fstream>

//1 - количество, 2 - период
int main(int argc, char* argv[])
{
	void* segment_addr=NULL;
	int count_str,period;
	count_str=atoi(argv[1]);
	period=atoi(argv[2]);
	
	
	
	//создание сигмента если не создан
	if(SID==-1)
		getSegment();
	//добавление в адрессное пространство процесса
	segment_addr=includeSegment();
	//создание указателя на общую переменную
	shared* share=(shared*)segment_addr;
	
	
	
	//цикл записей
	while(true)
	{
		sleep(period);
		std::cout<<"[1] - Подготовка к входу в сегмент"<<std::endl;
		lock(share,FIRST);//получение доступа
		
		if(count_str>0)
		{//вывод данных в разделяемый файл
			std::ofstream fout(FNAME,std::ios_base::app);
			fout<<"[1]"<<std::endl;
			fout.close();
			--count_str;
		}
		else
		{//сигнал окончания работы
			++(share->stop);
			unlock(share,FIRST);//выкл заинтересованность в ресурсе
			std::cout<<"[1] - Выход из сегмента"<<std::endl;
			break;//выход из цикла
		}
		
		unlock(share,FIRST);//выкл заинтересованность в ресурсе
		std::cout<<"[1] - Выход из сегмента"<<std::endl;
	}
	
	
	
	if(share->stop==3)
	{//если все процессы закончили работу и этот последний
		excludeSegment(segment_addr);
		destructSegment();
	}
	
	return 0;
}
