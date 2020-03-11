#include "sick_safetyscanners/sick_class.h"
#include "sick_safetyscanners/c_api.h"


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
