#include <stdio.h>
#include <pthread.h>

int data[] = {2, 7, 4, 9, 11, 6, 15, 20, 13};
int n = sizeof(data) / sizeof(data[0]);

// Satu data bersama untuk semua thread
struct Hasil {
    char jenis[20];
    double nilai;
};

struct Hasil data_bersama[20];
int jumlah_data = 0;

pthread_mutex_t lock;

// Menambahkan hasil ke data bersama
void tambah_data(const char *jenis, double nilai) {
    pthread_mutex_lock(&lock);

    jumlah_data++;
    int i = jumlah_data - 1;

    snprintf(data_bersama[i].jenis, 20, "%s", jenis);
    data_bersama[i].nilai = nilai;

    pthread_mutex_unlock(&lock);
}

// Thread 1: mencari bilangan prima
void *thread_prima(void *arg) {
    for (int i = 0; i < n; i++) {
        int prima = 1;

        if (data[i] < 2)
            prima = 0;

        for (int j = 2; j * j <= data[i]; j++) {
            if (data[i] % j == 0) {
                prima = 0;
                break;
            }
        }

        if (prima)
            tambah_data("Prima", data[i]);
    }
    return NULL;
}

// Thread 2: menghitung rata-rata
void *thread_rata(void *arg) {
    double total = 0;

    for (int i = 0; i < n; i++)
        total += data[i];

    tambah_data("Rata-rata", total / n);
    return NULL;
}

// Thread 3: mencari nilai terbesar
void *thread_max(void *arg) {
    int terbesar = data[0];

    for (int i = 1; i < n; i++) {
        if (data[i] > terbesar)
            terbesar = data[i];
    }

    tambah_data("Terbesar", terbesar);
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, thread_prima, NULL);
    pthread_create(&t2, NULL, thread_rata, NULL);
    pthread_create(&t3, NULL, thread_max, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("=== HASIL DATA BERSAMA ===\n");
    for (int i = 0; i < jumlah_data; i++)
        printf("%s : %.2f\n",
               data_bersama[i].jenis,
               data_bersama[i].nilai);

    pthread_mutex_destroy(&lock);

    return 0;
}