#include "stdlib.h"
#include "vsip.h"
#define N 1024
int main(int argc, char **argv)
{
    vsip_scalar_f userArray[2*N];
    vsip_cblock_f *blk;
    vsip_cvview_f *vw;
    vsip_dda_cvdata_f *data;
    vsip_dda_cvdataattr_f attrib;
    vsip_cscalar_f *arrayPtr;
    vsip_scalar_f *interPtr;
    vsip_scalar_f *splitRealPtr, *splitImagPtr;
    vsip_scalar_f *data1ptr, *data2ptr;    
    vsip_init((void *)0);   
    if(NULL == (blk = vsip_cblockbind_f(&userArray[0], 0, N, VSIP_MEM_NONE)))
        return -1;
    /* NOTE
     * The block bind call above implies an INTERLEAVED storage format in the
     * user array, but once admitted, the implementation may use a different
     * storage format.
     */    
    if(NULL == (vw = vsip_cvbind_f(blk, 0, 1, N))) return -1;    
    /* NOTE
     * No layout is provided when the data object is created, so the view's own
     * layout is used.
     */             
    data = vsip_dda_cvdatacreate_f(vw, 0, VSIP_DDA_SYNC_POLICY_OUT, 0);
    if(NULL == data) return -1;    
    vsip_dda_cvdatagetattrib_f(data, &attrib);    
    /* NOTE
     * Since the layout of the data object is not specified, the data object
     * attributes must retrieved, and the storage format checked, to determine
     * which complex pointer accessor to use.
     */    
    if(VSIP_STORAGE_FORMAT_ARRAY == attrib.layout->storage_format)
    {        
        if(NULL == (arrayPtr = vsip_dda_cvptr_as_array_f(data))) return -1;
        arrayPtr[0].r = 1.0F; arrayPtr[0].i = -1.0F;
    }
    else if(VSIP_STORAGE_FORMAT_INTERLEAVED_COMPLEX
        == attrib.layout->storage_format)
    {
        if(NULL == (interPtr = vsip_dda_cvptr_as_inter_f(data)))
            return -1; 
        interPtr[0] = 1.0F; interPtr[1] = -1.0F;       
    }
    else if(VSIP_STORAGE_FORMAT_SPLIT_COMPLEX == attrib.layout->storage_format)
    {
        if(0 != vsip_dda_cvptr_as_split_f(data, &splitRealPtr, &splitImagPtr))
            return -1;  
        splitRealPtr[0] = 1.0F; splitImagPtr[0] = -1.0F;
    }
    else
        return -1;    
    if(0 != vsip_dda_cvdatadestroy_f(data)) return -1;   
    /* NOTE
     * Because of the OUT synchronization policy, the proxied view will have
     * been updated with the contents of any DDA buffers.
     */
    if(NULL == vsip_cvdestroy_f(vw)) return -1;
    vsip_cblockrelease_f(blk, 1, &data1ptr, &data2ptr);
    if(NULL == data1ptr && NULL == data2ptr) return -1;
    vsip_cblockdestroy_f(blk);    
    vsip_finalize((void *)0);     
    return 0;
}
