#pragma once

#include<new>

namespace Freeman {
	//封装

	template <typename T>
	class allocator {
		typedef allocator_to_use Alloc;
		public:
			typedef size_t size_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			typedef ptrdiff_t difference_type;
			typedef T& reference;
			typedef const T& const_reference;
			typedef T value_type;

			template<class U>
				struct rebint{
					typedef allocator<U> other;
				};

			allocator() {}
			allocator(const allocator& ref)  {}
			template<typename U>
				allocator(const allocator<U>&) {}
			~allocator() {}

			pointer address(reference x)const {
				return &x;
			}

			const_pointer address(const_reference x)const{
				return &x;
			}

			//申请n个对象的空间(默认为1个)
			static T* allocate(size_t n){
				return (0==n) ? 0 : static_cast<T*>( Alloc::allocate(n * sizeof(T)) );
			}
			
			static T* allocate(){
				return static_cast<T*>( Alloc::allocator(sizeof(T)) );
			}

			//释放ptr指向空间的n个对象
			static void deallocate(T* ptr, size_t n){
				if(n){
					Alloc::deallocate(ptr, n * sizeof(T));
				}
			}

			static void deallocate(T* ptr){
				Alloc::deallocate(ptr, sizeof(T));
			}

			size_type max_size() const{
				return size_t(-1) / sizeof(T);
			}

			void construct(pointer p, const T& value){
				new(p) T(val);
			}

			void destory(pointer p){
				p->~T();
			}
	};

	template<>
	class allocator<void> {
		public:
			typedef size_t size_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			typedef ptrdiff_t difference_type;
			typedef T& reference;
			typedef const T& const_reference;
			typedef T value_type;

			template<class U>
				struct rebint{
					typedef allocator<U> other;
				};
	};

	class _New_Alloc {
		private:
			//处理out of memory
			void oom_handler(){
				std::cerr<<"Out of memory on heap"<<endl;
			}

		public:
			static void *allocate(size_t n){
				void *res = nullptr;
				std::set_new_handler(oom_handler);
				try{
					res = ::operator new[n];
				}catch(std::bad_alloc ba){
					cerr<<"Out of memory :"<<ba.what()<<endl;
				}
				return res;
			}

			static void deallocate(void *p){
				::operator delete(p);
			}

			static void *reallocate(void *p,size_t old_size,size_t new_size){
				//How we manage this ?
			}
	};


	class _Hash_Alloc {
		private:
			static const int ALIGN = 8;
			static const int MAX_BYTES = 128;
			static const int NUM_FREELISTS = 16;//MAX_BYTES / ALIGN

			//调整到8的倍数
			inline static size_t make_align(size_t size);

		private:
			union obj {
				union obj *next;
				char data[1];
			};

			static obj * volatile list[NUM_FREELISTS];

			//获取应当存储到的list索引
			static size_t get_index(size_t bytes);
			//申请内存,同时尝试申请额外内存以供备用
			static void *refill (size_t bytes);
			//从备用mem -> heap -> list中申请空间,可能会缩小num_blocks的数量
			static char *chunk_alloc(size_t bytes,int &num_blocks);

			//用于维护备用mem
			static char *start, *end;

		public:
			static void *allocate(size_t n);

			static void deallocate(void *p, size_t n);

			static void *reallocate(void *p,size_t old_sz,size_t new_sz);
	};

	char *_Hash_Alloc::start = nullptr;
	char *_Hash_Alloc::end = nullptr;
	_Hash_Alloc::obj * list[_Hash_Alloc::NUM_FREELISTS] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};

	//将_Hash_Alloc设为默认内存分配器
	typedef _Hash_Alloc alloc;

	static size_t _Hash_Allo::make_align(size_t size){
		return (size + ALIGN - 1) & (~(ALIGN - 1));
	}

	static size_t _Hash_Allo::cget_index(size_t num){
		try{
			if(num)
				return ((num + ALIGN - 1) / ALIGN) - 1;
			else
				throw runtime_error("invalid num");
		}catch(runtime_error &re){
			cerr<<re.what()<<endl;
		}
	}

	static void *_Hash_Alloc::allocate(size_t n){
		void *res = nullptr;

		//使用_new_alloc来处理较大内存的分配
		if(n > (size_t)MAX_BYTES){
			return _new_alloc::allocate(n);
		}else{
			obj ** dest_list = list + get_index(n);

			obj * __result = *dest_list;
			if( nullptr == *__result){
				//没有可用的list
				res = refill(n);
			}else{
				*dest_list = __result -> next;
				re = __result;
			}
		}

		return res;
	}

	static void _Hash_Alloc::deallocate(void *p, size_t n){
		if(n > (sizt_t)MAX_BYTES){
			_new_alloc::deallocate(p);
		}else{
			obj ** dest_list = list + get_index(n);

			//将p插入倒链表头,并更新list中的头指针
			(obj*)(p)->next = *dest_list;
			*dest_list = (obj*)(p);
		}
	}

	//申请空间,并且尝试获取备用的空间
	static void* _Hash_Alloc::refill(size_t bytes){
		bytes = make_align(bytes);//将n对齐

		int num_block = 20;
		char *mem = block_alloc(bytes, num_block);//申请空间

		if(num_block == 1){
			return blocks;
		}else{
			obj ** dest_list = list + get_index(bytes);//要插入的list中
			
	//		obj * little_block = (obj*)blocks;
			*dest_list = (obj*)(mem + bytes);//从第二个开始

			obj* current_block;
			obj* next_block = (obj*)(mem + bytes);

			for(int i = 1;i<num_block;i ++){
				current_block = next_block;
				next_block = (obj*)(next_block + bytes);

				current_block -> next = next_block;
			}
			current_block -> next = nullptr;

			return (obj*)mem;
		}
	}

	static char *_Hash_Alloc::chunk_alloc(size_t bytes,int &num_blocks){
		bytes = make_align(bytes);

		size_t bytes_total_need = bytes * num_blocks;
		size_t bytes_remain = end - start;

		char *mem = nullptr;
		if(bytes_total_need >= bytes_remain){
			//内存能满足全部需求
			mem = start;
			start += bytes_total_need;
			return mem;
		}else(bytes_remain >= bytes){
			//内存起码能满足一个需求
			num_blocks = bytes_remain / bytes;	
			mem = start;
			start += num_blocks * bytes;
			return mem;
		}else{
			//依据需要的内存来进行申请
			size_t bytes_to_apply = bytes_total_need << 1 ;	

			//将剩余空间放置到list中
			if(bytes_remain > 0){
				obj** dest_list = list + get_index(bytes);
				(obj*)start -> next = *dest_list;
				*dest_list = (obj*)start;
			}

			start = new char[bytes_to_apply];
			if(nullptr == start){
				//heap空间不足
				obj ** dest_list;
				obj * p;

				//从已分配的内存中尝试
				for(int i = bytes ;i <= MAX_BYTES ;i += ALIGN){
					dest_list = list + make_align(i);
					p = *dest_list;
					if(nullptr != p){
						//有空间可用, 选取第一个来配置
						*dest_list = p -> next;

						start = (char*)p;
						end = start + i;
						return chunk_alloc(bytes,num_blocks);
					}
				}

				//尝试激活out-of-memory处理
				end = 0;
				start = (char*)_New_Alloc::allocate(bytes_to_apply);
			}
			
			end = start + bytes_to_apply;

			return chunk_alloc(bytes, num_blocks);
		}
	}

	static void *_Hash_Alloc::reallocate(void *p,size_t old_sz,sizt_t new_sz){
		size_t copy_sz = 0;
		
		if(old_sz > (size_t)MAX_BYTES && new_sz > (size_t)MAX_BYTES){
			//对于大内存,直接调用_New_Alloc
			return _New_Alloc::reallocate(p,old_sz,new_sz);
		}

		if(make_align(old_sz) == make_align(new_sz)){
			return p;
		}

		//分配空间并搬运
		void *res = allocate(new_sz);
		copy_sz = new_sz > old_sz ? old_sz : new_sz;// 注意:只能搬运最小值
		memcpy(res, p, copy_sz);

		//释放原有空间
		deallocate(p,old_sz);

		return res;
	}
};
