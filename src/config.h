#ifndef _NAL_CONFIG_H
#define _NAL_CONFIG_H

#define USE_STD

#define UAL_NAMESPACE ual

#if defined (USE_STD)

#include <utility>
#include <cstddef>

namespace UAL_NAMESPACE {

using std::forward;
using std::size_t;

}

#endif

#endif

