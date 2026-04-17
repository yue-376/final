/*
 * 文件：common.c
 * 说明：通用工具函数实现文件
 */

#include "common.h"

/*
 * 函数：trim_newline - 去除字符串末尾的换行符
 * 参数：s - 待处理的字符串指针
 */
void trim_newline(char *s)
{
    if (!s)
        return;
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r'))
    {
        s[--len] = '\0';
    }
}

/*
 * 函数：clear_input_buffer - 清空输入缓冲区
 */
void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

/*
 * 函数：read_line - 读取一行用户输入
 * 参数：prompt 提示信息，buf 存储输入的缓冲区，size 缓冲区大小
 */
void read_line(const char *prompt, char *buf, int size)
{
    if (prompt)
        printf("%s", prompt);
    if (fgets(buf, size, stdin) == NULL)
    {
        buf[0] = '\0';
        return;
    }
    trim_newline(buf);
}

/*
 * 函数：safe_copy - 安全地复制字符串，防止缓冲区溢出
 * 参数：dst 目标缓冲区，src 源字符串，n 目标缓冲区大小
 */
void safe_copy(char *dst, const char *src, size_t n)
{
    if (!dst || !src || n == 0)
        return;
    strncpy(dst, src, n - 1);
    dst[n - 1] = '\0';
}

/*
 * 函数：validate_gender - 验证性别输入是否有效
 * 参数：gender 性别字符串
 * 返回值：1 表示有效（"男"或"女"），0 表示无效
 */
int validate_gender(const char *gender)
{
    if (!gender || strlen(gender) == 0)
        return 0;
    if (strcmp(gender, "男") == 0 || strcmp(gender, "女") == 0)
        return 1;
    return 0;
}

/*
 * 函数：validate_date - 验证日期格式是否为 YYYY-MM-DD
 * 参数：date 日期字符串
 * 返回值：1 表示有效，0 表示无效
 */
int validate_date(const char *date)
{
    if (!date || strlen(date) != 10)
        return 0;
    if (date[4] != '-' || date[7] != '-')
        return 0;
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (!isdigit((unsigned char)date[i]))
            return 0;
    }
    int year = atoi(date);
    int month = atoi(date + 5);
    int day = atoi(date + 8);
    
    if (year < 1900 || year > 2100)
        return 0;
    
    if (month < 1 || month > 12)
        return 0;
    
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month == 2)
    {
        int is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (is_leap)
            days_in_month[2] = 29;
    }
    
    if (day < 1 || day > days_in_month[month])
        return 0;
    
    return 1;
}

/*
 * 说明：验证手机号是否为 11 位数字且以 1 开头
 * 参数：phone 手机号字符串
 * 返回值：1 表示有效，0 表示无效
 * 
 * 验证规则（针对中国大陆手机号）：
 * 1. 必须是 11 位数字（不多不少正好 11 位）
 * 2. 第一位必须是'1'（中国手机号都以 1 开头）
 * 3. 所有位都必须是数字（不能有字母、空格或其他符号）
 * 函数用途：
 * 验证中国大陆手机号格式：11 位数字且以 1 开头。
 * 拒绝带分隔符、长度错误或首位非 1 的输入，确保号码格式统一。
 *
 * 注意：此函数仅验证格式，不验证号码是否真实有效。
 * 也就是说"11111111111"能通过验证（虽然这不是一个真实的号码）。
 */
int validate_phone(const char *phone)
{
    if (!phone)
        return 0;
    size_t len = strlen(phone);
    if (len != 11)
        return 0;
    if (phone[0] != '1')
        return 0;
    for (size_t i = 0; i < len; i++)
    {
        if (!isdigit((unsigned char)phone[i]))
            return 0;
    }
    return 1;
}

/*
 * 函数：read_line_with_validate - 读取带验证的输入，支持输入"0"返回上一步
 * 参数：prompt 提示信息，buf 存储输入的缓冲区，size 缓冲区大小
 *       validate_func 验证函数指针，error_msg 验证失败时的错误提示
 * 返回值：1 表示成功，0 表示用户选择返回
 */
int read_line_with_validate(const char *prompt, char *buf, int size, 
                            int (*validate_func)(const char *), 
                            const char *error_msg)
{
    char temp[256];
    while (1)
    {
        read_line(prompt, temp, sizeof(temp));
        if (strcmp(temp, "0") == 0)
        {
            return 0;
        }
        if (validate_func(temp))
        {
            safe_copy(buf, temp, size);
            return 1;
        }
        printf("%s\n", error_msg);
    }
}

/*
 * 函数：read_int - 读取指定范围内的整数
 * 参数：prompt 提示信息，min 最小值，max 最大值
 * 返回值：有效的整数值（在 min~max 范围内）
 */
int read_int(const char *prompt, int min, int max)
{
    char line[64];
    char *end;
    long value;
    while (1)
    {
        read_line(prompt, line, sizeof(line));
        if (strlen(line) == 0) {
            printf("输入不能为空，请输入 %d ~ %d 的整数。\n", min, max);
            continue;
        }
        value = strtol(line, &end, 10);
        if (*line != '\0' && *end == '\0' && value >= min && value <= max)
        {
            return (int)value;
        }
        printf("输入无效，请输入 %d ~ %d 的整数。\n", min, max);
    }
}

/*
 * 函数：pause_and_wait - 暂停程序，等待用户按回车键继续
 */
void pause_and_wait(void)
{
    printf("\n按回车继续...");
    getchar();
}

/*
 * 说明：忽略大小写比较两个字符串
 * 参数：a 第一个字符串
 * 参数：b 第二个字符串
 * 返回值：1 表示相等，0 表示不等
 */
int str_equal_ignore_case(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

int utf8_display_width(const char *s)
{
    int width = 0;
    const unsigned char *p = (const unsigned char *)s;
    if (!s)
        return 0;
    while (*p)
    {
        int bytes;
        int chWidth;
        utf8_char_info(p, &bytes, &chWidth);
        width += chWidth;
        p += bytes;
    }
    return width;
}

void utf8_char_info(const unsigned char *p, int *bytes, int *width)
{
    if (*p < 0x80)
    {
        *bytes = 1;
        *width = 1;
    }
    else if ((*p & 0xE0) == 0xC0)
    {
        *bytes = (p[1] ? 2 : 1);
        *width = 2;
    }
    else if ((*p & 0xF0) == 0xE0)
    {
        *bytes = (p[1] && p[2] ? 3 : 1);
        *width = 2;
    }
    else if ((*p & 0xF8) == 0xF0)
    {
        *bytes = (p[1] && p[2] && p[3] ? 4 : 1);
        *width = 2;
    }
    else
    {
        *bytes = 1;
        *width = 1;
    }
}

void print_utf8_cell(const char *s, int colWidth)
{
    int width;
    if (!s)
        s = "";
    width = utf8_display_width(s);
    printf("%s", s);
    while (width < colWidth)
    {
        putchar(' ');
        width++;
    }
}

void print_utf8_cell_fit(const char *s, int colWidth)
{
    int width = 0;
    const unsigned char *p;
    int bytes;
    int chWidth;
    if (!s)
        s = "";
    p = (const unsigned char *)s;
    while (*p)
    {
        utf8_char_info(p, &bytes, &chWidth);
        if (width + chWidth > colWidth)
            break;
        printf("%.*s", bytes, (const char *)p);
        width += chWidth;
        p += bytes;
    }
    while (width < colWidth)
    {
        putchar(' ');
        width++;
    }
}
