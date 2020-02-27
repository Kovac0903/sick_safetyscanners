#include "sick_safetyscanners/sick_class.h"

namespace sick {

SickSafetyscannersRos::SickSafetyscannersRos()
  : m_initialised(false)
  , m_time_offset(0.0)
  , m_range_min(0.0)
  , m_range_max(0.0)
  , m_angle_offset(-90.0)
  , m_use_pers_conf(false)
{
	std::cout << "-------usao sam u klasu----------" << std::endl;
	readParameters();
	m_communication_settings.setSensorTcpPort(2122);

	m_device = std::make_shared<sick::SickSafetyscanners>(
    boost::bind(&SickSafetyscannersRos::receivedUDPPacket,
    						 this, _1),
     						&m_communication_settings);
 	uint16_t time_blink = 3;

	m_device->run(); //RUN UDP service
	readTypeCodeSettings(); //read code with TCP

	m_device->changeSensorSettings(m_communication_settings);
	m_initialised = true;

	std::cout << "Successfully launched sensor." << std::endl;
}
void SickSafetyscannersRos::readTypeCodeSettings()
{
  std::cout << "Reading Type code settings" << std::endl;

  sick::datastructure::TypeCode type_code;
  m_device->requestTypeCode(m_communication_settings, type_code);
  //std::cout << "-------usao sam u interface----------" << std::endl;
  m_communication_settings.setEInterfaceType(type_code.getInterfaceType());
  m_range_min = 0.1;
  m_range_max = type_code.getMaxRange();
}

bool SickSafetyscannersRos::readParameters()
{
	 m_communication_settings.setSensorTcpPort(2122);

  std::string sensor_ip_adress = "169.254.249.81";
  m_communication_settings.setSensorIp(sensor_ip_adress);

  std::string host_ip_adress = "169.254.249.82";
  m_communication_settings.setHostIp(host_ip_adress);

  int host_udp_port = 0;
  m_communication_settings.setHostUdpPort(host_udp_port);

  int channel = 0;
  m_communication_settings.setChannel(channel);

  m_communication_settings.setEnabled(true);

	// skip to publishing freq
  m_communication_settings.setStartAngle(0);
  m_communication_settings.setEndAngle(0);
  int skip = 0;
  m_communication_settings.setPublishingFrequency(
  	skipToPublishFrequency(skip));

  bool general_system_state = true;
	bool derived_settings = true;
	bool measurement_data = true;
	bool intrusion_data = true;
	bool application_io_data = true;

  m_communication_settings.setFeatures(
    general_system_state, derived_settings,
    measurement_data, intrusion_data, application_io_data);
  return true;
}

void SickSafetyscannersRos::readPersistentConfig()
{
  std::cout << "Reading Persistent Configuration" << std::endl;
  sick::datastructure::ConfigData config_data;
  m_device->requestPersistentConfig(m_communication_settings, config_data);
  m_communication_settings.setStartAngle(config_data.getStartAngle());
  m_communication_settings.setEndAngle(config_data.getEndAngle());
}

bool SickSafetyscannersRos::isInitialised()
{
  return m_initialised;
}

void SickSafetyscannersRos::receivedUDPPacket(const sick::datastructure::Data& data)
{
	std::cout << "ovdje dobijem UUDP paket i projeveravam" << std::endl;
  if (!data.getMeasurementDataPtr()->isEmpty() && !data.getDerivedValuesPtr()->isEmpty())
  {

    //sensor_msgs::LaserScan scan = createLaserScanMessage(data);
    std::cout << " ---> ovje Publisham laser_scan " << std::endl;
    //m_laser_scan_publisher.publish(scan);

  	uint16_t num_scan_points = data.getDerivedValuesPtr()->getNumberOfBeams();

  	  std::vector<sick::datastructure::ScanPoint> scan_points =
    data.getMeasurementDataPtr()->getScanPointsVector();
	  for (uint16_t i = 0; i < num_scan_points; ++i)
	  {
	    const sick::datastructure::ScanPoint scan_point = scan_points.at(i);
	    std::cout << scan_point.getDistance() << std::endl;
	    std::cout << static_cast<float>(scan_point.getReflectivity()) << std::endl;


	    //scan.ranges[i]                                  = static_cast<float>(scan_point.getDistance()) *
	    //                 data.getDerivedValuesPtr()->getMultiplicationFactor() * 1e-3; // mm -> m
	    //scan.intensities[i] = static_cast<float>(scan_point.getReflectivity());
	  }


  }


  if (!data.getMeasurementDataPtr()->isEmpty() && !data.getDerivedValuesPtr()->isEmpty())
  {
    //sick_safetyscanners::ExtendedLaserScanMsg extended_scan = createExtendedLaserScanMessage(data);

    //m_extended_laser_scan_publisher.publish(extended_scan);
  	std::cout << " ---> ovje Publisham laser_scan -> 3 " << std::endl;
    //sick_safetyscanners::OutputPathsMsg output_paths = createOutputPathsMessage(data);
    //m_output_path_publisher.publish(output_paths);
  }

  //sick_safetyscanners::RawMicroScanDataMsg raw_data = createRawDataMessage(data);

  //m_raw_data_publisher.publish(raw_data);
}

SickSafetyscannersRos::~SickSafetyscannersRos() {}

}//namespace sick
