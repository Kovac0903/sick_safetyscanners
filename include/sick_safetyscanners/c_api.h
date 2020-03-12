

typedef struct SickSafetyscannersC SickSafetyscannersC;

typedef struct RawSickDataC{
  int64_t time;
  int num_points;
  double *scan_distances;
  double *remission_data;
}RawSickDataC;
extern "C" 	SickSafetyscannersC* SickSafetyscanners_create();


extern "C" void SickSafetyscanners_destroy(
  SickSafetyscannersC** pself);
