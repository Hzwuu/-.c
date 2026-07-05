#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ILLNESS 15
#define MAX_SYMPTOM 200
#define MAX_ADVICE 200
#define MAX_PRESCRIPTION 100

// 中医病症结构体
typedef struct {
    char name[50];               // 病症名称
    char symptoms[MAX_SYMPTOM];  // 症状描述
    char advice[MAX_ADVICE];     // 调理建议
    char prescription[MAX_PRESCRIPTION]; // 参考方剂
} TCM_Illness;

// 从文本文件初始化病症数据库
// 文件格式：每组病症占4行，依次为名称、症状、建议、方剂
// 返回0表示成功，-1表示文件打开失败
int initTCMDATABASE(TCM_Illness database[], const char* filename) {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0 || file == NULL) {
        printf("错误：无法打开数据库文件“%s”\n", filename);
        printf("请确认文件存在且位于程序同目录下。\n");
        return -1;
    }

    char buffer[300];  // 足够容纳最长字段
    int count = 0;

    while (count < MAX_ILLNESS) {
        // 读取名称
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy_s(database[count].name, sizeof(database[count].name), buffer);

        // 读取症状
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy_s(database[count].symptoms, sizeof(database[count].symptoms), buffer);

        // 读取建议
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy_s(database[count].advice, sizeof(database[count].advice), buffer);

        // 读取方剂
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy_s(database[count].prescription, sizeof(database[count].prescription), buffer);

        count++;
    }

    // 若文件中病症不足15条，剩余结构体置空
    for (int i = count; i < MAX_ILLNESS; i++) {
        database[i].name[0] = '\0';
        database[i].symptoms[0] = '\0';
        database[i].advice[0] = '\0';
        database[i].prescription[0] = '\0';
    }

    fclose(file);
    return 0;
}

// 显示所有病症列表
void showIllnessList(TCM_Illness database[]) {
    printf("\n========== 中医常见病症列表 ==========\n");
    for (int i = 0; i < MAX_ILLNESS; i++) {
        if (database[i].name[0] == '\0') continue; // 跳过空的条目
        printf("%2d. %s\n", i + 1, database[i].name);
    }
    printf("====================================\n");
}

// 根据症状关键词搜索
void searchBySymptom(TCM_Illness database[], char* keyword) {
    int found = 0;
    printf("\n搜索症状关键词: \"%s\"\n", keyword);
    printf("========================================\n");

    for (int i = 0; i < MAX_ILLNESS; i++) {
        if (database[i].name[0] == '\0') continue;
        // 在症状描述中搜索关键词
        if (strstr(database[i].symptoms, keyword) != NULL) {
            printf("\n匹配病症：%s\n", database[i].name);
            printf("典型症状：%s\n", database[i].symptoms);
            printf("调理建议：%s\n", database[i].advice);
            printf("参考方剂：%s\n", database[i].prescription);
            printf("----------------------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("未找到包含该关键词的病症。\n");
        printf("请尝试其他描述，如：发烧、胃痛、咳嗽等。\n");
    }
}

// 根据病症名称查询
void searchByName(TCM_Illness database[], char* name) {
    int found = 0;

    for (int i = 0; i < MAX_ILLNESS; i++) {
        if (database[i].name[0] == '\0') continue;
        if (strstr(database[i].name, name) != NULL) {
            printf("\n========== 病症信息详情 ==========\n");
            printf("病症名称：%s\n", database[i].name);
            printf("典型症状：%s\n", database[i].symptoms);
            printf("调理建议：%s\n", database[i].advice);
            printf("参考方剂：%s\n", database[i].prescription);
            printf("===================================\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("未找到病症：%s\n", name);
        printf("请查看病症列表确认正确的名称。\n");
    }
}

// 显示免责声明
void showDisclaimer() {
    printf("\n**************************************************\n");
    printf("*                重要声明                        *\n");
    printf("* 本程序仅供学习和参考使用，不能替代专业医疗诊断 *\n");
    printf("* 如有不适，请及时就医咨询专业中医师           *\n");
    printf("* 请勿自行用药，所有方剂需在医师指导下使用     *\n");
    printf("**************************************************\n\n");
}

// 主函数
int main() {
    TCM_Illness illnessDatabase[MAX_ILLNESS];
    char input[100];
    int choice;

    // 从文本文件初始化数据库
    if (initTCMDATABASE(illnessDatabase, "tcm_database.txt") != 0) {
        printf("按任意键退出...\n");
        (void)getchar();
        return 1;
    }

    printf("欢迎使用中医简易查询系统 (VS2022版)\n");
    showDisclaimer();

    while (1) {
        printf("\n========== 主菜单 ==========\n");
        printf("1. 查看所有病症列表\n");
        printf("2. 根据症状关键词查询\n");
        printf("3. 根据病症名称查询\n");
        printf("4. 退出系统\n");

        // 主菜单输入校验
        while (1) {
            printf("请选择操作 (1-4): ");
            if (scanf_s("%d", &choice) == 1 && choice >= 1 && choice <= 4) {
                (void)getchar(); // 清除换行符
                break;
            }
            else {
                printf("? 输入错误！请输入1-4之间的数字\n");
                while (getchar() != '\n');
            }
        }

        switch (choice) {
        case 1:
            showIllnessList(illnessDatabase);
            break;

        case 2:
            printf("\n请输入症状关键词（如：发烧、胃痛、咳嗽等）: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            searchBySymptom(illnessDatabase, input);
            break;

        case 3:
            printf("\n请输入病症名称（如：风热感冒、胃火旺盛等）: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            searchByName(illnessDatabase, input);
            break;

        case 4:
            printf("\n感谢使用中医简易查询系统！\n");
            printf("祝您健康！\n");
            return 0;

        default:
            printf("无效选择，请输入1-4之间的数字！\n");
        }
    }

    return 0;
}
