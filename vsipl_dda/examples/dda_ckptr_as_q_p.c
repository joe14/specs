#include "vsip.h"
#include "stdio.h"

#define N 1024

int main(int argc, char **argv)
{
    vsip_scalar_f userArray[2*N];
    vsip_cblock_f *blk;
    vsip_cvview_f *vw;
    vsip_cscalar_f v;
    vsip_dda_cvdata_f *data;
    vsip_dda_cvdataattr_f attrib;
    vsip_cscalar_f *arrayPtr;
    vsip_scalar_f *intrlvdPtr;
    vsip_scalar_f *splitRealPtr, *splitImagPtr;
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
    
    /* NOTE
     * No layout is provided when the data object is created, so the view's own
     * layout is used.
     */
             
    data = vsip_dda_cvdatacreate_f(vw, 0, VSIP_DDA_SYNC_POLICY_OUT);
    if(NULL == data)
    {
        printf("ERROR: Could not create the data object\n");
        return -1;
    }
    
    vsip_dda_cvdatagetattrib_f(data, &attrib);
    
    /* NOTE
     * Since the layout of the data object is not specified, the data object
     * attributes must retrieved, and the storage format checked, to determine
     * which complex pointer accessor to use.
     */
    
    if(VSIP_STORAGE_FORMAT_ARRAY == attrib.layout->storage_format)
    {
        arrayPtr = vsip_dda_cvptr_as_array_f(data);
        if(NULL == arrayPtr)
        {
            printf("ERROR: Could not get the array pointer\n");
            return -1;
        }

        printf("Pointer = 0x%p\n", (void*)arrayPtr);
        printf("Value Before Modify = %f + %fi\n", arrayPtr->r, arrayPtr->i);
        
        arrayPtr[0].r = 1.0F;
        arrayPtr[0].i = -1.0F;
       
        printf("Value After Modify = %f + %fi\n", arrayPtr->r, arrayPtr->i);
    }
    else if(VSIP_STORAGE_FORMAT_INTERLEAVED_COMPLEX
        == attrib.layout->storage_format)
    {
        intrlvdPtr = vsip_dda_cvptr_as_interleaved_f(data);
        if(NULL == intrlvdPtr)
        {
            printf("ERROR: Could not get the interleaved pointer\n");
            return -1;
        }
        
        printf("Pointer = 0x%p\n", (void*)intrlvdPtr);
        printf("Value Before Modify = %f + %fi\n", intrlvdPtr[0],
            intrlvdPtr[1]);
        
        intrlvdPtr[0] = 1.0F;
        intrlvdPtr[1] = -1.0F;
       
        printf("Value After Modify = %f + %fi\n", intrlvdPtr[0], intrlvdPtr[1]);
    }
    else if(VSIP_STORAGE_FORMAT_SPLIT_COMPLEX == attrib.layout->storage_format)
    {
        if(0 != vsip_dda_cvptr_as_split_f(data, &splitRealPtr, &splitImagPtr))
        {
            printf("ERROR: Could not get the split pointers\n");
            return -1;
        }
        
        printf("Real Pointer = 0x%p\n", (void*)splitRealPtr);
        printf("Imag Pointer = 0x%p\n", (void*)splitImagPtr);
        printf("Value Before Modify = %f + %fi\n", splitRealPtr[0],
            splitImagPtr[0]);
        
        splitRealPtr[0] = 1.0F;
        splitImagPtr[0] = -1.0F;

        printf("Value After Modify = %f + %fi\n", splitRealPtr[0],
            splitImagPtr[0]);
    }
    else
    {
        printf("ERROR: Unexpected storage format: %d\n",
            attrib.layout->storage_format);
        return -1;
    }
    
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
