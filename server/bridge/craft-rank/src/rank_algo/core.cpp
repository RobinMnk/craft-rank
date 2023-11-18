//
// Created by robin on 18.11.2023.
//

#include "core.h"

void computeBoundingCoordinates(const ZipCodeInfo& zipCode, BoundingCoordinates& boundingC) {
    double r = static_cast<double>(zipCode.distance) / static_cast<double>(EARTH_RADIUS_KM);
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


double calculateDistance(double lat1, double lon1, double lat2, double lon2){
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