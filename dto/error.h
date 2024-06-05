//
// Created by antoine on 04/06/24.
//

#ifndef FRIEND_SERVICE_ERROR_H
#define FRIEND_SERVICE_ERROR_H

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>
#include <bsoncxx/oid.hpp>

namespace request_errors
{
    namespace messages {
        static std::string REQUEST_ALREADY_EXISTS   = "The friend request already exists.";
        static std::string RECEIVER_ID_EMPTY        = "The receiver id is invalid or empty.";
        static std::string SENDER_ID_EMPTY          = "The sender id is invalid or empty.";
        static std::string NEW_REQUEST_ID_NOT_EMPTY = "Id should be empty when inserting a new request.";
        static std::string REQUEST_NOT_INSERTED     = "The request hasn't been inserted in the database.";
        static std::string REQUEST_NOT_DELETED      = "The request hasn't been deleted from the database.";
        static std::string REQUEST_ID_EMPTY         = "The request id is invalid or empty.";
    }

    namespace ids {
        static uint8_t REQUEST_ALREADY_EXISTS_ID   = 1;
        static uint8_t RECEIVER_ID_EMPTY_ID        = 2;
        static uint8_t SENDER_ID_EMPTY_ID          = 3;
        static uint8_t NEW_REQUEST_ID_NOT_EMPTY_ID = 4;
        static uint8_t REQUEST_NOT_INSERTED_ID     = 5;
        static uint8_t REQUEST_NOT_DELETE_ID       = 6;
        static uint8_t REQUEST_ID_EMPTY_ID         = 7;
    }

    namespace lang_id {
        static std::string REQUEST_ALREADY_EXISTS_LANG   = "friends.requests.alreadyExist";
        static std::string RECEIVER_ID_EMPTY_LANG        = "friends.requests.receiverIdEmpty";
        static std::string SENDER_ID_EMPTY_LANG          = "friends.requests.senderIdEmpty";
        static std::string NEW_REQUEST_ID_NOT_EMPTY_LANG = "friends.requests.newRequestIdNotEmpty";
        static std::string REQUEST_NOT_INSERTED_LANG     = "friends.requests.requestNotInserted";
        static std::string REQUEST_NOT_DELETED_LANG      = "friends.requests.requestNotDeleted";
        static std::string REQUEST_ID_EMPTY_LANG         = "friends.requests.idEmpty";
    }
}

namespace relation_errors
{
    namespace messages
    {
        static std::string BASE_USER_ID_EMPTY    = "The xuid of the base user (requester) is empty or invalid.";
        static std::string RELATION_NOT_EXIST    = "The relation between the requester and the target doesn't exist.";
        static std::string RELATION_NOT_DELETED  = "The relation hasn't been deleted.";
        // TODO Later
    }

    namespace ids
    {
        static uint8_t BASE_USER_ID_EMPTY_ID = 8;
        static uint8_t RELATION_NOT_EXIST_ID = 9;
        static uint8_t RELATION_NOT_DELETED  = 10;
        // TODO Later
    }

    namespace lang_id {
        static std::string BASE_USER_ID_EMPTY_LANG   = "friends.relations.userXuidEmpty";
        static std::string RELATION_NOT_EXIST_LANG   = "friends.relations.relationNotExist";
        static std::string RELATION_NOT_DELETED_LANG = "friends.relations.relationNotDeleted";
        // TODO Later
    };
}

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Class that represents the answer of the controllers when there are errors happening while processing the request.
 * An ID and a Message must be provided, used by the MiNET & Proxy instances.
 */
class error_dto : public oatpp::DTO {
    DTO_INIT(error_dto, DTO)

    DTO_FIELD(UInt8,  id);                      // Store the ID of the error.
    DTO_FIELD(String, message, "message");      // Store the message of the error.
    DTO_FIELD(String, lang, "lang");            // Store the lang id of the message, for internalization purposes.
};
#include OATPP_CODEGEN_END(DTO)

#endif //FRIEND_SERVICE_ERROR_H