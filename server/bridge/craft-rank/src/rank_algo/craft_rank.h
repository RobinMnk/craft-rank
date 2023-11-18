#ifndef CRAFT_RANK_H
#define CRAFT_RANK_H

#include <cmath>
#include <string>

const int EARTH_RADIUS_KM = 6371;
const float MIN_LAT = -M_PI / 2.0f;
const float MAX_LAT = M_PI / 2.0f;
const float MIN_LON = -M_PI;
const float MAX_LON = M_PI;

struct ZipCode {
    int group;
    float lat; // Latitude in radians
    float lon; // Longitude in radians
    int distance=40; // Neighbour distance
};

struct BoundingCoordinates {
    float minLat, maxLat, minLon, maxLon;
};

class CraftRankHandler {
public:
    // Constructor
    CraftRankHandler();

    // Destructor
    ~CraftRankHandler();

    // Function to query a database and fill the ZipCode struct
    void queryDatabase(ZipCode& zipCode, BoundingCoordinates& boundingC);

private:

    // Private helper function to compute latmin and latmax
    void computeBoundingCoordinates(const ZipCode& zipCode, BoundingCoordinates& boundingC);

};
#endif // CRAFT_RANK_H
