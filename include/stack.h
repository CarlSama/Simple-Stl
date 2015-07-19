#ifndef _freeman_stack_h_
#define _freeman_stack_h_

namespace Freeman {

	/*
	 * the stack directly use other container, not implement by its own. We call this type of class Adapter
	 */
	template<typename Tp,typename SequenceContainer>
		class stack {
			protected:
				SequenceContainer scon;
			public:
				typedef typename SequenceContainer::value_type value_type;
				typedef typename SequenceContainer::size_type size_type;
				typedef typename SequenceContainer container_type;
				typedef typename SequenceContainer::reference reference;
				typedef typename SequenceContainer::const_reference const_reference;

			public:
				stack() : scon() {}
				stack(const SequenceContainer &s) : scon(s) {}

				bool empty() const { return scon.empty(); }
				size_type size() const { return scon.size(); }
				reference top() { return scon.back(); }
				const_reference top const {return scon.back();}

				void push(const value_type& v) { scon.push_back(v); }
				void pop() { scon.pop_back(); } 
		};
}





	

