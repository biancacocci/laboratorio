cmake_minimum_required(VERSION 3.26)
project(laboratorio)

set(CMAKE_CXX_STANDARD 20)
include_directories(${PROJECT_SOURCE_DIR})
# Aggiungi i file sorgente principali
set(SOURCES
        user.cpp
        message.cpp
        chat.cpp
        chatregister.cpp
)

# Aggiungi i file header
set(HEADERS
        user.h
        message.h
        chat.h
        chatregister.h
)

# Crea l'eseguibile principale
add_executable(laboratorio main.cpp ${SOURCES} ${HEADERS})

# Abilita GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Assicura che GoogleTest sia nei percorsi di inclusione
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Crea l'eseguibile per i test
add_executable(runTests test/usertest.cpp ${SOURCES} ${HEADERS}
        test/messagetest.cpp
        test/chattest.cpp
        test/chatregistertest.cpp)

# Collega GoogleTest ai test
target_link_libraries(runTests GTest::gtest GTest::gtest_main)

# Abilita il framework di test
include(GoogleTest)
gtest_discover_tests(runTests)




