/*
 * ============================================================================
 *  BAO CAO DEBUG - Bai 3: Kiem dinh nhap kho lanh SmartLog
 * ============================================================================
 *
 * 1. VI TRI LOI
 *    Ham kiem_tra_nhap_kho_lanh(), phan "2. Kiem tra dieu kien nhiet do":
 *
 *        // Code cu (SAI)
 *        if (nhiet_do_bao_quan >= NHIET_DO_MIN || nhiet_do_bao_quan <= NHIET_DO_MAX)
 *
 *        // Code moi (DUNG)
 *        if (nhiet_do_bao_quan >= NHIET_DO_MIN && nhiet_do_bao_quan <= NHIET_DO_MAX)
 *
 * 2. NGUYEN NHAN
 *    Nhiet do hop le phai nam TRONG doan [-18.0, 5.0], tuc la dong thoi thoa
 *    hai dieu kien: >= -18.0 VA <= 5.0. Code cu dung toan tu || (hoac) nen chi
 *    can mot ve dung la du. Ma voi moi so thuc bat ky:
 *      - nhiet do > 5.0   -> ve ">= -18.0" dung  -> ca bieu thuc dung
 *      - nhiet do < -18.0 -> ve "<= 5.0" dung    -> ca bieu thuc dung
 *      - nhiet do trong doan -> ca hai ve deu dung
 *    Vi vay bieu thuc luon bang true, nhanh "return 0" khong bao gio chay,
 *    nen 25.0 do C (hang kho) va -25.0 do C (dong sau) deu duoc duyet nhap kho
 *    lanh.
 *    Phan kiem tra tai trong (buoc 1) da cong dung tai trong hien tai voi khoi
 *    luong pallet nen khong sai.
 *
 * 3. BANG TEST CASE
 *    (Tai trong hien tai trong main() la 320.0 kg; cot cuoi dung de thay doi
 *     tai trong khi goi ham truc tiep.)
 *
 *   | # | Khoi luong | Nhiet do | Tai trong hien tai | Ket qua SAI (code cu) | Ket qua DUNG (SmartLog)          |
 *   |---|-----------:|---------:|-------------------:|-----------------------|----------------------------------|
 *   | 1 |   100.0 kg |  25.0 C  |           320.0 kg | PHE DUYET             | TU CHOI (nhiet do > 5.0)         |
 *   | 2 |   100.0 kg | -25.0 C  |           320.0 kg | PHE DUYET             | TU CHOI (nhiet do < -18.0)       |
 *   | 3 |    50.0 kg |  -5.0 C  |           320.0 kg | PHE DUYET             | PHE DUYET (370 kg, nhiet do ok)  |
 *   | 4 |   100.0 kg | -18.0 C  |           320.0 kg | PHE DUYET             | PHE DUYET (bien duoi, bao gom)   |
 *   | 5 |   100.0 kg |   5.0 C  |           320.0 kg | PHE DUYET             | PHE DUYET (bien tren, bao gom)   |
 *   | 6 |   180.0 kg |   0.0 C  |           320.0 kg | PHE DUYET             | PHE DUYET (tong dung 500 kg)     |
 *   | 7 |   180.1 kg |   0.0 C  |           320.0 kg | TU CHOI (dung)        | TU CHOI (tong 500.1 kg > 500)    |
 *
 *    Ca 1 va 2 chinh la su co ma bo phan giam sat chat luong bao cao.
 *    Ca 3-7 la ca doi chung de dam bao ban sua khong chan nham hang hop le.
 *
 * 4. THAY DOI THEM (de chuong trinh chay dung voi moi du lieu nhap)
 *    - Kiem tra gia tri tra ve cua scanf: nhap chu thay vi so se bi bao loi
 *      thay vi dung bien chua khoi tao.
 *    - Tu choi khoi luong pallet <= 0 (khoi luong am se lam giam tai trong
 *      ke, khong hop le).
 *
 * Bien dich: gcc -Wall -Wextra bai3_debug_kho_lanh.c -o bai3_debug_kho_lanh
 * ============================================================================
 */

#include <stdio.h>

// Dinh nghia cac hang so nghiep vu kho hang
#define TAI_TRONG_TOI_DA 500.0  // Tải trọng tối đa ô kệ (kg)
#define NHIET_DO_MIN -18.0      // Nhiệt độ an toàn tối thiểu (độ C)
#define NHIET_DO_MAX 5.0        // Nhiệt độ an toàn tối đa (độ C)

// Ham kiem tra va thuc hien nhap pallet vao ke kho lanh
// Tra ve 1 neu duoc phep nhap kho, tra ve 0 neu bi tu choi
int kiem_tra_nhap_kho_lanh(float khoi_luong_pallet, float nhiet_do_bao_quan, float tai_trong_hien_tai) {
    // 1. Kiem tra dieu kien tai trong ke kho
    // Tong tai trong sau khi xep them phai KHONG vuot qua 500.0 kg
    // (dung dau ">" nen tong dung bang 500.0 kg van duoc chap nhan)
    if (tai_trong_hien_tai + khoi_luong_pallet > TAI_TRONG_TOI_DA) {
        printf("Loi: Tai trong vuot qua muc cho phep (Toi da %.1f kg)!\n", TAI_TRONG_TOI_DA);
        return 0;
    }

    // 2. Kiem tra dieu kien nhiet do an toan kho lanh
    // SUA LOI: doi || thanh &&
    // Nhiet do hop le khi nam trong doan [-18.0, 5.0], tuc la DONG THOI:
    //   - lon hon hoac bang -18.0 (khong qua lanh), VA
    //   - nho hon hoac bang 5.0   (khong qua nong).
    // Dung >= va <= de hai moc bien -18.0 va 5.0 van duoc chap nhan.
    if (nhiet_do_bao_quan >= NHIET_DO_MIN && nhiet_do_bao_quan <= NHIET_DO_MAX) {
        return 1;
    } else {
        printf("Loi: Nhiet do bao quan %.1f do C khong phu hop kho lanh!\n", nhiet_do_bao_quan);
        return 0;
    }
}

int main() {
    float tai_trong_hien_tai = 320.0; // Tải trọng hiện tại trên kệ (kg)
    float khoi_luong_pallet;
    float nhiet_do_bao_quan;

    printf("=== HE THONG KIEM DINH NHAP KHO LANH SMARTLOG ===\n");
    printf("Tai trong hien tai tren ke: %.1f kg / %.1f kg\n\n", tai_trong_hien_tai, TAI_TRONG_TOI_DA);

    printf("Nhap khoi luong pallet (kg): ");
    if (scanf("%f", &khoi_luong_pallet) != 1) {
        printf("\nLoi: Khoi luong nhap vao khong phai la so!\n");
        return 1;
    }

    printf("Nhap nhiet do bao quan yeu cau (do C): ");
    if (scanf("%f", &nhiet_do_bao_quan) != 1) {
        printf("\nLoi: Nhiet do nhap vao khong phai la so!\n");
        return 1;
    }

    // Khoi luong pallet phai duong, neu khong tai trong ke se bi tinh sai
    if (khoi_luong_pallet <= 0) {
        printf("\nLoi: Khoi luong pallet phai lon hon 0!\n");
        return 1;
    }

    int ket_qua = kiem_tra_nhap_kho_lanh(khoi_luong_pallet, nhiet_do_bao_quan, tai_trong_hien_tai);

    if (ket_qua == 1) {
        tai_trong_hien_tai += khoi_luong_pallet;
        printf("\n=> PHE DUYET: Nhap kho thanh cong!\n");
        printf("Tai trong ke kho sau khi nhap: %.1f kg\n", tai_trong_hien_tai);
    } else {
        printf("\n=> TU CHOI: Kien hang khong du dieu kien vao kho lanh!\n");
        printf("Tai trong ke kho giu nguyen: %.1f kg\n", tai_trong_hien_tai);
    }

    return 0;
}
