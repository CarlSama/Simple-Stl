#ifndef _freeman_rbtree_h_
#define _freeman_rbtree_h_

#include "iterator.h"
#include "allocator.h"
#include "construct.h"
#include "cstddef"

namespace Freeman {

	typedef bool color_type;
	const color_type red = false;
	const color_type black = true;




	/*
	 * rb_tree_node_base -- (derive) --> rb_tree_node
	 */
	struct rb_tree_node_base {
		typedef rb_tree_node_base* node_base_ptr;

		color_type color;
		node_base_ptr parent,left,right;

		static node_base_ptr minimum(node_base_ptr x) {
			while(x->left)	x = x->left;
			return x;
		}
		static node_base_ptr maximum(node_base_ptr x) {
			while(x->right)	x = x->right;
			return x;
		}
	};

	template<typename Tp>
	struct rb_tree_node : public rb_tree_node_base {
		typedef rb_tree_node<Tp>* node_ptr;
		Tp value;
	};





	/*
	 * rb_tree_iterator_base -- (derive) -- > rb_tree_iterator
	 */
	struct rb_tree_iterator_base {
		typedef bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;

		rb_tree_node_base::node_base_ptr node_base_ptr_in_iter_base;

		// When iterator point to header, which means end, we should return end still
		// Because we use header as end, there would be a sensitive situation, 
		//		which is root doesn't have left node and iterator point to root
		void _incr() {
			if(node_base_ptr_in_iter_base->right) {
				node_base_ptr_in_iter_base = node_base_ptr_in_iter_base->right;
				while(node_base_ptr_in_iter_base->left)
					node_base_ptr_in_iter_base = node_base_ptr_in_iter_base->left;
			}else{
				rb_tree_node_base::node_base_ptr  parent = node_base_ptr_in_iter_base->parent;
				while(node_base_ptr_in_iter_base==node_base_ptr_in_iter_base->right){
					node_base_ptr_in_iter_base = parent;
					parent = parent->parent;
				}
				if(_node_base_ptr->right != parent)
					_node_base_ptr = parent;
			}
		}

		// Because of iterator point to end
		void _decr() {
			if(_node_base_ptr->color == red && _node_base_ptr->parent->parent == _node_base_ptr) {//iterator point to header node, which means end
				_node_base_ptr = _node_base_ptr->right;
			}else if(_node_base_ptr->left) {
				_node_base_ptr = _node_base_ptr->left;
				while(_node_base_ptr->right)
					_node_base_ptr = _node_base_ptr->right;
			}else{
				rb_tree_node_base::node_base_ptr parent = _node_base_ptr->parent;
				while(parent->right == _node_base_ptr) {
					_node_base_ptr = parent;
					parent = parent->parent;
				}
				_node_base_ptr = parent;
			}
		}
	}

	template<typename Tp,typename Ref,typename Ptr>
		struct rb_tree_iterator : public rb_tree_base_iterator {
			typedef Tp value_type;
			typedef Ref reference;
			typedef Ptr pointer;
			typedef rb_tree_iterator<Tp,Tp&,Tp*> iterator;
			typedef rb_tree_iterator<Tp,const Tp&,const Tp*> const_iterator;
			typedef rb_tree_iterator<Tp,Ref,Ptr> _self;

			typedef rb_tree_node<Tp>::node_ptr node_ptr;

			rb_tree_iterator {}
			rb_tree_iterator(link_type x) { _node_base_ptr = x; }
			rb_tree_iterator(const iterator& it) { node_base_ptr_in_iter_base = it.node_base_ptr_in_iter_base; }

			reference operator* () const { (node_ptr)node_base_ptr_in_iter_base->value; }
			pointer operator->() const { return &(operator*()); }
			_self& operator ++ () { _incr(); return *this; }
			_self& operator -- () { _decr(); return *this; }
			_self  operator ++ (int) {
				_self tmp = *this;
				_incr();
				return tmp;
			}
			_self operator -- (int) {
				_self tmp = *this;
				_decr();
				return tmp;
			}
		};











#endif
