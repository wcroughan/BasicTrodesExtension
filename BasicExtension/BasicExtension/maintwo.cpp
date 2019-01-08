#include "main.h"

int maintwo() {
	//connect
	const std::string CLIENT_ID = "CHOIR";
	const std::string CLIENT_ADDR = "tcp://127.0.0.1";
	const int CLIENT_PORT = 49152;
	TestClient *client = new TestClient(CLIENT_ID.c_str(), CLIENT_ADDR.c_str(), CLIENT_PORT);
	if (client->initialize()) {
		printf("Couldn't initialize network connection\n");
		delete client;
		return -1;
	}

	printf("initialize went well!\n");


	client->sendTimeRateRequest();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}