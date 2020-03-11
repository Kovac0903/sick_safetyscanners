#include <iostream>
#include "sick_safetyscanners/c_api.h"

int main() {

	SickSafetyscannersC *microscan3_Qnx =
	SickSafetyscanners_create();
	while (true) {
		//std::cout << "/* message */" << '\n';
	}
	SickSafetyscanners_destroy(&microscan3_Qnx);
  return 0;

}
