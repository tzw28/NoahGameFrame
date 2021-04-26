# Install script for directory: D:/UnityProj/NoahGameFrame/NFComm

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/UnityProj/NoahGameFrame/out/install/x64-Debug (默认值)")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFPluginLoader/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFActorPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFConfigPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFCore/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFKernelPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFLogPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFMessageDefine/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFNetPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFSecurityPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFNavigationPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFNoSqlPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFTestPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFComm/NFLuaScriptPlugin/cmake_install.cmake")

endif()

