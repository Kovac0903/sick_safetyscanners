

typedef struct SickSafetyscannersC SickSafetyscannersC;

typedef struct SickCommunicationParameters {

  std::string sensor_ip_adress;
  std::string host_ip_adress;
  int host_udp_port;
  int channel;
  double start_angle;
  double end_angle;
  int skip;

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

extern "C" void SickSafetyscanners_getScanData(
  SickSafetyscannersC* self,RawSickDataC* data_);
