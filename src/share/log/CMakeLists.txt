SET(SHARE_LOG_HEADERS
	Log.hpp
)

SET(SHARE_LOG_SRCS
	Log.cpp
)

add_library(api_share.log ${SHARE_LOG_HEADERS} ${SHARE_LOG_SRCS})
target_link_libraries(api_share.log api_share
	# Log / string formatting stuff
	spdlog
	fmt
	
	# Boost
	boost::filesystem
)

target_compile_definitions(api_share.log
	PRIVATE
		BOOST_ALL_NO_LIB
)