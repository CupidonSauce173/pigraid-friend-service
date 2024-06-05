add_test([=[TestTopic.TrivialEquality]=]  /home/antoine/CLionProjects/pigraid-friend-service/test/build/request_dao_test [==[--gtest_filter=TestTopic.TrivialEquality]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[TestTopic.TrivialEquality]=]  PROPERTIES WORKING_DIRECTORY /home/antoine/CLionProjects/pigraid-friend-service/test/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  request_dao_test_TESTS TestTopic.TrivialEquality)
