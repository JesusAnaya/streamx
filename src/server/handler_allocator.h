//
// handler_allocator.h
//

#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// Class to manage the memory to be used for handler-based custom allocation.
// It contains a single block of memory which may be returned for allocation
// requests. If the memory is in use when an allocation request is made, the
// allocator delegates allocation to the global heap.
namespace streamx {


class HandlerAllocator {
public:
    HandlerAllocator();

    HandlerAllocator(const HandlerAllocator&) = delete;
    HandlerAllocator& operator=(const HandlerAllocator&) = delete;

    void* allocate(std::size_t size);
    void deallocate(void* pointer);

private:
    // Storage space used for handler-based custom memory allocation.
    typename std::aligned_storage<1024>::type storage_;

    // Whether the handler-based custom allocation storage has been used.
    bool in_use_;
};


// Wrapper class template for handler objects to allow handler memory
// allocation to be customised. Calls to operator() are forwarded to the
// encapsulated handler.
template <typename Handler>
class CustomAllocHandler {
public:
    CustomAllocHandler(HandlerAllocator& a, Handler h);

    template <typename ...Args>
    void operator()(Args&&... args);

    friend void* asio_handler_allocate(std::size_t size, CustomAllocHandler<Handler>* this_handler);
    friend void asio_handler_deallocate(void* pointer, std::size_t, CustomAllocHandler<Handler>* this_handler);

private:
    HandlerAllocator& allocator_;
    Handler handler_;
};

// Helper function to wrap a handler object to add custom allocation.
template <typename Handler>
inline CustomAllocHandler<Handler> make_custom_alloc_handler(
        HandlerAllocator& a, Handler h) {
    return CustomAllocHandler<Handler>(a, h);
}

} // namespace streamx
