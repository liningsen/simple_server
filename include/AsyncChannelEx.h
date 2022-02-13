//
// AsyncChannelEx.h
//
// $Id: //poco/1.4/Foundation/include/Poco/AsyncChannelEx.h#2 $
//
// Library: Foundation
// Package: Logging
// Module:  AsyncChannelEx
//
// Definition of the AsyncChannelEx class.
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_AsyncChannelEx_INCLUDED
#define Foundation_AsyncChannelEx_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Channel.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/Runnable.h"
#include "Poco/NotificationQueue.h"


namespace Poco {


class Foundation_API AsyncChannelEx: public Channel, public Runnable
	/// A channel uses a separate thread for logging.
	///
	/// Using this channel can help to improve the performance of
	/// applications that produce huge amounts of log messages or
	/// that write log messages to multiple channels simultaneously.
	///
	/// All log messages are put into a queue and this queue is
	/// then processed by a separate thread.
{
public:
	AsyncChannelEx(Channel* pChannel = 0, Thread::Priority prio = Thread::PRIO_NORMAL);
		/// Creates the AsyncChannelEx and connects it to
		/// the given channel.

	void setChannel(Channel* pChannel);
		/// Connects the AsyncChannelEx to the given target channel.
		/// All messages will be forwarded to this channel.
		
	Channel* getChannel() const;
		/// Returns the target channel.

	void open();
		/// Opens the channel and creates the 
		/// background logging thread.
		
	void close();
		/// Closes the channel and stops the background
		/// logging thread.

	void log(const Message& msg);
		/// Queues the message for processing by the
		/// background thread.

	void setProperty(const std::string& name, const std::string& value);
		/// Sets or changes a configuration property.
		///
		/// The "channel" property allows setting the target 
		/// channel via the LoggingRegistry.
		/// The "channel" property is set-only.
		///
		/// The "priority" property allows setting the thread
		/// priority. The following values are supported:
		///    * lowest
		///    * low
		///    * normal (default)
		///    * high
		///    * highest
		///
		/// The "priority" property is set-only.

protected:
	~AsyncChannelEx();
	void run();
	void setPriority(const std::string& value);
		
private:
	Channel*  _pChannel;
	Thread    _thread;
	FastMutex _threadMutex;
	FastMutex _channelMutex;
	NotificationQueue _queue;
};


} // namespace Poco


#endif // Foundation_AsyncChannel_INCLUDED
