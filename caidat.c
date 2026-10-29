#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/header.h" 
#include "student.h"

//xoa bo nho dem tu ban phim
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
//chong tran bo nho va kiem tra do dai cua thong tin nhap vao
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
//khoi tao mang dong
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

   
    return sv;
// ==========================================================
// PHAN CUA NGUYEN NGOC THUY: Ham xoa va ham sap xep
// ==========================================================
/ Ham xoa 
void xoaSinhVien(struct SinhVien danhSach[], int *n) {
    char maXoa[20]; int vt = -1;
    printf("Nhap ma can xoa: "); scanf("%s", maXoa);
    for (int i = 0; i < *n; i++) {
        if (strcmp(danhSach[i].maSV, maXoa) == 0) { vt = i; break; }
    }
    if (vt == -1) printf("Khong tim thay!\n");
    else {
        for (int i = vt; i < *n - 1; i++) danhSach[i] = danhSach[i + 1];
        (*n)--; printf("Xoa thanh cong!\n");
    }
}

//Ham sap xep
void sapXepGPAGiamDan(struct SinhVien danhSach[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (danhSach[i].gpa < danhSach[j].gpa) {
                struct SinhVien temp = danhSach[i];
                danhSach[i] = danhSach[j];
                danhSach[j] = temp;
            }
        }
    }
    printf("Sap xep xong!\n");
}
