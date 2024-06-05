//
// Created by antoine on 04/06/24.
//

#ifndef FRIEND_SERVICE_RELATION_CONTROLLER_H
#define FRIEND_SERVICE_RELATION_CONTROLLER_H

#include <iostream>
#include <utility>
#include <string>
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include "../dto/request.h"
#include "../daos/request_dao.h"
#include "../database/db_handler.h"
#include "../dto/error.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

class relation_controller : public oatpp::web::server::api::ApiController {
private:
    request_dao dao_;

    static oatpp::data::mapping::type::DTOWrapper<error_dto> create_error(uint8_t id, const std::string& message, const std::string& lang) {
        auto error = error_dto::createShared();
        error->id = id;
        error->message = message;
        error->lang = lang;
        return error;
    }
public:
    explicit relation_controller(relation_dao dao, OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper), dao_(std::move(dao)) {
    }
public:
    // TODO: Get friend list status
    ENDPOINT("GET", "/friends/relations/findAll", findAllFriendRequests, QUERY(String, xuid, "xuid")) {
        using namespace relation_errors::ids; using namespace relation_errors::messages; using namespace relation_errors::lang_id;
        std::string xuid_str = xuid;
        auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

        // Validate data
        if(xuid_str.empty()) {
            auto error = create_error(BASE_USER_ID_EMPTY_ID, BASE_USER_ID_EMPTY, BASE_USER_ID_EMPTY_LANG);
            auto response = createResponse(Status::CODE_200, jsonObjectMapper->writeToString(error));
            response->putHeader("Content-Type", "application/json");
            return response;
        }

        // Get the relatin list
    }
    // TODO: Get friend list
    // TODO: Create friend relation
    // TODO: Remove friend relation

};

#include OATPP_CODEGEN_END(ApiController) //<- Begin Codegen

#endif //FRIEND_SERVICE_RELATION_CONTROLLER_H
