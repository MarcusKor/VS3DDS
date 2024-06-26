#ifndef OMG_DDS_TTOPIC_TOPIC_HPP_
#define OMG_DDS_TTOPIC_TOPIC_HPP_

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

#include <dds/core/conformance.hpp>
#include <dds/core/types.hpp>
#include <dds/core/ref_traits.hpp>
#include <dds/core/cond/StatusCondition.hpp>
#include <dds/domain/DomainParticipant.hpp>
#include <dds/topic/qos/TopicQos.hpp>
#include <dds/topic/TopicDescription.hpp>


namespace dds { namespace topic {
  template <typename T, template <typename Q> class DELEGATE>
  class Topic;

  template <typename T>
  class TopicListener;
} }
/**
 * Topic is the most basic description of the data to be published and
 * subscribed.
 *
 * A Topic is identified by its name, which must be unique in the whole Domain.
 * In addition (by virtue of extending TopicDescription) it fully specifies the
 * type of the data that can be communicated when publishing or subscribing to
 * the Topic.
 * Topic is the only TopicDescription that can be used for publications and
 * therefore associated to a DataWriter.
 */
template <typename T, template <typename Q> class DELEGATE>
class dds::topic::Topic : public dds::topic::TopicDescription <T, DELEGATE> {
public:
  typedef TopicListener<T>                    Listener;

public:
  OMG_DDS_REF_TYPE_T(Topic, TopicDescription, T, DELEGATE)

  ~Topic() {}
public:
  /**
   * Create a new topic.
   *
   * @param dp the domain participant on which the topic will be defined.
   * @param topic_name the topic's name. The QoS will be set to
   *  dp.default_topic_qos().
   */
  Topic(const dds::domain::DomainParticipant& dp,
      const std::string& topic_name);

  /**
   * Create a new topic.The QoS will be set to
   *  dp.default_topic_qos().
   *
   * @param dp the domain participant on which the topic will be defined.
   * @param topic_name the topic's name.
   * @param type_name the name associated with the topic type.
   */
  Topic(const dds::domain::DomainParticipant& dp,
      const std::string& topic_name,
      const std::string& type_name);

  /**
   * Create a new topic.
   *
   * @param dp the domain participant on which the topic will be defined.
   * @param topic_name the topic's name.
   * @param qos the topic listener.
   * @param listener the topic listener.
   * @param mask the listener event mask.
   */
  Topic(const dds::domain::DomainParticipant& dp,
      const std::string& topic_name,
      const dds::topic::qos::TopicQos& qos,
      dds::topic::TopicListener<T>* listener = NULL,
      const dds::core::status::StatusMask& mask = dds::core::status::StatusMask::all());

  /**
   * Create a new topic.
   *
   * @param dp the domain participant on which the topic will be defined.
   * @param topic_name the topic's name.
   * @param type_name the name associated with the topic type.
   * @param qos the topic listener.
   * @param listener the topic listener.
   * @param mask the listener event mask.
   */
  Topic(const dds::domain::DomainParticipant& dp,
      const std::string& topic_name,
      const std::string& type_name,
      const dds::topic::qos::TopicQos& qos,
      dds::topic::TopicListener<T>* listener = NULL,
      const dds::core::status::StatusMask& mask = dds::core::status::StatusMask::all());

#if defined (OMG_DDS_X_TYPE_DYNAMIC_TYPE_SUPPORT)
  /**
   * Create a new topic with a dynamic type description. Notice that in this
   * case the data type has to be DynamicData. Thus the Topic type will be
   *  <code>Topic<DynamicData></code>
   *
   * @param dp the domain participant on which the topic will be defined.
   * @param topic_name the topic's name. The QoS will be set to
   *  dp.default_topic_qos().
   *  @param type the topic type
   */
  Topic(const dds::domain::DomainParticipant& dp,
      const std::string& topic_name,
      const dds::core::xtypes::DynamicType type);

  /**
	 Create a new topic with a dynamic type description. Notice that in this
   * case the data type has to be DynamicData. Thus the Topic type will be
   *  <code>Topic<DynamicData></code>
   *
   * @param dp the domain participant on which the topic will be defined.
   * @param topic_name the topic's name.
   * @param type the topic type
   * @param qos the topic listener.
   * @param listener the topic listener.
   * @param mask the listener event mask.
   */
  Topic(const dds::domain::DomainParticipant& dp,
      const std::string& topic_name,
      const dds::core::xtypes::DynamicType type
      const dds::topic::qos::TopicQos& qos,
      dds::topic::TopicListener<T>* listener = NULL,
      const dds::core::status::StatusMask& mask = dds::core::status::StatusMask::all());

#endif /* OMG_DDS_X_TYPE_DYNAMIC_TYPE_SUPPORT */

public:
  /**
   * Set the <code>Topic</code> listener.
   */
  void listener(Listener* the_listener,
      const ::dds::core::status::StatusMask& event_mask);

  /**
   * Get the <code>Topic</code> listener.
   */
  Listener* listener() const;


  /**
   * Get the <code>Topic</code> QoS.
   */
  const dds::topic::qos::TopicQos& qos() const;

  /**
   * Set the <code>Topic</code> QoS.
   */
  void qos(const dds::topic::qos::TopicQos& the_qos);

  /**
   * This method allows the application to retrieve the INCONSISTENT_TOPIC
   * status of the Topic.
   * Each DomainEntity has a set of relevant communication statuses.
   * A change of status causes the corresponding Listener to be invoked
   * and can also be monitored by means of the associated StatusCondition.
   * The complete list of communication status, their values, and the
   * DomainEntities they apply to is provided in Section 7.1.4.1,
   * Communication Status, on page 120.
   */
  const ::dds::core::status::InconsistentTopicStatus&
  inconsistent_topic_status() const;
};


#endif /* OMG_DDS_TTOPIC_TOPIC_HPP_ */
