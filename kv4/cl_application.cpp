#include "cl_application.h"

// вызов параметризированного конструктора класса cl_base с параметром p_head_object
cl_application::cl_application(cl_base* p_head_object) : cl_base(p_head_object) {}

int cl_application::get_class_num() {
	return 1;
}
void cl_application::signal_f(string& msg) {
	cout << endl << "Signal from " << get_abs_path();
	msg += " (class: 1)";
}
void cl_application::handler_f(string msg) {
	cout << endl << "Signal to " << get_abs_path() << " Text: " << msg;
}

void cl_application::build_tree_objects() {
	/*
	метод построения дерева иерархии объектов
	*/

	cout << "Object tree";
	//-------------------------------------------------------------
	string s_head_name, s_sub_name; // имя головного объекта, подчиненного объекта
	cl_base* p_head = this;
	cl_base* p_sub = nullptr;
	int class_number; // номер класса, номер состояния

	string s_sender, s_reciever; // отправитель, получатель сигнала
	cl_base* p_sender = nullptr;
	cl_base* p_reciever = nullptr;
	//-------------------------------------------------------------

	cin >> s_head_name;
	this->set_name(s_head_name);// вызов метода set_name этого объекта с параметром s_head_name 

	// ввод иерархии объектов
	cin >> s_head_name;
	while (s_head_name != "endtree") {
		cin >> s_sub_name >> class_number;
		p_head = find_obj_by_coord(s_head_name); //поиск головного объекта		

		if (p_head == nullptr) { // если головной объект ненулевой			
			print_tree();
			cout << endl << "The head object " << s_head_name << " is not found";
			exit(1);
		}
		if (p_head->get_subordinate_object(s_sub_name) != nullptr) { // если подчиненные объекты не равен пустому указателю
			cout << endl << s_head_name << "     Dubbing the names of subordinate objects";
		}
		else {
			switch (class_number) {
			case 1:
				p_sub = new cl_application(p_head);
				break;
			case 2:
				p_sub = new cl_2(p_head, s_sub_name);
				break;
			case 3:
				p_sub = new cl_3(p_head, s_sub_name);
				break;
			case 4:
				p_sub = new cl_4(p_head, s_sub_name);
				break;
			case 5:
				p_sub = new cl_5(p_head, s_sub_name);
				break;
			case 6:
				p_sub = new cl_6(p_head, s_sub_name);
				break;
			}
		}
		cin >> s_head_name;
	}
	// вектор элементов TYPE_SIGNAL хранит сигналы классов 1-6, указатель на метод signal_f
	vector<TYPE_SIGNAL> signals = { SIGNAL_D(cl_application::signal_f), SIGNAL_D(cl_2::signal_f), SIGNAL_D(cl_3::signal_f), SIGNAL_D(cl_4::signal_f), SIGNAL_D(cl_5::signal_f), SIGNAL_D(cl_6::signal_f) };
	// вектор элементов TYPE_HANDLER хранит обработчик классов 1-6, указатель на метод handler_f
	vector<TYPE_HANDLER> handlers = { HANDLER_D(cl_application::handler_f), HANDLER_D(cl_2::handler_f), HANDLER_D(cl_3::handler_f), HANDLER_D(cl_4::handler_f), HANDLER_D(cl_5::handler_f), HANDLER_D(cl_6::handler_f) };

	// установка соединений
	cin >> s_sender; // получаем путь текущего объекта, от которого будем отправлять сигнал
	while (s_sender != "end_of_connections") {
		p_sender = find_obj_by_coord(s_sender); // вызываем метод поиска объекта и присваиваем полученный путь к указателю

		if (p_sender == nullptr) {
			cout << endl << "Object " << s_sender << " not found";
			continue;
		}

		cin >> s_reciever;
		p_reciever = find_obj_by_coord(s_reciever); // вызываем метод поиска объекта и присваиваем полученный путь к указателю

		if (p_reciever == nullptr) {
			cout << endl << "Handler object " << s_reciever << " not found";
			continue;
		}
		// для объекта передающего сигнал вызываем метод установки соединения
		// передаем указатель на метод сигнала на объекте который посылает сигнал 
		// отнимаем 1, из-за индексов в векторе
		p_sender->set_connection(signals[p_sender->get_class_num() - 1], p_reciever, handlers[p_reciever->get_class_num() - 1]);

		cin >> s_sender;
	}
}

int cl_application::exec_app() {
	//---------------------------------------
	string s_command, s_coordinate, s_text; // команда, координата, текст 
	string s_sender, s_reciever; // отправитель, получатель
	cl_base* p_sender = nullptr;
	cl_base* p_reciever = nullptr;
	int object_status;

	// вектор элементов TYPE_SIGNAL хранит сигналы классов 1-6, указатель на метод signal_f
	vector<TYPE_SIGNAL> signals = { SIGNAL_D(cl_application::signal_f), SIGNAL_D(cl_2::signal_f), SIGNAL_D(cl_3::signal_f),
									SIGNAL_D(cl_4::signal_f), SIGNAL_D(cl_5::signal_f), SIGNAL_D(cl_6::signal_f) };
	// вектор элементов TYPE_HANDLER хранит обработчик классов 1-6, указатель на метод handler_f
	vector<TYPE_HANDLER> handlers = { HANDLER_D(cl_application::handler_f), HANDLER_D(cl_2::handler_f), HANDLER_D(cl_3::handler_f),
									HANDLER_D(cl_4::handler_f), HANDLER_D(cl_5::handler_f), HANDLER_D(cl_6::handler_f) };
	//-----------------------------------------
	this->set_status_tree(1); // установка состояний готовности для всех объектов

	this->print_tree();

	// цикл обработки комманд
	cin >> s_command;
	while (s_command != "END") {
		cin >> s_sender;
		p_sender = find_obj_by_coord(s_sender); // находим отправителя, с помощью метода поиска
		if (p_sender == nullptr) {
			cout << endl << "Object " << s_sender << " not found";
			cin >> s_sender;
			continue;
		}
		// |E|MIT
		if (s_command == "EMIT") {
			getline(cin, s_text);
			// передаем номер сигнала отправителя и текст
			p_sender->emit_signal(signals[p_sender->get_class_num() - 1], s_text);
		}
		// SET_CON|N|ECT
		else if (s_command[7] == 'N') {
			cin >> s_reciever;
			p_reciever = find_obj_by_coord(s_reciever); // находим получателя, с помощью метода поиска

			if (p_reciever == nullptr) {
				cout << endl << "Handler object " << s_reciever << " not found";
				continue;
			}
			else {
				// вызываем метод установки состояния
				p_sender->set_connection(signals[p_sender->get_class_num() - 1], p_reciever, handlers[p_reciever->get_class_num() - 1]);
			}
		}
		// |D|ELETE_CONNECT
		else if (s_command == "DELETE_CONNECT") {
			cin >> s_reciever;
			p_reciever = find_obj_by_coord(s_reciever); // находим получателя, с помощью метода поиска

			if (p_reciever == nullptr) {
				cout << endl << "Handler object " << s_reciever << " not found";
				continue;
			}
			else {
				// вызываем метод удаления состояния
				p_sender->delete_connection(signals[p_sender->get_class_num() - 1], p_reciever,
					handlers[p_reciever->get_class_num() - 1]);
			}
		}
		// SET_CON|D|ITION
		else if (s_command == "SET_CONDITION") {
			cin >> object_status;
			p_sender->set_status(object_status);
		}
		cin >> s_command;
	}
	return 0;
}
