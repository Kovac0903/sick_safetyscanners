cmake_minimum_required(VERSION 2.8.3)
project(sick_safetyscanners)

## Use C++11
#add_definitions(-std=c++11)
## By adding -Wall and -Werror, the compiler does not ignore warnings anymore,
## enforcing cleaner code.
add_definitions(-std=c++11 -Wno-ignored-attributes -Wno-narrowing)
#add_definitions(-std=c++11 -Wall -Werror)

## Find system libraries
#find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)


###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need



###########
## Build ##
###########


## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
#  ${EIGEN3_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
)

## Declare a cpp library
add_library(${PROJECT_NAME}_core SHARED
  #src/c_api.cpp
  src/sick_class.cpp
  src/SickSafetyscanners.cpp
  src/cola2/ApplicationNameVariableCommand.cpp
  src/cola2/ChangeCommSettingsCommand.cpp
  src/cola2/CloseSession.cpp
  src/cola2/Cola2Session.cpp
  src/cola2/Command.cpp
  src/cola2/ConfigMetadataVariableCommand.cpp
  src/cola2/CreateSession.cpp
  src/cola2/DeviceNameVariableCommand.cpp
  src/cola2/DeviceStatusVariableCommand.cpp
  src/cola2/FieldGeometryVariableCommand.cpp
  src/cola2/FieldHeaderVariableCommand.cpp
  src/cola2/FieldSetsVariableCommand.cpp
  src/cola2/FindMeCommand.cpp
  src/cola2/FirmwareVersionVariableCommand.cpp
  src/cola2/MeasurementCurrentConfigVariableCommand.cpp
  src/cola2/MeasurementPersistentConfigVariableCommand.cpp
  src/cola2/MethodCommand.cpp
  src/cola2/MonitoringCaseTableHeaderVariableCommand.cpp
  src/cola2/MonitoringCaseVariableCommand.cpp
  src/cola2/OrderNumberVariableCommand.cpp
  src/cola2/ProjectNameVariableCommand.cpp
  src/cola2/RequiredUserActionVariableCommand.cpp
  src/cola2/SerialNumberVariableCommand.cpp
  src/cola2/StatusOverviewVariableCommand.cpp
  src/cola2/TypeCodeVariableCommand.cpp
  src/cola2/UserNameVariableCommand.cpp
  src/cola2/VariableCommand.cpp
  src/communication/AsyncTCPClient.cpp
  src/communication/AsyncUDPClient.cpp
  src/data_processing/ParseApplicationData.cpp
  src/data_processing/ParseApplicationNameData.cpp
  src/data_processing/ParseConfigMetadata.cpp
  src/data_processing/ParseData.cpp
  src/data_processing/ParseDataHeader.cpp
  src/data_processing/ParseDatagramHeader.cpp
  src/data_processing/ParseDerivedValues.cpp
  src/data_processing/ParseDeviceName.cpp
  src/data_processing/ParseDeviceStatus.cpp
  src/data_processing/ParseFieldGeometryData.cpp
  src/data_processing/ParseFieldHeaderData.cpp
  src/data_processing/ParseFieldSetsData.cpp
  src/data_processing/ParseFirmwareVersion.cpp
  src/data_processing/ParseGeneralSystemState.cpp
  src/data_processing/ParseIntrusionData.cpp
  src/data_processing/ParseMeasurementCurrentConfigData.cpp
  src/data_processing/ParseMeasurementData.cpp
  src/data_processing/ParseMeasurementPersistentConfigData.cpp
  src/data_processing/ParseMonitoringCaseData.cpp
  src/data_processing/ParseOrderNumber.cpp
  src/data_processing/ParseProjectName.cpp
  src/data_processing/ParseRequiredUserAction.cpp
  src/data_processing/ParseSerialNumber.cpp
  src/data_processing/ParseStatusOverview.cpp
  src/data_processing/ParseTCPPacket.cpp
  src/data_processing/ParseTypeCodeData.cpp
  src/data_processing/ParseUserNameData.cpp
  src/data_processing/TCPPacketMerger.cpp
  src/data_processing/UDPPacketMerger.cpp
  src/datastructure/ApplicationData.cpp
  src/datastructure/ApplicationInputs.cpp
  src/datastructure/ApplicationName.cpp
  src/datastructure/ApplicationOutputs.cpp
  src/datastructure/CommSettings.cpp
  src/datastructure/ConfigData.cpp
  src/datastructure/ConfigMetadata.cpp
  src/datastructure/Data.cpp
  src/datastructure/DatagramHeader.cpp
  src/datastructure/DataHeader.cpp
  src/datastructure/DerivedValues.cpp
  src/datastructure/DeviceName.cpp
  src/datastructure/DeviceStatus.cpp
  src/datastructure/FieldData.cpp
  src/datastructure/FieldSets.cpp
  src/datastructure/FirmwareVersion.cpp
  src/datastructure/GeneralSystemState.cpp
  src/datastructure/IntrusionData.cpp
  src/datastructure/IntrusionDatum.cpp
  src/datastructure/LatestTelegram.cpp
  src/datastructure/MeasurementData.cpp
  src/datastructure/MonitoringCaseData.cpp
  src/datastructure/OrderNumber.cpp
  src/datastructure/PacketBuffer.cpp
  src/datastructure/ParsedPacketBuffer.cpp
  src/datastructure/ProjectName.cpp
  src/datastructure/RequiredUserAction.cpp
  src/datastructure/ScanPoint.cpp
  src/datastructure/SerialNumber.cpp
  src/datastructure/StatusOverview.cpp
  src/datastructure/TypeCode.cpp
  src/datastructure/UserName.cpp
)
add_library(${PROJECT_NAME}_c_api SHARED
  src/c_api.cpp
)
target_compile_options(${PROJECT_NAME}_core PUBLIC  -march=native)


add_executable(${PROJECT_NAME}_main
  nodes/${PROJECT_NAME}_main.cpp
  #src/sick_class.cpp
)
 target_link_libraries(${PROJECT_NAME}_c_api
   ${PROJECT_NAME}_core
   #socket
   ${Boost_LIBRARIES}
 )

target_compile_options(${PROJECT_NAME}_c_api PUBLIC  -march=native)

target_link_libraries(${PROJECT_NAME}_main
  ${PROJECT_NAME}_c_api
  #socket
  ${Boost_LIBRARIES}
)
