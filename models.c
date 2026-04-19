 // models.c -

#include "models.h"

 // free_patients: 功能说明：
void free_patients(Patient *head)
{
    while (head)
    {
        Patient *t = head;      /* 保存当前节点地址，防止丢失 */
        head = head->next;      /* 先移动到下一个节点，因为当前节点马上要被释放了 */
        free(t);                /* 释放当前节点占用的内存 */
    }
}

 // free_doctors: 功能说明：
void free_doctors(Doctor *head)
{
    while (head)
    {
        Doctor *t = head;       /* 保存当前医生节点 */
        head = head->next;      /* 移动到下一个医生节点 */
        free(t);                /* 释放当前节点 */
    }
}

 // free_regs: 功能说明：
void free_regs(Registration *head)
{
    while (head)
    {
        Registration *t = head;
        head = head->next;
        free(t);
    }
}

 // free_visits: 功能说明：
void free_visits(Visit *head)
{
    while (head)
    {
        Visit *t = head;
        head = head->next;
        free(t);
    }
}

 // free_exams: 功能说明：
void free_exams(Exam *head)
{
    while (head)
    {
        Exam *t = head;
        head = head->next;
        free(t);
    }
}

 // free_wards: 功能说明：
void free_wards(Ward *head)
{
    while (head)
    {
        Ward *t = head;
        head = head->next;
        free(t);
    }
}

 // free_inpatients: 功能说明：
void free_inpatients(Inpatient *head)
{
    while (head)
    {
        Inpatient *t = head;
        head = head->next;
        free(t);
    }
}

 // free_drugs: 功能说明：
void free_drugs(Drug *head)
{
    while (head)
    {
        Drug *t = head;
        head = head->next;
        free(t);
    }
}

 // free_druglogs: 功能说明：
void free_druglogs(DrugLog *head)
{
    while (head)
    {
        DrugLog *t = head;
        head = head->next;
        free(t);
    }
}

 // free_accounts: 功能说明：
void free_accounts(Account *head)
{
    while (head)
    {
        Account *t = head;
        head = head->next;
        free(t);
    }
}

 // init_database: 功能说明：
void init_database(Database *db)
{
    // 将所有链表头指针设为 NULL，表示空链表
    db->patients = NULL;      /* 患者链表 */
    db->doctors = NULL;       /* 医生链表 */
    db->registrations = NULL; /* 挂号记录链表 */
    db->visits = NULL;        /* 看诊记录链表 */
    db->exams = NULL;         /* 检查记录链表 */
    db->wards = NULL;         /* 病房链表 */
    db->inpatients = NULL;    /* 住院记录链表 */
    db->drugs = NULL;         /* 药品链表 */
    db->drugLogs = NULL;      /* 药品日志链表 */
    db->accounts = NULL;      /* 账号链表 */
}

 // free_database: 功能说明：
void free_database(Database *db)
{
    // 依次释放所有类型的链表节点
    free_patients(db->patients);      /* 释放所有患者记录 */
    free_doctors(db->doctors);        /* 释放所有医生记录 */
    free_regs(db->registrations);     /* 释放所有挂号记录 */
    free_visits(db->visits);          /* 释放所有看诊记录 */
    free_exams(db->exams);            /* 释放所有检查记录 */
    free_wards(db->wards);            /* 释放所有病房记录 */
    free_inpatients(db->inpatients);  /* 释放所有住院记录 */
    free_drugs(db->drugs);            /* 释放所有药品记录 */
    free_druglogs(db->drugLogs);      /* 释放所有药品日志记录 */
    free_accounts(db->accounts);      /* 释放所有账号记录 */
    
    // 重新初始化数据库结构，将所有指针重置为 NULL
    // 这是一个好习惯，可以避免悬空指针问题
    init_database(db);
}

// ==================== 账号管理函数实现 ====================

 // find_account: 功能说明：
Account *find_account(Database *db, const char *username)
{
    Account *curr = db->accounts;  /* 从链表头开始 */
    while (curr)  /* 遍历直到链表末尾（curr 为 NULL） */
    {
        if (strcmp(curr->username, username) == 0)  /* 找到匹配的 username */
        {
            return curr;  /* 返回该账号节点 */
        }
        curr = curr->next;  /* 移动到下一个节点 */
    }
    return NULL;  /* 遍历结束仍未找到，返回 NULL */
}

 // authenticate_user: 功能说明：
Account *authenticate_user(Database *db, const char *username, const char *password)
{
    // 先根据用户名查找账号
    Account *acc = find_account(db, username);
    // 账号存在且密码匹配时返回账号指针，否则返回 NULL
    if (acc && strcmp(acc->password, password) == 0)
    {
        return acc;  /* 返回账号信息，供调用者设置会话 */
    }
    return NULL;  /* 认证失败 */
}

 // create_account: 功能说明：
int create_account(Database *db, const char *username, const char *password, UserRole role, int linkedId)
{
    // 第一步：检查用户名是否已被占用
    // 用户名必须唯一，不能重复注册
    if (find_account(db, username) != NULL)
    {
        return 0;  /* 用户名已存在，返回失败 */
    }
    
    // 第二步：为新账号分配内存
    // malloc 会从堆区分配一块指定大小的内存
    Account *newAcc = (Account *)malloc(sizeof(Account));
    if (!newAcc)
    {
        return 0;  /* 内存分配失败（系统内存不足），返回失败 */
    }
    
    // 第三步：初始化账号的各个字段
    // 使用 strncpy 而不是 strcpy，防止缓冲区溢出
    // sizeof(newAcc->username) - 1 确保留出空间给结尾的 '\\0'
    strncpy(newAcc->username, username, sizeof(newAcc->username) - 1);
    newAcc->username[sizeof(newAcc->username) - 1] = '\0';  /* 强制加上字符串结束符 */
    
    strncpy(newAcc->password, password, sizeof(newAcc->password) - 1);
    newAcc->password[sizeof(newAcc->password) - 1] = '\0';  /* 强制加上字符串结束符 */
    
    newAcc->role = role;       /* 设置用户角色 */
    newAcc->linkedId = linkedId;  /* 设置关联的用户 ID */
    newAcc->next = NULL;       /* 新节点的 next 先设为 NULL */
    
    // 将新节点插入到账号链表头部
    // 这是最高效的插入方式，不需要遍历整个链表
    // 新节点的 next 指针指向原来的头节点，然后更新头指针指向新节点
    newAcc->next = db->accounts;  /* 新节点的 next 指向原来的头节点 */
    db->accounts = newAcc;        /* 更新头指针，让它指向新节点 */
    
    return 1;  /* 创建成功 */
}

 // get_role_name: 功能说明：
const char *get_role_name(UserRole role)
{
    switch (role)
    {
        case ROLE_PATIENT:
            return "患者";      /* 患者角色 */
        case ROLE_DOCTOR:
            return "医生";      /* 医生角色 */
        case ROLE_MANAGER:
            return "管理员";    /* 管理员角色 */
        default:
            return "未知角色";  /* 防御性编程：处理意外的角色值 */
    }
}
