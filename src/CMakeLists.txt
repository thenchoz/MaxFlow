
INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_BINARY_DIR}/src
)

ADD_LIBRARY(${PROJECT_NAME}-lib SHARED
        max_flow_lib.cc
        max_flow_initialisation.cc
        max-flow.cc
        export.cc
)

ADD_EXECUTABLE(${PROJECT_NAME}
        main.cc
        max_flow_initialisation.cc
        max-flow.cc
        export.cc
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}-lib
  ${LEMON_LIBRARIES}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  ${LEMON_LIBRARIES}
)

SET_TARGET_PROPERTIES(${PROJECT_NAME}-lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)
