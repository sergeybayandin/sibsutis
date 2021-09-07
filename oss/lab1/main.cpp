#include <iostream>
#include <fstream>
#include <unistd.h>
#include <limits>
#include <cstring>

template <class T>
class double_linked_list final
{
public:
	using size_type = std::size_t;
	double_linked_list() = default;
	~double_linked_list();
	void insert(const T&, size_type);
	void erase(size_type);
	T& operator[](size_type) noexcept;
	size_type size() const noexcept {return _size;}
	void clear();
private:
	struct __node__ {
		T value;
		__node__* next, *prev;
		__node__(const T& value, __node__* next, __node__* prev)
			: value{value}, next{next}, prev{prev} {}
	};

	__node__* _head, *_tail;
	size_type _size {};

	__node__* _get_node_starts_from(__node__*, size_type, auto) const noexcept;
};

template <class T>
typename double_linked_list<T>::__node__* 
double_linked_list<T>::_get_node_starts_from(__node__* node, size_type n, auto iterate) const noexcept
{
	while (n--)
		iterate(node);
	return node;	
}

template <class T>
double_linked_list<T>::~double_linked_list()
{
	while (_head) {
		auto new_head {_head->next};
		delete _head;
		_head = new_head;
	}
}

template <class T>
T& double_linked_list<T>::operator[](size_type pos) noexcept
{
	return (pos > _size / 2 
		? _get_node_starts_from(_tail, _size - pos - 1, [](auto& node){node = node->prev;}) 
			: _get_node_starts_from(_head, pos, [](auto& node){node = node->next;}))->value;
}

template <class T>
void double_linked_list<T>::insert(const T& value, size_type pos)
{
	if (pos > _size)
		return;
	if (!pos) {
		_head = _size ? _head->prev = new __node__(value, _head, nullptr)
			: _tail = new __node__(value, nullptr, nullptr);
	} else if (pos == _size) {
		_tail = _tail->next = new __node__(value, nullptr, _tail);
	} else {
		auto node {pos > _size / 2 
			? _get_node_starts_from(_tail, _size - pos - 1, [](auto& node){node = node->prev;})
				: _get_node_starts_from(_head, pos, [](auto& node){node = node->next;})};
		auto prev_node {node->prev};
		auto new_node {new __node__(value, node, prev_node)};
		prev_node->next = new_node;
		node->prev = new_node;
	}
	++_size;
}

template <class T>
void double_linked_list<T>::erase(size_type pos)
{
	if (pos >= _size)
		return;
	if (!pos) {
		auto new_head {_head->next};
		delete _head;
		_head = new_head;
		if (_head)
			_head->prev = nullptr;
		else
			_tail = _head;
	} else if (pos == _size - 1) {
		auto new_tail {_tail->prev};
		delete _tail;
		_tail = new_tail;
		if (_tail)
			_tail->next = nullptr;
		else
			_head = _tail;
	} else {
		auto node {pos > _size / 2 
			? _get_node_starts_from(_tail, _size - pos - 1, [](auto& node){node = node->prev;})
				: _get_node_starts_from(_head, pos, [](auto& node){node = node->next;})};
		auto next {node->next}, prev {node->prev};
		delete node;
		prev->next = next;
		next->prev = prev;
	}
	--_size;
}

template <class T>
void double_linked_list<T>::clear()
{
	while (_size)
		erase(0);
}

int main()
{
	using student_info = std::pair<char[30], int>;
	double_linked_list<student_info> l;
	while (true) {
		system("clear");
		std::cout << "1 - Создать новый список\n";
		std::cout << "2 - Просмотреть список\n";
		std::cout << "3 - Поиск в списке\n";
		std::cout << "4 - Cохранить списов в файл\n";
		std::cout << "5 - Загрузить список из файла\n";
		std::cout << "0 - Выход\n";
		char button; std::cin >> button;
		std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		system("clear");
		switch (button) {
			case '0' : {
				return EXIT_SUCCESS;
			} case '1' : {
				l.clear();
				std::cout << "Введите количество: ";
				std::size_t n; std::cin >> n;
				student_info si;
				for (std::size_t i {} ; n--; ++i) {
					std::cin >> si.first >> si.second;
					l.insert(si, i);
				}
				break;
			} case '2' : {
				for (std::size_t i {}; i < l.size(); ++i) {
					const auto& [name, mark] {l[i]};
					std::cout << i + 1 << ") "  << name << '\t' << mark << '\n';
				}
				std::cin.get();
				break;
			}	case '3' : {
				bool ran {true};
				while (ran) {
					system("clear");
					std::cout << "1 - По имени\n";
					std::cout << "2 - По номеру\n";
					std::cout << "0 - Выход\n";
					char button; std::cin >> button;		
					std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					system("clear");
					switch (button) {
						case '0' : {
							ran = false;
							break;
						} case '1' : {
							std::cout << "Введите имя: ";
							typename student_info::first_type name {}; std::cin >> name;
							std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							typename student_info::second_type mark {-1};
							for (std::size_t i {}; i < l.size(); ++i) {
								auto& [nm, mk] {l[i]};
								if (!std::strcmp(name, nm)) {
									mark = mk;
									break;
								}
							}
							if (mark != -1) {
								std::cout << name << ": " << mark << '\n';
							} else {
								std::cout << "Нет такого имени\n";
							}
							std::cin.get();
							break;
						} case '2' : {
							std::cout << "Ввдеите номер в списке: ";
							std::size_t i; std::cin >> i; --i;
							std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							if (i < l.size()) {
								const auto& [name, mark] {l[i]};							
								std::cout << name << ": " << mark << '\n';
							} else {
								std::cout << "Номера нет в списке\n";
							}
							std::cin.get();
						}
					}
				}
				break;
			} case '4' : {
				std::cout << "Введите имя файла: ";
				std::string filename; std::cin >> filename;
				std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::ofstream file {filename, std::ios::binary};
				for (std::size_t i {}; i < l.size(); ++i)
					file.write(reinterpret_cast<char*>(&l[i]), sizeof(student_info));
				break;
			} case '5' : {
				l.clear();
				std::cout << "Введите имя файла: ";
				std::string filename; std::cin >> filename;
				std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::ifstream file {filename, std::ios::binary};
				student_info si;
				for (std::size_t i {}; file.read(reinterpret_cast<char*>(&si), sizeof(student_info)); ++i)
					l.insert(si, i);
			}
		}
	}
	return EXIT_SUCCESS;
}
