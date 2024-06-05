//
// Created by antoine on 02/06/24.
//

#include <mongocxx/exception/exception.hpp>
#include "request_dao.h"

request_dao::request_dao(std::shared_ptr<db_handler>& _db_handler) : handler_(_db_handler) {}

// GET METHOD IMPL

request request_dao::get_by_oid(const bsoncxx::oid &oid) {
    request found_request;
    try {
        // Creating the filter for MongoDB
        auto filter = bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("_id", oid)
        );
        // Executing the query
        auto result = handler_->get_requests_collection().find_one(filter.view());
        if(result) {
            // Converting the BSON document to a request object
            found_request = request::from_bson(result.value());
        }
    } catch (const mongocxx::exception& exception) {
        std::cerr << "An exception occurred: " << exception.what() << "\n";
    }
    return found_request;
}

std::vector<request> request_dao::get_all_by_sender_id(const std::string &sender_identifier) {
    std::vector<request> requests;
    if(sender_identifier.empty()) {
        return requests;
    }
    try {
        // Creating the filter for mongodb
        bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> filter;
        filter = bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("sender_identifier", sender_identifier));
        auto cursor = handler_->get_requests_collection().find(filter);
        for(auto&& document : cursor) {
            // Converting to request and inserting it to the vector
            requests.push_back(request::from_bson(document));
        }
    } catch (const mongocxx::exception& exception) {
        std::cerr << "An exception occured: " << exception.what() << "\n";
    }
    return requests;
}

std::vector<request> request_dao::get_all_by_receiver_id(const std::string &receiver_identifier) {
    std::vector<request> requests;
    if(receiver_identifier.empty()) {
        return requests;
    }
    try {
        // Creating the filter for mongodb
        bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> filter;
        filter = bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("receiver_identifier", receiver_identifier));
        auto cursor = handler_->get_requests_collection().find(filter);
        for(auto&& document : cursor) {
            // Converting to request and inserting it to the vector
            requests.push_back(request::from_bson(document));
        }
    } catch (const mongocxx::exception& exception) {
        std::cerr << "An exception occured: " << exception.what() << "\n";
    }
    return requests;
}

request request_dao::get_by_sender_receiver_id(const std::string &sender_identifier,
                                               const std::string &receiver_identifier) {
    request found_request;
    if(sender_identifier.empty() || receiver_identifier.empty()) {
        return found_request;
    }
    try {
        // Creating the filter for mongodb
        bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> filter;
        filter = bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("sender_identifier", sender_identifier),
                bsoncxx::builder::basic::kvp("receiver_identifier", receiver_identifier));
        auto result = handler_->get_requests_collection().find_one(filter);
        if(result) {
            // Converting to request
            found_request = request::from_bson(result.value());
        }
    } catch (const mongocxx::exception& exception) {
        std::cerr << "An exception occurred: " << exception.what() << "\n";
    }
    return found_request;
}

// CRUD METHOD IMPL
request request_dao::insert(request &_request) {
    try {
        bsoncxx::document::value document = _request.to_bson();
        auto result = handler_->get_requests_collection().insert_one(document.view());
        if(result.has_value()) {
            // Fetch inserted it and set the _request.id to the result
            auto id = result.value().inserted_id();
            _request.set_id(id.get_oid().value);
        }
    } catch (const mongocxx::exception& exception) {
        std::cerr << "An exception occured: " << exception.what() << "\n";
    }
    // Return the _request, even if the ID wasn't set.
    return _request;
}

request request_dao::modify(const request &_request) {
    request updated_request;
    try {
        // Create filter for MongoDB
        auto filter = bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("_id", _request.id())
        );

        // Update operation
        auto result = handler_->get_requests_collection().update_one(
                filter.view(),
                _request.to_bson().view()
        );

        // Check if the update was successful
        if (result && result->modified_count() > 0) {
            // Fetch the updated document
            auto updated_doc = handler_->get_requests_collection().find_one(filter.view());
            if (updated_doc) {
                updated_request = request::from_bson(updated_doc.value());
            }
        }
    } catch (const mongocxx::exception& exception) {
        std::cerr << "An exception occurred: " << exception.what() << "\n";
        return {};
    }
    return updated_request;
}


request request_dao::remove(const std::string &_request_identifier) {
    request deleted_request;
    try {
        // Convert the request identifier to bsoncxx::oid
        bsoncxx::oid id(_request_identifier);

        // Create the filter for MongoDB
        auto filter = bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("_id", id)
        );

        // Find the document before deleting it to return it after deletion
        auto result = handler_->get_requests_collection().find_one(filter.view());
        if (result) {
            deleted_request = request::from_bson(result.value());
        } else {
            // If no document is found, return an empty request
            return deleted_request;
        }

        // Execute the delete operation
        auto delete_result = handler_->get_requests_collection().delete_one(filter.view());
        if (delete_result && delete_result->deleted_count() > 0) {
            // Deletion was successful
            return deleted_request;
        } else {
            // Deletion failed, return an empty request
            return {};
        }
    } catch (const mongocxx::exception& exception) {
        std::cerr << "An exception occurred: " << exception.what() << "\n";
        return {};
    } catch (const std::exception& exception) {
        std::cerr << "An exception occurred: " << exception.what() << "\n";
        return {};
    }
}