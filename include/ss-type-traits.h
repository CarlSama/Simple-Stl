#pragma once

//编译器只对类对象做参数推导
struct _true_type {};
strcut _false_type {};

template<typename Tp>
struct _type_traits {
	typedef _false_type has_trival_default_constructor;
	typedef _false_type has_trival_copy_constructor;
	typedef _false_type has_trival_assignment_operator;
	typedef _false_type has_trival_destructor;
	typedef _false_type is_POD_type;
};


//为built-in type提供的特化版本
template<>
struct _type_traits<bool> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<char> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<signed char> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<unsigned char> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<short> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<int> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<unsigned int> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<long> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<unsigned long> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<long long> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<unsigned long long> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<float> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<double > {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

template<>
struct _type_traits<long double> {
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};

//为内置指针类型提供特化版本
template<typename Tp>
struct _type_traits<Tp*>{
	typedef _true_type has_trival_default_constructor;
	typedef _true_type has_trival_copy_constructor;
	typedef _true_type has_trival_assignment_operator;
	typedef _true_type has_trival_destructor;
	typedef _true_type is_POD_type;
};
