#include<iostream>
#include<chrono>
// STD
//#include <string>
//#include <vector>

// Package
//#include <sick_safetyscanners/FieldData.h>
#include <sick_safetyscanners/SickSafetyscanners.h>
//#include <sick_safetyscanners/SickSafetyscanners/ConfigurationConfig.h>
#include <sick_safetyscanners/datastructure/CommSettings.h>
#include <sick_safetyscanners/datastructure/FieldData.h>
#include "sick_safetyscanners/c_api.h"

#include <cmath>


namespace sick {

/*!
 * \brief Converts degrees to radians.
 * \param deg Degrees to convert.
 * \return To radians converted degrees.
 */
inline float degToRad(float deg)
{
  return deg * M_PI / 180.0f;
}

/*!
 * \brief Converts radians to degrees.
 * \param rad Input radians to convert
 * \return To degrees converted radians
 */
inline float radToDeg(float rad)
{
  return rad * 180.0f / M_PI;
}

/*!
 * \brief Converts a skip value into a "publish frequency" value
 * \param skip The number of scans to skip between each measured scan.  For a 25Hz laser, setting
 * 'skip' to 0 makes it publish at 25Hz, 'skip' to 1 makes it publish at 12.5Hz. \return "Publish
 * Frequency" ie. One out of every n_th scan will be published.  1 is publish every scan.  2 is
 * publish at half rate, and so on.
 */
inline uint16_t skipToPublishFrequency(int skip)
{
  return skip + 1;
}

/*!
 * \brief The SickSafetyscannersQnx class
 *
 * Main class for the node to handle the Qnx interfacing.
 */
class SickSafetyscannersQnx
{
public:
	 /*!
   * \brief Constructor of the SickSafetyscannersQnx
   *
   * Constructor of the SickSafetyscannersQnx,
   * loads all parameters
   */
  SickSafetyscannersQnx(SickCommunicationParameters param);
   /*!
   * \brief ~SickSafetyscannersQnx
   * Destructor if the SickSafetyscanners Qnx
   */
  virtual ~SickSafetyscannersQnx();

  int64_t getRawSickDataTime();
  int getRawSickDataNumPoints();
  void getRawSickDataDistances(double *scan_distances);
  void getRawSickDataRemission(double *remission_data);
private:

	bool m_initialised;
  bool m_first_run;

	std::shared_ptr<sick::SickSafetyscanners> m_device;
	sick::datastructure::CommSettings m_communication_settings;

  std::string m_frame_id;
  double m_time_offset;
  double m_range_min;
  double m_range_max;

  bool m_use_sick_angles;
  float m_angle_offset;
  bool m_use_pers_conf;

  struct RawLidarData {
  int64_t time;
  std::vector<double> scan_distances;
  std::vector<double> remission_data;
  }m_RawLidarData;


    /*!
   * @brief Reads and %%%verifies%%% the parameters.
   * @return True if successful.
   */
  bool readParameters(SickCommunicationParameters param);

   /*!
   * \brief Function which is called
   * when a new complete DP Packet is received
   * \param data, the assortment of all data from the sensor
   */
  void receivedUDPPacket(const datastructure::Data& data);
	bool isInitialised();
  void readTypeCodeSettings();
  void readPersistentConfig();
};

}//namespace sick
