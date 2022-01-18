#include <fcntl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <deque>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <vector>



#include "Request.hpp"
// Пока-что response вместо файла
const char* response =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n"
"Connection: close\r\n"
"Content-Length: 872\r\n\r\n"
"<doctype !html>\n"
"    <html>\n"
"        <head>\n"
"            <title>Bye-bye baby bye-bye</title>\n"
"            <style>body { background-color: #111 }\n"
"                   h2 { font-size:2cm; text-align: center; color: black;\n"
"                        text-shadow: 0 0 2mm red}\n"
"                   h3 { font-size:1cm; text-align: center; color: white;\n"
"                        text-shadow: 0 0 2mm red}\n"
"            </style>\n"
"        </head>\n"
"<body>\n"
"        <h2>Hello!</h2>\n"
"		 <h3> this ... </h3>\n"
"         <form method=\"POST\" accept-charset=\"utf-8\">\n"
"            <label for=\"fname\">First name:</label><br>\n"
"            <input type=\"text\" id=\"fname\" name=\"fname\" value=\"John\"><br>\n"
"            <label for=\"lname\">Last name:</label><br>\n"
"            <input type=\"text\" id=\"lname\" name=\"lname\" value=\"Doe\"><br><br>\n"
"            <input type=\"submit\" value=\"Submit\">\n"
"          </form>\n"
"</body>\n"
"</html>\n";

int main (int argc, char **argv)
{
	// Создаем слушающий сокет
	// Слушающих может быть несколько штук, чтобы 
	// • Your server can listen on multiple ports (See config file).
	// Пока что одно подключение

	int listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


	// Создаем структуру которая свяжет сокет с адрессом и портом

	struct sockaddr_in addresServer;

	// Структура в которую запишется адресс подключаемого клинета
	struct sockaddr_in addresUser;

	addresServer.sin_family = PF_INET;
	addresServer.sin_port = htons(8080);
	addresServer.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Делаем неблокирщий слушающий сокет
	fcntl(listenFd, F_SETFL, O_NONBLOCK);
	
	// Чтобы не было залипания порта
	int one = 1;
	setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

	// Связываем сокет с адресом
	bind(listenFd, (struct sockaddr *)&addresServer, sizeof(addresServer));

	// Делаем сокеты слушающим
	listen(listenFd, 5);
	// Получить fd на очередь 

	int kqfd = kqueue();

	// Создадим структуру для слушающего сокета

	struct kevent listenEvent;
	// Регистрация ядрово-слушающего события
	EV_SET(&listenEvent, listenFd, EVFILT_READ , EV_ADD, 0 ,0 ,0);

	// Создаем 2 контейнера для регистрации событий
	std::vector<struct kevent> inputQueue; 
	std::vector<struct kevent> outputQueue; 


	inputQueue.push_back(listenEvent);
	
	outputQueue.resize(100);
	
	int amountEvent;
	// В этом цикле мы принимае соединения
	
	while(true)
	{
		amountEvent = kevent(kqfd, &*inputQueue.begin(), inputQueue.size(),
		&*outputQueue.begin(), outputQueue.size(), nullptr);
		// Высвобождаем для регистрации новых событий
		inputQueue.clear();

		int i = 0;
		while (i < amountEvent)
		{

			// Пытаемся получить новое подключение
			if (outputQueue[i].filter == EVFILT_READ &&
			outputQueue[i].ident ==  listenEvent.ident)
			{
				socklen_t lenght;
				int newConnect = accept(listenEvent.ident,
				(struct sockaddr *)&addresUser, &lenght);

				if (newConnect > -1)
				{
					// Делаем неблокирующиее соединение и 
					// говорим, чтобы не бросался сигнал при
					// аварийном завершении клиента 
					fcntl(newConnect, F_SETFL | SO_NOSIGPIPE, O_NONBLOCK);
					setsockopt(newConnect, SOL_SOCKET, SO_NOSIGPIPE, (void *)&one, sizeof(int));
					// Добавляем в очередь регистрации события
					inputQueue.insert(inputQueue.begin(), (struct kevent){});

					// Региструем на чтение
					EV_SET(&*inputQueue.begin(), newConnect, EVFILT_READ, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, 0);
					EV_SET(&*inputQueue.begin(), newConnect, EVFILT_READ , EV_ADD , 0, 0, 0);

				}
			}
			// Обрабатываем ошибку
			else if (outputQueue[i].flags & EV_ERROR)
			{
			}
			// Если кто-то закрыл соединение
			else if (outputQueue[i].flags & EV_EOF && 
			// но не закрываем слушющий сокет
			outputQueue[i].ident != listenEvent.ident)
			{
				close(outputQueue[i].ident);
			}
			// Пытаемся получить сообщение
			else if (outputQueue[i].filter == EVFILT_READ &&
			outputQueue[i].ident != listenEvent.ident)
			{
				char buf[66666];
				int amountRead = recv (outputQueue[i].ident, buf, 66665, 0);
				buf[amountRead] = 0;


				/* Тут накакано */
				Request request(buf);
				request.parseStartLine();
				std::cout << "-->" << request.METHOD << " | " << request.URI << " | " << request.HTTP_PROTOCOL << "<--" << std::endl;

				//  Нужно еще по ходу заголовок HOST проверять (((( Как же заебало уже это все, какой больной человек придумал это все...
				/* Кто-то потом уберет */


				// Печатаем на экран
				// Можем увидем какой запрос к нам пришел
				std::cout << buf << std::endl;
				std::cout << amountRead << std::endl;
				// Читаем до тех пор, пока не наберем заданную размерность в Content-length
					/* Временны код */
					inputQueue.insert(inputQueue.begin(), (struct kevent){});
					if (outputQueue[i].data - amountRead) {
						EV_SET(&*inputQueue.begin(), outputQueue[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE | EV_ONESHOT,
							   0, 0, 0);
					}
					else
						EV_SET(&*inputQueue.begin(), outputQueue[i].ident, EVFILT_WRITE , EV_ADD | EV_ENABLE | EV_ONESHOT, 0 ,0 ,0);

			}
			// Пытаемся отправить сообщение
			else if (outputQueue[i].filter == EVFILT_WRITE &&
			outputQueue[i].ident != listenEvent.ident) 
			{
				int amountWrite  = send(outputQueue[i].ident, response, strlen(response), 0);
				inputQueue.insert(inputQueue.begin(), (struct kevent){});
				EV_SET(&*inputQueue.begin(), outputQueue[i].ident, EVFILT_READ , EV_ADD | EV_ENABLE | EV_ONESHOT, 0 ,0 ,0);
			}
			i++;
		}
		

	}

}