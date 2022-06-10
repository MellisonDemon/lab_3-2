﻿#include <iostream>
#include <string>
#include "windows.h" 

using namespace std;

string operations = "+-*/";

struct list {
	string symbol;
	list* next;
};

struct Stack {
	list* tail;
	Stack() : tail(nullptr) {}  //конструктор
	bool isEmpty() {
		if (tail == nullptr) return 1;
		return 0;
	}
	void output() {
		cout << "Операторы: ";
		list* curr = tail;
		if (!curr) {
			cout << "Пусто";
		}
		while (curr) {
			cout << curr->symbol << " ";
			curr = curr->next;
		}
		cout << "\n";
	}
	string pop(bool print = 1) {
		if (tail == nullptr) return "конец";
		if (print) cout << tail->symbol << " ";
		string answer = tail->symbol;
		list* p = new list;
		p = tail;
		tail = tail->next;
		p->next = nullptr;
		delete p;
		return answer;
	}
	void push(string sym) {
		list* p = new list;
		p->symbol = sym;
		p->next = tail;
		tail = p;
	}
};

int prior(char c) {
	switch (c) {
	case '(': return 1;
	case '+': case '-': return 2;
	case '*': case '/': return 3;
	default: return 0;
	}
}

bool correctInputInf(string& data) {
	bool flag_oper = 0, flag_bracket = 0;
	int count_num = 0, count_oper = 0;
	for (int i = 0; i < data.length(); ++i) {
		if (data[i] == ' ') data.erase(i, 1);
	}
	for (int i = 0; i < data.length(); ++i) {
		if (data[i] == '(') {
			if (!flag_oper && i != 0 || flag_bracket || operations.find(data[i + 1]) != -1) {
				return 0;
			}
			flag_bracket = 1;
		}
		else if (data[i] == ')') {
			if (!flag_bracket || flag_oper || i != data.length() - 1 && operations.find(data[i + 1]) == -1) {
				return 0;
			}
			flag_bracket = 0;
		}
		else if (data[i] >= '0' && data[i] <= '9' || data[i] == '.') {
			flag_oper = 0;
		}
		else if (operations.find(data[i]) != -1) {
			if (i == 0 || i == data.length() - 1) {
				return 0;
			}
			if (flag_oper) {
				return 0;
			}
			flag_oper = 1;
		}
		else if (data[i] == '\0') continue;
		else {
			return 0;;
		}
	}
	return 1;
}

bool correctInputPo(string& data) {
	int count_oper = 0, count_num = 0;
	for (int i = 0; i < data.length(); ++i) {
		if (operations.find(data[i]) != -1) {
			if (i == 0) {
				return 0;
			}
			count_oper++;
		}
		else if (data[i] == ' ') {
			if (operations.find(data[i - 1]) == -1)
				count_num++;
		}
		else if (data[i] >= '0' && data[i] <= '9') {
			if (i == data.length() - 1) {
				return 0;
			}
			continue;
		}
		else if (data[i] == '.' || data[i] == '\0')continue;
		else {
			return 0;
		}
	}
	if (count_oper != count_num - 1) {
		return 0;
	}
	return 1;
}

bool correctInputPre(string& data) {
	int count_oper = 0, count_num = 0;
	if (data[0] == ' ') data.erase(0, 1);
	for (int i = 0; i < data.length(); ++i) {
		if (operations.find(data[i]) != -1) {
			if (i == data.length() - 1) {
				return 0;
			}
			count_oper++;
		}
		else if (data[i] == ' ' || data[i] == '\0') {
			if (operations.find(data[i - 1]) == -1)
				count_num++;
		}
		else if (data[i] >= '0' && data[i] <= '9') {
			if (i == 0) {
				return 0;
			}
			continue;
		}
		else if (data[i] == '.')continue;
		else {
			return 0;
		}
	}
	if (count_oper != count_num - 1) {
		return 0;
	}
	return 1;
}

double action(double& value1, double& value2, string& oper) {
	switch (oper[0]) {
	case '+':
		return value1 + value2;
		break;
	case '-':
		return value1 - value2;
		break;
	case '*':
		return value1 * value2;
		break;
	case '/':
		return value1 / value2;
		break;
	default:
		break;
	}
}

void calculate(string& data, bool prefix) {
	if (prefix) {
		if (!correctInputPre(data)) {
			cout << "ОШИБКА\n";
			return;
		}
		for (int j = data.length() - 1, i = 0; j >= i; --j, ++i) {
			swap(data[j], data[i]);
		}
	}
	else {
		if (!correctInputPo(data)) {
			cout << "ОШИБКА\n";
			return;
		}
	}
	Stack numbers;
	string num;
	string  sym;
	double val_1, val_2;
	for (int i = 0; i <= data.length(); i++) {
		sym = data[i];
		if (operations.find(sym) != -1) {
			if (prefix) {
				val_1 = stod(numbers.pop(0));
				val_2 = stod(numbers.pop(0));
			}
			else {
				val_2 = stod(numbers.pop(0));
				val_1 = stod(numbers.pop(0));
			}

			numbers.push(to_string(action(val_1, val_2, sym)));
		}
		if (sym >= "0" && sym <= "9" || sym == ".") {
			num += sym;
		}
		if (sym == " ") {
			if (operations.find(data[i - 1]) != -1) continue;
			if (prefix) {
				for (int j = num.length() - 1, i = 0; j >= i; --j, ++i) {
					swap(num[j], num[i]);
				}
			}
			numbers.push(num);
			num.erase(0, num.length());
		}
	}
	numbers.pop();
	cout << "\n";
}

void RevPN(string& data, bool print = 1)
{
	Stack steak;
	string note;
	if (!correctInputInf(data))
	{
		cout << "ОШИБКА\n";
		return;
	}
	string sym;
	for (int i = 0; i <= data.length(); ++i) {
		if (data[i] == ' ') data.erase(i, 1);
	}
	for (int i = 0; i < data.length(); i++) {
		sym = data[i];
		if (sym >= "0" && sym <= "9" || sym == ".") {
			note += sym;
			if (sym == "." || data[i + 1] == '.' || data[i + 1] >= '0' && data[i + 1] <= '9') continue;
			note += " ";
			if (print) {
				cout << "\nЧисла: " << note << "\n";
				steak.output();
			}
		}
		if (sym == "(") {
			steak.push(sym);
			if (print) {
				cout << "\nЧисла: " << note << "\n";
				steak.output();
			}
			continue;
		}
		if (sym == ")") {
			while (steak.tail->symbol != "(") {
				note += steak.pop(0);
				note += " ";
			}
			steak.pop(0);
			if (print) {
				cout << "\nЧисла: " << note << "\n";
				steak.output();
			}
			continue;
		}
		if (operations.find(sym) != -1) {

			if (steak.isEmpty() || prior(steak.tail->symbol[0] < prior(sym[0]))) {
				steak.push(sym);
				if (print) {
					cout << "\nЧисла: " << note << "\n";
					steak.output();					
				}
			}
			else {
				while (!steak.isEmpty() && prior(steak.tail->symbol[0]) >= prior(sym[0])) {
					note += steak.pop(0);
					note += " ";
				}
				steak.push(sym);
				if (print) {
					cout << "\nЧисла: " << note << "\n";
					steak.output();
					
				}
			}
		}
	}
	if (print) {
		cout << "\nЧисла: " << note << "\n";
		steak.output();
		
	}
	while (!steak.isEmpty()) {
		note += steak.pop(0);
		note += " ";
	};
	if (print) {
		cout << "\nСтек: ";
	}
	cout << note << "\n";
	data = note;
}

void StandPN(string& data) {
	if (!correctInputInf(data)) {
		cout << "ОШИБКА\n";
		return;
	}
	string note;
	for (int j = data.length() - 1; j >= 0; --j) {
		if (data[j] == '(') {
			note += ')';
		}
		else if (data[j] == ')') {
			note += '(';
		}
		else if (data[j] == ' ') continue;
		else {
			note += data[j];
		}
	}
	cout << "Инвертированная строка: " << note << "\n" << "Алгоритм: ";
	RevPN(note, 0);
	data.erase(0, data.length());
	for (int j = note.length() - 1; j >= 0; --j) {
		data += note[j];
	}
	data += '\0';
	cout << "Стек: " << data << "\n";
}

void menu(string& data)
{
	int switcher;
	cout << "Выберите действие\n" \
		"1. Конвертировать выражение в обратную польскую нотацию(+автоматическая проверка)\n" \
		"2. Конвертировать выражение в польскую нотацию(+автоматическая проверка)\n" \
		"3. Вычислить выражение из обратной польской нотации\n" \
		"4. Вычислить выражение из польской нотации\n" \
		"5. Проверить корректность введёного выражения\n";
	while (true)
	{
		cout << "Выполнить пункт: ";
		cin >> switcher;
		switch (switcher)
		{
		case 1:
			data.erase(0, data.length());
			cin.ignore();
			cout << "Введите выражение: ";
			getline(cin, data);
			RevPN(data);
			break;
		case 2:
			data.erase(0, data.length());
			cin.ignore();
			cout << "Введите выражение: ";
			getline(cin, data);
			StandPN(data);
			break;
		case 3:
			if (!correctInputPo(data)) {
				cout << "You should to enter new reverse(postfix) note: ";
				cin.ignore();
				getline(cin, data);
				data += '\0';
			}
			else {
				cout << "Do you want to enter new reverse(postfix) note? (\'no\'=0) ";
				cin >> switcher;
				if (switcher) {
					data.erase(0, data.length());
					cin.ignore();
					getline(cin, data);
					data += '\0';
				}
			}
			calculate(data, 0);
			break;
		case 4:
			if (!correctInputPre(data)) {
				cout << "You should to enter new prefix note: ";
				cin.ignore();
				getline(cin, data);
				data += '\0';
			}
			else {
				cout << "Do you want to enter new prefix note? (\'no\'=0) ";
				cin >> switcher;
				if (switcher) {
					data.erase(0, data.length());
					cin.ignore();
					getline(cin, data);
					data += '\0';
				}
				else
					cin.ignore();
			}
			calculate(data, 1);
			break;
		case 5:
			cout << "Какой вид нотации проверить?\n1. стандартная (обычная запись)\n2. польская\n3. обратная польская\n";
			cin >> switcher;
			cout << "Введите выражение: ";
			data.erase(0, data.length());
			cin.ignore();
			getline(cin, data);
			data += '\0';
			switch (switcher) {
			case 1:
				if (correctInputInf(data)) {
					cout << "Верно!\n";
				}
				else {
					cout << "Есть ошибка!\n";
				}
				break;
			case 2:
				if (correctInputPre(data)) {
					cout << "Верно!\n";
				}
				else {
					cout << "Есть ошибка!\n";
				}
				break;
			case 3:
				if (correctInputPo(data)) {
					cout << "Верно!\n";
				}
				else {
					cout << "Есть ошибка!\n";
				}
				break;
			default:
				cout << "ОШИБКА\n";
			}
			break;
		default:
			cout << "ОШИБКА\n";
			break;
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	string data = "";
	menu(data);
	return 0;
}