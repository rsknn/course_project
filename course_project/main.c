#include <stdio.h>
#include <locale.h>
#define N 5

typedef struct {
    int time;
    int type;
} Incident;

int evaluate_state(float q, float l_crit) {
    if (q < -l_crit) {
        return -1;
    }
    else if (q > l_crit) {
        return 1;
    }
    return 0;
}

const char* state_to_str(int state) {
    if (state == -1) return "СНИЖЕНИЕ";
    if (state == 1)  return "РОСТ";
    return "НОРМА";
}

int main(void) {
    setlocale(LC_ALL, "");

    float hdd[N] = { 43.7f, 44.2f, 44.7f, 50.6f, 51.1f };
    float ssd[N] = { 53.8f, 57.2f, 55.2f, 53.2f, 51.2f };
    float app[N] = { 32.0f, 31.3f, 30.6f, 35.2f, 34.5f };

    const int k = 2;
    const float L = 1.5f;

    float q_hdd[N] = { 0.0f };
    float q_ssd[N] = { 0.0f };
    float q_app[N] = { 0.0f };

    int st_hdd[N] = { 0 };
    int st_ssd[N] = { 0 };
    int st_app[N] = { 0 };

    Incident incidents[N];
    int incident_count = 0;

    for (int i = k; i < N; i++) {
        q_hdd[i] = (hdd[i] - hdd[i - k]) / hdd[i - k];
        q_ssd[i] = (ssd[i] - ssd[i - k]) / ssd[i - k];
        q_app[i] = (app[i] - app[i - k]) / app[i - k];
        st_hdd[i] = evaluate_state(q_hdd[i], L);
        st_ssd[i] = evaluate_state(q_ssd[i], L);
        st_app[i] = evaluate_state(q_app[i], L);

        int is_incident = ((st_hdd[i] != 0) && (st_ssd[i] != 0)) ||
            ((st_hdd[i] != 0) && (st_app[i] != 0)) ||
            ((st_ssd[i] != 0) && (st_app[i] != 0));

        if (is_incident) {
            incidents[incident_count].time = i + 1;
            incidents[incident_count].type = 1;
            incident_count++;
        }
    }
    printf("Контроль облачной инфраструктуры\n\n");
    printf("Исходные данные телеметрии:\n");
    printf("Отсчет\tHDD\tSSD\tAPP\n");
    for (int i = 0; i < N; i++) {
        printf("%d\t%.1f\t%.1f\t%.1f\n", i + 1, hdd[i], ssd[i], app[i]);
    }
    printf("\nОтносительное изменение Q при k = %d:\n", k);
    printf("Отсчет\tQ(HDD)\tQ(SSD)\tQ(APP)\n");
    for (int i = k; i < N; i++) {
        printf("%d\t%+.3f\t%+.3f\t%+.3f\n", i + 1, q_hdd[i], q_ssd[i], q_app[i]);
    }
    printf("\nСостояние компонентов (порог L = %.1f):\n", L);
    printf("Отсчет\tHDD\tSSD\tAPP\n");
    for (int i = 0; i < N; i++) {
        printf("%d\t%s\t%s\t%s\n",
            i + 1,
            state_to_str(st_hdd[i]),
            state_to_str(st_ssd[i]),
            state_to_str(st_app[i]));
    }
    printf("\nЖурнал инцидентов (правило \"Два из трёх\"):\n");
    if (incident_count == 0) {
        printf("Инцидентов не зафиксировано.\n");
    }
    else {
        for (int i = 0; i < incident_count; i++) {
            printf("Отсчет %d: зафиксирован инцидент (тип %d)\n",
                incidents[i].time, incidents[i].type);
        }
    }
    return 0;
}