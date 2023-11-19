//
// Created by robin on 18.11.2023.
//

#ifndef CRAFTRANK_CORE_H
#define CRAFTRANK_CORE_H

#include <cmath>
#include <string>
#include <utility>
#include <vector>

const int EARTH_RADIUS_KM = 6371000;
const double MIN_LAT = -M_PI / 2.0f;
const double MAX_LAT = M_PI / 2.0f;
const double MIN_LON = -M_PI;
const double MAX_LON = M_PI;


struct BoundingCoordinates {
    double minLat{0}, maxLat{0}, minLon{0}, maxLon{0};
};


struct ZipCodeInfo {
    int extraDistance{0};
    double lat{0}; // Latitude in radians
    double lon{0}; // Longitude in radians
};

struct LatLon {
    double lat{0};
    double lon{0};
};

struct WorkerInfo {
    double lat{0}; // Latitude in radians
    double lon{0}; // Longitude in radians
    int maxDrivingDistance{40}; // Neighbour distance
    double profileScore{0};
};


// // helper function to compute latmin and latmax
// void computeBoundingCoordinates(const ZipCodeInfo& zipCode, BoundingCoordinates& boundingC);

double calculateDistance(double lat1, double lon1, double lat2, double lon2);


#endif //CRAFTRANK_CORE_H
