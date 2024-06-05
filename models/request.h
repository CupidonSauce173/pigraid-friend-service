//
// Created by antoine on 02/06/24.
//

#ifndef PIGRAID_FRIEND_SERVICE_REQUEST_MODEL_H
#define PIGRAID_FRIEND_SERVICE_REQUEST_MODEL_H

#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <bsoncxx/oid.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <nlohmann/json.hpp>

struct request {
private:
    bsoncxx::oid id_;                                   // The id of the friend request
    std::string sender_identifier_;                     // The xuid of the sender (identifier)
    std::string receiver_identifier_;                   // The xuid of the receiver (identifier)
    std::chrono::system_clock::time_point created_at_;  // The date that the friend request has been created
    std::chrono::system_clock::time_point destroy_at_;  // The date that the friend request has to be destroyed
public:
    // default constructor
    request() = default;

    // Constructor for retrieval
    request(const bsoncxx::oid &id, std::string sender_identifier, std::string receiver_identifier,
            const std::chrono::system_clock::time_point created_at,
            const std::chrono::system_clock::time_point destroy_at)
            : id_(id), sender_identifier_(std::move(sender_identifier)),
              receiver_identifier_(std::move(receiver_identifier)), created_at_(created_at), destroy_at_(destroy_at) {}

    // Constructor for insertion
    request(std::string sender_identifier, std::string receiver_identifier,
            const std::chrono::system_clock::time_point created_at,
            const std::chrono::system_clock::time_point destroy_at)
            : id_(bsoncxx::oid{}), sender_identifier_(std::move(sender_identifier)),
              receiver_identifier_(std::move(receiver_identifier)), created_at_(created_at), destroy_at_(destroy_at) {}

    // Getters
    [[nodiscard]] bsoncxx::oid id() const { return id_; }

    [[nodiscard]] const std::string &get_sender_identifier() const { return sender_identifier_; }

    [[nodiscard]] const std::string &get_receiver_identifier() const { return receiver_identifier_; }

    [[nodiscard]] std::chrono::system_clock::time_point created_at() const { return created_at_; }

    [[nodiscard]] std::chrono::system_clock::time_point destroy_at() const { return destroy_at_; }

    // Setters
    void set_id(const bsoncxx::oid &oid) { id_ = oid; }

    void set_sender_identifier(const std::string &sender_identifier) { sender_identifier_ = sender_identifier; }

    void set_receiver_identifier(const std::string &receiver_identifier) { receiver_identifier_ = receiver_identifier; }

    void set_created_at(const std::chrono::system_clock::time_point &created_at) { created_at_ = created_at; }

    void set_destroy_at(const std::chrono::system_clock::time_point &destroy_at) { destroy_at_ = destroy_at; }

    // Conversions

    // Helper function to convert ISO 8601 string to time_point
    static std::chrono::system_clock::time_point from_iso_string(const std::string &s) {
        std::tm tm = {};
        strptime(s.c_str(), "%Y-%m-%dT%H:%M:%SZ", &tm);
        std::time_t time = std::mktime(&tm);
        return std::chrono::system_clock::from_time_t(time);
    }

    // Helper function to convert time_point to ISO 8601 string
    static std::string to_iso_string(const std::chrono::system_clock::time_point &tp) {
        std::time_t t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm = *std::gmtime(&t);
        std::ostringstream out;
        out << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
        return out.str();
    }

    // BSON Serialization
    [[nodiscard]] bsoncxx::document::value to_bson() const {
        bsoncxx::builder::basic::document document;
        document.append(
                bsoncxx::builder::basic::kvp("_id", id_),
                bsoncxx::builder::basic::kvp("sender_identifier", sender_identifier_),
                bsoncxx::builder::basic::kvp("receiver_identifier", receiver_identifier_),
                bsoncxx::builder::basic::kvp("created_at", bsoncxx::types::b_date(created_at_)),
                bsoncxx::builder::basic::kvp("destroy_at", bsoncxx::types::b_date(destroy_at_))
        );
        return document.extract();
    }

    // BSON Deserialization
    static request from_bson(const bsoncxx::document::view &document) {
        // Get the data from the document
        const auto id = document["_id"].get_oid().value;
        auto sender_identifier = std::string(document["sender_identifier"].get_string().value.data());
        auto receiver_identifier = std::string(document["receiver_identifier"].get_string().value.data());
        auto created_at = std::chrono::system_clock::time_point(
                std::chrono::milliseconds(document["created_at"].get_date().to_int64()));
        auto destroy_at = std::chrono::system_clock::time_point(
                std::chrono::milliseconds(document["destroy_at"].get_date().to_int64()));

        // Generate the request and return
        return request{
                id,
                std::move(sender_identifier),
                std::move(receiver_identifier),
                created_at,
                destroy_at
        };
    }

    // JSON Serialization
    [[nodiscard]] nlohmann::json to_json() const {
        return nlohmann::json{
                {"id",                  id_.to_string()},
                {"sender_identifier",   sender_identifier_},
                {"receiver_identifier", receiver_identifier_},
                {"created_at",          to_iso_string(created_at_)},
                {"destroy_at",          to_iso_string(destroy_at_)}
        };
    }

    // JSON Deserialization
    static request from_json(const nlohmann::json &json_data) {
        const auto id = bsoncxx::oid{json_data.at("id").get<std::string>()};
        auto sender_identifier = json_data.at("sender_identifier").get<std::string>();
        auto receiver_identifier = json_data.at("receiver_identifier").get<std::string>();
        const auto created_at = from_iso_string(json_data.at("created_at").get<std::string>());
        const auto destroy_at = from_iso_string(json_data.at("destroy_at").get<std::string>());

        return request{
                id,
                std::move(sender_identifier),
                std::move(receiver_identifier),
                created_at,
                destroy_at
        };
    }

};


#endif //PIGRAID_FRIEND_SERVICE_REQUEST_MODEL_H
