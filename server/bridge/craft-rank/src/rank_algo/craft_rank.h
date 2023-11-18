#ifndef CRAFT_RANK_H
#define CRAFT_RANK_H

#include <cmath>
#include <string>
#include <unordered_set>
#include <mutex>
#include <condition_variable>
#include <utility>
#include "../db_reader/database_reader.h"

const int EARTH_RADIUS_KM = 6371;
const float MIN_LAT = -M_PI / 2.0f;
const float MAX_LAT = M_PI / 2.0f;
const float MIN_LON = -M_PI;
const float MAX_LON = M_PI;

const float MAX_ZIP_DISTANCE = 200; // TODO: anpassen!


struct BoundingCoordinates {
    float minLat{0}, maxLat{0}, minLon{0}, maxLon{0};
};

struct Result {
    int workerId;
    float rank;
};


float distanceBetweenZips(int zipA, int zipB);
float distanceBetweenZipAndWorker(int zip, int workerId);

float rank(int zipCode, int workerId);


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

    float calculateDistance(float lat1, float lon1, float lat2, float lon2);

    static void generateRelevantZipCodes(int startingZip, std::vector<int> relevantZips);

    // Private helper function to compute latmin and latmax
    void computeBoundingCoordinates(const ZipCodeInfo& zipCode, BoundingCoordinates& boundingC);
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
