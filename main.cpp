#include <cstring>
#include <stdio.h>
#include <stdlib.h>

typedef struct book {
    char book_num[10]; /*图书编号*/
    char book_name[20]; /*图书名称*/
    char book_writer[20]; /*作者*/
    int book_xy; /*现存量*/
    int book_kc; /*库存量*/
    struct book *next;
    int length;
} Book;

typedef struct reader {
    char reader_num[20]; /*学生学号*/
    char reader_name[20]; /*学生姓名*/
    int permission; /*1可借 0 不可借*/
    Book borrow[100]; /*最大借书量*/
    int borrow_count;
    struct reader *next;
    int length;
    char *borrow_book_num[];
} Student;

void wait_for_keypress() {
    printf("按任意键返回主菜单...");
    while (getchar() != '\n'); // Clear the buffer
    getchar(); // Wait for a key press
};

Student *insert_student_information(Student *student) {
    printf("\n学生信息录入：\n");
    Student *new_student = (Student *) malloc(sizeof(Student));
    if (new_student == NULL) {
        printf("内存分配失败！\n");
        return 0;
    }
    printf("\n请输入学生信息：\n");
    printf("学号:"); /*输入学号*/
    scanf("%s", new_student->reader_num);

    Student *current = student;
    Student *previous = NULL;
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->reader_num, new_student->reader_num) == 0) {
            found = 1;
            break;
        }
        previous = current;
        current = current->next;
    }
    if (found) {
        printf("此学生信息已存在");
        free(new_student);
    } else {
        printf("姓名:"); /*输入姓名*/
        scanf("%s", new_student->reader_name);
        printf("权限:"); /*输入权限*/
        scanf("%d", &new_student->permission);
        new_student->borrow_count = 0;
        new_student->next = NULL;
        for (int i; i < 100; i++) {
            new_student->borrow[i].book_num[0] = '\0';
            new_student->borrow[i].book_name[0] = '\0';
            new_student->borrow[i].book_writer[0] = '\0';
            new_student->borrow[i].book_xy = 0;
            new_student->borrow[i].book_kc = 0;
            new_student->borrow[i].next = NULL;
        }
        if (student == NULL) {
            student = new_student;
        } else {
            Student *current = student;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_student;
        }
        printf("\n学生信息录入完毕!\n");
        student->length += 1;
    }
    wait_for_keypress();
    return student;
}


Book *insert_book(Book *books) {
    int book_number = 1;
    printf("您此次需要入库几本书？");
    scanf("%d", &book_number);
    for (int i = 0; i < book_number; i++) {
        printf("\n图书馆图书录入：\n");
        printf("\n请输入第%d本书的资料", i + 1);
        Book *new_book = (Book *) malloc(sizeof(Book));
        if (new_book == NULL) {
            printf("内存分配失败！\n");
            return 0;
        }
        printf("\n请输入书籍信息：\n");
        printf("图书编号:"); /*输入图书编号(唯一)*/
        scanf("%s", new_book->book_num);
        Book *current = books;
        Book *previous = NULL;
        int found = 0;

        while (current != NULL) {
            if (strcmp(current->book_num, new_book->book_num) == 0) {
                found = 1;
                break;
            }
            previous = current;
            current = current->next;
        }
        if (found) {
            printf("此图书已存在于管中，请输入入库数量");
            scanf("%d", &new_book->book_kc);
            books->book_kc += new_book->book_kc;
            books->book_xy += new_book->book_kc;
            free(new_book);
        } else {
            printf("图书名称:"); /*输入图书名称*/
            scanf("%s", new_book->book_name);
            printf("图书作者:"); /*输入图书作者*/
            scanf("%s", new_book->book_writer);
            printf("图书数量:"); /*输入图书数量*/
            scanf("%d", &new_book->book_kc);
            new_book->book_xy = new_book->book_kc; /*开始时图书现有量和库存量相等*/
            new_book->next = NULL;
            if (books == NULL) {
                books = new_book;
            } else {
                Book *current = books;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = new_book;
            }
            printf("\n图书信息录入完毕!\n");
            books->length += 1;
        }
    }
    wait_for_keypress();
    return books;
}

void borrow_book() {
}

void return_book(Book *books, Student *students) {
    Student *q = students;
    int i, find = 0;
    char return_book_num[10], return_reader_num[20];
    printf("\n归还书籍\n");
    printf("\n请输入学号:");
    scanf("%s", return_reader_num);
    // 查找学生
    while (q != NULL && strcmp(return_reader_num, q->reader_num) != 0)
        q = q->next;

    if (q == NULL) {
        find = 2;
        printf("\n此学号不存在!\n");
    }
    printf("\n请输入归还书籍编号:");
    scanf("%s", return_book_num);

    // 查找书籍编号在学生的借书列表中
    for (i = 0; i < q->borrow_count; i++) {
        if (strcmp(return_book_num, q->borrow_book_num[i]) == 0) {
            find = 1;
            // 将后续借书记录向前移动
            for (int j = i; j < q->borrow_count - 1; j++) {
                strcpy(q->borrow_book_num[j], q->borrow_book_num[j + 1]);
                strcpy(q->limit_date[j], q->limit_date[j + 1]);
            }
            // 清除最后一个借书记录
            strcpy(q->borrow_book_num[q->borrow_count - 1], "0");
            strcpy(q->limit_date[q->borrow_count - 1], "0");
            q->borrow_count--; // 减少借书计数

            printf("\n学号%s的学生还书完毕!", return_reader_num);
            break; // 找到后退出循环
        }
    }
    if (find == 0)
        printf("\n错误，该学生未借此书!\n");
}

void book_information_enquiry(Book *books) {
    if (books == NULL) {
        printf("没有图书信息");
    }
    Book *current = books;
    int count = 0;
    while (current != NULL) {
        printf("书籍 %d:    ", count + 1);
        printf("图书编号: %s    ", current->book_num);
        printf("图书名称: %s    ", current->book_name);
        printf("图书作者: %s    ", current->book_writer);
        printf("现存量: %d    ", current->book_xy);
        printf("库存量: %d    \n", current->book_kc);
        current = current->next;
        count++;
    }
}

void student_information_enquiry(Student *students) {
    if (students == NULL) {
        printf("没有学生信息");
    }
    Student *current = students;
    int count = 0;
    while (current != NULL) {
        printf("学生 %d:    ", count + 1);
        printf("学号: %s    ", current->reader_num);
        printf("姓名: %s    ", current->reader_name);
        printf("权限: %d    ", current->permission);
        printf("借出书本数量: %d\n", current->borrow_count);
        current = current->next;
        count++;
    }
}

int GetLength(Book *books) {
    int length = 0;
    Book *current = books;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

int main(void) {
    Book *books = NULL;
    Student *students = NULL;
    int choice;
    while (1) {
        printf("当前馆藏图书数量:%d\n", GetLength(books));
        printf("欢迎您使用图书管理系统，请选择功能：\n");
        printf("1.学生信息登记\n");
        printf("2.图书入库\n");
        printf("3.借阅图书\n");
        printf("4.归还图书\n");
        printf("5.图书信息查询\n");
        printf("6.学生信息查询\n");
        printf("0.退出系统\n");
        scanf("%d", &choice);
        if (choice < 0 || choice > 6) {
            printf("输入错误");
            continue;
        }
        switch (choice) {
            case 0:
                break;
            case 1:
                students = insert_student_information(students);
                break;
            case 2:
                books = insert_book(books);
                break;
            case 3:
                borrow_book();
                break;
            case 4:
                return_book(books,students);
                break;
            case 5:
                book_information_enquiry(books);
                break;
            case 6:
                student_information_enquiry(students);
                break;
        }
    }
}
