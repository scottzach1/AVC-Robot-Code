#include <stdio.h>
#include <time.h>
#include "E101.h"

void OpenGate() {
	init();
	char server_addr[15] = "130.195.6.196"; 
	int port = 1024;
	char message[24];
	char send[] = "Please";
	connect_to_server(server_addr, port);
	send_to_server(send);
	receive_from_server(message);
	send_to_server(message);
}

int main() {

init();
OpenGate();

}
