#include "oatpp/network/Server.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "AppComponent.h"
#include <iostream>

#include "controllers/request_controller.h"

void run() {
    AppComponent components; // Create scope Environment components

    /* Get router component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    oatpp::web::server::api::Endpoints docEndpoints;

    auto request_handler = std::make_shared<db_handler>(db_handler{mongocxx::uri{}});
    request_dao requestDao = request_dao{request_handler};

    router->addController(std::make_shared<request_controller>(requestDao));

    /* Get connection handler component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    /* Get connection provider component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    /* create server */
    oatpp::network::Server server(connectionProvider,
                                  connectionHandler);

    OATPP_LOGD("Server", "Running on port %s...", connectionProvider->getProperty("port").toString()->c_str());

    server.run();
}

int main() {
    oatpp::base::Environment::init();

    run();

    /* Print how much objects were created during app running, and what have left-probably leaked */
    /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
    std::cout << "\nEnvironment:\n";
    std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
    std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";

    oatpp::base::Environment::destroy();
    return 0;
}
