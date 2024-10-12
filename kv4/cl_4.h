#ifndef __CL_4__H
#define __CL_4__H

#include "cl_base.h"


class cl_4 : public cl_base {
public:
	// конструктор, создающий объект класса cl_4
	cl_4(cl_base* p_head_object, string s_object_name);
	void signal_f(string& msg);
	void handler_f(string msg);
	int get_class_num();
};

#endif
