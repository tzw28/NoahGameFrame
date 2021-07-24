set(ProjectName "ModelServerPlugin")
file(GLOB ModelServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB ModelServerPlugin_ROOT_Hpp 
	*.h)

add_library(ModelServerPlugin STATIC
	${ModelServerPlugin_ROOT_Cpp}
	${ModelServerPlugin_ROOT_Hpp})
	
set_target_properties( ModelServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "ModelServerPlugin")
set_target_properties( ModelServerPlugin PROPERTIES PREFIX "")
set_target_properties( ModelServerPlugin PROPERTIES 
	FOLDER "NFServer/ModelServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("ModelServerPlugin")

if(UNIX)
	target_link_libraries(ModelServerPlugin -static-libstdc++)
	add_definitions(
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	target_link_libraries(ModelServerPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-DELPP_NO_DEFAULT_LOG_FILE
	)

endif()