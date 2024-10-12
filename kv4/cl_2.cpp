#include "cl_2.h"

// вызов параметризированного конструктора класса cl_base с параметрами p_head_object и s_object_name
cl_2::cl_2(cl_base* p_head_object, string s_object_name) : cl_base(p_head_object, s_object_name) {}

int cl_2::get_class_num() {
	return 2;
}

void cl_2::signal_f(string& msg) {
	cout << endl << "Signal from " << get_abs_path();
	msg += " (class: 2)";
}
void cl_2::handler_f(string msg) {
	cout << endl << "Signal to " << get_abs_path() << " Text: " << msg;
}