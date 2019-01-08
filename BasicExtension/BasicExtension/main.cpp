#include "main.h"


struct timespec exec_start_time;
FILE *mylogoutfile;
bool IS_RUNNING = true;

void log_msg(const char* msg, ...) {
	char buf[4096];
	va_list args;
	va_start(args, msg);
	vsprintf(buf, msg, args);

	struct timespec t2;
	clock_gettime(CLOCK_REALTIME, &t2);
	long int total_exec_time = (t2.tv_sec - exec_start_time.tv_sec) * 1000000000 + (t2.tv_nsec - exec_start_time.tv_nsec);
	double log_time = double(total_exec_time) / double(1000000);

	fprintf(mylogoutfile, "%f\t%s\n", log_time, buf);
	va_end(args);
}

void log_msg(std::string msg) { log_msg(msg.c_str()); }

void thread_launch_fun() {

	struct timespec start_stream, current_stream;
	clock_gettime(CLOCK_REALTIME, &start_stream);

	do {
		for (size_t i = 0; i < 1000; i++)
		{
			double tmp = 2.0 * 4.0 / 3.0;
			tmp = tmp * tmp;
		}

		std::this_thread::yield();
		clock_gettime(CLOCK_REALTIME, &current_stream);
	} while (current_stream.tv_sec - start_stream.tv_sec < 10 && IS_RUNNING);

}

int pyfun_start() {
	printf("Hello!\n");
	std::string logfname = "/home/fosterlab/Downloads/test_extension_log.txt";
	printf("Hello!\n");
	printf(logfname.c_str());
	printf("\n");
	FILE *af = fopen(logfname.c_str(), "w");
	if ((mylogoutfile = af) == (FILE*)NULL) {
		printf("Couldn't create log file\n");
		return -1;
	}
	printf("made log file\n");
	std::cout.flush();
	log_msg("Hello, Log!");

	clock_gettime(CLOCK_REALTIME, &exec_start_time);
	printf("Created log file\n");
	log_msg("Hello, log!");

	//try priority switching

	std::thread th(thread_launch_fun);

	int sched_choice = SCHED_FIFO;
	int minparam = sched_get_priority_min(sched_choice);
	int maxparam = sched_get_priority_max(sched_choice);

	sched_param scp;
	int pri = (maxparam - minparam) * 4 / 3 + minparam;
	scp.__sched_priority = pri;
	int ret;
	if ((ret = pthread_setschedparam(th.native_handle(), sched_choice, &scp))) {
		log_msg("Couldn't set data-in thread priority to above normal: returned %d", ret);
		//TODOSG getting error 22:
		//ret 22 == EINVAL: "policy is not a recognized policy, or param does not make sense for the policy."
	}
	else {
		log_msg("Data-in thread priority set to %d", pri);
	}

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

	std::string outfname = "spike_data_outfile";
	FILE* nout;
	if ((nout = fopen(outfname.c_str(), "wb")) == (FILE*)NULL) {
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
			chans.push_back(std::to_string(i + 1) + "," + std::to_string(j + 1));

	//HFSubConsumer *vidcon = client->subscribeHighFreqData("PositionData", "CameraModule");
	//vidcon->initialize();

	std::vector<EventDataType> eventList = client->getEventList();
	for (EventDataType edt : eventList)
	{
		printf("%s\n\t%s\n\t%d\n", edt.getName().c_str(), edt.getOrigin().c_str(), edt.constgetsize());
		log_msg("%s\n\t\t%s\n\t\t%d", edt.getName().c_str(), edt.getOrigin().c_str(), edt.constgetsize());
	}

	std::vector<HighFreqDataType> hfList = client->getHighFreqList();
	for (HighFreqDataType dt : hfList)
	{
		printf("%s\n\t%s\n\t%d\n\t%s\n\t%s\n", dt.getName().c_str(), dt.getOrigin().c_str(), dt.getByteSize(), dt.getDataFormat().c_str(), dt.getSockAddr().c_str());
		log_msg("%s\n\t\t%s\n\t\t%d\n\t\t%s\n\t\t%s", dt.getName().c_str(), dt.getOrigin().c_str(), dt.getByteSize(), dt.getDataFormat().c_str(), dt.getSockAddr().c_str());
	}

	client->subscribeToEvent("CameraModule", "newZone");

	std::vector<std::string> ntrodes;
	for (size_t i = 0; i < num_tets; i++)
		ntrodes.push_back(std::to_string(i + 1) + ",0");

	SpikesConsumer *spkcon = client->subscribeSpikeData(TRODES_BUF_SIZE, ntrodes);
	spkcon->initialize();

	spikePacket spkbuf;
	//int16_t* neubuf = (int16_t*)malloc(sizeof(int16_t) * chans.size());
	//int vidsz = vidcon->getType().getByteSize();
	//char* vidbuf = (char*)malloc(vidsz);
	//log_msg("vidsz = %d", vidsz);

	struct timespec start_stream, current_stream;
	clock_gettime(CLOCK_REALTIME, &start_stream);

	bool nonzerodata = false;

	do {
		size_t n = spkcon->available(0);
		for (size_t i = 0; i < n; i++)
		{
			timestamp_t ts = spkcon->getData(&spkbuf);

			fwrite(&spkbuf, sizeof(spikePacket), 1, nout);
			fwrite(&ts, sizeof(timestamp_t), 1, nout);

			nonzerodata = true;
		}


		std::this_thread::yield();

		clock_gettime(CLOCK_REALTIME, &current_stream);

	} while (current_stream.tv_sec - start_stream.tv_sec < 30 && IS_RUNNING);

	printf("nonzerodata? %d\n", int(nonzerodata));

	client->unsubscribeHighFreqData(hfType_SPIKE, TRODES_NETWORK_ID);
	fclose(nout);

	std::this_thread::sleep_for(std::chrono::seconds(3));
	th.join();

	delete client;
	fclose(mylogoutfile);
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
	log_msg("recv_event: %s, %s, %s, %d", origin.c_str(), event.c_str(), msg.getformat().c_str(), msg.size());
	uint8_t i1;
	uint16_t i2;
	double d1, d2, d3, d4, d5, d6, d7, d8;
	msg.popcontents(msg.getformat(), i1, i2, d1, d2, d3, d4, d5, d6, d7, d8);
	log_msg("%d, %d, %f, %f, %f, %f, %f, %f, %f, %f", int(i1), int(i2), d1, d2, d3, d4, d5, d6, d7, d8);
	IS_RUNNING = false;
}

void pyfun_end_run()
{
	IS_RUNNING = false;
}

int main() {
	return pyfun_start();
}