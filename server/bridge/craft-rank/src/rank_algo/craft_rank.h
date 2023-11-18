#ifndef CRAFT_RANK_H
#define CRAFT_RANK_H

#include <cmath>
#include <string>
#include <unordered_set>
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


class CraftRankHandler {
public:

    // Constructor
    CraftRankHandler();

    // Destructor
    ~CraftRankHandler();

    // Function to query a database and fill the ZipCodeInfo struct
    void queryDatabase(ZipCodeInfo& zipCode, BoundingCoordinates& boundingC);

    void getListOfWorkers(int zipCode);

    float rank(int zipCode, int workerId);

private:

    float calculateDistance(float lat1, float lon1, float lat2, float lon2);

    void generateRelevantZipCodes(int startingZip, std::vector<int> relevantZips);

    // Private helper function to compute latmin and latmax
    void computeBoundingCoordinates(const ZipCodeInfo& zipCode, BoundingCoordinates& boundingC);
};
#endif // CRAFT_RANK_H
