// ==========================================================
// PHAN CUA PHAM THAO VY: Ham nhap/xuat va quan li bo nho
// ==========================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/header.h" 
#include "student.h"

// xoa bo nho dem tu ban phim
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// chong tran bo nho va kiem tra do dai cua thong tin nhap vao
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

// khoi tao mang dong
void initList(DanhSachSinhVien *list, int initialCapacity) {
    list->data = (SinhVien *)malloc(initialCapacity * sizeof(SinhVien));
    if (list->data == NULL) {
        printf("Loi: Khong du bo nho cap phat!\n");
        exit(1);
    }
    list->capacity = initialCapacity;
    list->size = 0;
}

// giai phong bo nho
void freeList(DanhSachSinhVien *list) {
    if (list->data != NULL) {
        free(list->data);
        list->data = NULL;
    }
    list->capacity = 0;
    list->size = 0;
}

// nhap thong tin sinh vien
SinhVien inputOneStudent(DanhSachSinhVien *list) {
    SinhVien sv;
 
    printf("\n--- NHAP THONG TIN SINH VIEN MONG MUON --- \n");
    printf("Nhap Ma SV: ");
    inputString(sv.maSV, sizeof(sv.maSV));

    printf("Nhap Ho va Ten: ");
    inputString(sv.hoTen, sizeof(sv.hoTen));

    do {
        printf("Nhap Diem Giua Ky: ");
        if (scanf("%f", &sv.diemGK) != 1 || sv.diemGK < 0 || sv.diemGK > 10) {
            printf("Diem khong hop le! Vui long nhap lai.\n");
            clearInputBuffer();
        } else {
            clearInputBuffer();
            break;
        }
    } while (1);

    do {
        printf("Nhap Diem Cuoi Ky: ");
        if (scanf("%f", &sv.diemCK) != 1 || sv.diemCK < 0 || sv.diemCK > 10) {
            printf("Diem khong hop le! Vui long nhap lai.\n");
            clearInputBuffer();
        } else {
            clearInputBuffer();
            break;
        }
    } while (1);

    // Tu dong tinh toan GPA va Xep loai truoc khi tra ve
    tinhGpaVaXepLoai(&sv);
   
    return sv;
} 

// ==========================================================
// PHAN CUA NGUYEN Chi Bao: Ham tim kiem theo ten sinh vien
// ==========================================================
// Tim kiem theo chuoi ho ten
void find_student_by_name(DanhSachSinhVien *list, char tuKhoa[]) {
    int dem = 0;
    printf("\n--- KET QUA TIM KIEM THEO TEN (%s) ---\n", tuKhoa);
    
    for (int i = 0; i < list->size; i++) {
        // ĐÃ SỬA: Chuyển sang strstr tiêu chuẩn và đồng bộ thuộc tính điểm diemGK, diemCK, gpa
if (strstr(list->data[i].hoTen, tuKhoa) != NULL) {
            printf("Ma SV: %s | Ho ten: %s | Diem GK: %.2f | Diem CK: %.2f | GPA: %.2f (%s)\n",
                   list->data[i].maSV, list->data[i].hoTen, 
                   list->data[i].diemGK, list->data[i].diemCK, 
                   list->data[i].gpa, list->data[i].diemChu);
            dem++;
        }
    }
    
    if (dem == 0) {
        printf("Khong tim thay sinh vien nao phu hop!\n");
    }
}
void suaThongTinSinhVien(DanhSachSinhVien *list) {
    char maTimKiem[50];
    int timThay = -1;

    printf("\n--- SUA THONG TIN SINH VIEN ---\n");
    printf("Nhap Ma SV cua sinh vien can sua: ");
    
    inputString(maTimKiem, sizeof(maTimKiem));

    // Duyet mang tim kiem theo Ma SV
    for (int i = 0; i < list->size; i++) {
        if (strcmp(list->data[i].maSV, maTimKiem) == 0) {
            timThay = i;
            break; 
        }
    }

    // Neu khong tim thay
    if (timThay == -1) {
        printf("Khong tim thay sinh vien co Ma SV la: %s\n", maTimKiem);
        return;
    }

    // Hien thi thong tin cu
    printf("\nDa tim thay sinh vien!\n");
    printf("Thong tin hien tai -> Ma SV: %s | Ho ten: %s | Diem GK: %.2f | Diem CK: %.2f\n",
           list->data[timThay].maSV, list->data[timThay].hoTen, 
           list->data[timThay].diemGK, list->data[timThay].diemCK);

    // Nhap thong tin moi
    printf("\n--- NHAP THONG TIN MOI CHO SINH VIEN ---\n");
    
    printf("Nhap Ho va Ten moi: ");
    inputString(list->data[timThay].hoTen, sizeof(list->data[timThay].hoTen));

    // Nhap diem giua ky moi
    do {
        printf("Nhap Diem Giua Ky moi (0 - 10): ");
        if (scanf("%f", &list->data[timThay].diemGK) != 1 || list->data[timThay].diemGK < 0 || list->data[timThay].diemGK > 10) {
            printf("Diem khong hop le! Vui long nhap lai.\n");
            clearInputBuffer();
        } else {
            clearInputBuffer();
            break;
        }
    } while (1);

    // Nhap diem cuoi ky moi
    do {
        printf("Nhap Diem Cuoi Ky moi (0 - 10): ");
        if (scanf("%f", &list->data[timThay].diemCK) != 1 || list->data[timThay].diemCK < 0 || list->data[timThay].diemCK > 10) {
            printf("Diem khong hop le! Vui long nhap lai.\n");
            clearInputBuffer();
        } else {
            clearInputBuffer();
            break;
        }
    } while (1);

    // Tu dong tinh lai GPA va Xep loai 
    tinhGpaVaXepLoai(&list->data[timThay]);

    printf("Sua thong tin sinh vien thanh cong!\n");
}

// ==========================================================
// PHAN CUA NGUYEN NGOC THUY: Ham xoa va ham sap xep
// ==========================================================
void xoaSinhVien(DanhSachSinhVien *list) {
    char maXoa[20]; 
    int vt = -1;
printf("Nhap ma can xoa: "); 
    scanf("%s", maXoa);
    clearInputBuffer();

    for (int i = 0; i < list->size; i++) {
        if (strcmp(list->data[i].maSV, maXoa) == 0) { 
            vt = i; 
            break; 
        }
    }

    if (vt == -1) {
        printf("Khong tim thay!\n");
    } else {
      
        for (int i = vt; i < list->size - 1; i++) {
            list->data[i] = list->data[i + 1];
        }
        (list->size)--; 
        printf("Xoa thanh cong!\n");
    }
}

// Ham sap xep
void sapXepGPAGiamDan(DanhSachSinhVien *list) {
    for (int i = 0; i < list->size - 1; i++) {
        for (int j = i + 1; j < list->size; j++) {
            if (list->data[i].gpa < list->data[j].gpa) {
               SinhVien temp = list->data[i];
               list->data[i] = list->data[j];
               list->data[j] = temp;
            }
        }
    }
    printf("Sap xep xong!\n");
}

// ======================================================================
// PHAN CUA LE THI CAM TU: Tinh GPA, Xep loai hoc luc, Phan loai diem chu
// ======================================================================
void tinhGpaVaXepLoai(SinhVien *sv) {
    // Tinh diem GPA he so 10 (Giua ki 40%, Cuoi ky 60%)
    sv->gpa = (sv->diemGK * 0.4f) + (sv->diemCK * 0.6f);

    // Xep loai hoc luc
    if (sv->gpa >= 8.5) {
        strcpy(sv->xepLoai, "Xuat sac");
    } else if (sv->gpa >= 7.0) {
        strcpy(sv->xepLoai, "Gioi");
    } else if (sv->gpa >= 5.5) {
        strcpy(sv->xepLoai, "Kha");
    } else if (sv->gpa >= 4.0) {
        strcpy(sv->xepLoai, "Trung binh");
    } else {
        strcpy(sv->xepLoai, "Yeu");
    }

    // Phan loai diem chu (Chuan he tinh chi)
    if (sv->gpa >= 8.5) {
        strcpy(sv->diemChu, "A");
    } else if (sv->gpa >= 8.0) {
        strcpy(sv->diemChu, "B+");
    } else if (sv->gpa >= 7.0) {
        strcpy(sv->diemChu, "B");
    } else if (sv->gpa >= 6.5) {
        strcpy(sv->diemChu, "C+");
    } else if (sv->gpa >= 5.5) {
        strcpy(sv->diemChu, "C");
    } else if (sv->gpa >= 5.0) {
        strcpy(sv->diemChu, "D+");
    } else if (sv->gpa >= 4.0) {
        strcpy(sv->diemChu, "D");
    } else {
        strcpy(sv->diemChu, "F");
    }
}
