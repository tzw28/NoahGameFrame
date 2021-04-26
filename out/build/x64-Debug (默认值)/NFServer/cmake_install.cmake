# Install script for directory: D:/UnityProj/NoahGameFrame/NFServer

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
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFGameServerNet_ClientPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFGameServerNet_ServerPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFGameServerPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFLoginLogicPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFLoginNet_ClientPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFLoginNet_ServerPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFLoginNet_HttpServerPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFMasterNet_ServerPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFMasterNet_HttpServerPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFMasterServerPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFProxyServerNet_ClientPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFProxyServerNet_ServerPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFProxyLogicPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFWorldNet_ClientPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFWorldNet_ServerPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFDBNet_ClientPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFDBNet_ServerPlugin/cmake_install.cmake")
  include("D:/UnityProj/NoahGameFrame/out/build/x64-Debug (默认值)/NFServer/NFDBLogicPlugin/cmake_install.cmake")

endif()

