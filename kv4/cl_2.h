#ifndef __CL_2__H
#define __CL_2__H

#include "cl_base.h"


class cl_2 : public cl_base {
public:
	// конструктор, создающий объект класса cl_2
	cl_2(cl_base* p_head_object, string s_object_name);
	void signal_f(string& msg);
	void handler_f(string msg);
	int get_class_num();
};

#endif
