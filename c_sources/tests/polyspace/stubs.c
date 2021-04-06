
#include "ed247.h"

ed247_signal_info_t si;

volatile char name[256];
volatile char comment[256];
volatile char icd[256];
volatile ed247_signal_info_type_t type; 

volatile ed247_signal_info_t* i = &si;
volatile ed247_status_t s;
ed247_status_t ed247_signal_get_info(ed247_signal_t signal,const ed247_signal_info_t **info){
	*info = i;
	i->name = name;
	i->comment = comment;
	i->icd = icd;
	i->info = type;
	return s;
}
