#ifndef OMG_DDS_SUB_TDATA_READER_HPP_
#define OMG_DDS_SUB_TDATA_READER_HPP_

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
#include <dds/core/detail/conformance.hpp>
#include <dds/core/TEntity.hpp>
#include <dds/topic/ContentFilteredTopic.hpp>
#include <dds/topic/TopicInstance.hpp>
#include <dds/sub/LoanedSamples.hpp>

namespace dds {
  namespace sub {
    class Query;

    template <typename T, template <typename Q> class DELEGATE>
    class DataReader;

    template <typename T>
    class DataReaderListener;
  }
}

template <typename T, template <typename Q> class DELEGATE>
class dds::sub::DataReader : public dds::core::TEntity< DELEGATE<T> > {

public:
  typedef T                                        DataType;
  typedef ::dds::sub::DataReaderListener<T>        Listener;

public:

  /**
   * The <code>Selector</code> class is used by the <code>DataReader</code>
   * to compose read operations.
   *
   * By default the instance is nil,
   */
  class Selector {
  public:
    Selector(DataReader& dr);

    Selector& instance(const dds::core::InstanceHandle& h);
    Selector& instance(const dds::core::InstanceHandle& h);
    Selector& state(const dds::sub::status::DataState& s);
    Selector& content(const dds::sub::Query& query);
    Selector& max_samples(uint32_t n);

    dds::sub::LoanedSamples<T> read();
    dds::sub::LoanedSamples<T> take();

    // --- Forward Iterators: --- //
    template <typename SamplesFWIterator>
    uint32_t
    read(SamplesFWIterator sfit, uint32_t max_samples);

    template <typename SamplesFWIterator>
    uint32_t
    take(SamplesFWIterator sfit,	uint32_t max_samples);

    // --- Back-Inserting Iterators: --- //
    template <typename SamplesBIIterator>
    uint32_t
    read(SamplesBIIterator sbit);


    template <typename SamplesBIIterator>
    uint32_t
    take(SamplesBIIterator sbit);

  private:
    typename DELEGATE<T>::Selector impl_;
  };

  /**
   * Selector class enabling the streaming API.
   */
  class ManipulatorSelector {
  public:
    ManipulatorSelector(DataReader& dr);

    bool read_mode();
    void read_mode(bool b);

    ManipulatorSelector& max_samples(uint32_t n);
    ManipulatorSelector& instance(const dds::core::InstanceHandle& h);
    ManipulatorSelector& next_instance(const dds::core::InstanceHandle& h);
    ManipulatorSelector& state(const dds::sub::status::DataState& s);
    ManipulatorSelector& content(const dds::sub::Query& query);

    ManipulatorSelector&
    operator >>(dds::sub::LoanedSamples<T>& samples);

    ManipulatorSelector&
    operator >> (ManipulatorSelector& (manipulator)(ManipulatorSelector&));

    template <typename Functor>
    ManipulatorSelector
    operator >> (Functor f);

  private:
    typename DELEGATE<T>::ManipulatorSelector impl_;

  };

public:
  OMG_DDS_REF_TYPE(DataReader, dds::core::TEntity, DELEGATE<T>)

public:
  /**
   * Create a <code>DataReader</code>. The QoS will be the same as
   * "sub.default_datareader_qos()".
   *
   * @param sub the subscriber owning this <code>DataReader</code>.
   * @param topic the topic associated with this <code>DataReader</code>.
   */
  DataReader(const dds::sub::Subscriber& sub,
      const ::dds::topic::Topic<T>& topic);
  /**
   * Create a <code>DataReader</code>.
   *
   * @param sub the subscriber owning this <code>DataReader</code>.
   * @param topic the topic associated with this <code>DataReader</code>.
   * @param qos the QoS settings for this <code>DataReader</code>.
   * @param listener the listener.
   * @param mask the event mask associated to the <code>DataReader</code> listener.
   */
  DataReader(const dds::sub::Subscriber& sub,
      const ::dds::topic::Topic<T>& topic,
      const dds::sub::qos::DataReaderQos& qos,
      dds::sub::DataReaderListener<T>* listener = NULL,
      const dds::core::status::StatusMask& mask = ::dds::core::status::StatusMask::all());

#ifdef OMG_DDS_CONTENT_SUBSCRIPTION_SUPPORT

  /**
   * Create a <code>DataReader</code> for a <code>ContentFilteredTopic</code>.
   * This <code>DataReader</code> will only receive that data that mathes the
   * <code>Filter</code> associated with the <code>ContentFilteredTopic</code>.
   * The QoS will be set to sub.default_datareader_qos().
   *
   * @param sub the subscriber owning this <code>DataReader</code>.
   * @param topic the content filtered topic.
   */
  DataReader(const dds::sub::Subscriber& sub,
      const ::dds::topic::ContentFilteredTopic<T>& topic);

  /**
   * Create a <code>DataReader</code> for a <code>ContentFilteredTopic</code>.
   * This <code>DataReader</code> will only receive that data that mathes the
   * <code>Filter</code> associated with the <code>ContentFilteredTopic</code>.
   *
   * @param sub the subscriber owning this <code>DataReader</code>.
   * @param topic the content filtered topic.
   * @param qos the QoS settings for this <code>DataReader</code>.
   * @param listener the listener.
   * @param mask the event mask associated to the <code>DataReader</code> listener.
   */
  DataReader(const dds::sub::Subscriber& sub,
      const ::dds::topic::ContentFilteredTopic<T>& topic,
      const dds::sub::qos::DataReaderQos& qos,
      dds::sub::DataReaderListener<T>* listener = NULL,
      const dds::core::status::StatusMask& mask = ::dds::core::status::StatusMask::all());
#endif /* OMG_DDS_CONTENT_SUBSCRIPTION_SUPPORT */

#ifdef OMG_DDS_MULTI_TOPIC_SUPPORT

  /**
   * Create a <code>DataReader</code> for a <code>MultiTopic</code>.
   * This <code>DataReader</code> will only receive that data that mathes the
   * <code>Filter</code> associated with the <code>ContentFilteredTopic</code>.
   * The QoS will be set to sub.default_datareader_qos().
   *
   * @param sub the subscriber owning this <code>DataReader</code>.
   * @param topic the multi-topic.
   */
  DataReader(const dds::sub::Subscriber& sub,
      const ::dds::topic::MultiTopic<T>& topic);


  /**
   * Create a <code>DataReader</code> for a <code>MultiTopic</code>.
   * This <code>DataReader</code> will only receive that data that mathes the
   * <code>Filter</code> associated with the <code>ContentFilteredTopic</code>.
   *
   * @param sub the subscriber owning this <code>DataReader</code>.
   * @param topic the multi-topic.
   * @param qos the QoS settings for this <code>DataReader</code>.
   * @param listener the listener.
   * @param mask the event mask associated to the <code>DataReader</code> listener.
   */
  DataReader(const dds::sub::Subscriber& sub,
      const ::dds::topic::MultiTopic<T>& topic,
      const dds::sub::qos::DataReaderQos& qos,
      dds::sub::DataReaderListener<T>* listener = NULL,
      const dds::core::status::StatusMask& mask = ::dds::core::status::StatusMask::all());

#endif /* OMG_DDS_MULTI_TOPIC_SUPPORT */
public:
  ~DataReader();


public:
  // == ReadState Management

  /**
   * Returns the default read-state (if not changed, it is set to
   * ReaderState::any()).
   */
  const dds::sub::status::DataState& default_filter_state();

  /**
   * Set the default state filter for read/take  operations.
   *
   * @param state the state mask that will be used to read/take samples.
   */
  DataReader& default_filter_state(const dds::sub::status::DataState& state);

  //== Streaming read/take

  /**
   * Manipulators are defined externally to make it possible to control whether the
   * streaming operators reads or takes.
   * dr >> read >> loanedSamples;
   * dr >> take >> loanedSamples;
   *
   */
  DataReader& operator >>(dds::sub::LoanedSamples<T>& ls);

  ManipulatorSelector
  operator >> (ManipulatorSelector& (manipulator)(ManipulatorSelector&));

  template <typename Functor>
  ManipulatorSelector
  operator >> (Functor f);


  ///////////////////////////////////////////////////////////////////////
public:
  //== Loan Read/Take API ==================================================

  /**
   * Read all samples using the default filter state. The memory used for
   * storing the sample may be loaned by the middleware thus allowing zero
   * copy operations.
   * Implementation should strike to makethis read implementation
   * exception safe.
   */
  LoanedSamples<T> read();

  /**
   * Take all samples using the default filter state. The memory used for
   * storing the sample may be loaned by the middleware thus allowing zero
   * copy operations.
   * Implementation should strike to make this read implementation
   * exception safe.
   */
  LoanedSamples<T> take();

  //== Copy Read/Take API ==================================================

  // --- Forward Iterators: --- //

  /**
   * Read up to <code>max_samples</code> samples using the default filter state.
   * The samples are copied into the application provided container using the
   * forward iterator parameter.
   *
   * @param sfit samples forward iterator
   * @param max_samples the maximum number of samples to read
   * @return the number of read samples.
   */
  template <typename SamplesFWIterator>
  uint32_t
  read(SamplesFWIterator sfit,
      uint32_t max_samples);

  /**
   * Take up to <code>max_samples</code> samples using the default filter state.
   * The samples are copied into the application provided container using the
   * forward iterator parameter.
   *
   * @param sfit samples forward iterator.
   * @param max_samples the maximum number of samples to take.
   * @return the number of taken samples.
   */
  template <typename SamplesFWIterator>
  uint32_t
  take(SamplesFWIterator sfit,
      uint32_t max_samples);


  // --- Back-Inserting Iterators: --- //

  /**
   * Read all samples available in the reader cache using the default filter state.
   * The samples are copied into the application provided container using a
   * back-inserting iterator. Notice that as a consequence of using a back-inserting
   * iterator, this operation may allocate memory to resize the underlying container.
   *
   * @param sbit samples back-inserting iterator.
   * @return the number of read samples.
   */
  template <typename SamplesBIIterator>
  uint32_t
  read(SamplesBIIterator sbit);

  /**
   * Take all samples available in the reader cache samples using the default filter state.
   * The samples are copied into the application provided container using a
   * back-inserting iterator. Notice that as a consequence of using a back-inserting
   * iterator, this operation may allocate memory to resize the underlying container.
   *
   * @param sbit samples back-inserting iterator.
   * @return the number of taken samples.
   */
  template <typename SamplesBIIterator>
  uint32_t
  take(SamplesBIIterator sbit);
public:
  //========================================================================
  //== DSL Method for dealing with instances, content and status filters.

  /**
   * Get a <code>Selector</code> to perform complex data selections, such as
   * per-instance selection, content and status filtering.
   *
   * The selector can be used as follows:
	   <pre>
	      dr.select()
	           .instance(handle)
	           .content(query)
	           .state(state)
	        .take(sbit);
      </pre>
   * This shows how samples can be taken by selecting a specific instance,
   * then filtering by state and content.
   */
  Selector select();

  //========================================================================
  //== Instance Management
public:
  /**
   * This operation can be used to retrieve the instance key that corresponds
   * to an instance_handle. The operation will only fill the fields that form
   * the key inside the key_holder instance.
   * This operation may raise a BadParameter exception if the InstanceHandle
   * does not correspond to an existing data-object known to the DataWriter.
   * If the implementation is not able to check invalid handles, then the
   * result in this situation is unspecified.
   */
  dds::topic::TopicInstance<T> key_value(const dds::core::InstanceHandle& h);

  /**
   * This operation can be used to retrieve the instance key that corresponds
   * to an instance_handle. The operation will only fill the fields that form
   * the key inside the key_holder instance.
   * This operation may raise a BadParameter exception if the InstanceHandle
   * does not correspond to an existing data-object known to the DataWriter.
   * If the implementation is not able to check invalid handles, then the
   * result in this situation is unspecified.
   */
  T& key_value(T& sample, const dds::core::InstanceHandle& h);

  /**
   * This operation takes as a parameter an instance and returns a handle
   * that can be used in subsequent operations that accept an instance handle
   * as an argument. The instance parameter is only used for the purpose
   * of examining the fields that define the key. This operation does not
   * register the instance in question. If the instance has not been
   * previously registered, or if for any other reason the Service is unable
   * to provide an instance handle, the Service will return a TopicInstance
   * whose handle will be set to the HANDLE_NIL value.
   */
  const dds::core::InstanceHandle
  lookup_instance(const T& key) const;

  //==========================================================================
  // -- Entity Navigation API
public:
  /**
   * Get the <code>TopicDescription</code> associated with this reader.
   */
  dds::topic::TopicDescription<DataType> topic_description() const;

  /**
   * Get the subscriber owning this reader.
   */
  const dds::sub::Subscriber& subscriber() const;

  /**
   * Set/Re-set the listener associated with this reader.
   *
   * @param the_listener the listener.
   * @param event_mask the event mask associated with the listener.
   */
  void listener(Listener* the_listener,
      const dds::core::status::StatusMask& event_mask);

  /**
   * Get the listener associated with this reader.
   */
  Listener* listener() const;

  // -- Qos Getter/Setter

  /**
   * Get the QoS associated with this reader.
   */
  const dds::sub::qos::DataReaderQos&
  qos() const;

  /**
   * Set the QoS associated with this reader.
   *
   * @param the_qos the new QoS.
   */
  void qos(const dds::sub::qos::DataReaderQos& the_qos);

  /**
   * Get the QoS associated with this reader.
   */
  DataReader& operator << (const dds::sub::qos::DataReaderQos& the_qos);

  /**
   * Set the QoS associated with this reader.
   *
   * @param the_qos the new QoS.
   */
  DataReader& operator >> (dds::sub::qos::DataReaderQos& the_qos);

  /**
   * Wait for historical data for a given amount of time. This time
   * may be set to infinite.
   *
   * @param timeout the time to wait for historical data.
   */
  void
  wait_for_historical_data(const dds::core::Duration& timeout);

  //========================================================================
  //== Status API
  const dds::core::status::LivelinessChangedStatus&
  liveliness_changed_status() const;

  const dds::core::status::SampleRejectedStatus&
  sample_rejected_status() const;

  const dds::core::status::SampleLostStatus&
  sample_lost_status() const;

  const dds::core::status::RequestedDeadlineMissedStatus&
  requested_deadline_missed_status();

  const dds::core::status::RequestedIncompatibleQosStatus&
  requested_incompatible_qos_status() const;

  const dds::core::status::SubscriptionMatchedStatus&
  subscription_matched_status() const;
};


#endif /* OMG_DDS_SUB_TDATA_READER_HPP_ */
