#ifndef CRAFT_RANK_H
#define CRAFT_RANK_H

#include <cmath>
#include <string>
#include <unordered_set>
#include <mutex>
#include <condition_variable>
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

    void getListOfWorkers(int zipCode);


private:

    float calculateDistance(float lat1, float lon1, float lat2, float lon2);

    void generateRelevantZipCodes(int startingZip, std::vector<int> relevantZips);

    // Private helper function to compute latmin and latmax
    void computeBoundingCoordinates(const ZipCodeInfo& zipCode, BoundingCoordinates& boundingC);
};


class ParallelRank {
    std::vector<int> queue;

    std::mutex mtx;
    std::condition_variable cv;

public:
    void reset() {
        queue.clear();
    }

    void insert(int value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push_back(value);
        cv.notify_one();
    }

    void process() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return !queue.empty(); });
            int data = queue.back();
            queue.pop_back();
            lock.unlock();


        }
    }


};



#endif // CRAFT_RANK_H
