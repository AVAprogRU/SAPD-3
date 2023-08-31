#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

struct task {
	int type;
	int timetask;
	int durationtime;
};
struct tasklist {
	task task;
	tasklist* next;
	tasklist* pred;
};
struct processor {
	bool empty;
	task task;
};

void pushinqueue(tasklist** back, tasklist** head, task neww) {
	tasklist* p = new tasklist;
	p->task = neww;
	if (*back == nullptr) {
		*back = p;
		p->next = nullptr;
		p->pred = nullptr;
		*head = p;
	}
	else {
		(*back)->next = p;
		p->next = nullptr;
		p->pred = *back;
		*back = p;
	}
}
void printqueue(tasklist* head) {
	tasklist* r = head;
	while (r != nullptr) {
		cout << "Время поступления " << r->task.timetask << " тип задачи " << r->task.type << " такты задачи " << r->task.durationtime << endl;
		r = r->next;
	}
}
void randbuild(tasklist** back, tasklist** head) {
	srand(time(NULL));
	int n = 2 + rand() % 4;
	for (int i = 0; i < n; i++) {
		task a;
		a.timetask = i + 1;
		a.durationtime = 1 + rand() % 4;
		a.type = rand() % 3;
		pushinqueue(back, head, a);
	}
	cout << "Созданные задачи " << endl;
	printqueue(*head);
}
void printprocessor(processor a, int n) {
	if (a.empty)
		cout << "Процессор " << n << " свободен." << endl;
	else
		cout << "Содержимое процессора " << n << endl << "Время поступления " << a.task.timetask << " тип задачи " << a.task.type << " такты задачи " << a.task.durationtime << endl;
}

processor changeprocessor(processor a) {
	if (!a.empty) {
		a.task.durationtime = a.task.durationtime - 1;
		cout << a.task.durationtime;
		if (a.task.durationtime == 0) {
			a.task.timetask = 0;
			a.empty = true;
			cout << "freeproc ";
		}
	}
	return a;
}

task getfromqueue(tasklist** head, tasklist** back) {
	task del;
	del = (*head)->task;

	if ((*head)->next == nullptr) {
		*head = nullptr;
		*back = nullptr;
	}
	else {
		*head = (*head)->next;
		(*head)->pred = nullptr;
	}
	return del;
}
bool freequeue(tasklist* back) {
	if (back == nullptr)
		return true;
	else
		return false;
}
int checktime(tasklist* hd) {
	if (hd == nullptr)
		return -1;
	else
		return (*hd).task.timetask;
}
bool finish(tasklist* bklist, tasklist* head, bool stacktasks, processor p0, processor p1, processor p2) {
	if ((freequeue(bklist)) && (freequeue(head)) && (stacktasks) && (p0.empty) && (p1.empty) && (p2.empty)) {
		cout << "bklist " << freequeue(bklist);
		return true;
	}
	else
		return false;
}

int main() {
	setlocale(LC_ALL, "rus");
	//Создаем и вводим с клавы список задач
	tasklist* hdlist = nullptr;
	tasklist* bklist = nullptr;
	char e;
	cout << "Чтобы сгенерировать рандомный список, нажмите 0, иначе будет предложен ввод с консоли.";
	cin >> e;
	if (e != '0') {
		int n;
		cout << "Введите колво задач : "; cin >> n;
		for (int i = 0; i < n; i++) {
			task* a = new task;
			cout << "Данные " << i + 1 << " задачи." << endl;
			cout << "Введите время поступления : ";
			cin >> a->timetask;
			cout << "Введите тип : ";
			cin >> a->type;
			cout << "Введите время выполнения : ";
			cin >> a->durationtime;
			pushinqueue(&bklist, &hdlist, *a);
			delete(a);
		}
	}
	else
		randbuild(&bklist, &hdlist);
	//основная часть
	task stack[5]; //создаем стек
	int index = -1;
	processor p[3]; //процессоры
	p[0].empty = true;
	p[1].empty = true;
	p[2].empty = true;
	//очередь
	tasklist* back = nullptr;
	tasklist* head = nullptr;
	bool freestack = true;//пустота стека
	int timer = 1;

	while (true) {
		if (!finish(bklist, back, freestack, p[0], p[1], p[2])) {
			if (!freequeue(bklist)) {
				while (true) {
					if (checktime(hdlist) == timer) {
						task d = getfromqueue(&hdlist, &bklist);
						pushinqueue(&back, &head, d);
					}
					else
						break;
				}
			}
			while (!freequeue(back)) {//распределение задач из очереди
				task x;
				x = getfromqueue(&head, &back);
				if ((x.type == 0) && (p[0].empty == true)) {
					p[0].empty = false;
					p[0].task = x;
				}
				else if ((x.type == 1) && (p[1].empty == true)) {
					p[1].empty = false;
					p[1].task = x;
				}
				else if ((x.type == 2) && (p[2].empty == true)) {
					p[2].empty = false;
					p[2].task = x;
				}
				else {
					if (index < 5) {
						index++;
						stack[index].type = x.type;
						stack[index].timetask = x.timetask;
						stack[index].durationtime = x.durationtime;
						freestack = false;

					}
					else {
						cout << "Внимание стек переполнен!!!Работа программы не может быть продолжена." << endl;
						return 0;
					}
				}
			}
			while ((!freestack) && (p[stack[index].type].empty == true)) {//если один из процессоров свободен а в стеке его задача
				p[stack[index].type].empty = false;
				p[stack[index].type].task = stack[index];
				index--;																	//(такое может произойти если в очередь не добавляли задачу,а проц успел закончить)
				if (index == -1)
					freestack = true;
			}																		//ну или же задач просто кончились
			//вывод таблицы
			cout << "\n\n\n\nИдет " << timer << " такт." << endl;
			if (!freequeue(bklist)) {
				cout << "Входные задачи" << endl;
				printqueue(hdlist);
			}
			if (!freequeue(back))
				cout << "Произошел троллинг" << endl;
			if (!freestack) {
				cout << "Содержимое стека " << endl;
				for (int i = 0; i <= index; i++)
					cout << "Время поступления " << stack[index].timetask << " тип задачи " << stack[index].type << " такты задачи " << stack[index].durationtime << endl;
			}
			printprocessor(p[0], 0);
			printprocessor(p[1], 1);
			printprocessor(p[2], 2);
			timer++;
			p[0] = changeprocessor(p[0]);
			p[1] = changeprocessor(p[1]);
			p[2] = changeprocessor(p[2]);
		}
		else {
			cout << "Работа закончена" << endl;
			break;
		}
	}
	return 0;
}
