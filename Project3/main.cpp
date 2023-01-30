#include <stdio.h>
#include <iostream>
#include <string>
#include "sqlite3.h"
using namespace std;

int rc1;  //관리자 데이터베이스
sqlite3* db;
char* zErrMsg = 0;
const char* sql;
sqlite3_stmt* stmt;

// 데이터베이스에서 데이터 불러오는 함수
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

// 관리자 데이터베이스 테이블을 생성하는 함수
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

// 학생 데이터베이스 테이블을 생성하는 함수
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

// 최종 결과를 보여주는 함수 -- floor로 연결하기
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

//student ID로 열을 받고 싶을 때
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

//manager ID로 열을 받고 싶을 때
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

//score 점수 이상인 애
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

//insert into에 사용되는 함수
string quotesql(const string& s) {
	return string("'") + s + string("'");
};

//매니저 정보를 입력하는 함수-입사
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

//매니저 기본 정보를 입력하는 함수 -- 퇴사했을때
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

//총 셋팅
int setting(string id) {
	setting_D(id);
	setting_F(id);
	setting_S(id);

	return(0);
}

//학생 기본 정보를 입력하는 함수 -- 입주했을때
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

//학생 기본 정보를 입력하는 함수 -- 퇴소했을때
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

//관리자가 딜리버리 표기
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

//학생이 수령 후 딜리버리 삭제
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

//매니저가 수리 했을 떄
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

//학생이 수리 원하는 것 기재
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

//관리자가 상벌점
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
	cout << "시스템을 시작하시겠습니까? (y/n) : ";
	cin >> start;
	if (start == 'n') {
		cout << "종료되었습니다";
	}
	else {
		setManager();
		setStudent();
		int oper_mode=1;
		while(oper_mode==1){
		cout << endl << "시스템을 사용하기 위해 관리자는 1, 학생은 2을 입력하세요 (프로그램을 종료 = 0) : ";
		int ch;
		cin >> ch;
		if (ch == 1) {
			int endsign = 0;
			while (endsign != -1) {
				cout << endl<<"관리자 모드입니다." << endl
					<<"기존 근무자면 0, 새로 입사하셨으면 1, 퇴사하실 경우 2를 입력하세요 : ";
				int new_m;
				cin >> new_m;
				if (new_m == 1) {
					cout <<endl<< "순서대로 사원번호 이름 관리층수를 작성하세요 : ";
					string id, name, floor;
					cin >> id >> name >> floor;
					managerIn(id, name, floor);
					cout << "등록되었습니다.";
				}
				else if (new_m == 2) {
					cout << endl<<"사원번호를 입략하세요 : ";
					string id;
					cin >> id;
					managerOut(id);
					cout << "정보가 삭제되었습니다. 노고에 갑사드립니다.";
					break;
				}
				int manage = 1;
				while (manage != 0) {
					cout << endl << "***관리자 모드의 기능***" << endl
						<< "0. 관리자 모드 종료" << endl
						<< "1. 관리해야 하는 학생 정보 데이터 출력" << endl
						<< "2. 택배 도착 입력" << endl
						<< "3. 수리 완료 입력" << endl
						<< "4. 상벌점 입력" << endl
						<< "5. 우수학생 데이터 출력" << endl
						<< "6. 모든 데이터 출력" << endl
						<<"(만약 해당 조건을 만족하는 데이터가 없다면 아무것도 출력되지 않습니다.)"<<endl
						<< endl<<"사용하고 싶으신 기능의 번호를 입력해주세요 : ";
					int oper;
					cin >> oper;
					if (oper == 0) { endsign = -1; break; }
					else if (oper == 1) {
						cout <<endl<< "사번은 입력해주세요 : ";
						string m_id;
						cin >> m_id;
						showDataByMaID(m_id);
					}
					else if (oper == 2) {
						cout <<endl<< "도착한 택배 수령 학생의 학번을 입력해주세요(1개) : ";
						string s_id;
						cin >> s_id;
						managerDelivery(s_id);
					}
					else if (oper == 3) {
						cout <<endl<< "수리를 완료한 호실의 학생 학번을 입력해주세요(1개) : ";
						string s_id;
						cin >> s_id;
						managerFix(s_id);
					}
					else if (oper == 4) {
						cout << endl<<"순서대로 해당 학생의 학번과 상점 혹은 벌점을 입력하세요(-9 ~ 9) : ";
						string s_id, score;
						cin >> s_id >> score;
						managerScore(s_id, score);
					}
					else if (oper == 5) {
						cout << endl << "우수학생 선발 점수 기준을 입력해주세요 : ";
						string sc;
						cin >> sc;
						showDataByScore(sc);
					}
					else if (oper == 6) { showAllData(); }
					cout << endl<<"관리자 모드를 계속 이용하고 싶으면 1, 종료하고 싶으면 0을 입력하세요 : ";
					int k;
					cin >> k;
					if (k == 0) { cout <<endl<< "관리자모드가 종료되었습니다."<<endl; endsign = -1; break; }
					else { break; }
				}

			}
		}
		else if (ch == 2) {
			int endsign = 0;
			while (endsign != -1) {
				cout << endl<<"학생 모드입니다."<<endl 
					<<"기존 거주 학생이면 0, 새로 입실하셨으면 1, 퇴실하실 경우 2를 입력하세요 : ";
				int new_s;
				cin >> new_s;
				if (new_s == 1) {
					cout << endl<<"순서대로 학번 이름 사생실 호수를 작성하세요 : ";
					string id, name, room, floor;
					cin >> id >> name >> room;
					floor = room[0];
					studentIn(id, name, floor, room);
					cout << "등록되었습니다.";
				}
				else if (new_s == 2) {
					cout <<endl<< "학번을 입력하세요 : ";
					string id;
					cin >> id;
					studentOut(id);
					cout << "정보가 삭제되었습니다. 겨울학기 기숙사 신청일은 11/21 ~ 11/25 입니다.";
					break;
				}
				int student = 1;
				while (student != 0) {
					cout << endl<<"***학생 모드의 기능***"<<endl
						<<"0. 학생 모드 종료"<<endl
						<<"1. 택배, 수리 상태 및 상/벌점 확인"<<endl
						<<"2. 택배 수령 입력"<<endl
						<<"3. 수리 신청"<<endl
						<<"4. 모든 데이터 출력이 있습니다." << endl
						<< "(만약 해당 조건을 만족하는 데이터가 없다면 아무것도 출력되지 않습니다.)" << endl
						<< endl<<"사용하고 싶으신 기능의 번호를 입력해주세요 : ";
					int oper;
					cin >> oper;
					if (oper == 0) { endsign = -1; break; }
					else if (oper == 1) {
						cout << endl<< "학번을 입력해주세요 : ";
						string m_id;
						cin >> m_id;
						showDataByStID(m_id);
					}
					else if (oper == 2) {
						cout <<endl<< "학번을 입력해주세요 : ";
						string s_id;
						cin >> s_id;
						studentDelivery(s_id);
					}
					else if (oper == 3) {
						cout << endl <<"순서대로 학번 수리가 필요한 곳을 입력해주세요 : ";
						string s_id, where;
						cin >> s_id >> where;
						studentFix(s_id, where);
					}
					else if (oper == 4) { showAllData(); }
					cout << endl<< "학생 모드를 계속 이용하고 싶으면 1, 종료하고 싶으면 0을 입력하세요 : ";
					int k;
					cin >> k;
					if (k == 0) { cout << "학생 모드가 종료되었습니다." << endl; endsign = -1; break; }
					else { break; }
				}
			}
		}
		else { cout <<endl<< "프로그램이 종료되었습니다."; break; }
		}

	}

}

