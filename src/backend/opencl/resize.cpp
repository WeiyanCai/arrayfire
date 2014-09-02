#include <af/array.h>
#include <af/dim4.hpp>
#include <Array.hpp>
#include <resize.hpp>
#include <kernel/resize.hpp>
#include <stdexcept>

namespace opencl
{
    template<typename T>
    Array<T>* resize(const Array<T> &in, const dim_type odim0, const dim_type odim1,
                     const af_interp_type method)
    {
        const af::dim4 iDims = in.dims();
        af::dim4 oDims(odim0, odim1, iDims[2], iDims[3]);

        if(iDims.elements() == 0 || oDims.elements() == 0) {
            throw std::runtime_error("Elements is 0");
        }

        Array<T> *out = createEmptyArray<T>(oDims);

        switch(method) {
            case AF_INTERP_NEAREST:
                kernel::resize<T, AF_INTERP_NEAREST>
                    (out->get(), oDims[0], oDims[1], in.get(), iDims[0], iDims[1], iDims[2],
                     out->strides().get(), in.strides().get(), in.getOffset());
                break;
            case AF_INTERP_BILINEAR:
                kernel::resize<T, AF_INTERP_BILINEAR>
                    (out->get(), oDims[0], oDims[1], in.get(), iDims[0], iDims[1], iDims[2],
                     out->strides().get(), in.strides().get(), in.getOffset());
                break;
            default:
                break;
        }
        return out;
    }


#define INSTANTIATE(T)                                                                            \
    template Array<T>* resize<T> (const Array<T> &in, const dim_type odim0, const dim_type odim1, \
                                  const af_interp_type method);


    INSTANTIATE(float)
    INSTANTIATE(double)
    INSTANTIATE(int)
    INSTANTIATE(uint)
    INSTANTIATE(uchar)
    INSTANTIATE(char)
}
