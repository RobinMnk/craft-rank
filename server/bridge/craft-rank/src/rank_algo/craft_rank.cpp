#include "craft_rank.h"
#include <iostream>
#include <queue>

// Constructor
CraftRankHandler::CraftRankHandler() {
    // Initialize any necessary resources here
}

// Destructor
CraftRankHandler::~CraftRankHandler() {
    // Cleanup or release resources here, if needed
}

// Private helper function to compute bounding coordinates
void CraftRankHandler::computeBoundingCoordinates(const ZipCodeInfo& zipCode, BoundingCoordinates& boundingC) {
    float r = static_cast<float>(zipCode.distance) / static_cast<float>(EARTH_RADIUS_KM);
    boundingC.minLat = zipCode.lat - r;
    boundingC.maxLat = zipCode.lat + r;

    std::cout << r << std::endl;

    if (boundingC.minLat > MIN_LAT && boundingC.maxLat < MAX_LAT) {
        double deltaLon = asin(sin(r) / cos(zipCode.lat));
        boundingC.minLon = zipCode.lon - deltaLon;
        if (boundingC.minLon < MIN_LON) boundingC.minLon += 2.0 * M_PI;
        boundingC.maxLon = zipCode.lon + deltaLon;
        if (boundingC.maxLon > MAX_LON) boundingC.maxLon -= 2.0 * M_PI;
    } else {
        // A pole is within the distance
        boundingC.minLat = fmax(boundingC.minLat, MIN_LAT);
        boundingC.maxLat = fmin(boundingC.maxLat, MAX_LAT);
        boundingC.minLon = MIN_LON;
        boundingC.maxLon = MAX_LON;
    }
}


// Function to query a database and fill the ZipCodeInfo struct
void CraftRankHandler::queryDatabase(ZipCodeInfo& zipCode, BoundingCoordinates& boundingC) {
    // Replace this with your actual database query logic
    // For demonstration purposes, we'll set some example values.

    // Example: Query extraDistance, latitude, longitude, and max_driving_dist from the database
    zipCode.extraDistance = 0;
    zipCode.lat = 1.3963; // Example latitude in radians
    zipCode.lon = -0.6981; // Example longitude in radians
    zipCode.distance = 1000;

    computeBoundingCoordinates(zipCode, boundingC);

}

void CraftRankHandler::generateRelevantZipCodes(int startZip, std::vector<int> relevantZips) {
    std::unordered_set<int> visited;
    relevantZips.clear();

    std::queue<int> q{};
    q.push(startZip);

    std::vector<int> neighbors;

    while(!q.empty()) {
        int currentZip = q.front();
        q.pop();
        if(visited.contains(currentZip) || distanceBetweenZips(startZip, currentZip) > MAX_ZIP_DISTANCE) {
            continue;
        }
        visited.insert(currentZip);
        relevantZips.push_back(currentZip);

        db::getNeighboringZips(currentZip, neighbors);
        for(int nb: neighbors) {
            q.push(nb);
        }
    }
}

void CraftRankHandler::getListOfWorkers(int zipCode) {
    std::vector<int> relevantZips;
    generateRelevantZipCodes(zipCode, relevantZips);

    std::vector<int> workers;
    db::allWorkersForZips(relevantZips, workers);



}

float CraftRankHandler::rank(int zipCode, int workerId) {
    return 0;
}
