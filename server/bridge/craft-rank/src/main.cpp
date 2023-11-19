#include <iostream>
#include "rank_algo/craft_rank.h"
#include "db_reader/database_reader.h"


void test() {
    CraftRankHandler craftRankHandler;
    ZipCodeInfo zipCode;
    BoundingCoordinates boundingCoordinates;

    // craftRankHandler.queryDatabase(zipCode, boundingCoordinates);

    std::cout << "minLat: " << boundingCoordinates.minLat << std::endl;
    std::cout << "maxLat: " << boundingCoordinates.maxLat << std::endl;
    std::cout << "minLon: " << boundingCoordinates.minLon << std::endl;
    std::cout << "maxLon: " << boundingCoordinates.maxLon << std::endl;
}

void handleZip(int zip) {


}


int main() {
    ZipCodeInfo zipCodeInfo;
    WorkerInfo workerInfo;
    std::vector<int> neighbors;
    std::vector<int> workers;
    const std::vector<int> zips = {std::stoi("01067"), std::stoi("01069")};
    db::getWorkerInfo(1, workerInfo);
    db::getZipInfo(std::stoi("01067"), zipCodeInfo);
    db::getNeighboringZips(std::stoi("01067"), neighbors);
    db::allWorkersForZips(zips, workers);

    std::cout << "Worker Info:" << std::endl;
    std::cout << "Latitude: " << workerInfo.lat << std::endl;
    std::cout << "Longitude: " << workerInfo.lon << std::endl;
    std::cout << "Max Driving Distance: " << workerInfo.maxDrivingDistance << std::endl;
    std::cout << "Profile Score: " << workerInfo.profileScore << std::endl;

    // Print values from ZipCodeInfo struct
    std::cout << "Zip Code Info:" << std::endl;
    std::cout << "Extra Distance: " << zipCodeInfo.extraDistance << std::endl;
    std::cout << "Latitude: " << zipCodeInfo.lat << std::endl;
    std::cout << "Longitude: " << zipCodeInfo.lon << std::endl;

        // Print values from neighbors vector
    std::cout << "Neighbors:" << std::endl;
    for (int neighbor : neighbors) {
        std::cout << neighbor << " ";
    }
    std::cout << std::endl;

    // Print values from workers vector
    std::cout << "Workers:" << std::endl;
    for (int worker : workers) {
        std::cout << worker << " ";
    }
    std::cout << std::endl;

    return 0;
}