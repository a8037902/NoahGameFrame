set(ProjectName "NFClientPlugin")
file(GLOB NFClientPlugin_ROOT_Cpp
	*.cpp)
file(GLOB NFClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFClientPlugin SHARED
	${NFClientPlugin_ROOT_Cpp}
	${NFClientPlugin_ROOT_Hpp})
	
set_target_properties( NFClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFClientPlugin_d")
set_target_properties( NFClientPlugin PROPERTIES PREFIX "")
set_target_properties( NFClientPlugin PROPERTIES 
	FOLDER "Work"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFClientPlugin")

if(UNIX)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

endif()
