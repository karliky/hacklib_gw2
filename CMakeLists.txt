PROJECT(hacklib_gw2)

ADD_LIBRARY(${PROJECT_NAME} STATIC
    gw2lib.h
    General.cpp
    Agent.cpp
    Character.cpp
    EspDraw.cpp
    Gadget.cpp
    Compass.cpp
    GameData.h
    GameData.cpp
    Gw2Hook.h
    Gw2Hook.cpp
    main.h
    main.cpp
    )

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES FOLDER ${PROJECT_NAME})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} hacklib)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC .)


ADD_LIBRARY(${PROJECT_NAME}_sample SHARED SampleApp.cpp)

SET_TARGET_PROPERTIES(${PROJECT_NAME}_sample PROPERTIES FOLDER ${PROJECT_NAME}_sample)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}_sample ${PROJECT_NAME})
