#ifndef OMG_DDS_CORE_TYPES_HPP_
#define OMG_DDS_CORE_TYPES_HPP_

/* Copyright 2010, Object Management Group, Inc.
 * Copyright 2010, PrismTech, Corp.
 * Copyright 2010, Real-Time Innovations, Inc.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// ISO C++ Includes
#include <string>
#include <vector>

// DDS Includes
#include <dds/core/detail/inttypes.hpp>
#include <dds/core/macros.hpp>
#include <dds/core/detail/conformance.hpp>

namespace dds { namespace core {
  typedef std::vector<uint8_t> ByteSeq;
  typedef std::vector<std::string> StringSeq;

  // DDS Null-Reference
  class null_type { };
  extern const null_type null;


#ifdef OMG_DDS_EXTENSIBLE_AND_DYNAMIC_TOPIC_TYPE_SUPPORT    
  namespace policy {

    typedef uint16_t DataRepresentationId;
    typedef std::vector<DataRepresentationId> DataRepresentationIdSeq;
  }
#endif

  namespace policy {
    typedef uint32_t QosPolicyId;
  }
} }

#endif /* OMG_DDS_CORE_TYPES_HPP_ */
