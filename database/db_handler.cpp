//
// Created by antoine on 02/06/24.
//

#include <mongocxx/instance.hpp>
#include "db_handler.h"

std::vector<std::string> get_databases(const mongocxx::client& client) {
    return client.list_database_names();
}

std::vector<std::string> get_collections(const std::string& database_name, const mongocxx::client& client) {
    return client.database(database_name).list_collection_names();
}


db_handler::db_handler(const mongocxx::uri& uri)
        : client_(uri), database_(client_["pigraid_db"]) {
}

mongocxx::collection db_handler::get_relations_collection() const {
    return database_.collection("friend_relations");
}

mongocxx::collection db_handler::get_requests_collection() const {
    return database_.collection("friend_requests");
}
