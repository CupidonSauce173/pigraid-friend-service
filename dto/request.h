//
// Created by antoine on 03/06/24.
//

#ifndef FRIEND_SERVICE_REQUEST_DTO_H
#define FRIEND_SERVICE_REQUEST_DTO_H

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>
#include <bsoncxx/oid.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class request_dto : public oatpp::DTO {

    DTO_INIT(request_dto, DTO)

    DTO_FIELD(String, id, "id"); // NOTE: Is converted back to a bsoncxx/oid
    DTO_FIELD(String, sender_identifier, "sender_identifier");
    DTO_FIELD(String, receiver_identifier, "receiver_identifier");
    DTO_FIELD(String, created_at, "created_at"); // NOTE: Is converted back to a time_point
    DTO_FIELD(String, destroy_at, "destroy_at"); // NOTE: Is converted back to a time_point
};

#include OATPP_CODEGEN_END(DTO)

#endif //FRIEND_SERVICE_REQUEST_DTO_H
