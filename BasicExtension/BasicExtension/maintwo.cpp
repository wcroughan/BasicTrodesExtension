#include "main.h"

int main() {
	//return pyfun_start();


	printf("hello, love!\n");


	//connect
	const std::string CLIENT_ID = "CHOIR";
	const std::string CLIENT_ADDR = "tcp://127.0.0.1";
	const int CLIENT_PORT = 4220;
	TestClient *client = new TestClient(CLIENT_ID.c_str(), CLIENT_ADDR.c_str(), CLIENT_PORT);
	if (!client->initialize()) {
		printf("Couldn't initialize network connection\n");
		delete client;
		return false;
	}

	printf("initialize went well!");


	client->sendTimeRateRequest();
}