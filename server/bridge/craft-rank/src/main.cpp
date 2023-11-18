#include <iostream>
#include "../rank_algo/craft_rank.h"
#include <sqlite3.h> 
#include "rank_algo/craft_rank.h"

void test() {
    CraftRankHandler craftRankHandler;
    ZipCodeInfo zipCode;
    BoundingCoordinates boundingCoordinates;

    craftRankHandler.queryDatabase(zipCode, boundingCoordinates);

    std::cout << "minLat: " << boundingCoordinates.minLat << std::endl;
    std::cout << "maxLat: " << boundingCoordinates.maxLat << std::endl;
    std::cout << "minLon: " << boundingCoordinates.minLon << std::endl;
    std::cout << "maxLon: " << boundingCoordinates.maxLon << std::endl;
}

int main() {
    return 0;
}