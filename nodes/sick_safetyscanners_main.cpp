#include <iostream>
#include "sick_safetyscanners/c_api.h"

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
	SickSafetyscanners_create();
	while (true) {
		//std::cout << "/* message */" << '\n';
	}
	SickSafetyscanners_destroy(&microscan3_Qnx);
  return 0;

}
