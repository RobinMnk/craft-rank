#include "craft_rank.h"
#include "../db_reader/database_reader.h"
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

float CraftRankHandler::CalculateDistance(float lat1, float lon1, float lat2, float lon2) {
    // Convert latitude and longitude values from degrees to radians
    lat1 = lat1 * M_PI / 180.0;
    lon1 = lon1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;
    lon2 = lon2 * M_PI / 180.0;

    // Calculate the great circle distance
    float deltaLon = lon1 - lon2;
    float distance = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(deltaLon)) * EARTH_RADIUS_KM;

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

}

void CraftRankHandler::generateRelevantZipCodes(int startZip) {
    visited.clear();
    relevantZips.clear();
    std::queue<int> q{};
    q.push(startZip);

    while(!q.empty()) {
        int currentZip = q.front();
        q.pop();
        if(visited.contains(currentZip) || distanceBetweenZips(startZip, currentZip) > MAX_ZIP_DISTANCE) {
            continue;
        }
        visited.insert(currentZip);
        relevantZips.push_back(currentZip);

        for(int neighbor: db::getNeighboringZips(currentZip)) {
            q.push(neighbor);
        }
    }
}

