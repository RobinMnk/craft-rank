#include <iostream>
#include "rank_algo/craft_rank.h"
#include "db_reader/database_reader.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/ostreamwrapper.h"

void writeResultsToStdout(ResultList& res) {
    rapidjson::Document d;
    d.SetObject();

    rapidjson::Value myArray(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    for(auto entry: res->list) {
        rapidjson::Value objValue;
        objValue.SetObject();
        d.AddMember("id", entry.workerId, allocator);
//      TODO:  d.AddMember("name", entry.name, d.GetAllocator());
        d.AddMember("rankingScore", entry.rank, allocator);

        myArray.PushBack(objValue, allocator);
    }

    d.AddMember("craftsmen", myArray, allocator);
    rapidjson::OStreamWrapper out(std::cout);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(out);
    d.Accept(writer);
}

void handleFindAllCraftsmen(int zipcode) {
    ResultList results;
    CraftRankHandler::generateRankedListOfWorkers(zipcode, results);
    writeResultsToStdout(results);
}


int main(int argc, char** argv) {
//    std::cout << "Test #3" << std::endl;

    if(argc == 2) {
        int zipcode = std::stoi(argv[1]);
        handleFindAllCraftsmen(zipcode);
        return 0;
    }
    else {
        std::cerr << "Usage: ./CraftRank <zipcode>" << std::endl;
        return 1;
    }
}