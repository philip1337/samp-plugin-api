SET(API_SHARE_SRC
	share.cpp
)

add_library(api_share ${API_SHARE_SRC})

target_include_directories(api_share
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)