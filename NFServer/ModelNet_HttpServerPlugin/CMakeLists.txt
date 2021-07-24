set(ProjectName "ModelNet_HttpServerPlugin")
file(GLOB ModelNet_HttpServerPlugin_ROOT_Cpp *.cpp)
file(GLOB ModelNet_HttpServerPlugin_ROOT_Hpp *.h)

add_library(ModelNet_HttpServerPlugin STATIC
	${ModelNet_HttpServerPlugin_ROOT_Cpp}
	${ModelNet_HttpServerPlugin_ROOT_Hpp})
	
set_target_properties( ModelNet_HttpServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "ModelNet_HttpServerPlugin")
set_target_properties( ModelNet_HttpServerPlugin PROPERTIES PREFIX "")
set_target_properties( ModelNet_HttpServerPlugin PROPERTIES 
	FOLDER "NFServer/ModelNet_HttpServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("ModelNet_HttpServerPlugin")

if(UNIX)
	target_link_libraries(ModelNet_HttpServerPlugin -static-libstdc++)
	add_definitions(
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()
	target_link_libraries(ModelNet_HttpServerPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
endif()
