#include "sick_safetyscanners/sick_class.h"

namespace sick {

SickSafetyscannersQnx::SickSafetyscannersQnx(SickCommunicationParameters param)
  : m_initialised(false)
  , m_time_offset(0.0)
  , m_range_min(0.0)
  , m_range_max(0.0)
  , m_angle_offset(-90.0)
  , m_use_pers_conf(false)
  , m_first_run(true)
  , m_new_scan_flag(true)
{
	readParameters(param);
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

bool SickSafetyscannersQnx::readParameters(SickCommunicationParameters param)
{
	m_communication_settings.setSensorTcpPort(2122);

  m_communication_settings.setSensorIp(param.sensor_ip_adress);

  m_communication_settings.setHostIp(param.host_ip_adress);

  m_communication_settings.setHostUdpPort(param.host_udp_port);

  m_communication_settings.setChannel(param.channel);

  m_communication_settings.setEnabled(true);

	// skip to publishing freq
  m_communication_settings.setStartAngle(param.start_angle);
  m_communication_settings.setEndAngle(param.start_angle);

  m_communication_settings.setPublishingFrequency(
  	skipToPublishFrequency(param.skip));

  m_communication_settings.setFeatures(
    param.general_system_state, param.derived_settings,
    param.measurement_data, param.intrusion_data,
    param.application_io_data);
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
    const auto time = std::chrono::system_clock::now();


    m_RawLidarData.time = std::chrono::nanoseconds(time.time_since_epoch()).count();
    //std::cout << "nanoseconds since epoch: "
    //          << m_RawLidarData.time
    //          << '\n';


	  for (uint16_t i = 0; i < num_scan_points; ++i)
	  {
	    const sick::datastructure::ScanPoint scan_point = scan_points.at(i);
      m_RawLidarData.scan_distances.at(i) = scan_point.getDistance();
      m_RawLidarData.remission_data.at(i)=
        static_cast<float>(scan_point.getReflectivity());
	  }
    std::cout << "scan processed" << '\n';
    m_new_scan_flag = true;
  }
}
bool SickSafetyscannersQnx::newScan(){
  if (m_new_scan_flag)
  {
    m_new_scan_flag = false;
    return true;
  }
}

int64_t SickSafetyscannersQnx::getRawSickDataTime(){
  return m_RawLidarData.time;
}

int SickSafetyscannersQnx::getRawSickDataNumPoints(){
  return m_RawLidarData.scan_distances.size();
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
