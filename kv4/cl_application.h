#ifndef __CL_APPLICATION__H
#define __CL_APPLICATION__H

#include "cl_base.h"
#include "cl_2.h"
#include "cl_3.h"
#include "cl_4.h"
#include "cl_5.h"
#include "cl_6.h"


class cl_application : public cl_base {
public:
	cl_application(cl_base* p_head_object); // конструктор, создающий объект класса cl_app..
	int exec_app(); // метод запуска приложения
	void build_tree_objects(); // метод, создающий иерархию объекта

	void signal_f(string& msg);
	void handler_f(string msg);
	int get_class_num();
};

#endif
