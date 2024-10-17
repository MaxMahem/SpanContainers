# SpanContainers

Provides a set of fixed length containers adapaters wrapping `std::span`, specifically:

* SpanQueue - provides FIFO access, via `push_back` and `front`/`pop_front`
* SpanStack - provides LIFO access, via `push_back` and `back`/`pop_back`
* SpanDequeue - provides FIFO and LIFO access, with push and pop access from both ends.
* SpanHeap - provdes sorted retrival via undirected `push` and `back`/`pop_back`

Requiries C++20.

## Design Decisions/Features
The default operators (push/pop/and get) enforce their boundaries via exceptions and offer non-throwing `try` 
alternatives. These exceptions are not really designed to be recovered from (they all derive from `std::logic_error`) 
but provide a universal way to gracefully fail if an invariant is failed in release mode, since in the authors opinion,
guarding against incorrect behavior is more important than performance in virtually all situtations. 

If you feel different, unguarded operators are possible via `unsafe` variants which guard only via assertions.

`push_back`/`push_front` and `back`/`front` were used over `top` or `enque` when either set of semantics would be 
apporprate to make use with adaptors like `std::back_iterator` easier. An undirected `push_iterator` was provided for 
use with `SpanHeap`. 

No iterators are provided, as iterating is inherently mutating for these collections.

