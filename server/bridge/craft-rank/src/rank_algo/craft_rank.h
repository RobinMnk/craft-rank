#ifndef CRAFT_RANK_H
#define CRAFT_RANK_H

#include "core.h"
#include "../db_reader/database_reader.h"
#include <unordered_set>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <queue>

const double MAX_ZIP_DISTANCE = 200; // TODO: anpassen!

struct Result {
    int workerId;
    double rank;
};

double distanceBetweenZips(int zipA, int zipB);
double distanceBetweenZipAndWorker(int zip, int workerId);

double rank(int zipCode, int workerId);


class CraftRankHandler {
public:

    // Constructor
    CraftRankHandler();

    // Destructor
    ~CraftRankHandler();

    // Function to query a database and fill the ZipCodeInfo struct
    void queryDatabase(ZipCodeInfo& zipCode, BoundingCoordinates& boundingC);

    static void getListOfWorkers(int zipCode);

private:

    static void generateRelevantZipCodes(int startingZip, std::vector<int> relevantZips);
};

class ParallelRank {
    std::shared_ptr<std::vector<int>> queue;

    std::mutex mtx_queue, mtx_results;
    std::condition_variable cv;

    int baseZip;
    std::shared_ptr<std::vector<Result>> results;
public:
    std::thread start();

    void init(int zip, std::shared_ptr<std::vector<int>>& q, std::shared_ptr<std::vector<Result>>& res);

    void insert(int value);

    void process();

private:
    void safeWrite(Result res);
};

#endif // CRAFT_RANK_H
