set(ProjectName "NFDBMysqlLogicPlugin")
file(GLOB NFDBMysqlLogicPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFDBMysqlLogicPlugin_ROOT_Hpp 
	*.h)

add_library(NFDBMysqlLogicPlugin SHARED
	${NFDBMysqlLogicPlugin_ROOT_Cpp}
	${NFDBMysqlLogicPlugin_ROOT_Hpp})
	
set_target_properties( NFDBMysqlLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFDBMysqlLogicPlugin_d")
set_target_properties( NFDBMysqlLogicPlugin PROPERTIES PREFIX "")
set_target_properties( NFDBMysqlLogicPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFDBMysqlLogicPlugin")

if(UNIX)
	target_link_libraries(NFDBMysqlLogicPlugin -static-libstdc++)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
else()
	target_link_libraries(NFDBMysqlLogicPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()