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
		cout << "����� ����������� " << r->task.timetask << " ��� ������ " << r->task.type << " ����� ������ " << r->task.durationtime << endl;
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
	cout << "��������� ������ " << endl;
	printqueue(*head);
}
void printprocessor(processor a, int n) {
	if (a.empty)
		cout << "��������� " << n << " ��������." << endl;
	else
		cout << "���������� ���������� " << n << endl << "����� ����������� " << a.task.timetask << " ��� ������ " << a.task.type << " ����� ������ " << a.task.durationtime << endl;
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
	//������� � ������ � ����� ������ �����
	tasklist* hdlist = nullptr;
	tasklist* bklist = nullptr;
	char e;
	cout << "����� ������������� ��������� ������, ������� 0, ����� ����� ��������� ���� � �������.";
	cin >> e;
	if (e != '0') {
		int n;
		cout << "������� ����� ����� : "; cin >> n;
		for (int i = 0; i < n; i++) {
			task* a = new task;
			cout << "������ " << i + 1 << " ������." << endl;
			cout << "������� ����� ����������� : ";
			cin >> a->timetask;
			cout << "������� ��� : ";
			cin >> a->type;
			cout << "������� ����� ���������� : ";
			cin >> a->durationtime;
			pushinqueue(&bklist, &hdlist, *a);
			delete(a);
		}
	}
	else
		randbuild(&bklist, &hdlist);
	//�������� �����
	task stack[5]; //������� ����
	int index = -1;
	processor p[3]; //����������
	p[0].empty = true;
	p[1].empty = true;
	p[2].empty = true;
	//�������
	tasklist* back = nullptr;
	tasklist* head = nullptr;
	bool freestack = true;//������� �����
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
			while (!freequeue(back)) {//������������� ����� �� �������
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
						cout << "�������� ���� ����������!!!������ ��������� �� ����� ���� ����������." << endl;
						return 0;
					}
				}
			}
			while ((!freestack) && (p[stack[index].type].empty == true)) {//���� ���� �� ����������� �������� � � ����� ��� ������
				p[stack[index].type].empty = false;
				p[stack[index].type].task = stack[index];
				index--;																	//(����� ����� ��������� ���� � ������� �� ��������� ������,� ���� ����� ���������)
				if (index == -1)
					freestack = true;
			}																		//�� ��� �� ����� ������ ���������
			//����� �������
			cout << "\n\n\n\n���� " << timer << " ����." << endl;
			if (!freequeue(bklist)) {
				cout << "������� ������" << endl;
				printqueue(hdlist);
			}
			if (!freequeue(back))
				cout << "��������� ��������" << endl;
			if (!freestack) {
				cout << "���������� ����� " << endl;
				for (int i = 0; i <= index; i++)
					cout << "����� ����������� " << stack[index].timetask << " ��� ������ " << stack[index].type << " ����� ������ " << stack[index].durationtime << endl;
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
			cout << "������ ���������" << endl;
			break;
		}
	}
	return 0;
}
