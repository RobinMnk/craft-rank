#include <iostream>
#include "rank_algo/craft_rank.h"
#include "db_reader/database_reader.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/ostreamwrapper.h"

void writeResultsToStdout(std::shared_ptr<std::vector<Result>>& res) {
    rapidjson::Document d;
    d.SetObject();

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

void handleFindAllCraftsmen(int zipcode) {
    ResultList results;

    CraftRankHandler::generateRankedListOfWorkers(zipcode, results);

}


int main(int argc, char** argv) {
//    std::cout << "Test #3" << std::endl;

    if(argc == 2) {
        int zipcode = std::atoi(argv[1]);
        handleFindAllCraftsmen(zipcode);
        return 0;
    }
    else {
        std::cerr << "Usage: ./CraftRank <zipcode>" << std::endl;
        return 1;
    }
}