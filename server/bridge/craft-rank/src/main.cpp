#include <iostream>
<<<<<<< HEAD:craft-rank/app/main.cpp
#include "../rank_algo/craft_rank.h"
#include <sqlite3.h> 
=======
#include "rank_algo/craft_rank.h"
>>>>>>> origin/master:server/bridge/craft-rank/src/main.cpp

void test() {
    CraftRankHandler craftRankHandler;
    ZipCodeInfo zipCode;
    BoundingCoordinates boundingCoordinates;

    craftRankHandler.queryDatabase(zipCode, boundingCoordinates);

    std::cout << "minLat: " << boundingCoordinates.minLat << std::endl;
    std::cout << "maxLat: " << boundingCoordinates.maxLat << std::endl;
    std::cout << "minLon: " << boundingCoordinates.minLon << std::endl;
    std::cout << "maxLon: " << boundingCoordinates.maxLon << std::endl;
<<<<<<< HEAD:craft-rank/app/main.cpp

=======
}

int main() {
>>>>>>> origin/master:server/bridge/craft-rank/src/main.cpp
    return 0;
}