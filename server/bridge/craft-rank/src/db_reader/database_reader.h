#include "../rank_algo/core.h"
#include <iostream>
#include <sqlite3.h>

#ifndef CRAFTRANK_DATABASE_READER_H
#define CRAFTRANK_DATABASE_READER_H

namespace db {

    void getNeighboringZips(int zipCode, std::vector<int>& neighbors);

    void allWorkersForZips(const std::vector<int>& zips, std::vector<int>& workers);

    void allWorkersForSingleZip(int zip, const std::vector<int>& workers);

    double distanceBetweenZips(int zipCode1, int zipCode2);

    double distanceBetweenZipAndWorker(int zipCode, int workerId);

    int Callback(void* data, int argc, char** argv, char** colNames);

    void getZipInfo(int zipCode, ZipCodeInfo& zipCodeInfo);

    void getWorkerInfo(int workerId, WorkerInfo& workerInfo);

}

#endif //CRAFTRANK_DATABASE_READER_H