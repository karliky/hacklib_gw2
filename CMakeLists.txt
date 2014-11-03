SET(PROJ_NAME hacklib_gw2)
PROJECT(${PROJ_NAME})

ADD_LIBRARY(${PROJ_NAME} SHARED
    gw2lib.h
    General.cpp
    Agent.cpp
    Character.cpp
    EspDraw.cpp
    GameData.h
    GameData.cpp
    main.h
    main.cpp
    SampleApp.cpp
    )

SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES FOLDER ${PROJ_NAME})

TARGET_LINK_LIBRARIES(${PROJ_NAME} hacklib)
