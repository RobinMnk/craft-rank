#include <iostream>
#include <sqlite3.h> 
#include "rank_algo/craft_rank.h"
#include "db_reader/database_reader.h"


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
    test();
    db::queryDatabaseForDistance("01067", "01069");
    return 0;
}