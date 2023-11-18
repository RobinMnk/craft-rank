#include <iostream>
#include "rank_algo/craft_rank.h"

int main() {
    CraftRankHandler craftRankHandler;
    ZipCode zipCode;
    BoundingCoordinates boundingCoordinates;

    craftRankHandler.queryDatabase(zipCode, boundingCoordinates);

    std::cout << "minLat: " << boundingCoordinates.minLat << std::endl;
    std::cout << "maxLat: " << boundingCoordinates.maxLat << std::endl;
    std::cout << "minLon: " << boundingCoordinates.minLon << std::endl;
    std::cout << "maxLon: " << boundingCoordinates.maxLon << std::endl;
    return 0;
}