#include "sick_safetyscanners/sick_class.h"



extern "C" SickSafetyscannersC* SickSafetyscanners_create()
{
  return reinterpret_cast<SickSafetyscannersC*>(
    new sick::SickSafetyscannersQnx());
}

extern "C" void SickSafetyscanners_destroy(
  SickSafetyscannersC** pself)
{
  delete reinterpret_cast<sick::SickSafetyscannersQnx*>(*pself);
  *pself = NULL;
}

extern "C" void SickSafetyscanners_getScanData(
  SickSafetyscannersC* self)
{
  auto* pekf = reinterpret_cast<sick::SickSafetyscannersQnx*>(self);

  RawSickDataC data;
  data.time = pekf->getRawSickDataTime();
  data.num_points = pekf->getRawSickDataNumPoints();

  if (data.scan_distances == NULL){
    data.scan_distances = (double *)malloc(sizeof(double)*data.num_points);
    data.remission_data = (double *)malloc(sizeof(double)*data.num_points);
  }
  pekf->getRawSickDataDistances(data.scan_distances);
  pekf->getRawSickDataRemission(data.remission_data);
  std::cout << " Raw_sick_DATA->  " << data.num_points << '\n';
}
