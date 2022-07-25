#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */
#include <string.h>  /* strlen() */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

typedef struct {
    const char* json;
}lept_context;

/**
 * @brief 过滤空格以及控制字符
 * 
 * @param c JSON
 */
static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

/**
 * @brief 解析字面量
 * 
 * @param c JSON
 * @param v 类型 
 * @return int 解析状态
 */
static int lept_parse_literal(lept_context* c, lept_value* v, char* str, lept_type ret) {
    EXPECT(c, *str);
    int len = strlen(str) - 1;
    for (int i = 0; i < len; ++i) {
        if (c->json[i] != str[i + 1])
            return LEPT_PARSE_INVALID_VALUE;
    }
    c->json += len;
    v->type = ret;
    return LEPT_PARSE_OK;
}

/**
 * @brief 解析值
 * 
 * @param c JSON
 * @param v 类型 
 * @return int 解析状态
 */
static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 'n':  return lept_parse_literal(c, v, "null", LEPT_NULL);
        case 't':  return lept_parse_literal(c, v, "true", LEPT_TRUE);
        case 'f':  return lept_parse_literal(c, v, "false", LEPT_FALSE);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return LEPT_PARSE_INVALID_VALUE;
    }
}

/**
 * @brief 解析函数
 * 
 * @param v 类型 
 * @param json 
 * @return int 解析状态
 */
int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    int ret = lept_parse_value(&c, v);
    if (ret == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

/**
 * @brief 获取类型
 * 
 * @param v 类型 
 * @return lept_type 解析类型
 */
lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}
