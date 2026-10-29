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
