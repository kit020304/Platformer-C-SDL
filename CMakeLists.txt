cmake_minimum_required(VERSION 3.25)
project(kursach2)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_INCLUDE_DIR C:/Games/lib/SDL/include)
set(SDL2_LIB_DIR C:/Games/lib/SDL/lib/x64)

set(SDL2_IMAGE_INCLUDE_DIR C:/Games/lib/SDL2_image/include)
set(SDL2_IMAGE_LIB_DIR C:/Games/lib/SDL2_image/lib/x64)

set(SDL2_TTF_INCLUDE_DIR C:/Games/lib/SDL2_ttf/include)
set(SDL2_TTF_LIB_DIR C:/Games/lib/SDL2_ttf/lib/x64)

set(SDL2_MIXER_INCLUDE_DIR C:/Games/lib/SDL2_mixer/include)
set(SDL2_MIXER_LIB_DIR C:/Games/lib/SDL2_mixer/lib/x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

include_directories(${SDL2_IMAGE_INCLUDE_DIR})
link_directories(${SDL2_IMAGE_LIB_DIR})

include_directories(${SDL2_TTF_INCLUDE_DIR})
link_directories(${SDL2_TTF_LIB_DIR})

include_directories(${SDL2_MIXER_INCLUDE_DIR})
link_directories(${SDL2_MIXER_LIB_DIR})

add_executable(kursach2 main.cpp Engine.cpp Engine.h TextureManager.cpp TextureManager.h Vector2D.h Transform.h Object.h GameObject.h Character.h Warrior.cpp Warrior.h Animation.cpp Animation.h RigidBody.h Input.cpp Input.h Timer.cpp Timer.h Layer.h GameMap.h TileLayer.cpp TileLayer.h MapParser.cpp MapParser.h
        tinystr.cpp tinystr.h tinyxml.cpp tinyxml.h tinyxmlerror.cpp tinyxmlparser.cpp Camera.cpp Camera.h Point.h CollisionHandler.cpp CollisionHandler.h Collider.h Enemy.cpp Enemy.h ObjectFactory.cpp ObjectFactory.h Menu.cpp Menu.h Button.h TextOutput.h Sound.cpp Sound.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)
target_link_libraries(${PROJECT_NAME} SDL2_IMAGE SDL2_image)
target_link_libraries(${PROJECT_NAME} SDL2_TTF SDL2_ttf)
target_link_libraries(${PROJECT_NAME} SDL2_TTF SDL2_mixer)