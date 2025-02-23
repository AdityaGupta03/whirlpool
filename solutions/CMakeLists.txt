# Echo challenge solution
ADD_EXECUTABLE(echo echo.cpp)
TARGET_LINK_LIBRARIES(echo PRIVATE whirlpool)

# Unique ID Generation Solution
ADD_EXECUTABLE(uuid uuid.cpp)
TARGET_LINK_LIBRARIES(uuid PRIVATE whirlpool)
