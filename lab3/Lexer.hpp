#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
namespace lexer
{
    struct TrieNode
    {
        int point[256]; ///下标型指针
        char value;     ///下表对应存储的字符
        bool endpos;    ///是否为一个单词
    } trie_empty_node;
    std::string type_chinese[] = {"错误", "整型常数", "浮点型常数", "字符型常数", "字符串型常数", "关键字", "符号", "变量"};
    std::string type_english[] = {"ERROR", "INTERGER CONSTANT", "FLOAT CONSTANT", "CHAR CONSTANT", "STRING CONSTANT", "KEYWORD", "PUNCTUATOR", "VARIABLE"};
    int txt_x = 1, txt_y = 1;
    enum TokenType
    {
        ERROR,
        INTEGER_CONSTANT,
        FLOAT_CONSTANT,
        CHAR_CONSTANT,
        STRING_CONSTANT,
        KEYWORD,
        PUNCTUATOR,
        VARIABLE
    };
    enum DFAStatus
    {
        S,
        INT_NUMBER,
        FLOAT_NUMBER,
        SUF_INT_L,
        SUF_INT_LL,
        SUF_INT_UNSIGNED,
        HEX_PRE,
        HEX,
        FLOAT_EXPONENT_PART,
        ZERO_POINT_FLOAT,
        CHARACTER_START,
        ESCAPE_CHARACTER,
        POSITIVE_FLOAT_EXPONENT_PART,
        NEGATIVE_FLOAT_EXPONENT_PART,
        CHARACTER,
        SPECIAL_CHARACTER,
        CHARACTER_END,
        FLOAT_TYPE,
        LONG_DOUBLE_TYPE,
        STRING,
        SPECIAL_STRING_CHARACTER,
        STRING_END,
        KEYWORD_DFA,
        PUNCTUATOR_DFA,
        DFA_ERROR
    };
    /**
     * @brief 词法分析器所切割出的Token
     *
     */
    class Token
    {
    private:
        TokenType type;       /// Token类型
        std::string token;    /// Token内容
        std::string extended; ///额外信息
    public:
        void Show()
        {
            switch (type)
            {
            case ERROR:
                printf("type:ERROR\n");
                break;
            case INTEGER_CONSTANT:
                printf("type:INTEGER_CONSTANT\n");
                break;
            case FLOAT_CONSTANT:
                printf("type:FLOAT_CONSTANT\n");
                break;
            case CHAR_CONSTANT:
                printf("type:CHAR_CONSTANT\n");
                break;
            case STRING_CONSTANT:
                printf("type:STRING_CONSTANT\n");
                break;
            case KEYWORD:
                printf("type:KEYWORD\n");
                break;
            case PUNCTUATOR:
                printf("type:PUNCTUATOR\n");
                break;
            case VARIABLE:
                printf("type:VARIABLE\n");
                break;
            }
            printf("token: %s\n", token.c_str());
            if (extended.length() != 0)
            {
                printf("extended information: %s\n", extended.c_str());
            }
            printf("line:%d %d\n", txt_x, txt_y);
            printf("\n");
        }
        /**
         * @brief Construct a new Token object
         *
         * @param type_ Token类型
         * @param token_ Token内容
         */
        Token()
        {
            token = "";
            type = ERROR;
            extended = "";
        }
        Token(TokenType type_, std::string token_, std::string extended_)
        {
            token = token_;
            type = type_;
            extended = extended_;
        }
        void GenerateToken(TokenType type_, std::string token_, std::string extended_)
        {
            token = token_;
            type = type_;
            extended = extended_;
        }
    };
    /**
     * @brief 主要用于对keyword文件来创立trie树拼接入DFA
     */
    class Trie
    {
        friend int ReceiveNextCharacter(char c);

    private:
        std::vector<TrieNode> node; /// Trie树的节点
        int size;                   /// Trie树的大小
    public:
        /**
         * @brief 为Trie树新增一个单词
         *
         * @param new_word 新增的单词
         */
        Trie()
        {
            size = 1;
            node.push_back(trie_empty_node);
            node.push_back(trie_empty_node);
        }
        void AddString(std::string new_word);
    };
    void KeywordTriePrepare();
    void PunctuatorTriePrepare();
    void CleanTokenBuffer();
    int ReceiveNextCharacter(char c);
    int GenerateToken(Token *token);
    int NextToken(Token *token);
    std::string token_buffer;    /// token缓存
    std::string extended_buffer; ///额外信息缓存
    TokenType type_buffer;       /// tokentype缓存
    DFAStatus dfa_status = S;    /// DFA的当前状态
    Trie keyword;                /// keyword的trie树
    Trie punctuator;             /// punctuator的trie树
    int trie_keyword;            /// dfa中keyword的trie指针
    int trie_punctuator;         /// dfa中punctuator的trie指针
    char flow_back;              /// 自动机的回退字符
    int character_times;         ///转义字符中转义八进制或转义十六进制的计数
}
