set(ProjectName "NFGameServerNet_ServerPlugin")
file(GLOB NFGameServerNet_ServerPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFGameServerNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFGameServerNet_ServerPlugin STATIC
	${NFGameServerNet_ServerPlugin_ROOT_Cpp}
	${NFGameServerNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFGameServerNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGameServerNet_ServerPlugin")
set_target_properties( NFGameServerNet_ServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFGameServerNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/NFGameServerNet_ServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFGameServerNet_ServerPlugin")

target_link_libraries(
	NFGameServerNet_ServerPlugin ${OpenCASCADE_LIBRARIES}
	NFGameServerNet_ServerPlugin stdc++fs
)
if(UNIX)
	target_link_libraries(NFGameServerNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()
	target_link_libraries(NFGameServerNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
