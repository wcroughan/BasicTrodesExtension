#include <stdarg.h>
#include "TrodesNetwork/AbstractModuleClient.h"

//Currently nothing special, but has potential!
class TestClient : public AbstractModuleClient {
public:
	using AbstractModuleClient::AbstractModuleClient;

	//optional overwrite functions
	void recv_file_open(std::string filename);
	void recv_file_close();
	void recv_acquisition(std::string acquisition_type, uint32_t timestamp);
	void recv_source(std::string sourcename);
	void recv_quit();
	void recv_time(uint32_t timestamp);
	void recv_timerate(int timerate);
	//! Event received. Reimplement to write in if/else statements for events
	void recv_event(std::string origin, std::string event, TrodesMsg &msg);
};


struct timespec exec_start_time;
FILE *logout;
void log_msg(const char* msg, ...) {
	char buf[4096];
	va_list args;
	va_start(args, msg);
	vsprintf(buf, msg, args);

	struct timespec t2;
	clock_gettime(CLOCK_REALTIME, &t2);
	long int total_exec_time = (t2.tv_sec - exec_start_time.tv_sec) * 1000000000 + (t2.tv_nsec - exec_start_time.tv_nsec);
	double log_time = double(total_exec_time) / double(1000000);

	fprintf(logout, "%f\t%s\n", log_time, buf);
	va_end(args);
}

void log_msg(std::string msg) { log_msg(msg.c_str()); }


int main() {
	//create log file
	std::string logfname = "test_extension_log.txt";
	if ((logout = fopen(logfname.c_str(), "w")) == (FILE*)NULL) {
		printf("Couldn't create log file\n");
		return -1;
	}
	clock_gettime(CLOCK_REALTIME, &exec_start_time);

	//connect
	std::string CLIENT_ID = "";
	std::string CLIENT_ADDR = "";
	int CLIENT_PORT = 420;
	TestClient *client = new TestClient(CLIENT_ID.c_str(), CLIENT_ADDR.c_str(), CLIENT_PORT);
	if (!client->initialize()) {
		log_msg("Couldn't initialize network connection");
		delete client;
		return false;
	}

	client->sendTimeRateRequest();

	return 0;
}

void TestClient::recv_file_open(std::string filename)
{
}

void TestClient::recv_file_close()
{
}

void TestClient::recv_acquisition(std::string acquisition_type, uint32_t timestamp)
{
}

void TestClient::recv_source(std::string sourcename)
{
}

void TestClient::recv_quit()
{
}

void TestClient::recv_time(uint32_t timestamp)
{
}

void TestClient::recv_timerate(int timerate)
{
}

void TestClient::recv_event(std::string origin, std::string event, TrodesMsg & msg)
{
}
