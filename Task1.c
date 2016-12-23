/*Задание №1.
Написать программу выполняющую следующую задачу.
С помощью UDP сокетов отправить UDP пакет на адрес
37.230.210.36:27500. Полезная нагрузка в UDP пакете (payload) должна
содержать только строку "hi!".
В ответ на отправленный запрос придет ответ. Прочитать ответ,
полученный на данный запрос.
Вывести на экран:
- IP-адрес и порт источника (в читаемом для человека формате, т.е.
123.123.123.123:12345);
- IP-адрес и порт назначения (в читаемом для человека формате, т.е.
123.123.123.123:12345);
- содержимое полученного ответа (UDP payload). */

#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#define BUFSIZE 1024


int main() {
	int socket_desc = 0;
	const char* my_msg = "hi!";
	const char* server_IP = "37.230.210.36";
	const int server_port = 27500;
	//creating socket
	if ((socket_desc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket");
		return 0;
	}
	//binding
	struct sockaddr_in my_address;
	//memset((char *)&my_address, 0, sizeof(my_address));
	my_address.sin_family = AF_INET;
	my_address.sin_addr.s_addr = htonl(INADDR_ANY);
	my_address.sin_port = htons(0);
	if (bind(socket_desc, (struct sockaddr *)&my_address, sizeof(my_address)) < 0) {
		perror("bind failed");
		return 0;
	}
	//sending msg
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	if (inet_pton(AF_INET, server_IP, &server_addr.sin_addr.s_addr) < 0) {
		perror("inet_pton failed");
		return 0;
	}
	if (sendto(socket_desc, my_msg, strlen(my_msg), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("sendto failed");
		return 0;
	}
	//receiving msg
	int recvlen;
	unsigned char buffer[BUFSIZE];
	unsigned int serv_addr_len = sizeof(server_addr);
	if (recvlen = recvfrom(socket_desc, buffer, BUFSIZE, 0, (struct sockaddr *)&server_addr, &serv_addr_len) < 0) {
		perror("recvfrom failed");
		return 0;
	}
	//getting recipient's ip and port
	unsigned int my_addr_len = sizeof(my_address);
	if (getsockname(socket_desc, (struct sockaddr *)&my_address, &my_addr_len) < 0) {
		perror("getsockname failed");
		return 0;
	}
	char result_ip[INET_ADDRSTRLEN];
	const char * result_ip2 = inet_ntop(AF_INET, &my_address, result_ip, INET_ADDRSTRLEN);
	int my_port = ntohs(my_address.sin_port);
	//server ip is known because we received the message from it
	printf("%s:%d\n%s:%d\n%s\n",  server_IP, server_port, result_ip, my_port, buffer);
	//closing socket
	close(socket_desc);
}
