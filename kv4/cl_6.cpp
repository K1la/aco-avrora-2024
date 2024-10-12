#include "cl_6.h"

// вызов параметризированного конструктора класса cl_base с параметрами p_head_object и s_object_name
cl_6::cl_6(cl_base* p_head_object, string s_object_name) : cl_base(p_head_object, s_object_name) {}

int cl_6::get_class_num() {
	return 6;
}

void cl_6::signal_f(string& msg) {
	cout << endl << "Signal from " << get_abs_path();
	msg += " (class: 6)";
}
void cl_6::handler_f(string msg) {
	cout << endl << "Signal to " << get_abs_path() << " Text: " << msg;
}