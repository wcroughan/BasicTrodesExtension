#include "main.h"


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

//bool IS_RUNNING = true;
int pyfun_start() {
	std::string logfname = "test_extension_log.txt";
	if ((logout = fopen(logfname.c_str(), "w")) == (FILE*)NULL) {
		printf("Couldn't create log file\n");
		return -1;
	}
	clock_gettime(CLOCK_REALTIME, &exec_start_time);
	printf("Created log file\n");
	log_msg("Hello, log!");

	//connect
	const std::string CLIENT_ID = "CHOIR";
	const std::string CLIENT_ADDR = "tcp://127.0.0.1";
	const int CLIENT_PORT = 49152;
	TestClient *client = new TestClient(CLIENT_ID.c_str(), CLIENT_ADDR.c_str(), CLIENT_PORT);
	if (client->initialize()) {
		log_msg("Couldn't initialize network connection");
		delete client;
		return false;
	}

	log_msg("initialize went well!");

	std::string outfname = "neural_data_outfile";
	FILE* nout;
	if ((nout = fopen(outfname.c_str(), "wb")) == (FILE*)NULL) {
		printf("couldn't open output file\n");
		return -1;
	}

	outfname = "neural_data_outfile_timestamps";
	FILE* nout_ts;
	if ((nout_ts = fopen(outfname.c_str(), "wb")) == (FILE*)NULL) {
		printf("couldn't open output file\n");
		return -1;
	}

	outfname = "neural_data_outfile2";
	FILE* nout2;
	if ((nout2 = fopen(outfname.c_str(), "wb")) == (FILE*)NULL) {
		printf("couldn't open output file\n");
		return -1;
	}

	outfname = "neural_data_outfile_timestamps2";
	FILE* nout_ts2;
	if ((nout_ts2 = fopen(outfname.c_str(), "wb")) == (FILE*)NULL) {
		printf("couldn't open output file\n");
		return -1;
	}

	client->sendTimeRateRequest();
	std::this_thread::sleep_for(std::chrono::seconds(1));

	const int TRODES_BUF_SIZE = 1024;
	std::vector<std::string> chans;
	const int num_tets = 64;
	for (size_t i = 0; i < num_tets; i++)
		for (size_t j = 0; j < 4; j++)
			chans.push_back(std::to_string(i+1) + "," + std::to_string(j+1));

	NeuralConsumer *neucon = client->subscribeNeuralData(TRODES_BUF_SIZE, chans);
	neucon->initialize();
	//HFSubConsumer *vidcon = client->subscribeHighFreqData("PositionData", "CameraModule");
	//vidcon->initialize();

	int16_t* neubuf = (int16_t*)malloc(sizeof(int16_t) * chans.size());
	//int vidsz = vidcon->getType().getByteSize();
	//char* vidbuf = (char*)malloc(vidsz);
	//log_msg("vidsz = %d", vidsz);

	struct timespec start_stream, current_stream;
	clock_gettime(CLOCK_REALTIME, &start_stream);

	bool nonzerodata = false;

	do {
		size_t n = neucon->available(0);
		for (int i = 0; i < n; i++)
		{
			timestamp_t ts = neucon->getData(neubuf);
			fwrite(&ts, sizeof(timestamp_t), 1, nout_ts);
			fwrite(neubuf, sizeof(int16_t), chans.size(), nout);

			for (int j = 0; j < chans.size(); j++)
			{
				if (neubuf[j])
					nonzerodata = true;
			}
		}
		

		std::this_thread::yield();

		clock_gettime(CLOCK_REALTIME, &current_stream);

	} while (current_stream.tv_sec - start_stream.tv_sec < 60);

	printf("nonzerodata? %d\n", int(nonzerodata));

	client->unsubscribeHighFreqData(hfType_NEURO, TRODES_NETWORK_ID);
	fclose(nout);
	fclose(nout_ts);

	std::this_thread::sleep_for(std::chrono::seconds(3));


	neucon = client->subscribeNeuralData(TRODES_BUF_SIZE, chans);
	neucon->initialize();
	//IS_RUNNING = true;
	clock_gettime(CLOCK_REALTIME, &start_stream);

	do {
		size_t n = neucon->available(0);
		for (int i = 0; i < n; i++)
		{
			timestamp_t ts = neucon->getData(neubuf);
			fwrite(&ts, sizeof(timestamp_t), 1, nout_ts2);
			fwrite(neubuf, sizeof(int16_t), chans.size(), nout2);
		}
		std::this_thread::yield();
		clock_gettime(CLOCK_REALTIME, &current_stream);
	} while (current_stream.tv_sec - start_stream.tv_sec < 60);

	client->unsubscribeHighFreqData(hfType_NEURO, TRODES_NETWORK_ID);
	fclose(nout2);
	fclose(nout_ts2);


	free(neubuf);
	delete client;
	return 0;
}

void TestClient::recv_file_open(std::string filename)
{
	log_msg("recv_file_open: %s", filename.c_str());
}

void TestClient::recv_file_close()
{
	log_msg("recv_file_close");
}

void TestClient::recv_acquisition(std::string acquisition_type, uint32_t timestamp)
{
	log_msg("recv_acquisition: %s, %u", acquisition_type.c_str(), timestamp);
}

void TestClient::recv_source(std::string sourcename)
{
	log_msg("recv_source: %s", sourcename.c_str());
}

void TestClient::recv_quit()
{
	log_msg("recv_quit");
}

void TestClient::recv_time(uint32_t timestamp)
{
	log_msg("recv_time: %u", timestamp);
}

void TestClient::recv_timerate(int timerate)
{
	log_msg("recv_timerate: %d", timerate);
}

void TestClient::recv_event(std::string origin, std::string event, TrodesMsg & msg)
{
	log_msg("recv_event: %s, %s, %u", origin.c_str(), event.c_str(), msg.numContents());
}

void pyfun_end_first_run()
{
	//IS_RUNNING = false;
}

void pyfun_end_second_run()
{
	//IS_RUNNING = false;
}

int main() {
	return pyfun_start();
}