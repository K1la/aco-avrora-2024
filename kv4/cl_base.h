#ifndef __CL_BASE__H
#define __CL_BASE__H

#include <string>
#include <vector>
#include <iostream>
#include <queue>

class cl_base;
using namespace std;

#define SIGNAL_D(signal_f)(TYPE_SIGNAL)(&signal_f)
#define HANDLER_D(handler_f)(TYPE_HANDLER)(&handler_f)

typedef void (cl_base::* TYPE_SIGNAL)(string& msg);
typedef void (cl_base::* TYPE_HANDLER)(string msg);

struct o_sh {
	TYPE_SIGNAL p_signal; // ��������� �� ����� �������
	cl_base* p_target; // ��������� �� ������� ������
	TYPE_HANDLER p_handler; // ��������� �� ����� ����������
};

class cl_base {
private:
	string s_object_name; // ��� �������
	cl_base* p_head_object; // ��������� �� ������������ ������
	vector <cl_base*> subordinate_objects; // ������ ����������� ��������
	int status = 0; // ������ ��������� �������
	vector <o_sh*> connects; // ������ ���������� !!!!!!!!! (����� ������ *) !!!!!!!!!
public:
	cl_base(cl_base* p_head_object, string s_object_name = "Base object"); // ������������������� �����������
	string get_name(); // ����� ��������� �����
	cl_base* get_p_head(); // ����� ��������� ��������� �� ������������ ������
	bool set_name(string new_name); //����� �������������� ����� �������
	cl_base* get_subordinate_object(string search_name); // ��������� ��������� �� ��������������� ����������� ������ �� �����
	~cl_base(); // ����������

	cl_base* search_object_from_current(string); // ����� ������� �� ����� �������� �� �������� �� �����
	cl_base* search_object_from_tree(string); // ����� �� ������ (� �����) ��������
	void set_status(int status); // ����� ��������� ������� �������
	void print_tree(int layer = 0); // ����� ������ ������ ��������

	cl_base* find_obj_by_coord(string); // ����� ������ ������� �� �������� ����������
	//void delete_sub_obj(string); // ����� �������� ������������ ������� �� �����
	//bool set_head_obj(cl_base*); // ����� ����� ��������� ��������

	void set_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler); // ����� ��������� ���������� ����� ��������
	void delete_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler); // ����� �������� ���������� 
	void emit_signal(TYPE_SIGNAL p_signal, string s_msg); // ����� �������� ����������
	int get_status(); // ����� ��������� �������
	string get_abs_path(); // ����� ��������� ����������� ���� �������
	virtual int get_class_num() = 0; // ����������� �������. ���� ����� 0, �� ��� ���� � ������, ����� ���������� � ����������� �������
	void delete_links(cl_base* p_target); // ����� ������� �����, ������ � p_target - ������� ������
	void set_status_tree(int status); // ����� ��������� ������� � ���� � ������
};



#endif
