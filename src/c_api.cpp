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

extern "C" int SickSafetyscanners_getNumPoints(SickSafetyscannersC* self)
{
  auto* plaser = reinterpret_cast<sick::SickSafetyscannersQnx*>(self);
  return plaser->getRawSickDataNumPoints();
}

extern "C" bool SickSafetyscanners_getScanData(
  SickSafetyscannersC* self,RawSickDataC* data_)
{
  auto* plaser = reinterpret_cast<sick::SickSafetyscannersQnx*>(self);

  if (!(plaser->newScan())) return false;

  else
  {

    (*data_).time = plaser->getRawSickDataTime();
    (*data_).num_points = plaser->getRawSickDataNumPoints();

    plaser->getRawSickDataDistances((*data_).scan_distances);
    plaser->getRawSickDataRemission((*data_).remission_data);
    return true;
  }

}
