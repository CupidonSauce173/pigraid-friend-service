# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/antoine/CLionProjects/friend_service/cmake-build-debug/_deps/oatpp-src"
  "/home/antoine/CLionProjects/friend_service/cmake-build-debug/_deps/oatpp-build"
  "/home/antoine/CLionProjects/friend_service/cmake-build-debug/_deps/oatpp-subbuild/oatpp-populate-prefix"
  "/home/antoine/CLionProjects/friend_service/cmake-build-debug/_deps/oatpp-subbuild/oatpp-populate-prefix/tmp"
  "/home/antoine/CLionProjects/friend_service/cmake-build-debug/_deps/oatpp-subbuild/oatpp-populate-prefix/src/oatpp-populate-stamp"
  "/home/antoine/CLionProjects/friend_service/cmake-build-debug/_deps/oatpp-subbuild/oatpp-populate-prefix/src"
  "/home/antoine/CLionProjects/friend_service/cmake-build-debug/_deps/oatpp-subbuild/oatpp-populate-prefix/src/oatpp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/antoine/CLionProjects/friend_service/cmake-build-debug/_deps/oatpp-subbuild/oatpp-populate-prefix/src/oatpp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/antoine/CLionProjects/friend_service/cmake-build-debug/_deps/oatpp-subbuild/oatpp-populate-prefix/src/oatpp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
