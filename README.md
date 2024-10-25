# SpanContainers

Provides a set of fixed length containers wrapping `std::span`, specifically:

* SpanQueue - a circular buffer providing FIFO access, via `push_back` and `front`/`pop_front`, 
and indexed access via `at` and subscript.
* SpanStack - a linear buffer providng LIFO access, via `push_back` and `back`/`pop_back`, 
and indexed access via `at` and subscript.
* SpanDequeue - a circular buffer providing FIFO and LIFO access, with push and pop access from both ends
and indexed access via `at` and subscript.
* SpanHeap - a heap provding sorted retrival via undirected `push` and `back`/`pop_back`. No index access.

Requiries C++20.

## Design Decisions/Features
The container operators (`push`, `pop`, `front`, `back`, and subscript) enforce their boundaries via exceptions and 
offer non-throwing `try` prefixed alternatives. These exceptions are not designed to be recovered from (they all derive
from `std::logic_error`) but provide a universal way to gracefully fail if a boundary invariant is violated in release 
mode, since in the authors opinion, guarding against incorrect behavior is more important than performance in virtually 
all situtations. 

If you feel different, these methods throwing exceptions by defineing `SC_USE_EXCEPTIONS` false, at which
point bounds will only be guarded by assertions.

`push_back`/`push_front` and `back`/`front` were used over `top` or `enque` when either set of semantics would be 
apporprate to make use with adaptors like `std::back_iterator` easier. An undirected `push_iterator` is provided for 
use with `SpanHeap`. 

No iterators are provided, as iterating is inherently mutating for these collections.

