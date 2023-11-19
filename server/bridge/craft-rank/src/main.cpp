#include <iostream>
#include "rank_algo/craft_rank.h"
#include "db_reader/database_reader.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/ostreamwrapper.h"

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


void writeResultsToStdout(std::shared_ptr<std::vector<Result>>& res) {
    rapidjson::Document d;
    d.SetObject();
    char* buffer = new char[2048];

    for(auto entry: *res) {
        // Add data to the JSON document
        d.AddMember("id", entry.workerId, d.GetAllocator());
//      TODO:  d.AddMember("name", entry.name, d.GetAllocator());
        d.AddMember("rankingScore", entry.rank, d.GetAllocator());
    }

    rapidjson::OStreamWrapper out(std::cout);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(out);
    d.Accept(writer);
}



int main() {



    return 0;
}