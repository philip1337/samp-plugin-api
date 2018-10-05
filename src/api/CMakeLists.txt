include(AMXConfig)
include(AddSAMPPlugin)

SET(SAMP_API_SRCS
	Api.cpp

	Endpoint.cpp
	Endpoint.hpp
	Session.cpp
	Session.hpp
	SessionManager.cpp
	SessionManager.hpp
	MimeTypes.cpp
	MimeTypes.hpp
	Request.cpp
	Request.hpp
	RequestParser.cpp
	RequestParser.hpp
	Route.cpp
	Route.hpp
	Reply.cpp
	Reply.hpp
	Handler.cpp
	Handler.hpp
)

SET(PLUGIN_FILES
	Api.def
	Api.rc
)

api_add_samp_plugin(api
	${SAMP_API_SRCS}
	${PLUGIN_FILES}
)

target_link_libraries(api
	api_share
	fmt
	spdlog
	spotify-json
	boost::asio
)