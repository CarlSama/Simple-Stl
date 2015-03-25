#ifndef _memory_
#define _memory_

#include "stl_alloc.h"
#include "ss-constructor.h"

template<typename Tp>
class auto_ptr {
	private:
		Tp *_content;
	
	public:
		typedef Tp element_type;
		
		explicit auto_ptr(Tp *__p == nullptr) : _content(__p) {}
		auto_ptr(const auto_ptr& __ref) : _content(ref._content) {}

		
		~auot_ptr() {
			delete _content;
		}

		Tp& operator *(){
			return *_content;
		}

		Tp* operator ->(){
			return _content;
		}

		Tp* get() const{
			return _content;
		}

		Tp* release(){
			T* __tmp = _content;
			_content = nullptr;
			return __tmp;
		}

		void reset(Tp* __p = 0){
			if(__p != _content){
				delete _content;
				_content = __p;
			}
		}

		auto_ptr& operator = (auot_ptr& __ref){
			reset(__ref._content);
			return *this;
		}
};

#endif
