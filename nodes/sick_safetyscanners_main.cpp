#include <iostream>
#include "sick_safetyscanners/c_api.h"
#include <fstream>

int main() {

	SickCommunicationParameters param;

	param.sensor_ip_adress = "169.254.249.81";
  param.host_ip_adress = "169.254.249.82";
  param.host_udp_port = 0;
  param.channel = 0;
  param.start_angle = 0;
  param.end_angle = 0;
  param.skip = 0;

  param.general_system_state = true;
	param.derived_settings = true;
	param.measurement_data = true;
	param.intrusion_data = true;
	param.application_io_data = true;

	SickSafetyscannersC *microscan3_Qnx =
	SickSafetyscanners_create(param);
	RawSickDataC* data;
	(*data).scan_distances = (double *)malloc(sizeof(double) * 715);
	(*data).remission_data = (double *)malloc(sizeof(double) * 715);

	 while (true) {
		SickSafetyscanners_getScanData(microscan3_Qnx,data);
		//std::cout << " Raw_sick_DATA->  " << *((*data).scan_distances+714) << '\n';
	 }

	printf("a\n");
	SickSafetyscanners_destroy(&microscan3_Qnx);

  return 0;

}
