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
	TYPE_SIGNAL p_signal; // указатель на метод сигнала
	cl_base* p_target; // указатель на целевой объект
	TYPE_HANDLER p_handler; // указатель на метод обработчик
};

class cl_base {
private:
	string s_object_name; // имя объекта
	cl_base* p_head_object; // указатель на родительский объект
	vector <cl_base*> subordinate_objects; // вектор подчиненных объектов
	int status = 0; // статус состояния объекта
	vector <o_sh*> connects; // вектор соединений !!!!!!!!! (можно убрать *) !!!!!!!!!
public:
	cl_base(cl_base* p_head_object, string s_object_name = "Base object"); // параметризированный конструктор
	string get_name(); // метод получения имени
	cl_base* get_p_head(); // метод получения указателя на родительский объект
	bool set_name(string new_name); //метод редактирования имени объекта
	cl_base* get_subordinate_object(string search_name); // получение указателя на непосредственно подчиненный объект по имени
	~cl_base(); // деструктор

	cl_base* search_object_from_current(string); // поиск объекта на ветке иерархии от текущего по имени
	cl_base* search_object_from_tree(string); // поиск по дереву (в корне) иерархии
	void set_status(int status); // метод установки статуса объекта
	void print_tree(int layer = 0); // метод вывода дерева иерархии

	cl_base* find_obj_by_coord(string); // метод поиска объекта по заданной координате
	//void delete_sub_obj(string); // метод удаления подчиненного объекта по имени
	//bool set_head_obj(cl_base*); // метод смены головного элемента

	void set_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler); // метод установки соединений между объектов
	void delete_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler); // метод удаления соединения 
	void emit_signal(TYPE_SIGNAL p_signal, string s_msg); // метод проверки соединения
	int get_status(); // метод получения статуса
	string get_abs_path(); // метод получения абсолютного пути объекта
	virtual int get_class_num() = 0; // виртуальная функция. если равно 0, то нет тела у метода, метод реализован в производных классах
	void delete_links(cl_base* p_target); // метод удаляет свзяи, идущие к p_target - целевой объект
	void set_status_tree(int status); // метод установки статуса у всех в дереве
};



#endif
