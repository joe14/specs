#include "vsip.h"
#include "stdio.h"

#define N 1024

int main(int argc, char **argv)
{
    vsip_scalar_f userArray[2*N];
    vsip_cblock_f *blk;
    vsip_cvview_f *vw;
    vsip_cscalar_f v;
    vsip_datalayout lyt;
    vsip_dda_cvdata_f *data;
    vsip_cscalar_f *ptr;
    vsip_scalar_f *data1ptr, *data2ptr;
    
    printf("Value = %f + %fi\n", userArray[0], userArray[1]);
    
    vsip_init((void *)0);
    
    blk = vsip_cblockbind_f(&userArray[0], 0, N, VSIP_MEM_NONE);
    if(NULL == blk)
    {
        printf("ERROR: Could not bind the block\n");
        return -1;
    }
    
    /* NOTE
     * The block bind call above implies an INTERLEAVED storage format in the
     * user array, but once admitted, the implementation may use a different
     * storage format.
     */
    
    if(0 != vsip_cblockadmit_f(blk, 0))
    {
        printf("ERROR: Could not admit the block\n");
        return -1;
    }
    
    vw = vsip_cvbind_f(blk, 0, 1, N);
    if(NULL == vw)
    {
        printf("ERROR: Could not bind the block to a view\n");
        return -1;
    }
    
    v = vsip_cvget_f(vw, 0);
    printf("Value From View = %f + %fi\n", v.r, v.i);
    
    lyt.dim = 1;
    lyt.storage_format = VSIP_STORAGE_FORMAT_ARRAY;
    lyt.packing = VSIP_PACK_TYPE_DENSE;
    lyt.order = VSIP_DIM_ORDER_VROW_MAJOR;
        
    data = vsip_dda_cvdatacreate_f(vw, &lyt, VSIP_DDA_SYNC_POLICY_OUT);
    if(NULL == data)
    {
        printf("ERROR: Could not create the data object\n");
        return -1;
    }
    
    ptr = vsip_dda_cvptr_as_array_f(data);
    if(NULL == ptr)
    {
        printf("ERROR: Could not get the pointer\n");
        return -1;
    }
    
    /* NOTE
     * Because the storage format of the data object is ARRAY, no other
     * complex pointer accessor would work for the data object in this example.
     */

    printf("Pointer = 0x%p\n", (void*)ptr);
    printf("Value Before Modify = %f + %fi\n", ptr->r, ptr->i);
     
    ptr[0].r = 1.0F;
    ptr[0].i = -1.0F;
    
    printf("Value After Modify = %f + %fi\n", ptr->r, ptr->i);
    
    if(0 != vsip_dda_cvdatadestroy_f(data))
    {
        printf("ERROR: Could not destroy the data object\n");
        return -1;
    }
    
    /* NOTE
     * Because of the OUT synchronization policy, the proxied view will have
     * been updated with the contents of any DDA buffers.
     */
     
    if(NULL == vsip_cvdestroy_f(vw))
    {
        printf("ERROR: Tried to destroy a NULL view object\n");
        return -1;
    }
    
    vsip_cblockrelease_f(blk, 1, &data1ptr, &data2ptr);
    if(NULL == data1ptr && NULL == data2ptr)
    {
        printf("ERROR: The block could not be released\n");
        return -1;
    }
    
    /* NOTE
     * Presumably, because the block was bound with an INTERLEAVED storage
     * format, it will also be released with one; but that may not be the case.
     */
    
    if(NULL == data2ptr)
    {
        printf("NOTE: The released block has an INTERLEAVED storage format\n");
        printf("Value After Release = %f + %fi\n", data1ptr[0], data1ptr[1]);
    }
    else
    {
        printf("NOTE: The released block has a SPLIT storage format\n");
        printf("Value After Release = %f + %fi\n", data1ptr[0], data2ptr[0]);
    }
    
    vsip_cblockdestroy_f(blk);
    
    vsip_finalize((void *)0);
     
    return 0;
}
