#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#define Max 100

typedef struct book {
    char book_num[10]; /*图书编号*/
    char book_name[20]; /*图书名称*/
    char book_writer[20]; /*作者*/
    int book_stock; /*现存量*/
    int book_original_quantity; /*库存量*/
    struct book *next;
    int length;
} Book;

typedef struct borrow {
    int borrow_count;
    char borrow_book_num[10]; //借阅书籍的编号
    char limit_date[100]; //借阅的截止日期
} BookBorrow;

typedef struct reader {
    char reader_num[20]; /*学生学号*/
    char reader_name[20]; /*学生姓名*/
    int permission; /*1可借 0 不可借*/
    BookBorrow borrow[Max]; /*最大借书量*/
    struct reader *next;
    int length;
} Student;


void wait_for_keypress() {
    printf("按任意键返回主菜单...");
    while (getchar() != '\n'); // Clear the buffer
    getchar(); // Wait for a key press
};

Student *find_student(Student *students, const char *input_num) {
    while (students != NULL && strcmp(students->reader_num, input_num) != 0) {
        students = students->next;
    }
    return students;
}

Book *find_book(Book *book, const char *input_book_num) {
    while (book != NULL && strcmp(book->book_num, input_book_num)) {
        book = book->next;
    }
    return book;
}


Student *insert_student_information(Student *student) {
    printf("\n学生信息录入：\n");
    int student_number = 1;
    printf("您此次需要添加几条学生信息？");
    scanf("%d", &student_number);
    for (int i = 0; i < student_number; i++) {
        Student *new_student = (Student *) malloc(sizeof(Student));
        if (new_student == NULL) {
            printf("内存分配失败！\n");
            wait_for_keypress();
            return student;
        }
        printf("\n请输入学生信息：\n");
        printf("学号:"); /*输入学号*/
        scanf("%s", new_student->reader_num);
        if (find_student(student, new_student->reader_num)) {
            printf("此学生信息已存在");
            free(new_student);
            wait_for_keypress();
            return student;
        }
        printf("姓名:"); /*输入姓名*/
        scanf("%s", new_student->reader_name);
        printf("权限:"); /*输入权限*/
        scanf("%d", &new_student->permission);
        new_student->next = NULL;
        for (int i = 0; i < Max; i++) {
            new_student->borrow[i].borrow_count=0;
            new_student->borrow[i].borrow_book_num[0] = '\0';
            new_student->borrow[i].limit_date[0] = '\0';
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
            return books;
        }
        printf("\n请输入书籍信息：\n");
        printf("图书编号:"); /*输入图书编号(唯一)*/
        scanf("%s", new_book->book_num);
        Book *exsiting_book = find_book(books, new_book->book_num);
        if (exsiting_book != NULL) {
            printf("此图书已存在于管中，请输入入库数量");
            scanf("%d", &new_book->book_original_quantity);
            exsiting_book->book_original_quantity += new_book->book_original_quantity;
            exsiting_book->book_stock += new_book->book_original_quantity;
            free(new_book);
        } else {
            printf("图书名称:"); /*输入图书名称*/
            scanf("%s", new_book->book_name);
            printf("图书作者:"); /*输入图书作者*/
            scanf("%s", new_book->book_writer);
            printf("图书数量:"); /*输入图书数量*/
            scanf("%d", &new_book->book_original_quantity);
            new_book->book_stock = new_book->book_original_quantity; /*开始时图书现有量和库存量相等*/
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

void borrow_book(Book *books, Student *students) {
    char reader_num[20];
    char book_num[10];
    char return_date[100];
    printf("\n借书\n");
    // 输入学号
    printf("请输入学号: ");
    scanf("%s", reader_num); // 防止缓冲区溢出
    // 查找学生
    Student *student = find_student(students, reader_num);
    if (student == NULL) {
        printf("没有找到该学生信息。\n");
        wait_for_keypress();
        return;
    }
    // 检查学生是否有借书权限
    if (student->permission == 0) {
        printf("该学生没有借书权限。\n");
        wait_for_keypress();
        return;
    }
    // 输入图书编号
    printf("请输入图书编号: ");
    scanf("%s", book_num); // 防止缓冲区溢出
    // 查找图书
    Book *book = find_book(books, book_num);
    if (book == NULL) {
        printf("没有找到该图书信息。\n");
        wait_for_keypress();
        return;
    }
    // 检查图书的现存量
    if (book->book_stock <= 0) {
        printf("该图书没有现存量。\n");
        wait_for_keypress();
        return;
    }
    // 检查学生是否已经借了这本书
    for (int i = 0; i < student->borrow->borrow_count; i++) {
        if (strcmp(student->borrow[i].borrow_book_num, book_num) == 0) {
            printf("该学生已经借了这本书。\n");
            wait_for_keypress();
            return;
        }
    }
    // 输入还书日期
    printf("请输入还书日期 (格式: YYYY-MM-DD): ");
    scanf("%s", return_date);
    // 更新图书的现存量
    book->book_stock--;
    // 更新学生的借书数，并添加借出的书
    if (student->borrow->borrow_count < Max) {
        // 确保不超过最大借书量
        strcpy(student->borrow[student->borrow->borrow_count].borrow_book_num, book_num);
        strcpy(student->borrow[student->borrow->borrow_count].limit_date, return_date);
        student->borrow->borrow_count++;
    } else {
        printf("已达到最大借书量！\n");
        student->permission = 0; //达到最大借书本数，不允许再借新书。
        wait_for_keypress();
        return;
    }
    printf("借书成功！\n");
    wait_for_keypress();
}


void return_book(Book *books, Student *students) {
    int i;
    char return_book_num[10], return_reader_num[20];
    printf("\n归还书籍\n");
    printf("\n请输入学号:");
    scanf("%s", return_reader_num);

    // 查找学生
    Student *student = find_student(students, return_reader_num);
    if (student == NULL) {
        printf("\n此学号不存在!\n");
        wait_for_keypress();
        return;
    }
    printf("\n请输入归还书籍编号:");
    scanf("%s", return_book_num);
    Book *borrow_book = find_book(books, return_book_num);
    if (borrow_book == NULL) {
        printf("此图书不存在");
        wait_for_keypress();
        return;
    }
    // 查找书籍编号在学生的借书列表中
    for (i = 0; i < student->borrow->borrow_count; i++) {
        if (strcmp(return_book_num, student->borrow[i].borrow_book_num) == 0) {
            char current_date[100];
            printf("请输入当前日期 (格式: YYYY-MM-DD): ");
            scanf("%s", current_date);
            if (strcmp(current_date, student->borrow[i].limit_date) > 0) {
                printf("该书籍已超期归还。\n");
            } else {
                printf("归还日期在规定时间内。\n");
            }
            // 将后续借书记录向前移动
            for (int j = i; j < student->borrow->borrow_count - 1; j++) {
                strcpy(student->borrow[j].borrow_book_num, student->borrow[j + 1].borrow_book_num);
                strcpy(student->borrow[j].limit_date, student->borrow[j + 1].limit_date);
            }
            // 清除最后一个借书记录
            strcpy(student->borrow[student->borrow->borrow_count - 1].borrow_book_num, "0");
            strcpy(student->borrow[student->borrow->borrow_count - 1].limit_date, "0");
            student->borrow->borrow_count--; // 减少借书计数
            borrow_book->book_stock++;
            printf("\n学号%s的学生还书完毕!", return_reader_num);
            if (student->borrow->borrow_count < Max) {
                student->permission=1;
            }
            break; // 找到后退出循环
        }
        printf("\n错误，该学生未借此书!\n");
    }
    wait_for_keypress();
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
        printf("现存量: %d    ", current->book_stock);
        printf("库存量: %d    \n", current->book_original_quantity);
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
        printf("借出书本数量: %d    ", current->borrow->borrow_count);
        printf("借出书籍编号:%s    ", current->borrow->borrow_book_num);
        printf("归还日期:%s    ", current->borrow->limit_date);
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

int main() {
    Book *books = NULL;
    Student *students = NULL;
    int choice;
    while (true) {
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
                exit(0);
            case 1:
                students = insert_student_information(students);
                break;
            case 2:
                books = insert_book(books);
                break;
            case 3:
                borrow_book(books, students);
                break;
            case 4:
                return_book(books, students);
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
