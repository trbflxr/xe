//
// Created by trbflxr on 4/3/2020.
//

#ifndef XE_GPU_RESOURCE_HPP
#define XE_GPU_RESOURCE_HPP

namespace xe {

  struct GPUResource {
    virtual void destroy() = 0;
  };

}

#endif //XE_GPU_RESOURCE_HPP
