#include "cl_base.h"

cl_base::cl_base(cl_base* p_head_object, string s_object_name)
	: p_head_object(p_head_object), s_object_name(s_object_name) {
	/*
	параметризированный конструктор
	p_head_object - указатель на головной объект
	s_object_name - имя узла дерева
	*/

	//полю p_head_object этого объекта присваивается параметр p_head_object
	//полю s_object_name этого объекта присваивается параметр s_object_name

	// если p_head_object ненулевой, то в subordinate_objects добавляется указатель на этот объект
	if (p_head_object) {
		p_head_object->subordinate_objects.push_back(this);
	}
}
cl_base::~cl_base() {
	this->delete_links(this); // удаляем соединения
	// проходимся по каждому элементу subordinate_objects и удаляем его
	for (auto p_sub : subordinate_objects) {
		delete p_sub;
	}
}

bool cl_base::set_name(string new_name) {
	/*
	метод редактирования имени объекта
	new_name - новое имя узла дерева
	*/
	// проходимся по каждому элементу вектора указателей subordinate_objects объекта по указателю p_head_object
	// если он равен new_name, то возвращаем false
	if (p_head_object != nullptr) {
		for (auto p_sub : p_head_object->subordinate_objects) {
			if (p_sub->get_name() == new_name) {
				return false;
			}
		}
	}
	// полю s_object_name этого объекта присваивается new_name
	this->s_object_name = new_name;
	return true;
}

string cl_base::get_name() {
	// возвращаем s_object_name
	return this->s_object_name;
}

cl_base* cl_base::get_p_head() {
	// возвращаем p_head_object
	return this->p_head_object;
}

cl_base* cl_base::get_subordinate_object(string search_name) {
	/*
	получение указателя на непосредственно подчиненный объект по имени
	search_name - имя искомого объекта
	*/

	// проходимся по элементам subordinate_objects, если он равен search_name
	// возвращаем i-ый subordinate_objects
	for (auto p_sub : subordinate_objects) {
		if (p_sub->get_name() == search_name) {
			return p_sub;
		}
	}
	// иначе возвращается нулевой указатель
	return nullptr;
}



cl_base* cl_base::search_object_from_current(string s_name) {
	/*
	метод поиска объекта по имене в поддереве (в ветке) (обход графа в ширину)
	s_name - имя искомого объекта
	*/
	cl_base* p_found = nullptr; // указатель на объект, который был найден
	queue<cl_base*> q; // очередь элементов

	q.push(this); // добавляем в очередь текущий элемент

	// пока очередь не пустая
	while (!q.empty()) {
		cl_base* p_front = q.front(); // хранится указатель на наш объект

		q.pop(); // удаляем элемент из начала очереди, чтобы пройтись по всем элементам
		if (p_front->get_name() == s_name) { // если имя указателя на элемент в очереди совпадает с имкомым объектом
			if (p_found == nullptr) // указатель не пустой и объект не уникальный
				p_found = p_front; // присваиваем указатель на элемент в очереди
			else // нашли дубликат 
				return nullptr;
		}
		// добавляем дочерние элементы p_front в очередь
		for (auto p_sub : p_front->subordinate_objects) {
			q.push(p_sub);
		}
	}
	return p_found;
}

cl_base* cl_base::search_object_from_tree(string s_name) {
	/*
	поиск объекта по имене во всем дереве
	s_name - имя искомого объекта
	*/

	cl_base* p_root = this;
	// пока вышестоящий объект в дереве не пустой, поднимаемся по дереву
	while (p_root->get_p_head() != nullptr) {
		p_root = p_root->get_p_head();
	}

	return p_root->search_object_from_current(s_name);
}
void cl_base::print_tree(int layer) {
	/*
	метод вывода иерархии объектов (дерева/ветки) от текущего объекта
	layer - уровень на дереве иерархии
	*/

	// выводим layer-ое кол-во ' ' и имя объекта
	cout << endl << string(layer, ' ') << this->get_name();
	// проходимся по элемента subordinate_objects и вызываем рекурсию
	for (auto p_sub : subordinate_objects)
		p_sub->print_tree(layer + 4);

}

void cl_base::set_status(int status) {
	/*
	метод установки статуса объекта
	status - номер состояния
	*/

	// если значение status ненулевое у головного объекта
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
	метод поиска объекта по координате
	s_coord - координата искомого объекта
	*/

	cl_base* p_root = this; // указатель на текущий объект
	int i_slash_2 = 0; // хранит индекс 2-ого слэша
	string s_name = "";
	cl_base* p_obj;

	if (s_coord == "/") {
		//поднимаемся по корню дереву
		while (p_root->get_p_head() != nullptr) { // пока головной объект не равен пустому указателю
			p_root = p_root->get_p_head();
		}
		return p_root; // нашли корневой объект
	}
	if (s_coord == ".") {
		return this; // вовзращаем текущий объект, т.к. считаем, что метод вызван от текущего объекта
	}
	if (s_coord[0] == '/' && s_coord[1] == '/') { // второй символ строки равен / (/ '/' )
		return this->search_object_from_tree(s_coord.substr(2)); // текущему объекту вызывваем метод с парам. s_coord (начиная с 3 символа)
	}
	if (s_coord[0] == '.') {
		return this->search_object_from_current(s_coord.substr(1)); // текущему объекту вызывваем метод с парам. s_coord (начиная со 2 символа)
	}

	i_slash_2 = s_coord.find("/", 1);
	if (s_coord[0] == '/') {
		//поднимаемся по корню дереву
		while (p_root->get_p_head() != nullptr) { // пока головной объект не равен пустому указателю
			p_root = p_root->get_p_head();
		}

		if (i_slash_2 != -1) { // нашли индекс слэша 
			s_name = s_coord.substr(1, i_slash_2 - 1); // получили имя объекта, после слэша и до след. слэша
			p_obj = p_root->get_subordinate_object(s_name); // корневому объекту вызываем метод поиска подчиненных объектов с парам. s_name
			if (p_obj != nullptr) // если p_obj не равен нулевому указателю
				return p_obj->find_obj_by_coord(s_coord.substr(i_slash_2 + 1));
			else
				return p_obj;
		}
		else {
			s_name = s_coord.substr(1);
			return p_root->get_subordinate_object(s_name); // от корневого объекта ищем подчиненные
		}
	} // ob1/ob2/ob3 
	else {
		if (i_slash_2 != -1) { // если слэш был найден в s_coord
			s_name = s_coord.substr(0, i_slash_2); // получили имя объекта
			p_obj = this->get_subordinate_object(s_name); // текущему объекту вызываем метод с парам. s_name
			if (p_obj != nullptr) // если p_obj не равен нулевому указателю
				return p_obj->find_obj_by_coord(s_coord.substr(i_slash_2 + 1)); // рекурсивно вызываем этот метод, передавая строку, после / в s_coord
			else
				return p_obj;
		}
		else { // второго слэша не нашлось (ob3)
			return this->get_subordinate_object(s_coord); // от текущего объекта ищем подчиненные
		}
	}

	return nullptr;
}
/*
void cl_base::delete_sub_obj(string s_name) {
	//
	//метод удаления подчиненного объекта по имени
	//s_name - имя удаляемого объекта
	//

	// проходимся по подчиненным элементам
	for (int i = subordinate_objects.size() - 1; i >= 0; --i) {
		if (subordinate_objects[i]->get_name() == s_name) { // если подчиненный элемент равен имени удаляемого объекта
			subordinate_objects.erase(subordinate_objects.begin() + i);
			break;
		}
	}
}
*/

void cl_base::set_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler) {
	// новое соединение устанавливает

	o_sh* p_value; // новый указатель на элемент структуры 
	// проверка в векторе соединений для исключения повторного установления связи
	for (auto c : connects) {
		if (c->p_signal == p_signal && c->p_target == p_target && c->p_handler == p_handler)
			return; // выходим из метода, т.к есть такое соединение
	}
	p_value = new o_sh(); // вызываем структуру по умолчанию, создаст новый экземпляр структуры 

	// присваивание переменных
	p_value->p_signal = p_signal;
	p_value->p_target = p_target;
	p_value->p_handler = p_handler;

	connects.push_back(p_value); // добавляем p_value в вектор
}
void cl_base::delete_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler) {

	vector<o_sh*>::iterator p_it; // объявляем p_it типа итератор по элементам вектора o_sh
	// проходимся по элементам connects до (указателя на следующий элемент после последнего)
	for (p_it = connects.begin(); p_it != connects.end(); p_it++)
		// p_it (просто указатель) (*p_it) - получаем элемент, на который он указывает (разыменовали итератор)
		if ((*p_it)->p_signal == p_signal && (*p_it)->p_target == p_target && (*p_it)->p_handler == p_handler) {
			// удаляем соединение
			delete* p_it;
			p_it = connects.erase(p_it);
			p_it--;
		}
}
void cl_base::emit_signal(TYPE_SIGNAL p_signal, string s_msg) {
	if (this->get_status() == 0)
		return;

	(this->*p_signal)(s_msg); // от текущего объекта вызывается разыменованный метод p_signal в качестве аргумента получает s_msg
	//цикл по всем соединениям
	for (auto c : connects) {
		if (c->p_signal == p_signal) {
			cl_base* p_target = c->p_target; // объявляем p_target и сохраняем целевой объект из connects
			TYPE_HANDLER p_handler = c->p_handler; // объявляем p_handler и значение p_handler из connects
			// проверка целевого объекта на готовность
			if (p_target->get_status() != 0)
				(p_target->*p_handler)(s_msg); // объекту p_target вызываем разыменованный метод p_handler в качестве аргумента получает s_msg
		}
	}
}

string cl_base::get_abs_path() {
	string s_abs_path = "";
	cl_base* p_obj = this; // присваиваем указатель на текущий объект

	// пока головной объект не равен пустому указателю
	while (p_obj->get_p_head() != nullptr) {
		s_abs_path = "/" + p_obj->get_name() + s_abs_path; // записываем путь текущего объекта
		p_obj = p_obj->get_p_head(); // вызов геттера головного объекта
	}
	if (s_abs_path == "")
		s_abs_path = "/";

	return s_abs_path;
}

void cl_base::delete_links(cl_base* p_target) {
	// метод удаляет свзяи, идущие к p_target - целевой объект
	// из всех других элементов дерева

	// для текущего объекта удаляем соединения
	for (int i = 0; i < this->connects.size(); i++) {
		if (this->connects[i]->p_target == p_target) {
			delete this->connects[i];
			this->connects.erase(connects.begin() + 1);
			i--;
		}
	}
	// для всех векторов подчиненных вызываем этот метод
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