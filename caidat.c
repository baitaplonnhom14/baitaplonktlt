#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
    #define strcasestr strstr
#endif

#define ACCOUNT_FILE "accounts.dat"
#define STUDENT_FILE "students.dat"

typedef enum { ROLE_ADMIN = 1, ROLE_STUDENT = 2 } Role;

typedef struct SinhVien {
    char maSV[11];
    char hoTen[50];
    float diemGK;
    float diemCK;
    float gpa;
    char diemChu[3];
    char xepLoai[20];
} SinhVien;

typedef struct DanhSachSinhVien {
    SinhVien *data;
    int capacity;
    int size;
} DanhSachSinhVien;

typedef struct TaiKhoan {
    char username[30];
    char password[30];
    Role role;
    char maSV[11];
} TaiKhoan;

typedef struct DanhSachTaiKhoan {
    TaiKhoan *data;
    int capacity;
    int size;
} DanhSachTaiKhoan;

// Khai báo nguyên mẫu hàm
void clearInputBuffer();
void inputString(char *buffer, int length);
void initStudentList(DanhSachSinhVien *list, int initialCapacity);
void freeStudentList(DanhSachSinhVien *list);
void initAccountList(DanhSachTaiKhoan *list, int initialCapacity);
void freeAccountList(DanhSachTaiKhoan *list);

void loadStudentsFromFile(DanhSachSinhVien *list);
void saveStudentsToFile(DanhSachSinhVien list);
void loadAccountsFromFile(DanhSachTaiKhoan *list);
void saveAccountsToFile(DanhSachTaiKhoan list);

void tinhGpaVaXepLoai(SinhVien *sv);
int findStudentIndexByMaSV(DanhSachSinhVien list, const char *maSV);
int findAccountByUsername(DanhSachTaiKhoan list, const char *username);
int isMaSVRegistered(DanhSachTaiKhoan list, const char *maSV);

void addStudent(DanhSachSinhVien *list, SinhVien sv);
void printStudentList(DanhSachSinhVien list);
void findStudentByName(DanhSachSinhVien list, const char tuKhoa[]);
void suaThongTinSinhVien(DanhSachSinhVien *list);
void xoaSinhVien(DanhSachSinhVien *list);
void sapXepGPAGiamDan(DanhSachSinhVien *list);

void menuDangKy(DanhSachTaiKhoan *accList, DanhSachSinhVien svList);
TaiKhoan* menuDangNhap(DanhSachTaiKhoan *accList);
void adminMenu(DanhSachSinhVien *svList);
void studentMenu(DanhSachSinhVien svList, TaiKhoan currentUser);
void tinhDiemNhapThu();


// ==========================================================
// PHAN CUA PHAM THAO VY: Quan li bo nho va ham nhap/xuat co ban
// ==========================================================

// Hàm nhập/xuat: Xóa bộ nhớ đệm bàn phím
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Hàm nhập: Nhập chuỗi an toàn (trống tràn bộ đệm)
void inputString(char *buffer, int length) {
    if (fgets(buffer, length, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            clearInputBuffer();
        }
    }
}

// Khởi tạo danh sách sinh viên
void initStudentList(DanhSachSinhVien *list, int initialCapacity) {
    list->data = (SinhVien *)malloc(initialCapacity * sizeof(SinhVien));
    if (!list->data) {
        printf("Loi cap phat bo nho danh sach sinh vien!\n");
        exit(1);
    }
    list->capacity = initialCapacity;
    list->size = 0;
}

// Giải phóng bộ nhớ danh sách sinh viên
void freeStudentList(DanhSachSinhVien *list) {
    if (list->data) free(list->data);
    list->data = NULL;
    list->capacity = 0;
    list->size = 0;
}

// Khởi tạo danh sách tài khoản
void initAccountList(DanhSachTaiKhoan *list, int initialCapacity) {
    list->data = (TaiKhoan *)malloc(initialCapacity * sizeof(TaiKhoan));
    if (!list->data) {
        printf("Loi cap phat bo nho danh sach tai khoan!\n");
        exit(1);
    }
    list->capacity = initialCapacity;
    list->size = 0;
}

// Giải phóng bộ nhớ danh sách tài khoản
void freeAccountList(DanhSachTaiKhoan *list) {
    if (list->data) free(list->data);
    list->data = NULL;
    list->capacity = 0;
    list->size = 0;
}

// Hàm nhập/xuất file: Đọc danh sách sinh viên từ file
void loadStudentsFromFile(DanhSachSinhVien *list) {
    FILE *f = fopen(STUDENT_FILE, "rb");
    if (!f) return;

    SinhVien sv;
    while (fread(&sv, sizeof(SinhVien), 1, f) == 1) {
        if (list->size >= list->capacity) {
            list->capacity *= 2;
            list->data = (SinhVien *)realloc(list->data, list->capacity * sizeof(SinhVien));
        }
        list->data[list->size++] = sv;
    }
    fclose(f);
}

// Hàm nhập/xuất file: Ghi danh sách sinh viên ra file
void saveStudentsToFile(DanhSachSinhVien list) {
    FILE *f = fopen(STUDENT_FILE, "wb");
    if (!f) {
        printf("Loi: Khong the ghi file sinh vien!\n");
        return;
    }
    fwrite(list.data, sizeof(SinhVien), list.size, f);
    fclose(f);
}

// Hàm nhập/xuất file: Đọc danh sách tài khoản từ file
void loadAccountsFromFile(DanhSachTaiKhoan *list) {
    FILE *f = fopen(ACCOUNT_FILE, "rb");
    if (!f) {
        TaiKhoan adminAcc = {"admin", "123456", ROLE_ADMIN, ""};
        list->data[0] = adminAcc;
        list->size = 1;
        saveAccountsToFile(*list);
        return;
    }

    TaiKhoan acc;
    while (fread(&acc, sizeof(TaiKhoan), 1, f) == 1) {
        if (list->size >= list->capacity) {
            list->capacity *= 2;
            list->data = (TaiKhoan *)realloc(list->data, list->capacity * sizeof(TaiKhoan));
        }
        list->data[list->size++] = acc;
    }
    fclose(f);
}

// Hàm nhập/xuất file: Ghi danh sách tài khoản ra file
void saveAccountsToFile(DanhSachTaiKhoan list) {
    FILE *f = fopen(ACCOUNT_FILE, "wb");
    if (!f) {
        printf("Loi: Khong the ghi file tai khoan!\n");
        return;
    }
    fwrite(list.data, sizeof(TaiKhoan), list.size, f);
    fclose(f);
}

int findStudentIndexByMaSV(DanhSachSinhVien list, const char *maSV) {
    for (int i = 0; i < list.size; i++) {
        if (strcmp(list.data[i].maSV, maSV) == 0) return i;
    }
    return -1;
}

int findAccountByUsername(DanhSachTaiKhoan list, const char *username) {
    for (int i = 0; i < list.size; i++) {
        if (strcmp(list.data[i].username, username) == 0) return i;
    }
    return -1;
}

int isMaSVRegistered(DanhSachTaiKhoan list, const char *maSV) {
    for (int i = 0; i < list.size; i++) {
        if (list.data[i].role == ROLE_STUDENT && strcmp(list.data[i].maSV, maSV) == 0) {
            return 1;
        }
    }
    return 0;
}

// Hàm nhập: Thêm một sinh viên vào danh sách
void addStudent(DanhSachSinhVien *list, SinhVien sv) {
    if (findStudentIndexByMaSV(*list, sv.maSV) != -1) {
        printf("Loi: Ma SV %s da ton tai trong he thong!\n", sv.maSV);
        return;
    }
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = (SinhVien *)realloc(list->data, list->capacity * sizeof(SinhVien));
        if (list->data == NULL) {
            printf("Loi nghiem trong: Khong the mo rong bo nho RAM!\n");
            exit(1);
        }
    }
    list->data[list->size++] = sv;
    printf("=> Them sinh vien thanh cong!\n");
}

// Hàm xuất: In toàn bộ danh sách sinh viên ra màn hình dạng bảng
void printStudentList(DanhSachSinhVien list) {
    if (list.size == 0) {
        printf("\nDanh sach sinh vien hien dang rong!\n");
        return;
    }
    printf("\n=========================================================================================================\n");
    printf("%-5s | %-12s | %-25s | %-8s | %-8s | %-6s | %-8s | %-12s\n",
           "STT", "Ma SV", "Ho va Ten", "Diem GK", "Diem CK", "GPA", "Diem Chu", "Xep Loai");
    printf("=========================================================================================================\n");
    for (int i = 0; i < list.size; i++) {
        printf("%-5d | %-12s | %-25s | %-8.2f | %-8.2f | %-6.2f | %-8s | %-12s\n",
               i + 1, list.data[i].maSV, list.data[i].hoTen, list.data[i].diemGK,
               list.data[i].diemCK, list.data[i].gpa, list.data[i].diemChu, list.data[i].xepLoai);
    }
    printf("=========================================================================================================\n");
}


// ==========================================================
// PHAN CUA NGUYEN CHI BAO: Ham tim kiem theo ten va sua thong tin
// ==========================================================

// Hàm tìm kiếm: Tìm kiếm sinh viên theo tên hoặc từ khóa (xuất kết quả tìm kiếm)
void findStudentByName(DanhSachSinhVien list, const char tuKhoa[]) {
    int dem = 0;
    printf("\n--- KET QUA TIM KIEM THEO TEN (%s) ---\n", tuKhoa);
    for (int i = 0; i < list.size; i++) {
        if (strcasestr(list.data[i].hoTen, tuKhoa) != NULL) {
            printf("Ma SV: %-10s | Ho ten: %-25s | Diem GK: %5.2f | Diem CK: %5.2f | GPA: %5.2f | Xep loai: %s\n",
                   list.data[i].maSV, list.data[i].hoTen, list.data[i].diemGK, list.data[i].diemCK, list.data[i].gpa, list.data[i].xepLoai);
            dem++;
        }
    }
    if (dem == 0) printf("Khong tim thay sinh vien nao phu hop!\n");
}

// Hàm nhập/sửa: Nhập và cập nhật thông tin mới cho sinh viên theo mã SV
void suaThongTinSinhVien(DanhSachSinhVien *list) {
    char maTimKiem[11];
    printf("\n--- SUA THONG TIN SINH VIEN ---\n");
    printf("Nhap Ma SV cua sinh vien can sua: ");
    inputString(maTimKiem, sizeof(maTimKiem));

    int idx = findStudentIndexByMaSV(*list, maTimKiem);
    if (idx == -1) {
        printf("Khong tim thay sinh vien co Ma SV: %s\n", maTimKiem);
        return;
    }

    printf("Nhap Ho va Ten moi: ");
    inputString(list->data[idx].hoTen, sizeof(list->data[idx].hoTen));

    do {
        printf("Nhap Diem Giua Ky moi (0 - 10): ");
        if (scanf("%f", &list->data[idx].diemGK) != 1 || list->data[idx].diemGK < 0 || list->data[idx].diemGK > 10) {
            printf("Diem khong hop le! Vui long nhap lai.\n");
            clearInputBuffer();
        } else {
            clearInputBuffer();
            break;
        }
    } while (1);

    do {
        printf("Nhap Diem Cuoi Ky moi (0 - 10): ");
        if (scanf("%f", &list->data[idx].diemCK) != 1 || list->data[idx].diemCK < 0 || list->data[idx].diemCK > 10) {
            printf("Diem khong hop le! Vui long nhap lai.\n");
            clearInputBuffer();
        } else {
            clearInputBuffer();
            break;
        }
    } while (1);

    tinhGpaVaXepLoai(&list->data[idx]);
    printf("=> Sua thong tin sinh vien thanh cong!\n");
}


// ==========================================================
// PHAN CUA NGUYEN NGOC THUY: Ham xoa va ham sap xep
// ==========================================================

// Hàm xóa: Xóa sinh viên khỏi danh sách dựa vào mã SV
void xoaSinhVien(DanhSachSinhVien *list) {
    char maXoa[11];
    printf("\n--- XOA SINH VIEN ---\n");
    printf("Nhap Ma SV can xoa: ");
    inputString(maXoa, sizeof(maXoa));
 
    int vt = findStudentIndexByMaSV(*list, maXoa);
    if (vt == -1) {
        printf("Khong tim thay sinh vien co ma: %s\n", maXoa);
    } else {
        for (int i = vt; i < list->size - 1; i++) {
            list->data[i] = list->data[i + 1];
        }
        list->size--;
        printf("=> Xoa sinh vien thanh cong!\n");
    }
}

// Hàm sắp xếp: Sắp xếp danh sách sinh viên theo điểm GPA giảm dần
void sapXepGPAGiamDan(DanhSachSinhVien *list) {
    if (list->size < 2) {
        printf("Danh sach qua it sinh vien de sap xep!\n");
        return;
    }
    for (int i = 0; i < list->size - 1; i++) {
        for (int j = i + 1; j < list->size; j++) {
            if (list->data[i].gpa < list->data[j].gpa) {
               SinhVien temp = list->data[i];
               list->data[i] = list->data[j];
               list->data[j] = temp;
            }
        }
    }
    printf("=> Sap xep danh sach theo GPA giam dan thanh cong!\n");
}


// ======================================================================
// PHAN CUA LE THI CAM TU: Tinh GPA, Xep loai hoc luc, Phan loai diem chu
// ======================================================================

// Hàm tính toán: Tính GPA (GK 40%, CK 60%), xếp loại học lực và quy đổi điểm chữ
void tinhGpaVaXepLoai(SinhVien *sv) {
    sv->gpa = (sv->diemGK * 0.4f) + (sv->diemCK * 0.6f);

    if (sv->gpa >= 8.5) strcpy(sv->xepLoai, "Xuat sac");
    else if (sv->gpa >= 7.0) strcpy(sv->xepLoai, "Gioi");
    else if (sv->gpa >= 5.5) strcpy(sv->xepLoai, "Kha");
    else if (sv->gpa >= 4.0) strcpy(sv->xepLoai, "Trung binh");
    else strcpy(sv->xepLoai, "Yeu");

    if (sv->gpa >= 8.5) strcpy(sv->diemChu, "A");
    else if (sv->gpa >= 8.0) strcpy(sv->diemChu, "B+");
    else if (sv->gpa >= 7.0) strcpy(sv->diemChu, "B");
    else if (sv->gpa >= 6.5) strcpy(sv->diemChu, "C+");
    else if (sv->gpa >= 5.5) strcpy(sv->diemChu, "C");
    else if (sv->gpa >= 5.0) strcpy(sv->diemChu, "D+");
    else if (sv->gpa >= 4.0) strcpy(sv->diemChu, "D");
    else strcpy(sv->diemChu, "F");
}


// ==========================================================
// PHAN HE THONG MENU, DANG NHAP, DANG KY VA MAIN CHUNG
// ==========================================================

// Hàm nhập: Đăng ký tài khoản mới (Admin hoặc Sinh viên)
void menuDangKy(DanhSachTaiKhoan *accList, DanhSachSinhVien svList) {
    TaiKhoan newAcc;
    printf("\n=== DANG KY TAI KHOAN MOI ===\n");
    printf("Chon loai tai khoan (1. Giao vien | 2. Sinh vien): ");
    int choice;
    if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Lua chon khong hop le!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    newAcc.role = (choice == 1) ? ROLE_ADMIN : ROLE_STUDENT;

    if (newAcc.role == ROLE_STUDENT) {
        printf("Nhap Ma SV cua ban: ");
        inputString(newAcc.maSV, sizeof(newAcc.maSV));

        if (findStudentIndexByMaSV(svList, newAcc.maSV) == -1) {
            printf("Loi: Ma SV nay chua co trong he thong cua nha truong!\n");
            return;
        }
        if (isMaSVRegistered(*accList, newAcc.maSV)) {
            printf("Loi: Ma SV nay da duoc tao tai khoan truoc do!\n");
            return;
        }
    } else {
        strcpy(newAcc.maSV, "");
    }

    printf("Nhap Ten dang nhap: ");
    inputString(newAcc.username, sizeof(newAcc.username));

    if (findAccountByUsername(*accList, newAcc.username) != -1) {
        printf("Loi: Ten dang nhap da ton tai! Vui long chon ten khac.\n");
        return;
    }

    printf("Nhap Mat khau: ");
    inputString(newAcc.password, sizeof(newAcc.password));

    if (accList->size >= accList->capacity) {
        accList->capacity *= 2;
        accList->data = (TaiKhoan *)realloc(accList->data, accList->capacity * sizeof(TaiKhoan));
    }
    accList->data[accList->size++] = newAcc;
    saveAccountsToFile(*accList);
    printf("=> Dang ky tai khoan thanh cong!\n");
}

// Hàm nhập: Đăng nhập vào hệ thống
TaiKhoan* menuDangNhap(DanhSachTaiKhoan *accList) {
    char user[30], pass[30];
    printf("\n=== DANG NHAP HE THONG ===\n");
    printf("Ten dang nhap: ");
    inputString(user, sizeof(user));
    printf("Mat khau: ");
    inputString(pass, sizeof(pass));

    int idx = findAccountByUsername(*accList, user);
    if (idx != -1 && strcmp(accList->data[idx].password, pass) == 0) {
        printf("=> Dang nhap thanh cong!\n");
        return &accList->data[idx];
    }

    printf("Loi: Sai ten dang nhap hoac mat khau!\n");
    return NULL;
}

// Giao diện menu dành cho giáo viên (Admin)
void adminMenu(DanhSachSinhVien *svList) {
    int choice;
    do {
        printf("\n========================================================\n");
        printf("        PANEL QUAN LY DANG CHO GIAO VIEN (ADMIN)        \n");
        printf("========================================================\n");
        printf("1. Nhap / Them sinh vien moi\n");
        printf("2. Hien thi danh sach sinh vien\n");
        printf("3. Sap xep danh sach theo GPA giam dan\n");
        printf("4. Tim kiem sinh vien theo ten\n");
        printf("5. Sua thong tin sinh vien theo Ma SV\n");
        printf("6. Xoa sinh vien theo Ma SV\n");
        printf("0. Dang xuat\n");
        printf("========================================================\n");
        printf("Vui long chon chuc nang (0-6): ");

        if (scanf("%d", &choice) != 1) {
            printf("Loi: Nhap so nguyen hop le!\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: {
                SinhVien sv;
                printf("\n--- NHAP THONG TIN SINH VIEN MOI --- \n");
                printf("Nhap Ma SV: ");
                inputString(sv.maSV, sizeof(sv.maSV));
                printf("Nhap Ho va Ten: ");
                inputString(sv.hoTen, sizeof(sv.hoTen));
 
                do {
                    printf("Nhap Diem Giua Ky (0 - 10): ");
                    if (scanf("%f", &sv.diemGK) != 1 || sv.diemGK < 0 || sv.diemGK > 10) {
                        printf("Diem khong hop le!\n");
                        clearInputBuffer();
                    } else { clearInputBuffer(); break; }
                } while (1);

                do {
                    printf("Nhap Diem Cuoi Ky (0 - 10): ");
                    if (scanf("%f", &sv.diemCK) != 1 || sv.diemCK < 0 || sv.diemCK > 10) {
                        printf("Diem khong hop le!\n");
                        clearInputBuffer();
                    } else { clearInputBuffer(); break; }
                } while (1);

                tinhGpaVaXepLoai(&sv);
                addStudent(svList, sv);
                saveStudentsToFile(*svList);
                break;
            }
            case 2:
                printStudentList(*svList);
                break;
            case 3:
                sapXepGPAGiamDan(svList);
                printStudentList(*svList);
                saveStudentsToFile(*svList);
                break;
            case 4: {
                if (svList->size == 0) {
                    printf("\nDanh sach rong!\n");
                    break;
                }
                char tuKhoa[50];
                printf("Nhap ten/tu khoa can tim: ");
                inputString(tuKhoa, sizeof(tuKhoa));
                findStudentByName(*svList, tuKhoa);
                break;
            }
            case 5:
                suaThongTinSinhVien(svList);
                saveStudentsToFile(*svList);
                break;
            case 6:
                xoaSinhVien(svList);
                saveStudentsToFile(*svList);
                break;
            case 0:
                printf("=> Da dang xuat tai khoan Admin!\n");
                break;
            default:
                printf("Lua chon khong hop le!\n");
                break;
        }
    } while (choice != 0);
}

// Công cụ tính điểm thử nghiệm cá nhân (không lưu file)
void tinhDiemNhapThu() {
    SinhVien svTemp;
    printf("\n--- CONG CU TU TINH DIEM CA NHAN (KHONG LUU) ---\n");
    do {
        printf("Nhap Diem Giua Ky du kien (0 - 10): ");
        if (scanf("%f", &svTemp.diemGK) != 1 || svTemp.diemGK < 0 || svTemp.diemGK > 10) {
            printf("Diem khong hop le!\n");
            clearInputBuffer();
        } else { clearInputBuffer(); break; }
    } while (1);

    do {
        printf("Nhap Diem Cuoi Ky du kien (0 - 10): ");
        if (scanf("%f", &svTemp.diemCK) != 1 || svTemp.diemCK < 0 || svTemp.diemCK > 10) {
            printf("Diem khong hop le!\n");
            clearInputBuffer();
        } else { clearInputBuffer(); break; }
    } while (1);

    tinhGpaVaXepLoai(&svTemp);
    printf("\n=> KET QUA DU KIEN:\n");
    printf("- Diem GPA: %.2f\n", svTemp.gpa);
    printf("- Diem Chu: %s\n", svTemp.diemChu);
    printf("- Xep Loai : %s\n", svTemp.xepLoai);
    printf("(Luu y: Ket qua nay chi de tham khao va khong luu vao he thong)\n");
}

// Giao diện menu dành cho sinh viên
void studentMenu(DanhSachSinhVien svList, TaiKhoan currentUser) {
    int choice;
    do {
        printf("\n========================================================\n");
        printf("         PORTAL TRA CUU DANG CHO SINH VIEN            \n");
        printf("========================================================\n");
        printf("1. Tra cuu diem & thong tin ca nhan\n");
        printf("2. Tu tinh diem nhap thu (Khong luu)\n");
        printf("0. Dang xuat\n");
        printf("========================================================\n");
        printf("Vui long chon chuc nang (0-2): ");

        if (scanf("%d", &choice) != 1) {
            printf("Loi: Nhap so nguyen hop le!\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: {
                int idx = findStudentIndexByMaSV(svList, currentUser.maSV);
                if (idx == -1) {
                    printf("\nLoi: Khong tim thay thong tin diem cua Ma SV %s!\n", currentUser.maSV);
                } else {
                    SinhVien sv = svList.data[idx];
                    printf("\n--- THONG TIN DIEM CA NHAN ---\n");
                    printf("Ma SV    : %s\n", sv.maSV);
                    printf("Ho va Ten: %s\n", sv.hoTen);
                    printf("Diem GK  : %.2f\n", sv.diemGK);
                    printf("Diem CK  : %.2f\n", sv.diemCK);
                    printf("GPA      : %.2f\n", sv.gpa);
                    printf("Diem Chu : %s\n", sv.diemChu);
                    printf("Xep Loai : %s\n", sv.xepLoai);
                }
                break;
            }
            case 2:
                tinhDiemNhapThu();
                break;
            case 0:
                printf("=> Da dang xuat!\n");
                break;
            default:
                printf("Lua chon khong hop le!\n");
                break;
        }
    } while (choice != 0);
}

// Hàm chính điều phối chương trình
int main() {
    DanhSachSinhVien svList;
    DanhSachTaiKhoan accList;

    initStudentList(&svList, 10);
    initAccountList(&accList, 10);

    loadStudentsFromFile(&svList);
    loadAccountsFromFile(&accList);

    int choice;
    do {
        printf("\n========================================================\n");
        printf("   HE THONG DANG NHAP QUAN LY SINH VIEN & DIEM          \n");
        printf("========================================================\n");
        printf("1. Dang nhap\n");
        printf("2. Dang ky tai khoan\n");
        printf("0. Thoat\n");
        printf("========================================================\n");
        printf("Vui long chon (0-2): ");

        if (scanf("%d", &choice) != 1) {
            printf("Loi: Nhap so nguyen hop le!\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: {
                TaiKhoan *user = menuDangNhap(&accList);
                if (user != NULL) {
                    if (user->role == ROLE_ADMIN) {
                        adminMenu(&svList);
                    } else {
                        studentMenu(svList, *user);
                    }
                }
                break;
            }
            case 2:
                menuDangKy(&accList, svList);
                break;
            case 0:
                printf("\n=> Luu du lieu va thoat chuong trinh...\n");
                saveStudentsToFile(svList);
                saveAccountsToFile(accList);
                freeStudentList(&svList);
                freeAccountList(&accList);
                printf("=> Da thoat an toan!\n");
                break;
            default:
                printf("Lua chon khong hop le!\n");
                break;
        }
    } while (choice != 0);

    return 0;
}
