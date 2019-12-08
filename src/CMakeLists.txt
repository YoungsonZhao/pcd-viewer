set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
message(STATUS ${PROJECT_BINARY_DIR})
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS ${CMAKE_SOURCE_DIR})
message(STATUS ${CMAKE_MODULE_PATH})
message(STATUS ${CMAKE_INSTALL_PREFIX})
message(STATUS ${CMAKE_BINARY_DIR})

include_directories(../include)
add_definitions(-std=c++11)
add_definitions(-w)

find_package(PCL REQUIRED)
find_package(Glog REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
include_directories(${GLOG_INCLUDE_DIR})

link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(pcd-viewer viewer.cpp)
target_link_libraries(pcd-viewer ${PCL_LIBRARIES})

set(APP_DESKTOP_DIR "/usr/share/applications/")
# set(APP_PIXMAPS_DIR "/usr/share/icons/")
# include(KDEInstallDirs)
# include(InstallRequiredSystemLibraries)

install(TARGETS pcd-viewer
        BUNDLE DESTINATION . COMPONENT Runtime 
        RUNTIME DESTINATION bin COMPONENT Runtime)
install(FILES ../share/pcd-viewer.desktop DESTINATION ${APP_DESKTOP_DIR})
# install(CODE "message(STATUS ${CMAKE_INSTALL_PREFIX})
#               message(STATUS ${CMAKE_BINARY_DIR})
#               function(gp_resolved_file_type_override resolved_file type_var) 
#                        set(\${type_var} local PARENT_SCOPE) 
#               endfunction() 
#               include(BundleUtilities) 
#               fixup_bundle(\"${CMAKE_INSTALL_PREFIX}/bin/pcd-viewer\" \"\" \"\")"
#         COMPONENT Runtime)

# install(CODE "function(gp_resolved_file_type_override resolved_file type_var) 
#                        set(\${type_var} local PARENT_SCOPE) 
#               endfunction() 
#               include(BundleUtilities) 
#               fixup_bundle(\"${CMAKE_BINARY_DIR}/pcd-viewer\" \"\" \"\")"
#         DESTINATION bin
#         COMPONENT pcd-viewer)

# install(FILES ../script/postinst DESTINATION ${PROJECT_BINARY_DIR})
# install(FILES ../share/pcd-visualizer.png DESTINATION ${APP_PIXMAPS_DIR})

# find_program(XDG-DESKTOP-MENU_EXECUTABLE xdg-desktop-menu)
# install(CODE "execute_process(COMMAND ${XDG-DESKTOP-MENU_EXECUTABLE} install --novendor ${APP_DESKTOP_DIR}/pcd-visualizer.desktop)")

set(DEBIAN_POSTINST postinst.in)
configure_file(${CMAKE_SOURCE_DIR}/script/${DEBIAN_POSTINST} ${PROJECT_BINARY_DIR}/postinst)

set(CPACK_GENERATOR "DEB")
set(CPACK_SET_DESTDIR "on")
set(CPACK_PACKAGE_NAME "pcd-viewer")
set(CPACK_PACKAGE_DESCRIPTION "This application visualizes pcd files on Ubuntu. Using this application, we could view pcd files by double-click the pcd files or by input the path of pcd file using command line in terminal. This application is tested on Ubuntu 18.04 with PCL 1.8.1.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "This application visualizes pcd files on Ubuntu.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/YoungsonZhao/pcd-viewer")
set(CPACK_PACKAGE_VENDOR "YongshengZhao")
set(CPACK_PACKAGE_VERSION "1.0.3")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "3")
set(CPACK_PACKAGE_EXECUTABLES "pcd-viewer")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "zhaoyongsheng@zju.edu.cn")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SECTION "kde")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${PROJECT_BINARY_DIR}/postinst")
set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libvtk6.3 (>=6.3), libboost-dev (>=1.58.0), libpcl-dev (>=1.7)")
# set(CPACK_COMPONENTS_ALL Libraries ApplicationData)
include(CPack)

add_executable(test_folderParser test_folder_parser.cpp ./source/folder_parser.cpp)
