/*
 * 文件：auth.c
 * 说明：认证与授权功能实现文件
 *
 * 本文件实现了用户登录、注册和权限验证功能。
 */

#include "models.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 全局登录会话 */
extern UserSession g_session;

/*
 * 函数：check_permission - 检查用户是否有指定角色权限
 *
 * 参数：requiredRole - 执行该操作需要的最低角色类型
 * 返回值：1 - 有权限，0 - 无权限
 */
int check_permission(UserRole requiredRole)
{
    if (!g_session.isLoggedIn)
    {
        return 0;
    }
    
    if (g_session.role == ROLE_MANAGER)
    {
        return 1;
    }
    
    return g_session.role == requiredRole;
}

/*
 * 函数：logout_user - 登出当前用户
 *
 * 清除当前用户的登录会话信息，将系统状态恢复到未登录状态。
 */
void logout_user(void)
{
    g_session.isLoggedIn = 0;
    g_session.role = ROLE_PATIENT;
    g_session.userId = 0;
    g_session.username[0] = '\0';
}
