

typedef struct SickSafetyscannersC SickSafetyscannersC;

typedef struct SickCommunicationParameters {
//https://cdn.sick.com/media/docs/1/01/701/Technical_information_microScan3_outdoorScan3_en_IM0083701.PDF

  std::string sensor_ip_adress;
  std::string host_ip_adress;
  int host_udp_port;
  int channel;
  double start_angle; //deg
  double end_angle; //deg
  int skip;
   // param skip The number of scans to skip between each measured scan.  For a 25Hz laser, setting
   // 'skip' to 0 makes it publish at 25Hz, 'skip' to 1 makes it publish at 12.5Hz. \return "Publish
   // Frequency" ie. One out of every n_th scan will be published.  1 is publish every scan.  2 is
   // publish at half rate, and so on.

  bool general_system_state;
  bool derived_settings;
  bool measurement_data;
  bool intrusion_data;
  bool application_io_data;

} SickCommunicationParameters;


typedef struct RawSickDataC{
  int64_t time;
  int num_points;
  double *scan_distances;
  double *remission_data;
}RawSickDataC;

extern "C" 	SickSafetyscannersC* SickSafetyscanners_create(
  SickCommunicationParameters param);


extern "C" void SickSafetyscanners_destroy(
  SickSafetyscannersC** pself);

extern "C" int SickSafetyscanners_getNumPoints(
  SickSafetyscannersC* self);

extern "C" bool SickSafetyscanners_getScanData(
  SickSafetyscannersC* self,RawSickDataC* data_);
