#include "craft_rank.h"
#include <iostream>

// Constructor
CraftRankHandler::CraftRankHandler() {
    // Initialize any necessary resources here
}

// Destructor
CraftRankHandler::~CraftRankHandler() {
    // Cleanup or release resources here, if needed
}

// Private helper function to compute bounding coordinates
void CraftRankHandler::computeBoundingCoordinates(const ZipCode& zipCode, BoundingCoordinates& boundingC) {
    float r = zipCode.distance / EARTH_RADIUS_KM;
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


// Function to query a database and fill the ZipCode struct
void CraftRankHandler::queryDatabase(ZipCode& zipCode, BoundingCoordinates& boundingC) {
    // Replace this with your actual database query logic
    // For demonstration purposes, we'll set some example values.
    
    // Example: Query group, latitude, longitude, and max_driving_dist from the database
    zipCode.group = 2;
    zipCode.lat = 0.7854f; // Example latitude in radians
    zipCode.lon = -1.7453f; // Example longitude in radians
    zipCode.distance = 40;

    computeBoundingCoordinates(zipCode, boundingC);

}