SET(PROJ_NAME hacklib_gw2)
PROJECT(${PROJ_NAME})

ADD_LIBRARY(${PROJ_NAME} STATIC
    gw2lib.h
    General.cpp
    Agent.cpp
    Character.cpp
    EspDraw.cpp
    GameData.h
    GameData.cpp
    main.h
    main.cpp
    )

SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES FOLDER ${PROJ_NAME})

TARGET_LINK_LIBRARIES(${PROJ_NAME} hacklib)


ADD_LIBRARY(${PROJ_NAME}_sample SHARED SampleApp.cpp)

SET_TARGET_PROPERTIES(${PROJ_NAME}_sample PROPERTIES FOLDER ${PROJ_NAME}_sample)

TARGET_LINK_LIBRARIES(${PROJ_NAME}_sample ${PROJ_NAME})