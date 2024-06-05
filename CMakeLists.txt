cmake_minimum_required(VERSION 3.27)
project(friend_service)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

# Fetch nlohmann JSON
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.10.5
)

# Make available the content
FetchContent_MakeAvailable(nlohmann_json)

# Find the MongoDB C++ driver
find_package(mongocxx CONFIG REQUIRED)
find_package(bsoncxx CONFIG REQUIRED)

# Find Oat++ (installed via make install)
find_package(oatpp 1.3.0 REQUIRED)

# Add your executable
add_executable(friend_service main.cpp models/request.h
        daos/request_dao.cpp
        daos/request_dao.h
        controllers/request_controller.h
        database/db_handler.cpp
        database/db_handler.h
        AppComponent.h
        dto/request.h
        dto/error.h
        controllers/relation_controller.h
        daos/relation_dao.cpp
        daos/relation_dao.h
        models/relation.h
        dto/relation.h)

# Link libraries
target_link_libraries(friend_service PRIVATE $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>)
target_link_libraries(friend_service PRIVATE $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>)
target_link_libraries(friend_service PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(friend_service PRIVATE oatpp::oatpp oatpp::oatpp-test)

# Include directories
include_directories(${oatpp_INCLUDE_DIRS})
