#ifndef __CL_5__H
#define __CL_5__H

#include "cl_base.h"


class cl_5 : public cl_base {
public:
	// конструктор, создающий объект класса cl_5
	cl_5(cl_base* p_head_object, string s_object_name);
	void signal_f(string& msg);
	void handler_f(string msg);
	int get_class_num();
};

#endif
