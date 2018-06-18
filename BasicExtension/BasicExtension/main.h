#pragma once

#include <stdarg.h>
#include "TrodesNetwork/AbstractModuleClient.h"

extern "C" {
	__attribute__((visibility("default"))) void pyfun_end_first_run();
	__attribute__((visibility("default"))) void pyfun_end_second_run();
	__attribute__((visibility("default"))) int pyfun_start();
}

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

