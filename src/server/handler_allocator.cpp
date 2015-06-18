#include "server.h"

namespace streamx {

//
// handler_allocator.h
//

#include "handler_allocator.h"


HandlerAllocator::HandlerAllocator()
    : in_use_(false)
{
}

void* HandlerAllocator::allocate(std::size_t size) {
    if (!in_use_ && size < sizeof(storage_)){
        in_use_ = true;
        return &storage_;
    } else {
        return ::operator new(size);
    }
}

void HandlerAllocator::deallocate(void* pointer) {
    if (pointer == &storage_) {
        in_use_ = false;
    }
    else {
        ::operator delete(pointer);
    }
}


CustomAllocHandler::CustomAllocHandler(HandlerAllocator& a, Handler h)
    : allocator_(a), handler_(h)
{
}

template <typename ...Args>
void CustomAllocHandler::operator()(Args&&... args) {
    handler_(std::forward<Args>(args)...);
}

void* CustomAllocHandler::asio_handler_allocate(std::size_t size,
    CustomAllocHandler<Handler>* this_handler) {
    return this_handler->allocator_.allocate(size);
}

void CustomAllocHandler::asio_handler_deallocate(void* pointer, std::size_t /*size*/,
        CustomAllocHandler<Handler>* this_handler) {
    this_handler->allocator_.deallocate(pointer);
}

} // namespace streamx
