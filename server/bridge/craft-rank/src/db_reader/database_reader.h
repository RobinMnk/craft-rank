#include "../rank_algo/core.h"
#include <iostream>
#include <sqlite3.h>

#ifndef CRAFTRANK_DATABASE_READER_H
#define CRAFTRANK_DATABASE_READER_H

namespace db {

    void getNeighboringZips(int zipCode, const std::vector<int>& neighbors);

    void allWorkersForZips(const std::vector<int>& zips, const std::vector<int>& workers);

    double queryDatabaseForDistance(const std::string& zipCode1, const std::string& zipCode2);

    int Callback(void* data, int argc, char** argv, char** colNames);

    ZipCodeInfo getZipInfo(int zipCode);

    WorkerInfo getWorkerInfo(int workerId);

}

#endif //CRAFTRANK_DATABASE_READER_H