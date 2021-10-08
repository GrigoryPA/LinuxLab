#include <iostream>
#include <fstream>
#include <pthread.h>

//создаем и открываем потоки ввода и вывода в файлы
std::ofstream Output1("Thread1.txt");
std::ofstream Output2("Thread2.txt");
std::ifstream Input("Text.txt");

//работа первого потока
void* work_thread1(void* _arg)
{
	std::string arg=*static_cast<std::string*>(_arg);
	Output1<<arg<<std::endl;//вывод строки в файл
	pthread_exit(NULL);//окончание потока
}

//работа второго потока
void* work_thread2(void* _arg)
{
	std::string arg=*static_cast<std::string*>(_arg);
	Output2<<arg<<std::endl;//вывод в файл
	pthread_exit(NULL);//конец работы потока
}

int main()
{
	pthread_t thread1,thread2;//объявление потоков
	std::string str;//строка чтения
	
	while(true)
	{
		std::string str1,str2;
		bool end_flag1=false;
		bool end_flag2=false; 
		
		//создание потока (ид-тор потока, атрибуты, функция, аргументы)
		if(std::getline(Input,str1))
			pthread_create(&thread1,NULL,work_thread1,&str1);
		else
			end_flag1=true;
		if(std::getline(Input,str2))
			pthread_create(&thread2,NULL,work_thread2,&str2);
		else
			end_flag2=true;
		
		//ожидание завершения потока (иден-тор потка, код завершения)
		pthread_join(thread1,NULL);
		pthread_join(thread2,NULL);
		
		if(end_flag1||end_flag2)//выход из цикла
		{
			std::cout<<"Окончание чтения из файла"<<std::endl;
			break;
		}
	}
	//закрытие файловых потоков
	Output1.close();
	Output2.close();
	Input.close();
	return 0;
}
