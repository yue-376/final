/*
 * 文件：main.c
 * 说明：医院管理系统 (HIS) 主程序入口
 */

#include "menu.h"
#include <stdio.h>

#ifndef _WIN32
#define SetConsoleCP(x)
#define SetConsoleOutputCP(x)
#else
#include <windows.h>
#endif

/*
 * 程序主函数
 * 返回值：0 表示程序正常退出，非 0 表示异常退出
 */
int main(void)
{
    /* 设置控制台编码为 UTF-8，确保中文正常显示 */
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    Database db;
    const char *dataDir = ".";
    init_database(&db);
    load_all(&db, dataDir);
    
    /* 执行登录流程 */
    int loginResult = login_menu(&db);
    while (loginResult == -1) {
        loginResult = login_menu(&db);
    }
    
    if (loginResult) {
        /* 根据用户角色进入不同的菜单界面 */
        switch (g_session.role) {
            case ROLE_PATIENT:
                patient_menu(&db, dataDir);
                break;
            case ROLE_DOCTOR:
                doctor_menu(&db, dataDir);
                break;
            case ROLE_MANAGER:
                manager_menu(&db, dataDir);
                break;
            default:
                main_menu(&db, dataDir);
                break;
        }
        
        /* 用户从角色菜单返回后，重新进入登录流程 */
        while (1) {
            loginResult = login_menu(&db);
            while (loginResult == -1) {
                loginResult = login_menu(&db);
            }
            
            if (loginResult == 0) {
                break;
            }
            
            switch (g_session.role) {
                case ROLE_PATIENT:
                    patient_menu(&db, dataDir);
                    break;
                case ROLE_DOCTOR:
                    doctor_menu(&db, dataDir);
                    break;
                case ROLE_MANAGER:
                    manager_menu(&db, dataDir);
                    break;
                default:
                    main_menu(&db, dataDir);
                    break;
            }
        }
    }
    
    free_database(&db);
    return 0;
}
