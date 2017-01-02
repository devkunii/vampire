#include <sstream>
#include <iostream>
#include "atoms.hpp"

#include "data.hpp"
#include "internal.hpp"
#include "opencl_include.hpp"
#include "opencl_utils.hpp"

#ifdef OPENCL

namespace vcl = ::vopencl::internal;

namespace vopencl
{
   namespace internal
   {
      namespace rng
      {
         bool compiled_grng = false;

         cl::Kernel grng;

         void update_grands(void)
         {
            if (!compiled_grng)
            {
               std::ostringstream opts;
               opts << "-DN=" << ::atoms::num_atoms*3;
               grng = vcl::build_kernel_from_file("src/opencl/cl/random.cl",
                                                  "gen_grands",
                                                  vcl::context, vcl::default_device,
                                                  opts.str());
               compiled_grng = true;
            }

            const cl::NDRange global(::atoms::num_atoms*3);

            vcl::kernel_call(grng, vcl::queue, global, vcl::local,
                             vcl::rng::urands,
                             vcl::rng::grands);
         }
      }
   }
}

#endif // OPENCL