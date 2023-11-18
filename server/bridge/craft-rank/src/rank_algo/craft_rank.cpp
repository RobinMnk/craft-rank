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

float CraftRankHandler::calculateDistance(float lat1, float lon1, float lat2, float lon2){
    // Convert latitude and longitude values from degrees to radians
    lat1 = lat1 * M_PI / 180.0;
    lon1 = lon1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;
    lon2 = lon2 * M_PI / 180.0;

    // Calculate the great circle distance using the provided formula
    double deltaLon = lon1 - lon2;
    double distance = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(deltaLon)) * EARTH_RADIUS_KM;

    return distance;
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
    std::cout << "Calculate distance" << calculateDistance(40.6892, -74.0444, 48.8583, 2.2945) << std::endl;

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

float rank(int zipCode, int workerId) {
    WorkerInfo workerInfo = db::getWorkerInfo(workerId);
    ZipCodeInfo zipCodeInfo = db::getZipInfo(zipCode);

    auto adjustedMaxDriveDistance = static_cast<float>(workerInfo.maxDrivingDistance + zipCodeInfo.extraDistance);
    float distance = distanceBetweenZipAndWorker(zipCode, workerId);

    if (distance > adjustedMaxDriveDistance) {
        return -10;
    }

    float distance_score = 1 - (distance / adjustedMaxDriveDistance);
    float distance_weight = distance > 80 ? 0.01 : 0.15;

    return distance_weight * distance_score + (1 - distance_weight) * workerInfo.profileScore;
}

