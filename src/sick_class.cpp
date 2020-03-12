#include "sick_safetyscanners/sick_class.h"

namespace sick {

SickSafetyscannersQnx::SickSafetyscannersQnx()
  : m_initialised(false)
  , m_time_offset(0.0)
  , m_range_min(0.0)
  , m_range_max(0.0)
  , m_angle_offset(-90.0)
  , m_use_pers_conf(false)
  , m_first_run(true)
{
	readParameters();
	m_communication_settings.setSensorTcpPort(2122);

	m_device = std::make_shared<sick::SickSafetyscanners>(
    boost::bind(&SickSafetyscannersQnx::receivedUDPPacket,
    						 this, _1),
     						&m_communication_settings);
 	uint16_t time_blink = 3;

	m_device->run(); //RUN UDP service
	readTypeCodeSettings(); //read code with TCP

	m_device->changeSensorSettings(m_communication_settings);
	m_initialised = true;

	std::cout << "Successfully launched sensor." << std::endl;
}
void SickSafetyscannersQnx::readTypeCodeSettings()
{
  std::cout << "Reading Type code settings" << std::endl;

  sick::datastructure::TypeCode type_code;
  m_device->requestTypeCode(m_communication_settings, type_code);
  //std::cout << "-------usao sam u interface----------" << std::endl;
  m_communication_settings.setEInterfaceType(type_code.getInterfaceType());
  m_range_min = 0.1;
  m_range_max = type_code.getMaxRange();
}

bool SickSafetyscannersQnx::readParameters()
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

void SickSafetyscannersQnx::readPersistentConfig()
{
  std::cout << "Reading Persistent Configuration" << std::endl;
  sick::datastructure::ConfigData config_data;
  m_device->requestPersistentConfig(m_communication_settings, config_data);
  m_communication_settings.setStartAngle(config_data.getStartAngle());
  m_communication_settings.setEndAngle(config_data.getEndAngle());
}

bool SickSafetyscannersQnx::isInitialised()
{
  return m_initialised;
}

void SickSafetyscannersQnx::receivedUDPPacket(const sick::datastructure::Data& data)
{
  if (!data.getMeasurementDataPtr()->isEmpty() && !data.getDerivedValuesPtr()->isEmpty())
  {
  	uint16_t num_scan_points = data.getDerivedValuesPtr()->getNumberOfBeams();

    if (m_first_run){
      m_RawLidarData.remission_data.resize((double) num_scan_points);
      m_RawLidarData.scan_distances.resize((double) num_scan_points);
      m_first_run = false;
    }

  	std::vector<sick::datastructure::ScanPoint> scan_points =
    data.getMeasurementDataPtr()->getScanPointsVector();
    m_RawLidarData.time = data.getDerivedValuesPtr()->getScanTime();

	  for (uint16_t i = 0; i < num_scan_points; ++i)
	  {
	    const sick::datastructure::ScanPoint scan_point = scan_points.at(i);
      m_RawLidarData.scan_distances.at(i) = scan_point.getDistance();
      m_RawLidarData.remission_data.at(i)=
        static_cast<float>(scan_point.getReflectivity());
	  }
    std::cout << "scan processed" << '\n';
  }
}
int64_t SickSafetyscannersQnx::getRawSickDataTime(){
  return c_sick_data.time;
}

int SickSafetyscannersQnx::getRawSickDataNumPoints(){
  return c_sick_data.num_points;
}

void SickSafetyscannersQnx::getRawSickDataDistances(
  double *scan_distances)
{
  for (uint16_t i = 0; i < m_RawLidarData.scan_distances.size(); ++i)
  {
    *(scan_distances+i) = m_RawLidarData.scan_distances.at(i);
  }
}

void SickSafetyscannersQnx::getRawSickDataRemission(
  double *remission_data)
{
  for (uint16_t i = 0; i < m_RawLidarData.remission_data.size(); ++i)
  {
    *(remission_data+i) = m_RawLidarData.remission_data.at(i);
  }
}

SickSafetyscannersQnx::~SickSafetyscannersQnx() {}

}//namespace sick
