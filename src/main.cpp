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
#include "Response.hpp"
#include "ConnectedNode.hpp"
// Пока-что response вместо файла
const char* response =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n"
"Connection: close\r\n"
"Content-Length: 893\r\n\r\n"
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
"          <img src=\"/src.jpg\">\n"
"</body>\n"
"</html>\n";

std::ifstream::pos_type filesize(const char* filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}


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

		//Чистим чтобы не осталось никаких мусорных значений
		bzero(&(*outputQueue.begin()), outputQueue.capacity() * sizeof(struct kevent));

		amountEvent = kevent(kqfd, &*inputQueue.begin(), inputQueue.size(),
		&*outputQueue.begin(), outputQueue.size(), nullptr);
		
		// Высвобождаем для регистрации новых событий
		inputQueue.clear();


		// Обрбатываем произошедшие события
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
										std::cout << "}{yita" << std::endl;

					// Делаем неблокирующиее соединение и 
					// говорим, чтобы не бросался сигнал при
					// аварийном завершении клиента 
					fcntl(newConnect, F_SETFL , O_NONBLOCK);
					setsockopt(newConnect, SOL_SOCKET, SO_NOSIGPIPE, (void *)&one, sizeof(int));
					// Добавляем в очередь регистрации события
					inputQueue.insert(inputQueue.begin(), (struct kevent){});

					// Здесь создаем объект ConnectedNode адресс которого кладем в структуру для регистрации события
					// для последующий обработки и помечаем соответсвующее состояние
					 ConnectedNode *node = new ConnectedNode(ConnectedNode::Create);

					// Региструем на чтение
					EV_SET(&*inputQueue.begin(), newConnect, EVFILT_READ, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, node);
				}
			}
			// Обрабатываем ошибку
			else if (outputQueue[i].flags & EV_ERROR)
			{
				std::cout << "Error" << std::endl;
				close(outputQueue[i].ident);

			}
			// Если кто-то закрыл соединение
			else if (outputQueue[i].flags & EV_EOF &&
			// но не закрываем слушющий сокет
			outputQueue[i].ident != listenEvent.ident)
			{
				std::cout << "close connected" << std::endl;
				close(outputQueue[i].ident);
			}
			// Пытаемся получить сообщение
			else if (outputQueue[i].filter == EVFILT_READ &&
			outputQueue[i].ident != listenEvent.ident)
			{

				char buf[8193];
				int amountRead = recv (outputQueue[i].ident, buf, 8192, 0);
				buf[amountRead] = 0;

				ConnectedNode &node = *((ConnectedNode *)outputQueue[i].udata);
				if (node.GetConnectState() == ConnectedNode::Create)
				{
					node.CreateRequest();
					node.SetConnectState(ConnectedNode::RecvRequest);
				}
				if (node.GetConnectState() == ConnectedNode::RecvRequest)
				{
					std::cout << "I AM HERE" << std::endl;
					 std::cout << buf << std::endl;
					if (node.PutNextChunkRequest(buf) == 404)
					{
						node.SetConnectState(ConnectedNode::Error);
					}
				}

				if (node.GetConnectState() == ConnectedNode::Error)
				{
					std::cout << "CHTO-TO NE TAK, BRAT" << std::endl;
					close(outputQueue[i].ident);
					i++;
					delete &node;
					continue;
				}
				// Читаем до тех пор, пока не наберем заданную размерность в Content-length или не встретим два раза подряд /r/n что является
				// концом заголовка
					/* Временны код */
					inputQueue.insert(inputQueue.begin(), (struct kevent){});
					if (node.GetConnectState() == ConnectedNode::RecvRequest
						||
						node.GetConnectState() == ConnectedNode::RecvBodyMessage) {
						EV_SET(&*inputQueue.begin(), outputQueue[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE | EV_ONESHOT,
							   0, 0, &node);
					}
					else if (node.GetConnectState() == ConnectedNode::SendResponse)
					{
						EV_SET(&*inputQueue.begin(), outputQueue[i].ident, EVFILT_WRITE,
							   EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, &node);
					}

			}
			// Пытаемся отправить сообщение
			else if (outputQueue[i].filter == EVFILT_WRITE &&
			outputQueue[i].ident != listenEvent.ident) 
			{
				std::cout << "SEND RESPONSE FROM HERE" << std::endl;
				ConnectedNode &node = *((ConnectedNode *)outputQueue[i].udata);
				if (node.GetConnectState() == ConnectedNode::SendResponse)
				{
					node.SetConnectState(ConnectedNode::SendBodyMessage);
					node.CreateResponse();
					node.FormHeaders();
					node.SetResponseHeaderState(NotSend);
				}

				//BUFF-SIZE 8192 <-------
				node.FormResponseChunk();
				send(outputQueue[i].ident, node.GetResponseChunkBuf().c_str(), node.GetResponseChunkBuf().size(), 0);

				inputQueue.insert(inputQueue.begin(), (struct kevent){});
				if (node.GetConnectState() == ConnectedNode::ReRecvRequest)
				{
					EV_SET(&*inputQueue.begin(), outputQueue[i].ident, EVFILT_READ , EV_ADD | EV_ENABLE | EV_ONESHOT, 0 ,0 ,0);
				}
				else if (node.GetConnectState() == ConnectedNode::SendResponse
				||
				node.GetConnectState() == ConnectedNode::SendBodyMessage
				)
				{
					EV_SET(&*inputQueue.begin(), outputQueue[i].ident, EVFILT_WRITE,
						   EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, &node);
				}
			}
			i++;
		}
		

	}

}