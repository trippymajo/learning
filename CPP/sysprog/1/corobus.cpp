#include "corobus.h"

#include "libcoro.h"
#include "rlist.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ringbuf.h"

/**
 * One coroutine waiting to be woken up in a list of other
 * suspended coros.
 */
struct wakeup_entry {
	struct rlist base;
	struct coro *coro;
};

/** A queue of suspended coros waiting to be woken up. */
struct wakeup_queue {
	struct rlist coros;
};

#if 0 /* Uncomment this if want to use */

/** Suspend the current coroutine until it is woken up. */
static void
wakeup_queue_suspend_this(struct wakeup_queue *queue)
{
	struct wakeup_entry entry;
	entry.coro = coro_this();
	rlist_add_tail_entry(&queue->coros, &entry, base);
	coro_suspend();
	rlist_del_entry(&entry, base);
}

/** Wakeup the first coroutine in the queue. */
static void
wakeup_queue_wakeup_first(struct wakeup_queue *queue)
{
	if (rlist_empty(&queue->coros))
		return;
	struct wakeup_entry *entry = rlist_first_entry(&queue->coros,
		struct wakeup_entry, base);
	coro_wakeup(entry->coro);
}

#endif

struct coro_bus_channel {
	/** Channel max capacity. */
	size_t size_limit;
	/** Coroutines waiting until the channel is not full. */
	struct wakeup_queue send_queue;
	/** Coroutines waiting until the channel is not empty. */
	struct wakeup_queue recv_queue;
	/** Message queue. */
	RingBuf ring_buf;

	explicit coro_bus_channel(size_t limit)
		: size_limit(limit),
			send_queue{},
			recv_queue{},
			ring_buf(limit)
	{}
};

struct coro_bus {
	struct coro_bus_channel **channels;
	int channel_count;
};

static enum coro_bus_error_code global_error = CORO_BUS_ERR_NONE;

enum coro_bus_error_code
coro_bus_errno(void)
{
	return global_error;
}

void
coro_bus_errno_set(enum coro_bus_error_code err)
{
	global_error = err;
}

struct coro_bus *
coro_bus_new(void)
{
	auto bus = new coro_bus;
	bus->channel_count = 0;
	bus->channels = nullptr;

	coro_bus_errno_set(CORO_BUS_ERR_NONE);
	return bus;
}

void
coro_bus_delete(struct coro_bus *bus)
{
	if (!bus)
		return;

	for (size_t i = 0; i < bus->channel_count; ++i)
	{
		auto& channel = bus->channels[i];
		if (channel != nullptr)
		{
			delete channel;
		}
	}

	delete[] bus->channels;
	delete bus;
}

int
coro_bus_channel_open(struct coro_bus *bus, size_t size_limit)
{
	if (!bus || size_limit == 0)
		return -1;
	
	// 1. Try find free slot
	for (size_t i = 0; i < bus->channel_count; ++i)
	{
		auto& channel = bus->channels[i];
		if (channel == nullptr)
		{
			channel = new coro_bus_channel(size_limit);
			return (int)i;
		}
	}

	// 2. No free slot, grow channels array.
	const int old_count = bus->channel_count;
	const int new_count = old_count + 1;

	auto new_channels = new coro_bus_channel*[new_count];

	// Copy old channel pointers.
	for (int i = 0; i < old_count; ++i)
	{
		new_channels[i] = bus->channels[i];
	}

	// Create new channel at the end.
	new_channels[old_count] = new coro_bus_channel(size_limit);

	// Delete old array of pointers.
	delete[] bus->channels;

	bus->channels = new_channels;
	bus->channel_count = new_count;

	coro_bus_errno_set(CORO_BUS_ERR_NONE);
	return old_count;
}

void
coro_bus_channel_close(struct coro_bus *bus, int channel)
{
	if (!bus)
		return;
	

	/*
	 * Be very attentive here. What happens, if the channel is
	 * closed while there are coroutines waiting on it? For
	 * example, the channel was empty, and some coros were
	 * waiting on its recv_queue.
	 *
	 * If you wakeup those coroutines and just delete the
	 * channel right away, then those waiting coroutines might
	 * on wakeup try to reference invalid memory.
	 *
	 * Can happen, for example, if you use an intrusive list
	 * (rlist), delete the list itself (by deleting the
	 * channel), and then the coroutines on wakeup would try
	 * to remove themselves from the already destroyed list.
	 *
	 * Think how you could address that. Remove all the
	 * waiters from the list before freeing it? Yield this
	 * coroutine after waking up the waiters but before
	 * freeing the channel, so the waiters could safely leave?
	 */
}

int
coro_bus_send(struct coro_bus *bus, int channel, unsigned data)
{
	if (!bus)
		return -1;

	if (channel < 0 || channel >= bus->channel_count)
	{
		coro_bus_errno_set(CORO_BUS_ERR_NO_CHANNEL);
		return -1;
	}

	auto* ch = bus->channels[channel];
	if (!ch)
	{
		coro_bus_errno_set(CORO_BUS_ERR_NO_CHANNEL);
		return -1;
	}

	while (true)
	{
		if (ch->ring_buf.isFull())
			break;
		
		bool isDone = ch->ring_buf.push(data);

		if (!isDone)
		{
			coro_bus_errno_set(CORO_BUS_ERR_WOULD_BLOCK);
			return -1;
		}

		
	}

	/* IMPLEMENT THIS FUNCTION */
	(void)bus;
	(void)channel;
	(void)data;
	/*
	 * Try sending in a loop, until success. If error, then
	 * check which one is that. If 'wouldblock', then suspend
	 * this coroutine and try again when woken up.
	 *
	 * If see the channel has space, then wakeup the first
	 * coro in the send-queue. That is needed so when there is
	 * enough space for many messages, and many coroutines are
	 * waiting, they would then wake each other up one by one
	 * as lone as there is still space.
	 */
	coro_bus_errno_set(CORO_BUS_ERR_NOT_IMPLEMENTED);
	return -1;
}

int
coro_bus_try_send(struct coro_bus *bus, int channel, unsigned data)
{
	/* IMPLEMENT THIS FUNCTION */
	(void)bus;
	(void)channel;
	(void)data;
	/*
	 * Append data if has space. Otherwise 'wouldblock' error.
	 * Wakeup the first coro in the recv-queue! To let it know
	 * there is data.
	 */
	coro_bus_errno_set(CORO_BUS_ERR_NOT_IMPLEMENTED);
	return -1;
}

int
coro_bus_recv(struct coro_bus *bus, int channel, unsigned *data)
{
	/* IMPLEMENT THIS FUNCTION */
	(void)bus;
	(void)channel;
	(void)data;
	coro_bus_errno_set(CORO_BUS_ERR_NOT_IMPLEMENTED);
	return -1;
}

int
coro_bus_try_recv(struct coro_bus *bus, int channel, unsigned *data)
{
	/* IMPLEMENT THIS FUNCTION */
	(void)bus;
	(void)channel;
	(void)data;
	coro_bus_errno_set(CORO_BUS_ERR_NOT_IMPLEMENTED);
	return -1;
}


#if NEED_BROADCAST

int
coro_bus_broadcast(struct coro_bus *bus, unsigned data)
{
	/* IMPLEMENT THIS FUNCTION */
	(void)bus;
	(void)data;
	coro_bus_errno_set(CORO_BUS_ERR_NOT_IMPLEMENTED);
	return -1;
}

int
coro_bus_try_broadcast(struct coro_bus *bus, unsigned data)
{
	/* IMPLEMENT THIS FUNCTION */
	(void)bus;
	(void)data;
	coro_bus_errno_set(CORO_BUS_ERR_NOT_IMPLEMENTED);
	return -1;
}

#endif

#if NEED_BATCH

int
coro_bus_send_v(struct coro_bus *bus, int channel, const unsigned *data, unsigned count)
{
	/* IMPLEMENT THIS FUNCTION */
	(void)bus;
	(void)channel;
	(void)data;
	(void)count;
	coro_bus_errno_set(CORO_BUS_ERR_NOT_IMPLEMENTED);
	return -1;
}

int
coro_bus_try_send_v(struct coro_bus *bus, int channel, const unsigned *data, unsigned count)
{
	/* IMPLEMENT THIS FUNCTION */
	(void)bus;
	(void)channel;
	(void)data;
	(void)count;
	coro_bus_errno_set(CORO_BUS_ERR_NOT_IMPLEMENTED);
	return -1;
}

int
coro_bus_recv_v(struct coro_bus *bus, int channel, unsigned *data, unsigned capacity)
{
	/* IMPLEMENT THIS FUNCTION */
	(void)bus;
	(void)channel;
	(void)data;
	(void)capacity;
	coro_bus_errno_set(CORO_BUS_ERR_NOT_IMPLEMENTED);
	return -1;
}

int
coro_bus_try_recv_v(struct coro_bus *bus, int channel, unsigned *data, unsigned capacity)
{
	/* IMPLEMENT THIS FUNCTION */
	(void)bus;
	(void)channel;
	(void)data;
	(void)capacity;
	coro_bus_errno_set(CORO_BUS_ERR_NOT_IMPLEMENTED);
	return -1;
}

#endif
