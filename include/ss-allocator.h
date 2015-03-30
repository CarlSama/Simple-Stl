#ifndef _ss_alloc_
#define _ss_alloc_

#include<new>
#include<cstddef>//ptrdiff_t
#include"ss-constructor.h"
#include<cstring>//memcpy

namespace Freeman {

	/*
	 * 迭代器规范接口
	 */
	template <typename T>
		class Allocator {
			protected:
				//设定_Hash_Alloc为默认的空间配置器
				_Hash_Alloc<T> alloc; 

			public:
				typedef size_t size_type;

				typedef T value_type;
				typedef T* pointer;
				typedef const T* const_pointer;
				typedef T& reference;
				typedef const T& const_reference;
				typedef ptrdiff_t difference_type;

				/*
				 * 辅助实现不能类型使用同一空间配置器
				 * list<int>中节点的类型为:
				 * template<typename T>
				 * struct node {
				 *		node* next;
				 *		T* val;
				 * };
				 * allocator<T>无法分配node对象,因为只能看到Ｔ
				 * 此时，使用allocator<T>::rebind<node<T> >::other
				 * 来表征要配置的对象node<T>
				 */ 
				template<class U>
					struct rebind{
						typedef Allocator<U> other;
					};

			public:
				Allocator() {}
				Allocator(const Allocator<T>& _ref)  {}
				template<typename U>
					Allocator(const allocator<U>&) {}

				~Allocator() {}

				pointer address(reference _x)const { return &_x; }

				const_pointer address(const_reference _x)const{ return &_x; }

				//申请n个对象的空间
				//To do : hint used for locality ?
				static T* allocate(size_type _n, const void* hint=0){
					try {
						if( _n < 0 )
							throw  invalid_argument("allocat in allocator");

						return (0 == _n) ? 0 : static_cast<T*>( alloc::allocate(n * sizeof(T)) );
					}catch (invalid_argument &ia){
						cerr << ia.what()<<endl;
					}
				}

				//释放ptr指向空间的n个对象
				static void deallocate(T* _ptr, size_type _n){
					try {
						if( _n <= 0　)
							throw invalid_argument("size to be realsed must be positive");

						alloc::deallocate(_ptr, _n * sizeof(T));
					}catch (invalid_argument &ia){
						cerr<<ia.what()<<endl;
					}
				}

				size_type max_size() const{
					/*
					 * the size_t means a unsigned object, 
					 * then -1 would be transformed to the biggest unsigned 
					 */
					return size_t(-1) / sizeof(T);
				}

				void construct(pointer _ptr, const T& _value){
					Construct<T,T>(_ptr, _value);
				}

				void destory(pointer _ptr){
					Destory(_ptr, _ptr+1);
				}
			};

	//特化版本
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
				std::cerr<<"Out of memory on heap"<<std::endl;
			}

		public:
			static void *allocate(size_t _n){
				void *_res = nullptr;
				std::set_new_handler(oom_handler);
				try{
					_res = ::operator new(_n);
					return _res;
				}catch(std::bad_alloc _ba){
					std::cerr<<"Out of memory on heap:"<<_ba.what()<<std::endl;
				}
			}

			static void deallocate(void *_ptr){
				::operator delete(_ptr);
			}

			static void *reallocate(void *_ptr,size_t _old_sz, size_t _new_sz){
				//How we manage this ?
				try {
					void * _res = ::operator new(_new_sz);

					delete _ptr;
					_ptr = _res;

					return _ptr;
				}catch (bad_alloc &ba){
					std::cerr<ba.what()<<std::endl;
			}
	};


	class _Hash_Alloc {
		private:
			static const int ALIGN = 8;
			static const int MAX_BYTES = 128;
			static const int NUM_FREELISTS = 16;//MAX_BYTES / ALIGN

			//调整到8的倍数
			inline static size_t make_aligned(size_t size);

		private:
			/*
			 * 第一个槽：因为是8字节对齐，所以低３位为０
			 */ 
			union obj {
				union obj *next;
				char data[1];
			};

			static obj * free_list[NUM_FREELISTS];

			//获取应当存储到的list索引
			static size_t get_index(size_t bytes);
			//申请内存,同时尝试申请额外内存以供备用
			static void *refill (size_t bytes);
			//从备用mem -> heap -> list中申请空间,可能会缩小num_blocks的数量
			static char *block_alloc(size_t bytes,int &num_blocks);

			//用于维护备用mem
			static char *start, *end;

		public:
			static void *allocate(size_t n);

			static void deallocate(void *p, size_t n);

			static void *reallocate(void *p,size_t old_sz,size_t new_sz);
	};

	char *_Hash_Alloc::start = nullptr;
	char *_Hash_Alloc::end = nullptr;
	_Hash_Alloc::obj * free_list[_Hash_Alloc::NUM_FREELISTS] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};

	static size_t _Hash_Alloc::make_aligned(size_t size){
		return (size + ALIGN - 1) & (~(ALIGN - 1));
	}

	static size_t _Hash_Alloc::get_index(size_t n){
		try{
			if(n)
				return ((n - 1) / ALIGN);
			else
				throw invalid_agument("invalid num");
		}catch(runtime_error &re){
			cerr<<re.what()<<endl;
		}
	}

	static void _Hash_Alloc::deallocate(void *p, size_t n){
		if(n > (sizt_t)MAX_BYTES){
			_New_alloc::deallocate(p);
		}else{
			obj * dest_list_ptr = list[get_index(n)];

			//将p插入倒链表头,并更新list中的头指针
			(static_cast<obj*>(p))->next = dest_list_ptr;
			dest_list_ptr = static_cast<obj*>(p);
		}
	}

	static void *_Hash_Alloc::allocate(size_t n){
		void *res = nullptr;

		//使用_new_alloc来处理较大内存的分配
		if(n > (size_t)MAX_BYTES){
			return _new_alloc::allocate(n);
		}else{
			obj * dest_list_ptr = list[get_index(n)];

			if( nullptr == *dest_list_ptr){ //没有可用的list
				res = refill(n);
			}else{
				res = dest_list_ptr;
				dest_list_ptr = res -> next;
			}
		}

		return res;
	}

	//申请空间,并且尝试获取备用的空间
	static void* _Hash_Alloc::refill(size_t bytes){
		bytes = make_aligned(bytes);//将n对齐

		int num_block = 20;
		char *mem = block_alloc(bytes, num_block);//申请空间

		if(num_block == 1){
			return blocks;
		}else{
			obj * dest_list_ptr = list[get_index(bytes)];//要插入的list中

			dest_list_ptr = static_cast<obj*>(mem + bytes);//从第二个开始

			obj* current_block;
			obj* next_block = (obj*)(mem + bytes);

			for(int i = 1 ; i < num_block - 1 ; ++i){
				current_block = next_block;
				next_block = static_cast<obj*>(next_block + bytes);

				current_block -> next = next_block;
			}
			next_block -> next = nullptr;

			return (obj*)mem;
		}
	}

	static char *_Hash_Alloc::block_alloc(size_t bytes,int &num_blocks){
		bytes = make_aligned(bytes);

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
				obj* dest_list_ptr = list[get_index(bytes)];
				static_cast<obj*>(start) -> next = dest_list_ptr;
				dest_list_ptr = static_cast<obj*>(start);
			}

			start = ::operator new(bytes_to_apply);
			if(nullptr == start){
				//heap空间不足
				obj * frontAvail;

				//从已分配的内存中尝试
				for(int i = bytes ;i <= MAX_BYTES ;i += ALIGN){
					obj *dest_list_ptr = list[make_aligned(i)];
					if(nullptr != dest_list_ptr){
						//有空间可用, 选取第一个来配置
						frontAvail = dest_list_ptr;
						dest_list_ptr = dest_list_ptr -> next;

						start = static_cast<char*>(frontAvail);
						end = start + i;
						return block_alloc(bytes,num_blocks);
					}
				}

				//尝试激活out-of-memory处理
				//end = 0;
				start = (char*)_New_Alloc::allocate(bytes_to_apply);
			}
			end = start + bytes_to_apply;
			return block_alloc(bytes, num_blocks);
		}
	}

	static void *_Hash_Alloc::reallocate(void *p,size_t old_sz,sizt_t new_sz){
		size_t copy_sz = 0;

		if(old_sz > (size_t)MAX_BYTES && new_sz > (size_t)MAX_BYTES){
			//对于大内存,直接调用_New_Alloc
			return _New_Alloc::reallocate(p,old_sz,new_sz);
		}

		if(make_aligned(old_sz) == make_align(new_sz)){
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
}

#endif
