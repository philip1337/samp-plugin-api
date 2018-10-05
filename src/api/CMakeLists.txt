SET(SAMP_API_SRCS
	Api.cpp
	
	Config.cpp
	Config.hpp
	Endpoint.cpp
	Endpoint.hpp
	Handler.cpp
	Handler.hpp
	LogSink.cpp
	LogSink.hpp
	Player.cpp
	Player.hpp
	PlayerManager.cpp
	PlayerManager.hpp
	Route.cpp
	Route.hpp
	Session.cpp
	Session.hpp
	SessionManager.cpp
	SessionManager.hpp
	Types.hpp
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
	api_share.log

	spotify-json

	boost::system
	boost::asio
	boost::coroutine
	boost::beast
)
