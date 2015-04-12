#ifndef _temp_buffer_h_
#define _temp_buffer_h_

#include <cstddef>//ptrdiff_t
#include <new>
#include "type-traits.h"
#include <climits>

namespace Freeman {

	template<typename ForwardIterator,typename Tp>
		class Temporary_buffer {
			private:
				ptrdiff_t original_len;
				ptrdiff_t len;
				Tp *buffer;

			public:
				ptrdiff_t size() const { return len; }
				ptrdiff_t requested_size() const { return original_len; }
				Tp* begin() { return buffer; }
				Tp* end() { return buffer + len; }

			public:
				Temporary_buffer(ForwardIterator begin,ForwardIterator end) {
					original_len = len;
					buffer = (Tp*)(nullptr);

					if( len > (ptrdiff_t)(INT_MAX/sizeof(Tp)) ) {
						len = (ptrdiff_t)(INT_MAX/sizeof(Tp));
					}

					while(len > 0) {
						buffer =  new(std::nothrow) Tp[len];
						if( buffer)
							break;
						len = len / 2;
					}
				}

				~Temporary_buffer() {
					delete [] buffer;
				}

				Temporary_buffer& operator = (const Temporary_buffer& ) = delete;

		};
}


#endif
