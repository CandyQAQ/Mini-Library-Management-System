#include <windows.h>
#include <sql.h>
#include <stdio.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <iostream>
using namespace std;
#define LOGIN_TIMEOUT 30
#define MAXBUFLEN 255
RETCODE error;
HENV henv = NULL; //�������
HDBC conn = NULL; //���Ӿ��
HSTMT hstmt = NULL; //�����
void DBTest();
void check_Book();
void borrow_Book();
void return_Book();
void add_Book();
void proof_Manag();
int main()
{
	DBTest();
	system("pause");
	return 0;
}

void CreateTable()
{
	char *table,*table2,*table3;
	//���������
	error = SQLAllocStmt(conn, &hstmt);
	//ִ��SQL���
	table = "create table book (bno char(10),"
			"category varchar(10),"
			"title 	varchar(20),"
			"press varchar(20),"
			"year int,"
			"author varchar(10),"
			"price	decimal(7,2),"
			"total	int,"
			"stock	int,"
			" primary key(bno));";
	error = SQLExecDirect(hstmt, (SQLCHAR *)table, SQL_NTS);
	SQLFreeStmt(hstmt, SQL_CLOSE);

	error = SQLAllocStmt(conn, &hstmt);
	table2 = "create table card (cno char(7),"
			"name varchar(10),"
			"department varchar(40),"
			"type char(1),"
			"primary key(cno),"
			"check(type in('T','S')));";
	error = SQLExecDirect(hstmt, (SQLCHAR *)table2, SQL_NTS);
	SQLFreeStmt(hstmt, SQL_CLOSE);

	error = SQLAllocStmt(conn, &hstmt);
	table3 = "create table borrow (cno char(7),"
			"bno char(8),"
			"borrow_date int,"
			"return_date int,"
			"primary key(cno,bno),"
			"foreign key (cno) references card(cno),"
			"foreign key (bno) references book(bno));";
	error = SQLExecDirect(hstmt, (SQLCHAR *)table3, SQL_NTS);
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

void DBTest()
{
	int choice = 0;//����û�ѡ��
	/*******************************��ʼ����**********************************/
	//���价�����
	error = SQLAllocEnv(&henv);
	//�������Ӿ��
	error = SQLAllocConnect(henv, &conn);
	SQLSetConnectAttr(conn, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_POINTER);
	//�������ݿ�
	error = SQLConnect(conn, (SQLCHAR *)("DB"), SQL_NTS, (SQLCHAR *)("root"), SQL_NTS, (SQLCHAR *)("Xin13968517316!"), SQL_NTS);
	/*******************************���岿��**********************************/
	printf("********************************************************\n");
	printf("\t\t\tͼ�����ϵͳ\n");
	printf("********************************************************\n");

	CreateTable();

	while (1) {
		printf("1.ͼ���ѯ 2.���� 3.���� 4.ͼ����� 5.����֤���� 0.�˳�ϵͳ\n");
		printf("��������Ҫ�ķ�����:\n");
		scanf_s("%d", &choice);

		switch (choice)
		{
		case 0:
			//�Ͽ�����
			SQLDisconnect(conn);
			SQLFreeConnect(conn);
			SQLFreeEnv(henv);
			exit(0);
		case 1:
			check_Book();
			break;
		case 2:
			borrow_Book();
			break;
		case 3:
			return_Book();
			break;
		case 4:
			add_Book();
			break;
		case 5:
			proof_Manag();
			break;
		default:
			printf("�����Ŵ���\n");

		}
	}
	/*******************************��������**********************************/
	MessageBox(NULL, TEXT("ִ�гɹ�"), TEXT("����"), MB_OK);
}
void check_Book()
{
	char* query;//��ѯ���
	int choice = 0;//����û�ѡ��
	//���ͼ����Ϣ�ı���
	char book_ID[10] = { '0','0','0','0','0','0','0','0','0','0' };
	char book_category[10] = { '0','0','0','0','0','0','0','0','0','0' };
	char book_name[20] = { '0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0' };
	char press[20] = { '0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0' };
	int year = 0;
	char writer[10] = { '0','0','0','0','0','0','0','0','0','0' };
	float price = 0.0;
	int total_Amount = 0;
	int now_Amount = 0;
	SQLLEN lent1, lent2, lent3, lent4, lent5, lent6, lent7, lent8, lent9;
	SQLLEN ind_ptr = SQL_NTS;

	while (1) {
		printf("1.��ѯȫ�� 2.��������ѯ 3.����Ų�ѯ 0.�˳�\n");
		printf("��������Ҫ�ķ�����\n");
		scanf_s("%d", &choice);
		switch (choice)
		{
		case 0:
			return;
		case 1:
			//���������
			error = SQLAllocStmt(conn, &hstmt);
			//ִ��SQL���
			query = "SELECT * FROM book";
			/*error = SQLPrepare(hstmt,(SQLCHAR *)query,SQL_NTS);
			error = SQLExecute(hstmt);*/
			error = SQLExecDirect(hstmt, (SQLCHAR *)query, SQL_NTS);
			if (error == SQL_SUCCESS) {
				SQLBindCol(hstmt, 1, SQL_C_CHAR, book_ID, 10, &lent1);
				SQLBindCol(hstmt, 2, SQL_C_CHAR, book_category, 10, &lent2);
				SQLBindCol(hstmt, 3, SQL_C_CHAR, book_name, 20, &lent3);
				SQLBindCol(hstmt, 4, SQL_C_CHAR, press, 20, &lent4);
				SQLBindCol(hstmt, 5, SQL_C_SHORT, &year, 0, &lent5);
				SQLBindCol(hstmt, 6, SQL_C_CHAR, writer, 10, &lent6);
				SQLBindCol(hstmt, 7, SQL_C_FLOAT, &price, 0, &lent7);
				SQLBindCol(hstmt, 8, SQL_C_SHORT, &total_Amount, 0, &lent8);
				SQLBindCol(hstmt, 9, SQL_C_SHORT, &now_Amount, 0, &lent9);

				printf("��ѯ�������\n");
				printf("********************************************************************************\n");
				printf("���\t\t���\t����\t\t������\t���\t����\t�۸�\t�ܲ�����\t���\n");
				printf("********************************************************************************\n");
				while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
				{
					printf("%s\t%s\t%s\t%s\t%d\t%s\t%.2f\t%d\t%d\n", book_ID, book_category, book_name, press, year, writer, price, total_Amount, now_Amount);
				}
			}
			break;
		case 2:
			char name[20];
			getchar();
			scanf_s("%[^\n]", name, 20);
			//���������
			error = SQLAllocStmt(conn, &hstmt);
			//ִ��SQL���
			query = "SELECT * FROM book WHERE title = ?;";
			SQLPrepare(hstmt, (SQLCHAR *)query, SQL_NTS);
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, (SQLPOINTER)name, 20, &ind_ptr);
			error = SQLExecute(hstmt);
			if (error == SQL_SUCCESS) {
				SQLBindCol(hstmt, 1, SQL_C_CHAR, book_ID, 10, &lent1);
				SQLBindCol(hstmt, 2, SQL_C_CHAR, book_category, 10, &lent2);
				SQLBindCol(hstmt, 3, SQL_C_CHAR, book_name, 20, &lent3);
				SQLBindCol(hstmt, 4, SQL_C_CHAR, press, 20, &lent4);
				SQLBindCol(hstmt, 5, SQL_C_SHORT, &year, 0, &lent5);
				SQLBindCol(hstmt, 6, SQL_C_CHAR, writer, 10, &lent6);
				SQLBindCol(hstmt, 7, SQL_C_FLOAT, &price, 0, &lent7);
				SQLBindCol(hstmt, 8, SQL_C_SHORT, &total_Amount, 0, &lent8);
				SQLBindCol(hstmt, 9, SQL_C_SHORT, &now_Amount, 0, &lent9);
				if (SQLFetch(hstmt) == SQL_SUCCESS)
				{
					printf("��ѯ�������\n");
					printf("********************************************************************************\n");
					printf("���\t���\t����\t������\t���\t����\t�۸�\t�ܲ�����\t���\n");
					printf("********************************************************************************\n");
					printf("%s\t%s\t%s\t%s\t%d\t%s\t%.2f\t%d\t%d\n", book_ID, book_category, book_name, press, year, writer, price, total_Amount, now_Amount);
				}	
			}
			break;
		case 3:
			char ID[10];
			getchar();
			scanf_s("%[^\n]", ID, 10);
			//���������
			error = SQLAllocStmt(conn, &hstmt);
			//ִ��SQL���
			query = "SELECT * FROM book WHERE bno = ?;";
			SQLPrepare(hstmt, (SQLCHAR *)query, SQL_NTS);
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, (SQLPOINTER)ID, 10, &ind_ptr);
			error = SQLExecute(hstmt);
			if (error == SQL_SUCCESS) {
				SQLBindCol(hstmt, 1, SQL_C_CHAR, book_ID, 10, &lent1);
				SQLBindCol(hstmt, 2, SQL_C_CHAR, book_category, 10, &lent2);
				SQLBindCol(hstmt, 3, SQL_C_CHAR, book_name, 20, &lent3);
				SQLBindCol(hstmt, 4, SQL_C_CHAR, press, 20, &lent4);
				SQLBindCol(hstmt, 5, SQL_C_SHORT, &year, 0, &lent5);
				SQLBindCol(hstmt, 6, SQL_C_CHAR, writer, 10, &lent6);
				SQLBindCol(hstmt, 7, SQL_C_FLOAT, &price, 0, &lent7);
				SQLBindCol(hstmt, 8, SQL_C_SHORT, &total_Amount, 0, &lent8);
				SQLBindCol(hstmt, 9, SQL_C_SHORT, &now_Amount, 0, &lent9);
				if (SQLFetch(hstmt) == SQL_SUCCESS)
				{
					printf("��ѯ�������\n");
					printf("********************************************************************************\n");
					printf("���\t���\t����\t������\t���\t����\t�۸�\t�ܲ�����\t���\n");
					printf("********************************************************************************\n");
					printf("%s\t%s\t%s\t%s\t%d\t%s\t%.2f\t%d\t%d\n", book_ID, book_category, book_name, press, year, writer, price, total_Amount, now_Amount);
				}
			}
			break;
		default:
			printf("�����Ŵ���\n");
		}
	}
}
void borrow_Book()
{
	int choice;
	char *borrow, *query;
	char book_ID[10], card_number[7];
	int total_Amount = 0;
	int now_Amount = 0;
	SQLLEN lent1, lent2, lent3, lent4, lent5, lent6, lent7, lent8, lent9;
	SQLLEN ind_ptr = SQL_NTS;
	printf("������ѡ��1.���ĵ���ż�����֤��� 0.�˳�ϵͳ\n");
	scanf_s("%d", &choice);
	switch (choice)
	{
		case 0:
			return;
		case 1:
			//scanf_s("%s %s", book_ID, 10, card_number, 7);
			getchar();
			cin >> book_ID >> card_number;
			error = SQLAllocStmt(conn, &hstmt);
			query = "select * from book where bno = ?;";
			SQLPrepare(hstmt, (SQLCHAR *)query, SQL_NTS);
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, (SQLPOINTER)book_ID, 10, &ind_ptr);
			error = SQLExecute(hstmt);
			if (error == SQL_SUCCESS)
			{
				SQLBindCol(hstmt, 1, SQL_C_CHAR, book_ID, 10, &lent1);
				SQLBindCol(hstmt, 8, SQL_C_SHORT, &total_Amount, 0, &lent8);
				SQLBindCol(hstmt, 9, SQL_C_SHORT, &now_Amount, 0, &lent9);
				if (SQLFetch(hstmt) == SQL_SUCCESS)
				{
					if (now_Amount == 0)
					{
						printf("������Ϊ�㣡\n");
					}
					else
					{
						error = SQLAllocStmt(conn, &hstmt);
						borrow = "update book set stock = stock - 1 where bno = ?;";
						SQLPrepare(hstmt, (SQLCHAR *)borrow, SQL_NTS);
						SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, (SQLPOINTER)book_ID, 10, &ind_ptr);
						error = SQLExecute(hstmt);
						if (error == SQL_SUCCESS)
						{
							printf("���ĳɹ���\n");
						}
						else
						{
							printf("����ʧ�ܣ�\n");
						}
					}
				}
			}
			else
			{
				printf("����ʧ�ܣ�\n");
			}
			break;
	}
}
void return_Book()
{
	int choice;
	char *back, *query;
	char book_ID[10], card_number[7];
	int total_Amount = 0;
	int now_Amount = 0;
	SQLLEN lent1, lent2, lent3, lent4, lent5, lent6, lent7, lent8, lent9;
	SQLLEN ind_ptr = SQL_NTS;
	printf("������ѡ��1.�黹����ż�����֤��� 0.�˳�ϵͳ\n");
	scanf_s("%d", &choice);
	switch (choice)
	{
	case 0:
		return;
	case 1:
		//scanf_s("%s %s", book_ID, 10, card_number, 7);
		getchar();
		cin >> book_ID >> card_number;
		error = SQLAllocStmt(conn, &hstmt);
		query = "select * from book where bno = ?;";
		SQLPrepare(hstmt, (SQLCHAR *)query, SQL_NTS);
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, (SQLPOINTER)book_ID, 10, &ind_ptr);
		error = SQLExecute(hstmt);
		if (error == SQL_SUCCESS)
		{
			SQLBindCol(hstmt, 1, SQL_C_CHAR, book_ID, 10, &lent1);
			SQLBindCol(hstmt, 8, SQL_C_SHORT, &total_Amount, 0, &lent8);
			SQLBindCol(hstmt, 9, SQL_C_SHORT, &now_Amount, 0, &lent9);
			if (SQLFetch(hstmt) == SQL_SUCCESS)
			{
				error = SQLAllocStmt(conn, &hstmt);
				back = "update book set stock = stock + 1 where bno = ?;";
				SQLPrepare(hstmt, (SQLCHAR *)back, SQL_NTS);
				SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, (SQLPOINTER)book_ID, 10, &ind_ptr);
				error = SQLExecute(hstmt);
				if (error == SQL_SUCCESS)
				{
					printf("�黹�ɹ���\n");
				}
				else
				{
					printf("�黹ʧ�ܣ�\n");
				}
			}
		}
		else
		{
			printf("�黹ʧ�ܣ�\n");
		}
		break;
	}
}
void add_Book()
{
	char book_ID[10] = { '0','0','0','0','0','0','0','0','0','0' };
	char book_category[10] = { '0','0','0','0','0','0','0','0','0','0' };
	char book_name[20] = { '0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0' };
	char press_name[20] = { '0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0' };
	int time = 0;
	char writer[10] = { '0','0','0','0','0','0','0','0','0','0' };
	float cost = 0.0;
	int total_Amount = 0;
	int now_Amount = 0;
	char * new_book,*query,*add,*add_t;
	SQLLEN lent1, lent2, lent3, lent4, lent5, lent6, lent7, lent8, lent9;
	SQLLEN ind_ptr = SQL_NTS;

	//���������
	error = SQLAllocStmt(conn, &hstmt);
	printf("��������š����͡������������硢��ݡ����ߡ��۸�\n");
	new_book = "insert into book values(?, ?, ?, ?, ?, ?, ?, 1, 1);";
	cin >> book_ID >> book_category >> book_name >> press_name >> time >> writer >> cost;
	//scanf_s("%s %s %s %s %d %s %f %d %d", book_ID, 10, book_category, 10, book_name,20, press_name, 20, &time, writer,10, &cost, &total_Amount, &now_Amount);
	query = "select * from book where bno = ?;";
	SQLPrepare(hstmt, (SQLCHAR *)query, SQL_NTS);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, (SQLPOINTER)book_ID, 10, &ind_ptr);
	error = SQLExecute(hstmt);
	if (error == SQL_SUCCESS)
	{
		SQLBindCol(hstmt, 1, SQL_C_CHAR, book_ID, 10, &lent1);
		SQLBindCol(hstmt, 8, SQL_C_SHORT, &total_Amount, 0, &lent8);
		SQLBindCol(hstmt, 9, SQL_C_SHORT, &now_Amount, 0, &lent9);
		if (SQLFetch(hstmt) == SQL_SUCCESS)
		{
			error = SQLAllocStmt(conn, &hstmt);
			add = "update book set stock = stock + 1 where bno = ?;";
			SQLPrepare(hstmt, (SQLCHAR *)add, SQL_NTS);
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, (SQLPOINTER)book_ID, 10, &ind_ptr);
			error = SQLExecute(hstmt);
			if (error == SQL_SUCCESS)
				printf("���ɹ���\n");
		}
		else
		{
			SQLPrepare(hstmt, (SQLCHAR *)new_book, SQL_NTS);
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 11, 0, (SQLPOINTER)book_ID, 11, &ind_ptr);
			SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 11, 0, (SQLPOINTER)book_category, 11, &ind_ptr);
			SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 21, 0, (SQLPOINTER)book_name, 21, &ind_ptr);
			SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 21, 0, (SQLPOINTER)press_name, 21, &ind_ptr);
			SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&time, 0, &ind_ptr);
			SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 11, 0, (SQLPOINTER)writer, 11, &ind_ptr);
			SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_NUMERIC, 0, 0, (SQLPOINTER)&cost, 0, &ind_ptr);
			error = SQLExecute(hstmt);
			if (error == SQL_SUCCESS)
			{
				SQLBindCol(hstmt, 1, SQL_C_CHAR, book_ID, 10, &lent1);
				SQLBindCol(hstmt, 2, SQL_C_CHAR, book_category, 10, &lent2);
				SQLBindCol(hstmt, 3, SQL_C_CHAR, book_name, 20, &lent3);
				SQLBindCol(hstmt, 4, SQL_C_CHAR, press_name, 20, &lent4);
				SQLBindCol(hstmt, 5, SQL_C_SHORT, &time, 0, &lent5);
				SQLBindCol(hstmt, 6, SQL_C_CHAR, writer, 10, &lent6);
				SQLBindCol(hstmt, 7, SQL_C_FLOAT, &cost, 0, &lent7);
				SQLBindCol(hstmt, 8, SQL_C_SHORT, &total_Amount, 0, &lent8);
				SQLBindCol(hstmt, 9, SQL_C_SHORT, &now_Amount, 0, &lent9);
				printf("���ɹ���\n");
			}
		}
	}
}

void proof_Manag()
{
	int choice;
	char *add;
	char card_number[7];
	char card_name[10];
	char card_department[40];
	char card_type[1];
	char s[100];
	SQLLEN lent1, lent2, lent3, lent4, lent5, lent6, lent7, lent8, lent9;
	SQLLEN ind_ptr = SQL_NTS;
	printf("1.ɾ������֤ 2.���ӽ���֤ 3.����֤�޸� 0.�˳�ϵͳ\n");
	scanf_s("%d", &choice);
	switch (choice)
	{
	case 1:
		printf("������Ҫɾ���Ľ���֤���\n");
		error = SQLAllocStmt(conn, &hstmt);
		cin >> card_number;
		add = "delete from card where cno = ?";
		SQLPrepare(hstmt, (SQLCHAR *)add, SQL_NTS);
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 7, 0, (SQLPOINTER)card_number, 7, &ind_ptr);
		error = SQLExecute(hstmt);
		if (error == SQL_SUCCESS)
		{
			printf("ɾ������֤�ɹ���\n");
		}
		break;
	case 2:
		error = SQLAllocStmt(conn, &hstmt);
		cin >> card_number >> card_name >> card_department >> card_type;
		add = "insert into card values(?, ?, ?, ?)";
		SQLPrepare(hstmt, (SQLCHAR *)add, SQL_NTS);
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 7, 0, (SQLPOINTER)card_number, 7, &ind_ptr);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, (SQLPOINTER)card_name, 10, &ind_ptr);
		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 40, 0, (SQLPOINTER)card_department, 40, &ind_ptr);
		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 1, 0, (SQLPOINTER)card_type, 1, &ind_ptr);
		error = SQLExecute(hstmt);
		if (error == SQL_SUCCESS)
		{
			SQLBindCol(hstmt, 1, SQL_C_CHAR, card_number, 7, &lent1);
			SQLBindCol(hstmt, 2, SQL_C_CHAR, card_name, 10, &lent2);
			SQLBindCol(hstmt, 3, SQL_C_CHAR, card_department, 40, &lent3);
			SQLBindCol(hstmt, 4, SQL_C_CHAR, card_type, 1, &lent4);
			printf("���ӽ���֤�ɹ���\n");
		}
		break;
	case 3:
		printf("��ѡ��Ҫ�޸ĵ����ݣ�1.��� 2.���� 3.���� 4.����\n");
		scanf_s("%d", &choice);
		switch (choice)
		{
			case 1:
				printf("�޷��޸Ľ���֤��ţ�\n");
				break;
			case 2:
				printf("���������֤��ź��µ�����\n");
				error = SQLAllocStmt(conn, &hstmt);
				cin >> card_number >> card_name;
				add = "update card set name = ? where cno = ?;";
				SQLPrepare(hstmt, (SQLCHAR *)add, SQL_NTS);
				SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, (SQLPOINTER)card_name, 10, &ind_ptr);
				SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 7, 0, (SQLPOINTER)card_number, 7, &ind_ptr);
				error = SQLExecute(hstmt);
				if (error == SQL_SUCCESS)
				{
					printf("�޸ĳɹ���\n");
				}
				break;
			case 3:
				printf("���������֤��ź��µĲ���\n");
				error = SQLAllocStmt(conn, &hstmt);
				cin >> card_number >> card_department;
				add = "update card set department = ? where cno = ?;";
				SQLPrepare(hstmt, (SQLCHAR *)add, SQL_NTS);
				SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 40, 0, (SQLPOINTER)card_department, 40, &ind_ptr);
				SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 7, 0, (SQLPOINTER)card_number, 7, &ind_ptr);
				error = SQLExecute(hstmt);
				if (error == SQL_SUCCESS)
				{
					printf("�޸ĳɹ���\n");
				}
				break;
			case 4:
				printf("���������֤��ź��µ�����\n");
				error = SQLAllocStmt(conn, &hstmt);
				cin >> card_number >> card_type;
				add = "update card set type = ? where cno = ?;";
				SQLPrepare(hstmt, (SQLCHAR *)add, SQL_NTS);
				SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 1, 0, (SQLPOINTER)card_type, 1, &ind_ptr);
				SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 7, 0, (SQLPOINTER)card_number, 7, &ind_ptr);
				error = SQLExecute(hstmt);
				if (error == SQL_SUCCESS)
				{
					printf("�޸ĳɹ���\n");
				}
				break;
		}
		break;
	case 0:
		return;
	default:
		printf("�����Ŵ���\n");
		break;
	}
}
