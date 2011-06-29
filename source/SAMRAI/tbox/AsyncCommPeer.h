/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2010 Lawrence Livermore National Security, LLC
 * Description:   Staged peer-to-peer communication. 
 *
 ************************************************************************/
#ifndef included_tbox_AsyncCommPeer
#define included_tbox_AsyncCommPeer

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/tbox/AsyncCommStage.h"
#include "SAMRAI/tbox/SAMRAI_MPI.h"
#include "SAMRAI/tbox/Timer.h"
#include "SAMRAI/tbox/TimerManager.h"

#include <algorithm>
#include <cstring>

namespace SAMRAI {
namespace tbox {

/*!
 * @brief Supports point-to-point asynchronous communication
 * operations that can overlap other communcations using a
 * AsyncCommStage.
 *
 * To use this class:
 *
 * -# Construct an object with a stage or @c initialize() it with a
 *    stage.
 *
 * -# Set the MPI communicator and tag values for use in the MPI
 *    nonblocking message passing functions.
 *
 * -# Set the peer rank using setPeerRank().
 *
 * -# Call the beginSend() and beginRecv() functions.  Make sure
 *    there is a matching call on the peer processor.
 *
 * -# Use the stage to advance the message passing operations.  The
 *    stage tells you which members have completed their operations.
 *
 * -# Received data can be accessed using getRecvSize() and getRecvData().
 *
 * It is not neccesary to know how much data is being sent.  This
 * class will figure that out (using overhead data attached to the
 * actual user data being communicated).
 *
 * To check on sends and receives without using the stage, use
 * checkSend() and checkRecv() or proceedToNextWait().  To wait for
 * the current operation to complete, use completeCurrentOperation().
 */
template<class TYPE>
class AsyncCommPeer:public AsyncCommStage::Member
{

public:
   /*!
    * @brief Default constructor does not set up anything.
    * You must initialize() the object before using it.
    */
   AsyncCommPeer();

   /*!
    * @brief Construct staged communication object.
    *
    * @see initialize().
    */
   AsyncCommPeer(
      AsyncCommStage* stage,
      AsyncCommStage::Handler* handler = NULL);

   /*!
    * @brief Destructor.
    */
   virtual ~AsyncCommPeer(
      void);

   /*!
    * @brief Initialize the object.
    *
    * Attach self to the given stage and set the Handler.
    *
    * @param state The stage handling communicaiton requests for the object.
    * @param hander Optional pointer to user-defined data.
    */
   void
   initialize(
      AsyncCommStage* stage,
      AsyncCommStage::Handler* handler = NULL);

   //@{
   //! @name Define the peer relationship.

   /*!
    * @brief Set the peer rank.
    *
    * We assume (without checking) that the peer processor does
    * a complementary call using this processor as its peer.
    * If this assumption is wrong, there will likely be
    * communication errors.
    */
   void
   setPeerRank(
      int peer_rank);

   int
   getPeerRank() const;

   /*!
    * @brief Limit the data length in first message of a communication.
    *
    * Sometimes, the receiver does not know how much data is being
    * sent.  In such cases, the receiver would assume the data is no
    * more than a size that the sender and receiver have agreed upon
    * before hand, @c max_first_data_len.  If the message is longer,
    * the sender sends it in two parts.  The first message contains no
    * more than @c max_first_data_len items and indicates the true
    * length of the data.  A second message is used if needed.
    *
    * The receiver must set a first data length no less than
    * the sender's or risk a fatal MPI "truncated message" error.
    *
    * The limit is set to one by default, meaning that any message
    * more than one item long would be sent in two parts.
    */
   void
   limitFirstDataLength(
      size_t max_first_data_len);
   //@}

   /*!
    * @brief Set the MPI tags used for communication.
    *
    * The @c primary_tag is used for every message.  If a message
    * must be sent in two parts (see limitFirstDataLength()),
    * the secondary tag is used for the follow-up message.  It is
    * advisable to use different values for the two tags to ensure
    * that the second message is not mistaken for the first.
    *
    * @attention This class is NOT (and cannot be) responsible for
    * ensuring that the MPI communicator and tag are sufficient to
    * select the correct messages.  Please specify appropriate values
    * for the MPI communicator and tag.  Very elusive bugs can occur
    * if incorrect messages are received.
    */
   void
   setMPITag(
      const int primary_tag,
      const int secondary_tag);

   /*!
    * @brief Set the MPI object used for communications.
    *
    * @attention This class is NOT (and cannot be) responsible for
    * ensuring that the MPI object and tag are sufficient to
    * select the correct messages.  Please specify appropriate values
    * for the MPI object and tag.  Very elusive bugs can occur
    * if incorrect messages are received.  To be safe, it is best
    * to create a new communicator to avoid interference with other
    * communications within SAMRAI.
    */
   void
   setMPI(
      const SAMRAI_MPI& mpi);

   //@{

   /*!
    * @name Communication methods
    */

   /*!
    * @brief Begin a send communication.
    *
    * This must be paired with a beginRecv() by the peer processor.
    *
    * On return, the data in @b buffer would have been copied so it is
    * safe to deallocate or modify @c buffer.
    *
    * @return Whether operation is completed.
    */
   bool
   beginSend(
      const TYPE* buffer,
      int size);

   /*!
    * @brief Check the current broadcast communication and complete
    * the broadcast if all MPI requests are fulfilled.
    *
    * If no communication is in progress, this call does nothing.
    *
    * @return Whether operation is completed.
    */
   bool
   checkSend();

   /*!
    * @brief Begin a receive communication.
    *
    * This must be paired with a beginSend() by the peer processor.
    *
    * The received data is stored in an internal buffer accessible
    * through getRecvData().  After receiving, the internal buffer is
    * available until the object goes out of scope, another
    * communication operation is initiated or
    * deallocateInternalRecvBuffer() is called.
    *
    * The actual length of data received by the last receive operation
    * is returned by getRecvSize().
    *
    * @return Whether operation is completed.
    *
    * @internal Once everthing is working, we should implement a
    * version that takes an external buffer.  This optimizes out the
    * need to copy from the internal to an external buffer.  But it
    * requires some thought about size checking, especially with
    * regards to how to use the limit on the first message size.
    */
   bool
   beginRecv();

   /*!
    * @brief Check the current receive communication and complete the
    * receive if the MPI request has been fulfilled.
    *
    * @return Whether operation is completed.
    */
   bool
   checkRecv();

   /*!
    * @brief Return the size of received data.
    */
   int
   getRecvSize() const;

   /*!
    * @brief Get access to the received data.
    */
   const TYPE *
   getRecvData() const;

   /*!
    * @brief Clear the receive buffer immediately.
    *
    * This is optional.  The received data remains inside the object
    * the next communicaiton operation starts, or until it is
    * explicitly freed.
    *
    * It is an error to clear the receive buffer in the middle of a
    * communication operation.
    */
   void
   clearRecvData();

   /*!
    * @brief Check the current communication and complete it if all
    * MPI requests are fulfilled.
    */
   bool
   proceedToNextWait();

   /*!
    * @brief Whether the last communication operation has finished.
    *
    * This means more than just whether there is outstanding MPI
    * requests such as that returned by hasOutstandingRequests().  The
    * communication is more complex, requiring up to two messages
    * and copying of the received message into the correct buffer.
    * When isDone() returns true, the operation is truly finished.
    */
   bool
   isDone() const;

   /*!
    * @brief Wait for the current operation to complete.
    */
   void
   completeCurrentOperation();

   //@}

   /*!
    * @brief For use in debugging.
    */
   void
   logCurrentState(
      std::ostream& co) const;

private:
   /*
    * @brief Data structure for combining integer overhead data along with
    * user data TYPE in the same MPI message.
    */
   union FlexData {
      int i;
      TYPE t;
      FlexData()
      {
#ifdef DEBUG_INITIALIZE_UNDEFINED
         memset(& i, 0, std::max(sizeof(int), sizeof(TYPE)));
#endif
      }
   };

   /*
    * @brief Assert that user-set MPI parameters are valid.
    */
   void
   checkMPIParams();

   /*!
    * @brief Return the number of FlexData storage space to store a given
    * number of TYPE data.
    */
   size_t
   getNumberOfFlexData(
      size_t number_of_type_data) const;

   /*!
    * @brief Operation disallowed due to primitive internal memory management.
    */
   AsyncCommPeer(
      const AsyncCommPeer& r);

   /*!
    * @brief Operation disallowed by primitive internal memory management.
    */
   AsyncCommPeer&
   operator = (
      const AsyncCommPeer& r);

   //! @brief Operations users would want to do.
   enum BaseOp { undefined,
                 send,
                 recv };
   //! @brief Tasks used to complete a base operation.
   enum TaskOp { send_start,
                 send_check,
                 recv_start,
                 recv_check0,
                 recv_check1,
                 none };

   void
   resetStatus(
      SAMRAI_MPI::Status& mpi_status);

   /*
    * @brief Resize the internal buffer to hold size FlexData unions.
    *
    * Note this is FlexData's not TYPE's.
    */
   void
   resizeBuffer(
      size_t size);

   /*!
    * Free static timers.
    *
    * To be called by shutdown registry to make sure memory for timers
    * does not leak.
    */
   static void
   finalizeCallback();

   /*!
    * @brief Rank of peer process.
    *
    * If negative, peer has not been set.
    */
   int d_peer_rank;

   /*!
    * @brief Operation being performed.
    *
    * At the completion of the operation, d_base_op does NOT get
    * reset to undefined.  Its value is used in getRecvData() and
    * is also useful for debugging.
    */
   BaseOp d_base_op;

   /*!
    * @brief Next task in a current communication operation.
    *
    * If d_next_task_op is none, there is no current communication
    * operation (the last one is completed).
    */
   TaskOp d_next_task_op;

   /*!
    * @brief Max data size of first message when the receiver does
    * not know the message size.
    *
    * @see limitFirstDataLength().
    */
   size_t d_max_first_data_len;

   /*!
    * @brief The full count of data items before splitting up
    * to into the limit of the first chunk.
    */
   size_t d_full_count;

   /*!
    * @brief External data input buffer.
    *
    * Though we keep this around after beginSend() returns, in the
    * current implementation, it is not needed.  It is kept around
    * to help debugging.  After beginSend() returns, the pointer
    * is no longer guaranteed to be valid.
    */
   const TYPE* d_external_buf;

   /*!
    * @brief Internal buffer.
    *
    * Used for sends and receives.  This is typed on FlexData because
    * it must be able to store the templated type TYPE and some integer
    * for overhead data.
    */
   size_t d_internal_buf_size;
   FlexData* d_internal_buf;

   /*!
    *
    */
   SAMRAI_MPI d_mpi;
   /*!
    * @brief Tag for the first message.
    */
   int d_tag0;
   /*!
    * @brief Tag for the first message.
    */
   int d_tag1;

   /*!
    * @brief Whether send completion should be reported when
    * AsyncCommPeer_DEBUG_OUTPUT is defined.
    *
    * This is non-essential data used in debugging.
    */
   bool d_report_send_completion[2];

   // Make some temporary variable statuses to avoid repetitious allocations.
   int d_mpi_err;

   static Pointer<Timer> t_waitall_timer;
   static Pointer<Timer> t_send_timer;
   static Pointer<Timer> t_recv_timer;

   /**
    * \brief Has shutdown handler been initialized.
    *
    * This should be checked and set in every ctor.
    */
   static bool s_initialized;

   /**
    * \brief Initialize static state
    */
   static bool
   initialize(
      void);

};

}
}

#ifdef SAMRAI_INLINE
#include "SAMRAI/tbox/AsyncCommPeer.I"
#endif

#ifdef INCLUDE_TEMPLATE_IMPLEMENTATION
#include "SAMRAI/tbox/AsyncCommPeer.C"
#endif

#endif  // included_tbox_AsyncCommPeer