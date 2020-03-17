#include "sick_safetyscanners/sick_class.h"



extern "C" SickSafetyscannersC* SickSafetyscanners_create(SickCommunicationParameters param)
{
  return reinterpret_cast<SickSafetyscannersC*>(
    new sick::SickSafetyscannersQnx(param));
}

extern "C" void SickSafetyscanners_destroy(
  SickSafetyscannersC** pself)
{
  delete reinterpret_cast<sick::SickSafetyscannersQnx*>(*pself);
  *pself = NULL;
}

extern "C" void SickSafetyscanners_getScanData(
  SickSafetyscannersC* self,RawSickDataC* data_)
{
  auto* pekf = reinterpret_cast<sick::SickSafetyscannersQnx*>(self);

  (*data_).time = pekf->getRawSickDataTime();
  (*data_).num_points = pekf->getRawSickDataNumPoints();

  pekf->getRawSickDataDistances((*data_).scan_distances);
  pekf->getRawSickDataRemission((*data_).remission_data);
}
