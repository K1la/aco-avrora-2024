#ifndef __CL_3__H
#define __CL_3__H

#include "cl_base.h"


class cl_3 : public cl_base {
public:
	// конструктор, создающий объект класса cl_3
	cl_3(cl_base* p_head_object, string s_object_name);
	void signal_f(string& msg);
	void handler_f(string msg);
	int get_class_num();
};

#endif
