#include "stdlib.h"
#include "vsip.h"
#define N 1024
int main(int argc, char **argv)
{
    vsip_vview_f *vw;
    vsip_dda_sync_policy pol = VSIP_DDA_SYNC_POLICY_IN;
    vsip_dda_vdata_f *data;
    vsip_scalar_f *ptr;
    vsip_init((void*)0);
    if(NULL == (vw = vsip_vcreate_f(N, VSIP_MEM_NONE))) return -1;
    if(NULL == (data = vsip_dda_vdatacreate_f(vw, 0, pol, 0))) return -1;
    vsip_vput_f(vw, 0, 1.0F);
    if(NULL == (ptr = vsip_dda_vptr_f(data))) return -1; /* ptr[0] == unknown */
    if(0 != (vsip_dda_vsync_in_f(data))) return -1; /* ptr[0] == 1.0 */
    if(0 != vsip_dda_vdatadestroy_f(data)) return -1;
    vsip_valldestroy_f(vw);
    vsip_finalize((void*)0);
    return 0;
}
