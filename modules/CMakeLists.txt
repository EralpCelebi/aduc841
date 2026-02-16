file(GLOB MODULE *.c)
set(MODULES "Dispatch_Modules")

add_library(${MODULES} STATIC ${MODULE})
target_include_directories(${MODULES} PUBLIC ${PROJECT_SOURCE_DIR}/include)

set(MODULES ${MODULES} PARENT_SCOPE)