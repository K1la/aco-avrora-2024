#include "cl_5.h"

// вызов параметризированного конструктора класса cl_base с параметрами p_head_object и s_object_name
cl_5::cl_5(cl_base* p_head_object, string s_object_name) : cl_base(p_head_object, s_object_name) {}

int cl_5::get_class_num() {
	return 5;
}

void cl_5::signal_f(string& msg) {
	cout << endl << "Signal from " << get_abs_path();
	msg += " (class: 5)";
}
void cl_5::handler_f(string msg) {
	cout << endl << "Signal to " << get_abs_path() << " Text: " << msg;
}