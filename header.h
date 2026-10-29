// ==========================================================
// PHAN CUA PHAM THAO VY: Khai bao Struct va Prototype
// ==========================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef STUDENT_H
#define STUDENT_H
// struct luu tru sinh vien
typedef struct SinhVien {
    char maSV[11];       
    char hoTen[50];     
    float diemGK;      
    float diemCK;       
    float gpa;          
    char diemChu[3];     
    char xepLoai[20];   
} SinhVien;

// quan li nhan vien
typedef struct DanhSachSinhVien {
    SinhVien *data;      
    int capacity;        
    int size;          
} DanhSachSinhVien;
// Khai khai bao ham nhap, xuat va bo nho
void clearInputBuffer();
void inputString(char *buffer, int length);
void initList(DanhSachSinhVien *list, int initialCapacity);
void freeList(DanhSachSinhVien *list);
void tinhGpaVaXepLoai(SinhVien *sv);
SinhVien inputOneStudent(DanhSachSinhVien *list);
void addStudent(DanhSachSinhVien *list, SinhVien sv);
void printStudentList(DanhSachSinhVien list);
void find_student_by_name(DanhSachSinhVien list, char tuKhoa[]);
void suaThongTinSinhVien(DanhSachSinhVien *list);
void xoaSinhVien(DanhSachSinhVien *list);
void sapXepGPAGiamDan(DanhSachSinhVien *list);
void displayMenu();
#endif
