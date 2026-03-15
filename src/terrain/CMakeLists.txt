add_library(synthflight_terrain
    TerrainGrid.cpp
    TerrainCollisionDetector.cpp
)
target_include_directories(synthflight_terrain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
