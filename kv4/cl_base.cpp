#include "cl_base.h"

cl_base::cl_base(cl_base* p_head_object, string s_object_name)
	: p_head_object(p_head_object), s_object_name(s_object_name) {
	/*
	������������������� �����������
	p_head_object - ��������� �� �������� ������
	s_object_name - ��� ���� ������
	*/

	//���� p_head_object ����� ������� ������������� �������� p_head_object
	//���� s_object_name ����� ������� ������������� �������� s_object_name

	// ���� p_head_object ���������, �� � subordinate_objects ����������� ��������� �� ���� ������
	if (p_head_object) {
		p_head_object->subordinate_objects.push_back(this);
	}
}
cl_base::~cl_base() {
	this->delete_links(this); // ������� ����������
	// ���������� �� ������� �������� subordinate_objects � ������� ���
	for (auto p_sub : subordinate_objects) {
		delete p_sub;
	}
}

bool cl_base::set_name(string new_name) {
	/*
	����� �������������� ����� �������
	new_name - ����� ��� ���� ������
	*/
	// ���������� �� ������� �������� ������� ���������� subordinate_objects ������� �� ��������� p_head_object
	// ���� �� ����� new_name, �� ���������� false
	if (p_head_object != nullptr) {
		for (auto p_sub : p_head_object->subordinate_objects) {
			if (p_sub->get_name() == new_name) {
				return false;
			}
		}
	}
	// ���� s_object_name ����� ������� ������������� new_name
	this->s_object_name = new_name;
	return true;
}

string cl_base::get_name() {
	// ���������� s_object_name
	return this->s_object_name;
}

cl_base* cl_base::get_p_head() {
	// ���������� p_head_object
	return this->p_head_object;
}

cl_base* cl_base::get_subordinate_object(string search_name) {
	/*
	��������� ��������� �� ��������������� ����������� ������ �� �����
	search_name - ��� �������� �������
	*/

	// ���������� �� ��������� subordinate_objects, ���� �� ����� search_name
	// ���������� i-�� subordinate_objects
	for (auto p_sub : subordinate_objects) {
		if (p_sub->get_name() == search_name) {
			return p_sub;
		}
	}
	// ����� ������������ ������� ���������
	return nullptr;
}



cl_base* cl_base::search_object_from_current(string s_name) {
	/*
	����� ������ ������� �� ����� � ��������� (� �����) (����� ����� � ������)
	s_name - ��� �������� �������
	*/
	cl_base* p_found = nullptr; // ��������� �� ������, ������� ��� ������
	queue<cl_base*> q; // ������� ���������

	q.push(this); // ��������� � ������� ������� �������

	// ���� ������� �� ������
	while (!q.empty()) {
		cl_base* p_front = q.front(); // �������� ��������� �� ��� ������

		q.pop(); // ������� ������� �� ������ �������, ����� �������� �� ���� ���������
		if (p_front->get_name() == s_name) { // ���� ��� ��������� �� ������� � ������� ��������� � ������� ��������
			if (p_found == nullptr) // ��������� �� ������ � ������ �� ����������
				p_found = p_front; // ����������� ��������� �� ������� � �������
			else // ����� �������� 
				return nullptr;
		}
		// ��������� �������� �������� p_front � �������
		for (auto p_sub : p_front->subordinate_objects) {
			q.push(p_sub);
		}
	}
	return p_found;
}

cl_base* cl_base::search_object_from_tree(string s_name) {
	/*
	����� ������� �� ����� �� ���� ������
	s_name - ��� �������� �������
	*/

	cl_base* p_root = this;
	// ���� ����������� ������ � ������ �� ������, ����������� �� ������
	while (p_root->get_p_head() != nullptr) {
		p_root = p_root->get_p_head();
	}

	return p_root->search_object_from_current(s_name);
}
void cl_base::print_tree(int layer) {
	/*
	����� ������ �������� �������� (������/�����) �� �������� �������
	layer - ������� �� ������ ��������
	*/

	// ������� layer-�� ���-�� ' ' � ��� �������
	cout << endl << string(layer, ' ') << this->get_name();
	// ���������� �� �������� subordinate_objects � �������� ��������
	for (auto p_sub : subordinate_objects)
		p_sub->print_tree(layer + 4);

}

void cl_base::set_status(int status) {
	/*
	����� ��������� ������� �������
	status - ����� ���������
	*/

	// ���� �������� status ��������� � ��������� �������
	if (p_head_object == nullptr || p_head_object->status != 0) {
		this->status = status;
	}
	if (status == 0) {
		this->status = status;
		for (int i = 0; i < subordinate_objects.size(); i++) {
			subordinate_objects[i]->set_status(status);
		}
	}
}

int cl_base::get_status() {
	return this->status;
}

cl_base* cl_base::find_obj_by_coord(string s_coord) {
	/*
	����� ������ ������� �� ����������
	s_coord - ���������� �������� �������
	*/

	cl_base* p_root = this; // ��������� �� ������� ������
	int i_slash_2 = 0; // ������ ������ 2-��� �����
	string s_name = "";
	cl_base* p_obj;

	if (s_coord == "/") {
		//����������� �� ����� ������
		while (p_root->get_p_head() != nullptr) { // ���� �������� ������ �� ����� ������� ���������
			p_root = p_root->get_p_head();
		}
		return p_root; // ����� �������� ������
	}
	if (s_coord == ".") {
		return this; // ���������� ������� ������, �.�. �������, ��� ����� ������ �� �������� �������
	}
	if (s_coord[0] == '/' && s_coord[1] == '/') { // ������ ������ ������ ����� / (/ '/' )
		return this->search_object_from_tree(s_coord.substr(2)); // �������� ������� ��������� ����� � �����. s_coord (������� � 3 �������)
	}
	if (s_coord[0] == '.') {
		return this->search_object_from_current(s_coord.substr(1)); // �������� ������� ��������� ����� � �����. s_coord (������� �� 2 �������)
	}

	i_slash_2 = s_coord.find("/", 1);
	if (s_coord[0] == '/') {
		//����������� �� ����� ������
		while (p_root->get_p_head() != nullptr) { // ���� �������� ������ �� ����� ������� ���������
			p_root = p_root->get_p_head();
		}

		if (i_slash_2 != -1) { // ����� ������ ����� 
			s_name = s_coord.substr(1, i_slash_2 - 1); // �������� ��� �������, ����� ����� � �� ����. �����
			p_obj = p_root->get_subordinate_object(s_name); // ��������� ������� �������� ����� ������ ����������� �������� � �����. s_name
			if (p_obj != nullptr) // ���� p_obj �� ����� �������� ���������
				return p_obj->find_obj_by_coord(s_coord.substr(i_slash_2 + 1));
			else
				return p_obj;
		}
		else {
			s_name = s_coord.substr(1);
			return p_root->get_subordinate_object(s_name); // �� ��������� ������� ���� �����������
		}
	} // ob1/ob2/ob3 
	else {
		if (i_slash_2 != -1) { // ���� ���� ��� ������ � s_coord
			s_name = s_coord.substr(0, i_slash_2); // �������� ��� �������
			p_obj = this->get_subordinate_object(s_name); // �������� ������� �������� ����� � �����. s_name
			if (p_obj != nullptr) // ���� p_obj �� ����� �������� ���������
				return p_obj->find_obj_by_coord(s_coord.substr(i_slash_2 + 1)); // ���������� �������� ���� �����, ��������� ������, ����� / � s_coord
			else
				return p_obj;
		}
		else { // ������� ����� �� ������� (ob3)
			return this->get_subordinate_object(s_coord); // �� �������� ������� ���� �����������
		}
	}

	return nullptr;
}
/*
void cl_base::delete_sub_obj(string s_name) {
	//
	//����� �������� ������������ ������� �� �����
	//s_name - ��� ���������� �������
	//

	// ���������� �� ����������� ���������
	for (int i = subordinate_objects.size() - 1; i >= 0; --i) {
		if (subordinate_objects[i]->get_name() == s_name) { // ���� ����������� ������� ����� ����� ���������� �������
			subordinate_objects.erase(subordinate_objects.begin() + i);
			break;
		}
	}
}
*/

void cl_base::set_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler) {
	// ����� ���������� �������������

	o_sh* p_value; // ����� ��������� �� ������� ��������� 
	// �������� � ������� ���������� ��� ���������� ���������� ������������ �����
	for (auto c : connects) {
		if (c->p_signal == p_signal && c->p_target == p_target && c->p_handler == p_handler)
			return; // ������� �� ������, �.� ���� ����� ����������
	}
	p_value = new o_sh(); // �������� ��������� �� ���������, ������� ����� ��������� ��������� 

	// ������������ ����������
	p_value->p_signal = p_signal;
	p_value->p_target = p_target;
	p_value->p_handler = p_handler;

	connects.push_back(p_value); // ��������� p_value � ������
}
void cl_base::delete_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler) {

	vector<o_sh*>::iterator p_it; // ��������� p_it ���� �������� �� ��������� ������� o_sh
	// ���������� �� ��������� connects �� (��������� �� ��������� ������� ����� ����������)
	for (p_it = connects.begin(); p_it != connects.end(); p_it++)
		// p_it (������ ���������) (*p_it) - �������� �������, �� ������� �� ��������� (������������ ��������)
		if ((*p_it)->p_signal == p_signal && (*p_it)->p_target == p_target && (*p_it)->p_handler == p_handler) {
			// ������� ����������
			delete* p_it;
			p_it = connects.erase(p_it);
			p_it--;
		}
}
void cl_base::emit_signal(TYPE_SIGNAL p_signal, string s_msg) {
	if (this->get_status() == 0)
		return;

	(this->*p_signal)(s_msg); // �� �������� ������� ���������� �������������� ����� p_signal � �������� ��������� �������� s_msg
	//���� �� ���� �����������
	for (auto c : connects) {
		if (c->p_signal == p_signal) {
			cl_base* p_target = c->p_target; // ��������� p_target � ��������� ������� ������ �� connects
			TYPE_HANDLER p_handler = c->p_handler; // ��������� p_handler � �������� p_handler �� connects
			// �������� �������� ������� �� ����������
			if (p_target->get_status() != 0)
				(p_target->*p_handler)(s_msg); // ������� p_target �������� �������������� ����� p_handler � �������� ��������� �������� s_msg
		}
	}
}

string cl_base::get_abs_path() {
	string s_abs_path = "";
	cl_base* p_obj = this; // ����������� ��������� �� ������� ������

	// ���� �������� ������ �� ����� ������� ���������
	while (p_obj->get_p_head() != nullptr) {
		s_abs_path = "/" + p_obj->get_name() + s_abs_path; // ���������� ���� �������� �������
		p_obj = p_obj->get_p_head(); // ����� ������� ��������� �������
	}
	if (s_abs_path == "")
		s_abs_path = "/";

	return s_abs_path;
}

void cl_base::delete_links(cl_base* p_target) {
	// ����� ������� �����, ������ � p_target - ������� ������
	// �� ���� ������ ��������� ������

	// ��� �������� ������� ������� ����������
	for (int i = 0; i < this->connects.size(); i++) {
		if (this->connects[i]->p_target == p_target) {
			delete this->connects[i];
			this->connects.erase(connects.begin() + 1);
			i--;
		}
	}
	// ��� ���� �������� ����������� �������� ���� �����
	for (auto sub : subordinate_objects) {
		sub->delete_links(p_target);
	}
}
void cl_base::set_status_tree(int status) {
	if (get_p_head() != nullptr && get_p_head()->status == 0)
		return;
	set_status(status);
	for (auto sub : subordinate_objects) {
		sub->set_status_tree(status);
	}
}