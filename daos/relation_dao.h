//
// Created by antoine on 05/06/24.
//

#ifndef PIGRAID_FRIEND_SERVICE_RELATION_DAO_H
#define PIGRAID_FRIEND_SERVICE_RELATION_DAO_H


#include <memory>
#include "../database/db_handler.h"

class relation_dao {
private:
    std::shared_ptr<db_handler> handler_;
public:
    explicit relation_dao(std::shared_ptr<db_handler>& _db_handler);

    // CRUD Methods


    // Get methods
};


#endif //PIGRAID_FRIEND_SERVICE_RELATION_DAO_H
