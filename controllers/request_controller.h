#ifndef PIGRAID_FRIEND_SERVICE_REQUEST_CONTROLLER_H
#define PIGRAID_FRIEND_SERVICE_REQUEST_CONTROLLER_H


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

class request_controller : public oatpp::web::server::api::ApiController {
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
    explicit request_controller(request_dao dao, OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            : oatpp::web::server::api::ApiController(objectMapper), dao_(std::move(dao)) {
    }
public:

    ENDPOINT("GET", "/friends/requests/receiver/findAll", getRequestsByReceiverId, QUERY(String, receiver_identifier, "id")) {
        // Now the logic to find all the requests bound to the receiver
        std::string receiver_id = receiver_identifier;
        std::vector<request> requests = dao_.get_all_by_receiver_id(receiver_id);
        // TODO: Add data validation
        // Create the Data Transfer Object (DTO) list
        auto requests_dto = oatpp::List<oatpp::Object<request_dto>>::createShared();
        for (request& current_request : requests) {
            auto dto_object = request_dto::createShared();
            dto_object->id = current_request.id().to_string().c_str();
            dto_object->sender_identifier = current_request.get_sender_identifier().c_str();
            dto_object->receiver_identifier = current_request.get_receiver_identifier().c_str();
            dto_object->created_at = request::to_iso_string(current_request.created_at()).c_str();
            dto_object->destroy_at = request::to_iso_string(current_request.destroy_at()).c_str();
            requests_dto->push_back(dto_object);
        }

        // Create response and send it back
        auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        oatpp::String jsonData = jsonObjectMapper->writeToString(requests_dto);
        auto response = createResponse(Status::CODE_200, jsonData);
        response->putHeader("Content-Type", "application/json");
        return response;
    }

    ENDPOINT("GET", "/friends/requests/sender/findAll", getRequestsBySenderId, QUERY(String, sender_identifier, "id")) {
        // Now the logic to find all the requests bound to the sender
        // TODO: Add data validation
        std::string sender_id = sender_identifier;
        std::vector<request> requests = dao_.get_all_by_sender_id(sender_id);

        // Create the Data Transfer Object (DTO) list
        auto requests_dto = oatpp::List<oatpp::Object<request_dto>>::createShared();
        for (request& current_request : requests) {
            auto dto_object = request_dto::createShared();
            dto_object->id = current_request.id().to_string().c_str();
            dto_object->sender_identifier = current_request.get_sender_identifier().c_str();
            dto_object->receiver_identifier = current_request.get_receiver_identifier().c_str();
            dto_object->created_at = request::to_iso_string(current_request.created_at()).c_str();
            dto_object->destroy_at = request::to_iso_string(current_request.destroy_at()).c_str();
            requests_dto->push_back(dto_object);
        }

        // Create response and send it back
        auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        oatpp::String jsonData = jsonObjectMapper->writeToString(requests_dto);
        auto response = createResponse(Status::CODE_200, jsonData);
        response->putHeader("Content-Type", "application/json");
        return response;
    }

    ENDPOINT("POST", "/friends/requests/add", insertRequest, BODY_DTO(Object<request_dto>, dto_object)) {
        using namespace request_errors::ids; using namespace request_errors::messages; using namespace request_errors::lang_id;
        auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

        // First, check if insertion, ID must be empty.
        if(!dto_object->id->empty()) {
            auto error = create_error(NEW_REQUEST_ID_NOT_EMPTY_ID, NEW_REQUEST_ID_NOT_EMPTY, NEW_REQUEST_ID_NOT_EMPTY_LANG);
            auto response = createResponse(Status::CODE_422, jsonObjectMapper->writeToString(error));
            response->putHeader("Content-Type", "application/json");
            return response;
        }
        // Check other inputs
        if(dto_object->sender_identifier->empty()) {
            auto error = create_error(SENDER_ID_EMPTY_ID, SENDER_ID_EMPTY, SENDER_ID_EMPTY_LANG);
            auto response = createResponse(Status::CODE_422, jsonObjectMapper->writeToString(error));
            response->putHeader("Content-Type", "application/json");
            return response;
        } else if (dto_object->receiver_identifier->empty()) {
            auto error = create_error(RECEIVER_ID_EMPTY_ID, RECEIVER_ID_EMPTY, RECEIVER_ID_EMPTY_LANG);
            auto response = createResponse(Status::CODE_422, jsonObjectMapper->writeToString(error));
            response->putHeader("Content-Type", "application/json");
            return response;
        }

        // DTO fields OK - Does the request already exist ?
        request searched_request = dao_.get_by_sender_receiver_id(dto_object->sender_identifier, dto_object->receiver_identifier);
        if((String) searched_request.get_receiver_identifier() == dto_object->receiver_identifier){
            auto error = create_error(REQUEST_ALREADY_EXISTS_ID, REQUEST_ALREADY_EXISTS, REQUEST_ALREADY_EXISTS_LANG);
            auto response = createResponse(Status::CODE_409, jsonObjectMapper->writeToString(error));
            response->putHeader("Content-Type", "application/json");
            return response;
        }

        // Request doesn't exist, inserting the request in the database.
        // Creating entity
        auto created_at = std::chrono::system_clock::now();
        auto destroy_at = created_at + std::chrono::hours (100 * 24);
        request request_obj = request{
            dto_object->sender_identifier,
            dto_object->receiver_identifier,
            created_at,
            destroy_at
        };
        // Inserting it in the database
        request inserted_request = dao_.insert(request_obj);
        if(inserted_request.id().to_string().empty()) {
            auto error = create_error(REQUEST_NOT_INSERTED_ID, REQUEST_NOT_INSERTED, REQUEST_NOT_INSERTED_LANG);
            auto response = createResponse(Status::CODE_500, jsonObjectMapper->writeToString(error));
            response->putHeader("Content-Type", "application/json");
            return response;
        }
        // No error, returning the request as response.
        // Updating the dto_object with new data.
        dto_object->id = inserted_request.id().to_string();
        dto_object->sender_identifier = inserted_request.get_sender_identifier();
        dto_object->receiver_identifier = inserted_request.get_receiver_identifier();
        dto_object->created_at = request::to_iso_string(inserted_request.created_at());
        dto_object->destroy_at = request::to_iso_string(inserted_request.destroy_at());

        // Mapping dto to oatpp::String and sending it in response
        oatpp::String jsonData = jsonObjectMapper->writeToString(dto_object);
        auto response = createResponse(Status::CODE_200, jsonData);
        response->putHeader("Content-Type", "application/json");
        return response;
    }

    ENDPOINT("PATCH", "/friends/requests/update", updateRequest, BODY_DTO(Object<request_dto>, dto_object)) {
        using namespace request_errors::ids; using namespace request_errors::messages; using namespace request_errors::lang_id;
        auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

        // Validate request ID
        if(dto_object->id->empty()) {
            auto error = create_error(REQUEST_ID_EMPTY_ID, REQUEST_ID_EMPTY, REQUEST_ID_EMPTY_LANG);
            auto response = createResponse(Status::CODE_422, jsonObjectMapper->writeToString(error));
            response->putHeader("Content-Type", "application/json");
            return response;
        }

        // Fetch the existing request from the database
        request existing_request = dao_.get_by_oid(bsoncxx::oid(*dto_object->id));
        if(existing_request.get_receiver_identifier().empty()) {
            auto error = create_error(REQUEST_ID_EMPTY_ID, REQUEST_ID_EMPTY, REQUEST_ID_EMPTY_LANG);
            auto response = createResponse(Status::CODE_404, jsonObjectMapper->writeToString(error));
            response->putHeader("Content-Type", "application/json");
            return response;
        }

        // Update request object with new data
        existing_request.set_sender_identifier(*dto_object->sender_identifier);
        existing_request.set_receiver_identifier(*dto_object->receiver_identifier);
        // Update created_at and destroy_at ? TODO: See if I should do that, too drunk for that right now.

        // Perform the update operation
        request updated_request = dao_.modify(existing_request);
        if(updated_request.id().to_string().empty()) {
            auto error = create_error(REQUEST_NOT_INSERTED_ID, REQUEST_NOT_INSERTED, REQUEST_NOT_INSERTED_LANG);
            auto response = createResponse(Status::CODE_500, jsonObjectMapper->writeToString(error));
            response->putHeader("Content-Type", "application/json");
            return response;
        }

        // Create DTO object with updated data
        auto updated_dto = request_dto::createShared();
        updated_dto->id = updated_request.id().to_string();
        updated_dto->sender_identifier = updated_request.get_sender_identifier().c_str();
        updated_dto->receiver_identifier = updated_request.get_receiver_identifier().c_str();
        // Update other fields...

        // Sending response
        oatpp::String jsonData = jsonObjectMapper->writeToString(updated_dto);
        auto response = createResponse(Status::CODE_200, jsonData);
        response->putHeader("Content-Type", "application/json");
        return response;
    }


    ENDPOINT("DELETE", "/friends/requests/delete", deleteRequestById, QUERY(String, request_identifier, "id")) {
        using namespace request_errors::ids; using namespace request_errors::messages; using namespace request_errors::lang_id;
        auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

        std::string request_id = request_identifier;
        // Validating fields
        if(request_id.empty()) {
            auto error = create_error(REQUEST_ID_EMPTY_ID, REQUEST_ID_EMPTY, REQUEST_ID_EMPTY_LANG);
            auto response = createResponse(Status::CODE_422, jsonObjectMapper->writeToString(error));
            response->putHeader("Content-Type", "application/json");
            return response;
        }
        // Fields OK - Deleting request from the database
        request delete_request = dao_.remove(request_id);
        if(delete_request.get_receiver_identifier().empty()) {
            auto error = create_error(REQUEST_NOT_DELETE_ID, REQUEST_NOT_DELETED, REQUEST_NOT_DELETED_LANG);
            auto response = createResponse(Status::CODE_500, jsonObjectMapper->writeToString(error));
            response->putHeader("Content-Type", "application/json");
            return response;
        }
        // Creating the dto object
        auto delete_dto = request_dto::createShared();
        delete_dto->id = delete_request.id().to_string();
        delete_dto->receiver_identifier = delete_request.get_receiver_identifier();
        delete_dto->sender_identifier = delete_request.get_sender_identifier();
        delete_dto->created_at = request::to_iso_string(delete_request.created_at());
        delete_dto->destroy_at = request::to_iso_string(delete_request.destroy_at());

        // Sending response
        oatpp::String jsonData = jsonObjectMapper->writeToString(delete_dto);
        auto response = createResponse(Status::CODE_200, jsonData);
        response->putHeader("Content-Type", "application/json");
        return response;
    }
};

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // PIGRAID_FRIEND_SERVICE_REQUEST_CONTROLLER_H
