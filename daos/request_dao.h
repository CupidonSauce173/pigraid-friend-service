//
// Created by antoine on 02/06/24.
//

#ifndef PIGRAID_FRIEND_SERVICE_REQUEST_DAO_H
#define PIGRAID_FRIEND_SERVICE_REQUEST_DAO_H


#include "../models/request.h"
#include "../database/db_handler.h"

class request_dao {
private:
    std::shared_ptr<db_handler> handler_;
public:
    explicit request_dao(std::shared_ptr<db_handler>& _db_handler);

    // CRUD methods
    request insert(request& _request);
    request modify(const request& _request);
    request remove(const std::string& _request_identifier);

    // Get methods
    request              get_by_oid(const bsoncxx::oid& oid);
    request              get_by_sender_receiver_id(const std::string& sender_identifier, const std::string& receiver_identifier);
    std::vector<request> get_all_by_sender_id(const std::string& sender_identifier);
    std::vector<request> get_all_by_receiver_id(const std::string& receiver_identifier);
};


#endif //PIGRAID_FRIEND_SERVICE_REQUEST_DAO_H
