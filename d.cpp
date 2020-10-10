#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // for exit()
#include <algorithm> // for sort()
#include <sstream>
#include <string_view>

using namespace std;


class Info_sys {
private:
	struct Student {
		string Name;
		string Student_ID;
		string Email_address;
		string dName;
		string Tel;

	};

	fstream* myfile;

	//file2.txt 에 저장된 과목들
	vector<string> Department_Num;
	vector<string> Department_Name;


	vector<Student> StudentVec;


public:
	//생성자
	Info_sys() {}
	Info_sys(fstream& file1, fstream& file2);

	//소멸자
	~Info_sys()
	{
		myfile->close();
	}

	//Interface
	void Menu();
	void Insert();
	void Search();
	void Sort();

	//FileManager
	void writeFile(Student _newStudent);

	//others
	void detect_Department(Student _Student);
	static bool compare_name(const Student &a, const Student &b) {
		return a.Name < b.Name;
	}
	static bool compare_id(const Student &a, const Student &b) {
		return a.Student_ID < b.Student_ID;
	}
	static bool compare_dname(const Student &a, const Student &b) {
		return a.dName < b.dName;
	}
};

Info_sys::Info_sys(fstream& file1, fstream& file2) {
	if (file2.is_open()) {
		string buffer;

		string input_dname; //department name
		string input_dnum; //department number

		while (file2.peek() != EOF) {
			getline(file2, buffer);

			input_dnum = buffer.substr(0, 3);
			input_dname = buffer.substr(4);

			Department_Name.push_back(input_dname);
			Department_Num.push_back(input_dnum);
		}

	}
	if (file1.is_open()) {
		string buffer;
		
		string::iterator it;
		string::iterator prev_it;
		string::iterator token;

		string input_name;
		string input_id;
		string input_email;
		string input_tel;

		Student _student;

		while (file1.peek() != EOF) {
			getline(file1, buffer);

			prev_it = buffer.begin();

			while (1) {
				it = find(prev_it, buffer.end(), '/');
				token = find(prev_it, it, ':');

				string title(prev_it, token );
				string content(token + 1, it);

				if (title == "Name") {
					input_name = content;
					_student.Name = input_name;
				}
				else if (title == "Student ID") {
					input_id = content;
					_student.Student_ID = input_id;

					for (int i = 0; i < Department_Name.size(); ++i) {
						if (input_id.substr(4, 3) == Department_Num[i]) {
							_student.dName = Department_Name[i];
						}
					}
				}
				else if (title == "Email address") {
					input_email = content;
					_student.Email_address = input_email;
				}	
				else if (title == "Tel") {
					input_tel = content;
					_student.Tel = input_tel;
				}

				if (title.compare("Tel") == 0)
					break;

				prev_it = it + 1;
			}

			StudentVec.push_back(_student);
		}
		sort(StudentVec.begin(), StudentVec.end(),compare_name);
	}
	else
	{
		file1.open("file1.txt");
	}

	
}

void Info_sys::Menu() {
	int m_input = 0; // 사용자가 입력한 숫자


	cout << "Main Menu" << endl;
	cout << "1. Insertion" << endl;
	cout << "2. Search" << endl;
	cout << "3. Sortion Option" << endl;
	cout << "4. Exit" << endl;

	cout << endl;

	cin >> m_input;
	cout << endl;

	switch (m_input)
	{
	case 1:
		Insert();
		break;
	case 2:
		Search();
		break;
	case 3:
		Sort();
		break;
	case 4:
		exit(0);
	default:
		break;
	}
}

void Info_sys::Insert() {
	string Input_Name;
	string Input_ID;
	string Input_Email;
	string Input_Tel;

	Student newStudent;

	cout << "Name: " << endl;
	cin >> Input_Name;
	if (Input_Name.empty() == true) {
		cout << "이름을 입력해주세요.";
		cin >> Input_Name;
	}
	cout << "Student ID: " << endl;
	cin >> Input_ID;
	if (Input_ID.empty()) {
		cout << "아이디를 입력해주세요.";
		cin >> Input_ID;
	}
	cout << "Email Address: " << endl;
	cin >> Input_Email;
	cout << "Tel: " << endl;
	cin >> Input_Tel;

	cout << endl;

	string dnum = Input_ID.substr(4, 3);
	string dname;

	for (int i = 0; i < Department_Num.size(); ++i) {
		if (dnum == Department_Num[i])
			dname = Department_Name[i];
	}

	newStudent = { Input_Name, Input_ID, Input_Email, dname, Input_Tel};

	StudentVec.push_back(newStudent);

	Info_sys::writeFile(newStudent);

	Menu();

}

void Info_sys::Search() {
	string s_name; // user name
	string s_id;
	string s_year; // admission year
	string s_dname; // department name
	

	int s_input = 0;

	cout << "- Search -" << endl;
	cout << "1. Search by name" << endl;
	cout << "2. Search by student ID (10 numbers)" << endl;
	cout << "3. Search by admission year (4 numbers)" << endl;
	cout << "4. Search by department name" << endl;
	cout << "5. List All" << endl;

	cout << endl;

	cin >> s_input;

	cout << endl;

	

	switch (s_input)
	{
	case 1:
		cout << "Name keyword? ";
		cin >> s_name;
		cout << endl;
		cout << "Name" << "            " << "Student ID" << " " << "Dept" << "                  " << "Email" << "           " << "Tel" << endl;

		for (int i = 0; i < StudentVec.size(); ++i) {
			if (StudentVec[i].Name.compare(s_name) == 0) {
				cout << StudentVec[i].Name << " " << StudentVec[i].Student_ID << " " << StudentVec[i].dName << " " << StudentVec[i].Email_address << " " << StudentVec[i].Tel << endl;
			}
		}

		break;
	case 2:
		cout << "Student ID? ";
		cin >> s_id;
		cout << endl;
		cout << "Name" << "            " << "Student ID" << " " << "Dept" << "                  " << "Email" << "           " << "Tel" << endl;
		for (int i = 0; i < StudentVec.size(); ++i) {
			if (StudentVec[i].Student_ID.compare(s_id) == 0) {
				cout << StudentVec[i].Name << " " << StudentVec[i].Student_ID << " " << StudentVec[i].dName << " " << StudentVec[i].Email_address << " " << StudentVec[i].Tel << endl;
			}
		}
		break;
	case 3:
		cout << "admission year? ";
		cin >> s_year;
		cout << endl;
		cout << "Name" << "            " << "Student ID" << " " << "Dept" << "                  " << "Email" << "           " << "Tel" << endl;
		for (int i = 0; i < StudentVec.size(); ++i) {

			string student_year;
			student_year = StudentVec[i].Student_ID.substr(0, 4);

			if (student_year.compare(s_year) == 0) {
				cout << StudentVec[i].Name << " " << StudentVec[i].Student_ID << " " << StudentVec[i].dName << " " << StudentVec[i].Email_address << " " << StudentVec[i].Tel << endl;
			}
		}
		break;
	case 4:
		cout << "Department Name keyword? ";
		getline(cin, s_dname);
		getline(cin, s_dname);
		cout << endl;
		cout << "Name" << "            " << "Student ID" << " " << "Dept" << "                  " << "Email" << "           " << "Tel" << endl;
		for (int i = 0; i < StudentVec.size(); ++i) {

			if (StudentVec[i].dName.compare(s_dname) == 0) {
				cout << StudentVec[i].Name << " " << StudentVec[i].Student_ID << " " << StudentVec[i].dName << " " << StudentVec[i].Email_address << " " << StudentVec[i].Tel << endl;
			}
		}
		break;
	case 5:
		cout << endl;
		cout << "Name" << "            " << "Student ID" << " " << "Dept" << "                  " << "Email" << "           " << "Tel" << endl;
		for (int i = 0; i < StudentVec.size(); ++i) {
			cout << StudentVec[i].Name << " " << StudentVec[i].Student_ID << " " << StudentVec[i].dName << " " << StudentVec[i].Email_address << " " << StudentVec[i].Tel << endl;
		}
		break;

	default:
		break;
	}

	Menu();

}

void Info_sys::Sort() {
	int sort_input = 0;

	cout << "- Sorting Option" << endl;
	cout << "1. Sort by Name" << endl;
	cout << "2. Sort by Student ID" << endl;
	cout << "3. Sort by department name" << endl;

	cin >> sort_input;

	switch (sort_input)
	{
	case 1:
		std::sort(StudentVec.begin(), StudentVec.end(), compare_name);
		break;
	case 2:
		std::sort(StudentVec.begin(), StudentVec.end(), compare_id);
		break;
	case 3:
		std::sort(StudentVec.begin(), StudentVec.end(), compare_dname);
		break;
	default:
		break;
	}

	Menu();
} 

void Info_sys::writeFile(Student _newStudent) {
	fstream outFile("file1.txt", ios::out|ios::app);

	if (outFile.is_open()) {
		outFile << "Name:" << _newStudent.Name << "/" << "Student ID:" << _newStudent.Student_ID << "/" << "Email address:" << _newStudent.Email_address << "/" << "Tel:" << _newStudent.Tel << "\n";
	}
	outFile.close();
}

void Info_sys::detect_Department(Student _Student) {
	string department_code;
	department_code = _Student.Student_ID.substr(4, 3);

	for (int i = 0; i < Department_Num.size(); ++i) {
		if (Department_Num[i].compare(department_code) == 0) {
			_Student.dName = Department_Name[i];
		}
	}
}


int main()
{

	fstream infile1("file1.txt", ios::in|ios::out);

	fstream infile2("file2.txt", ios::in);
	
	Info_sys mysystem(infile1, infile2);

	mysystem.Menu();
	
	return 0;
}
