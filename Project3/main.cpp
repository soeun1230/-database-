#include <stdio.h>
#include <iostream>
#include <string>
#include "sqlite3.h"
using namespace std;

int rc1;  //������ �����ͺ��̽�
sqlite3* db;
char* zErrMsg = 0;
const char* sql;
sqlite3_stmt* stmt;

// �����ͺ��̽����� ������ �ҷ����� �Լ�
int callback(void*NotUsed, int argc, char**argv, char**azColName) {
	NotUsed = 0;
	for (int i = 0; i < argc; i++) {
		if (argv[i] != NULL) {
			cout << azColName[i] <<" : "<< argv[i]<<endl;
		}
		else
	        cout << "Null"<<endl;
	}
	cout<<endl;
	return 0;
}

// ������ �����ͺ��̽� ���̺��� �����ϴ� �Լ�
int setManager() {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't make database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		sql = "CREATE TABLE Manager(Manager_ID INT, Name TEXT, Floor INT, PRIMARY KEY(Manager_ID));";
		rc1 = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
		sqlite3_close(db);
		fprintf(stderr, "Create Manager Table Sucess!!\n");
		return (0);
	}
}

// �л� �����ͺ��̽� ���̺��� �����ϴ� �Լ�
int setStudent() {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't make database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		sql = "CREATE TABLE Student(Student_ID INT, Name TEXT, Floor INT, Room INT, Delivery TEXT, Fix TEXT, Score INT, PRIMARY KEY(Student_ID));";
		rc1 = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
		sqlite3_close(db);
		fprintf(stderr, "Create Student Table Success!!\n");
		return (0);
	}
}

// ���� ����� �����ִ� �Լ� -- floor�� �����ϱ�
void showAllData() {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't show database: %s\n", sqlite3_errmsg(db));
	}
	else {
		sql = "SELECT Manager_ID, Manager.Name As Manager_Name, Manager.Floor, Student_ID, Student.Name As Student_Name, Room, Delivery, Fix, Score FROM Student, Manager;";
		rc1 = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
		sqlite3_close(db);
	}
}

//student ID�� ���� �ް� ���� ��
void showDataByStID(string st_id) {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't show database: %s\n", sqlite3_errmsg(db));
	}
	else {
		string order = "SELECT * FROM Student WHERE Student_ID="
			+st_id+";";
		rc1 = sqlite3_exec(db, order.c_str(), callback, 0, &zErrMsg);
	}
	sqlite3_close(db);
}

//manager ID�� ���� �ް� ���� ��
void showDataByMaID(string ma_id) {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't show database: %s\n", sqlite3_errmsg(db));
	}
	else {
		string order = "SELECT Student_ID, Student.Name, Room FROM Student, Manager WHERE Student.Floor=Manager.Floor and Manager.Manager_ID="
			+ ma_id + ";";
		rc1 = sqlite3_exec(db, order.c_str(), callback, 0, &zErrMsg);
	}
	sqlite3_close(db);
}

//score ���� �̻��� ��
void showDataByScore(string low) {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't show database: %s\n", sqlite3_errmsg(db));
	}
	else {
		string order = "SELECT Student_ID, Student.Name, Room, Score FROM Student WHERE Score >= "
			+ low +" ORDER BY Score DESC;";
		rc1 = sqlite3_exec(db, order.c_str(), callback, 0, &zErrMsg);
	}
	sqlite3_close(db);
}

//insert into�� ���Ǵ� �Լ�
string quotesql(const string& s) {
	return string("'") + s + string("'");
};

//�Ŵ��� ������ �Է��ϴ� �Լ�-�Ի�
int managerIn(string id, string name, string floor) {
	int id_check = 1;
	int name_check = 1;
	int floor_check = 1;

	if (id.empty()) { id_check = 0; };
	if (name.empty()) { name_check = 0; };
	if (floor.empty()) { floor_check = 0; };

	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't input your data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		if (id_check == 1 && name_check == 1 && floor_check == 1) {
		    string order = "INSERT INTO Manager('Manager_ID', 'Name', 'Floor') Values("
			    + id + "," + quotesql(name) + "," + floor + ");";
			sqlite3_prepare(db, order.c_str(), -1, &stmt, NULL);//preparing the statement
			sqlite3_step(stmt);

			return(0);
	    }
		else {
			fprintf(stderr, "You have to write all: %s\n", sqlite3_errmsg(db));
			return(0);
		}
		return(0);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

//�Ŵ��� �⺻ ������ �Է��ϴ� �Լ� -- ���������
int managerOut(string id) {
	int id_check = 1;

	if (id.empty()) { id_check = 0; };

	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't input your data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		if (id_check == 1) {
			string order = "DELETE FROM Manager WHERE Manager_ID=" + id + ";";
			sqlite3_prepare(db, order.c_str(), -1, &stmt, NULL);//preparing the statement
			sqlite3_step(stmt);

			return(0);
		}
		else {
			fprintf(stderr, "You have to write all: %s\n", sqlite3_errmsg(db));
			return(0);
		}
		return(0);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

//delivery
int setting_D(string id) {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't set data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		string order2 = "UPDATE Student SET Delivery='nothing' where Student_ID="
			+ id + ";";
		sqlite3_prepare(db, order2.c_str(), -1, &stmt, NULL);//preparing the statement
		sqlite3_step(stmt);
		return(0);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

//fix
int setting_F(string id) {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't set data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		string order3 = "UPDATE Student SET Fix='nothing' where Student_ID=" + id + ";";
		sqlite3_prepare(db, order3.c_str(), -1, &stmt, NULL);//preparing the statement
		sqlite3_step(stmt);
		return(0);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

//score
int setting_S(string id) {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't set data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		string order4 = "UPDATE Student SET Score=0 where Student_ID=" + id + ";";
		sqlite3_prepare(db, order4.c_str(), -1, &stmt, NULL);//preparing the statement
		sqlite3_step(stmt);
		return(0);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

//�� ����
int setting(string id) {
	setting_D(id);
	setting_F(id);
	setting_S(id);

	return(0);
}

//�л� �⺻ ������ �Է��ϴ� �Լ� -- ����������
int studentIn(string id, string name, string floor, string room) {
	int id_check = 1;
	int name_check = 1;
	int floor_check = 1;
	int room_check = 1;

	if (id.empty()) { id_check = 0; };
	if (name.empty()) { name_check = 0; };
	if (floor.empty()) { floor_check = 0; };
	if (room.empty()) { room_check = 0; };

	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't input your data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		if (id_check == 1 && name_check == 1 && floor_check == 1 && room_check == 1) {
			string order = "INSERT INTO Student('Student_ID', 'Name', 'Floor', 'Room') Values("
				+ id + "," + quotesql(name) + "," + floor + "," + room + ");";
			sqlite3_prepare(db, order.c_str(), -1, &stmt, NULL);//preparing the statement
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
			sqlite3_close(db);

			setting(id);

			return(0);
		}
		else {
			fprintf(stderr, "You have to write all: %s\n", sqlite3_errmsg(db));
			return(0);
		}
		return(0);
	}
}

//�л� �⺻ ������ �Է��ϴ� �Լ� -- ���������
int studentOut(string id) {
	int id_check = 1;

	if (id.empty()) { id_check = 0; };

	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't input your data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		if (id_check == 1) {
			string order = "DELETE FROM Student WHERE Student_ID=" + id + ";";
			sqlite3_prepare(db, order.c_str(), -1, &stmt, NULL);//preparing the statement
			sqlite3_step(stmt);

			return(0);
		}
		else {
			fprintf(stderr, "You have to write all: %s\n", sqlite3_errmsg(db));
			return(0);
		}
		return(0);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

//�����ڰ� �������� ǥ��
int managerDelivery(string st_ID) {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't input Delivery data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		string order = "UPDATE Student SET Delivery='new_arrive' WHERE Student_ID=" 
			+st_ID +";";
		sqlite3_prepare(db, order.c_str(), -1, &stmt, NULL);//preparing the statement
		sqlite3_step(stmt);
		return(0);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

//�л��� ���� �� �������� ����
int studentDelivery(string st_ID) {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't input Delivery data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		string order = "UPDATE Student SET Delivery='take' where Student_ID=" 
			+st_ID + ";";
		sqlite3_prepare(db, order.c_str(), -1, &stmt, NULL);//preparing the statement
		sqlite3_step(stmt);
		return(0);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

//�Ŵ����� ���� ���� ��
int managerFix(string st_ID) {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't input Fix data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		string order = "UPDATE Student SET Fix='fixed' WHERE Student_ID="
			+ st_ID + ";";
		sqlite3_prepare(db, order.c_str(), -1, &stmt, NULL);//preparing the statement
		sqlite3_step(stmt);
		return(0);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

//�л��� ���� ���ϴ� �� ����
int studentFix(string st_ID, string what) {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't input Fix data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		string order = "UPDATE Student SET Fix="+ quotesql(what) + "WHERE Student_ID = "
			+ st_ID + ";";
		sqlite3_prepare(db, order.c_str(), -1, &stmt, NULL);//preparing the statement
		sqlite3_step(stmt);
		return(0);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

//�����ڰ� �����
int managerScore(string st_ID, string plus) {
	rc1 = sqlite3_open("total.db", &db);
	if (rc1) {
		fprintf(stderr, "Can't input Score data: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		if (plus.front() == '-') {
			plus=plus[1];
			string order = "UPDATE Student SET Score=Score-" + plus + " WHERE Student_ID = "
				+ st_ID + ";";
			sqlite3_prepare(db, order.c_str(), -1, &stmt, NULL);
			sqlite3_step(stmt);
			return(0);
		}
		else {
			if (plus.front() == '+') {
				plus=plus[1];
			}
			string order = "UPDATE Student SET Score=Score+" + plus + " WHERE Student_ID = "
				+ st_ID + ";";
			sqlite3_prepare(db, order.c_str(), -1, &stmt, NULL);
			sqlite3_step(stmt);
			return(0);
		}
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

int main(int argc, char* argv[]) {
	char start;
	cout << "�ý����� �����Ͻðڽ��ϱ�? (y/n) : ";
	cin >> start;
	if (start == 'n') {
		cout << "����Ǿ����ϴ�";
	}
	else {
		setManager();
		setStudent();
		int oper_mode=1;
		while(oper_mode==1){
		cout << endl << "�ý����� ����ϱ� ���� �����ڴ� 1, �л��� 2�� �Է��ϼ��� (���α׷��� ���� = 0) : ";
		int ch;
		cin >> ch;
		if (ch == 1) {
			int endsign = 0;
			while (endsign != -1) {
				cout << endl<<"������ ����Դϴ�." << endl
					<<"���� �ٹ��ڸ� 0, ���� �Ի��ϼ����� 1, ����Ͻ� ��� 2�� �Է��ϼ��� : ";
				int new_m;
				cin >> new_m;
				if (new_m == 1) {
					cout <<endl<< "������� �����ȣ �̸� ���������� �ۼ��ϼ��� : ";
					string id, name, floor;
					cin >> id >> name >> floor;
					managerIn(id, name, floor);
					cout << "��ϵǾ����ϴ�.";
				}
				else if (new_m == 2) {
					cout << endl<<"�����ȣ�� �Է��ϼ��� : ";
					string id;
					cin >> id;
					managerOut(id);
					cout << "������ �����Ǿ����ϴ�. ��� ����帳�ϴ�.";
					break;
				}
				int manage = 1;
				while (manage != 0) {
					cout << endl << "***������ ����� ���***" << endl
						<< "0. ������ ��� ����" << endl
						<< "1. �����ؾ� �ϴ� �л� ���� ������ ���" << endl
						<< "2. �ù� ���� �Է�" << endl
						<< "3. ���� �Ϸ� �Է�" << endl
						<< "4. ����� �Է�" << endl
						<< "5. ����л� ������ ���" << endl
						<< "6. ��� ������ ���" << endl
						<<"(���� �ش� ������ �����ϴ� �����Ͱ� ���ٸ� �ƹ��͵� ��µ��� �ʽ��ϴ�.)"<<endl
						<< endl<<"����ϰ� ������ ����� ��ȣ�� �Է����ּ��� : ";
					int oper;
					cin >> oper;
					if (oper == 0) { endsign = -1; break; }
					else if (oper == 1) {
						cout <<endl<< "����� �Է����ּ��� : ";
						string m_id;
						cin >> m_id;
						showDataByMaID(m_id);
					}
					else if (oper == 2) {
						cout <<endl<< "������ �ù� ���� �л��� �й��� �Է����ּ���(1��) : ";
						string s_id;
						cin >> s_id;
						managerDelivery(s_id);
					}
					else if (oper == 3) {
						cout <<endl<< "������ �Ϸ��� ȣ���� �л� �й��� �Է����ּ���(1��) : ";
						string s_id;
						cin >> s_id;
						managerFix(s_id);
					}
					else if (oper == 4) {
						cout << endl<<"������� �ش� �л��� �й��� ���� Ȥ�� ������ �Է��ϼ���(-9 ~ 9) : ";
						string s_id, score;
						cin >> s_id >> score;
						managerScore(s_id, score);
					}
					else if (oper == 5) {
						cout << endl << "����л� ���� ���� ������ �Է����ּ��� : ";
						string sc;
						cin >> sc;
						showDataByScore(sc);
					}
					else if (oper == 6) { showAllData(); }
					cout << endl<<"������ ��带 ��� �̿��ϰ� ������ 1, �����ϰ� ������ 0�� �Է��ϼ��� : ";
					int k;
					cin >> k;
					if (k == 0) { cout <<endl<< "�����ڸ�尡 ����Ǿ����ϴ�."<<endl; endsign = -1; break; }
					else { break; }
				}

			}
		}
		else if (ch == 2) {
			int endsign = 0;
			while (endsign != -1) {
				cout << endl<<"�л� ����Դϴ�."<<endl 
					<<"���� ���� �л��̸� 0, ���� �Խ��ϼ����� 1, ����Ͻ� ��� 2�� �Է��ϼ��� : ";
				int new_s;
				cin >> new_s;
				if (new_s == 1) {
					cout << endl<<"������� �й� �̸� ����� ȣ���� �ۼ��ϼ��� : ";
					string id, name, room, floor;
					cin >> id >> name >> room;
					floor = room[0];
					studentIn(id, name, floor, room);
					cout << "��ϵǾ����ϴ�.";
				}
				else if (new_s == 2) {
					cout <<endl<< "�й��� �Է��ϼ��� : ";
					string id;
					cin >> id;
					studentOut(id);
					cout << "������ �����Ǿ����ϴ�. �ܿ��б� ����� ��û���� 11/21 ~ 11/25 �Դϴ�.";
					break;
				}
				int student = 1;
				while (student != 0) {
					cout << endl<<"***�л� ����� ���***"<<endl
						<<"0. �л� ��� ����"<<endl
						<<"1. �ù�, ���� ���� �� ��/���� Ȯ��"<<endl
						<<"2. �ù� ���� �Է�"<<endl
						<<"3. ���� ��û"<<endl
						<<"4. ��� ������ ����� �ֽ��ϴ�." << endl
						<< "(���� �ش� ������ �����ϴ� �����Ͱ� ���ٸ� �ƹ��͵� ��µ��� �ʽ��ϴ�.)" << endl
						<< endl<<"����ϰ� ������ ����� ��ȣ�� �Է����ּ��� : ";
					int oper;
					cin >> oper;
					if (oper == 0) { endsign = -1; break; }
					else if (oper == 1) {
						cout << endl<< "�й��� �Է����ּ��� : ";
						string m_id;
						cin >> m_id;
						showDataByStID(m_id);
					}
					else if (oper == 2) {
						cout <<endl<< "�й��� �Է����ּ��� : ";
						string s_id;
						cin >> s_id;
						studentDelivery(s_id);
					}
					else if (oper == 3) {
						cout << endl <<"������� �й� ������ �ʿ��� ���� �Է����ּ��� : ";
						string s_id, where;
						cin >> s_id >> where;
						studentFix(s_id, where);
					}
					else if (oper == 4) { showAllData(); }
					cout << endl<< "�л� ��带 ��� �̿��ϰ� ������ 1, �����ϰ� ������ 0�� �Է��ϼ��� : ";
					int k;
					cin >> k;
					if (k == 0) { cout << "�л� ��尡 ����Ǿ����ϴ�." << endl; endsign = -1; break; }
					else { break; }
				}
			}
		}
		else { cout <<endl<< "���α׷��� ����Ǿ����ϴ�."; break; }
		}

	}

}

