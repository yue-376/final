// auth.c - 认证与授权功能

#include "models.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 全局登录会话
extern UserSession g_session;

// 检查用户是否有指定角色权限
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

// 登出当前用户
void logout_user(void)
{
    g_session.isLoggedIn = 0;
    g_session.role = ROLE_PATIENT;
    g_session.userId = 0;
    g_session.username[0] = '\0';
}
