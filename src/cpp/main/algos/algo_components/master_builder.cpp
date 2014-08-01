/*
 * master_builder.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "master_builder.h"
#include "inner_master_factory.h"
#include <cstdlib>

MasterBuilder::MasterBuilder() {
	logger_ = &log4cpp::Category::getInstance(std::string("MasterBuilder"));
}

Master* MasterBuilder::createMaster(unordered_map<string, string>* params) {
	int master_port, num_slaves;
	long numLine;
	char logfile_name[1024], slavery_cfg[1024];

	sscanf((*params)["MASTER_PORT"].c_str(), "%d", &master_port);
	sscanf((*params)["NUM_SLAVES"].c_str(), "%d", &num_slaves);
	sscanf((*params)["NUMLINE"].c_str(), "%ld", &numLine);

	logger_->info("numline %ld", numLine);
	strcpy(logfile_name, (*params)["LOGFILE_NAME"].c_str());
	strcpy(slavery_cfg, (*params)["REMOTE_DIR"].c_str());
	strcat(slavery_cfg, (*params)["SLAVERY_CFG"].c_str());

	vector<Slave>* slaves = readSlaveConfigs(slavery_cfg, num_slaves);
	Master* master = new Master(master_port, slaves, numLine);
	return master;
}

vector<Slave>* MasterBuilder::readSlaveConfigs(char* cfg, int num_slaves) {
	vector<Slave>* slaves;
	FILE* slavery_par = fopen(cfg, "r");
	if (slavery_par == NULL)
	{
		// TODO node-oknak is meg kell halniuk
		logger_->error("Error opening %s in master main.", cfg);
		exit(1);
	}
	slaves = new vector<Slave>(num_slaves);
	for (int i = 0; i < num_slaves; ++i)
	{
	  fscanf(slavery_par,"%d %*s %ld %*ld %ld", &((*slaves)[i].port), &((*slaves)[i].numNode),  &((*slaves)[i].minNode));
	}
	fclose(slavery_par);
	return slaves;
}
