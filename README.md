# SpanContainers

Provides a set of fixed length containers adapaters wrapping std::span, specifically:

* SpanQueue - provides FIFO access, via `push_back` and `front`/`pop_front`
* SpanStack - provides LIFO access, via `push_back` and `back`/`pop_back`
* SpanDequeue - provides FIFO and LIFO access, with push and pop access from both ends

Requiries C++20.

## Design Decisions/Features
The containers enforce their boundaries via exceptions and offer try alternatives for operations that might fail.

`push_back`/`push_front` and `back`/`front` were used over `top` or `enque` to make use with adaptors like `std::back_iterator`
easier.

No iterators are provided, as iterating is inherently mutating for these collections.

