//
// Created by antoine on 02/06/24.
//

#ifndef PIGRAID_FRIEND_SERVICE_DB_HANDLER_H
#define PIGRAID_FRIEND_SERVICE_DB_HANDLER_H


#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>

class db_handler {
private:
    mongocxx::uri uri_;
    mongocxx::client client_;
    mongocxx::database database_;
public:
    explicit db_handler(const mongocxx::uri& uri);
    [[nodiscard]] mongocxx::collection get_requests_collection() const;
    [[nodiscard]] mongocxx::collection get_relations_collection() const;
};


#endif //PIGRAID_FRIEND_SERVICE_DB_HANDLER_H
