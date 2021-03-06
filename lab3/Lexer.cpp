#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Lexer.hpp"
namespace lexer
{
    void Trie::AddString(std::string new_word)
    {
        int n = new_word.length();
        int trie_index = 1;
        for (int i = 0; i < n; i++)
        {
            char c = new_word[i];
            if (node[trie_index].point[c] == 0)
            {
                node.push_back(trie_empty_node);
                size++;
                node[trie_index].point[c] = size;
            }
            trie_index = node[trie_index].point[c];
        }
        node[trie_index].endpos = 1;
    }
    void KeywordTriePrepare()
    {
        freopen("keyword.txt", "r", stdin);
        std::string word;
        char c = getchar();
        while (c != EOF)
        {
            if (c == '\n' || c == ' ')
            {
                //            	printf("%s\n",word.c_str());
                keyword.AddString(word);
                word = "";
            }
            else
                word = word + c;
            c = getchar();
        }
        keyword.AddString(word);
    }
    void PunctuatorTriePrepare()
    {
        freopen("punctuatortrie.txt", "r", stdin);
        std::string word;
        char c = getchar();
        while (c != EOF)
        {
            if (c == '\n' || c == ' ')
            {
                punctuator.AddString(word);
                word = "";
            }
            else
                word = word + c;
            c = getchar();
        }
        punctuator.AddString(word);
    }
    void CleanTokenBuffer()
    {
        token_buffer = "";
        dfa_status = S;
        type_buffer = ERROR;
        extended_buffer = "";
        trie_keyword = 1;
        trie_punctuator = 1;
    }
    int ReceiveNextCharacter(char c)
    {
        if (c == '\n')
            txt_x++, txt_y = 1;
        else
            txt_y++;
        switch (dfa_status)
        {
        case S:
            if (c == '0')
            {
                dfa_status = HEX_PRE;
                token_buffer += c;
                return 0;
            }
            if (c >= '1' && c <= '9')
            {
                dfa_status = INT_NUMBER;
                token_buffer += c;
                return 0;
            }
            if (c == '.')
            {
                dfa_status = ZERO_POINT_FLOAT;
                token_buffer += c;
                return 0;
            }
            if (c == '\'')
            {
                dfa_status = CHARACTER;
                token_buffer += c;
                return 0;
            }
            if (c == '\"')
            {
                dfa_status = STRING;
                token_buffer += c;
                return 0;
            }
            if (c != -1 && punctuator.node[trie_punctuator].point[c] != 0)
            {
                dfa_status = PUNCTUATOR_DFA;
                trie_punctuator = punctuator.node[trie_punctuator].point[c];
                token_buffer += c;
                return 0;
            }
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_')
            {
                dfa_status = KEYWORD_DFA;
                trie_keyword = keyword.node[trie_keyword].point[c];
                token_buffer += c;
                return 0;
            }
            break;
        case PUNCTUATOR_DFA:
            if (c != -1 && punctuator.node[trie_punctuator].point[c] != 0)
            {
                trie_punctuator = punctuator.node[trie_punctuator].point[c];
                token_buffer += c;
                return 0;
            }
            if (punctuator.node[trie_punctuator].endpos)
            {
                type_buffer = PUNCTUATOR;
                flow_back = c;
                return 1;
            }
            else
            {
                type_buffer = ERROR;
                extended_buffer += "UNKNOW PUNCTUATOR";
                flow_back = c;
                return 1;
            }
        case KEYWORD_DFA:
            if (keyword.node[trie_keyword].point[c] != 0)
            {
                trie_keyword = keyword.node[trie_keyword].point[c];
                token_buffer += c;
                return 0;
            }
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_' || ('0' <= c && c <= '9'))
            {
                trie_keyword = 0;
                token_buffer += c;
                return 0;
            }
            flow_back = c;
            if (keyword.node[trie_keyword].endpos != 0)
            {
                type_buffer = KEYWORD;
            }
            else
            {
                type_buffer = VARIABLE;
            }
            return 1;
        //???????????????????????????
        case INT_NUMBER:
            if ('0' <= c && c <= '9')
            {
                token_buffer += c;
                return 0;
            }
            if (c == 'l' || c == 'L')
            {
                dfa_status = SUF_INT_L;
                token_buffer += c;
                extended_buffer += "LONG ";
                return 0;
            }
            if (c == 'u' || c == 'U')
            {
                dfa_status = SUF_INT_UNSIGNED;
                token_buffer += c;
                extended_buffer += "UNSIGNED ";
                return 0;
            }
            if (c == '.')
            {
                dfa_status = FLOAT_NUMBER;
                token_buffer += c;
                return 0;
            }
            if (c == 'e' || c == 'E')
            {
                dfa_status = FLOAT_EXPONENT_PART;
                token_buffer += c;
                return 0;
            }
            type_buffer = INTEGER_CONSTANT;
            extended_buffer = "INT";
            flow_back = c;
            return 1;

        case FLOAT_NUMBER:
            if ('0' <= c && c <= '9')
            {
                token_buffer += c;
                return 0;
            }
            if (c == 'e' || c == 'E')
            {
                dfa_status = FLOAT_EXPONENT_PART;
                token_buffer += c;
                return 0;
            }
            if (c == 'f' || c == 'F')
            {
                dfa_status = FLOAT_TYPE;
                token_buffer += c;
                extended_buffer = "FLOAT";
                return 0;
            }
            if (c == 'l' || c == 'L')
            {
                dfa_status = LONG_DOUBLE_TYPE;
                token_buffer += c;
                extended_buffer = "LONG DOUBLE";
                return 0;
            }
            extended_buffer = "DOUBLE";
            type_buffer = FLOAT_CONSTANT;
            flow_back = c;
            return 1;

        case SUF_INT_L:
            if (c == 'l' || c == 'L')
            {
                dfa_status = SUF_INT_LL;
                token_buffer += c;
                extended_buffer += "LONG";
                return 0;
            }
            type_buffer = INTEGER_CONSTANT;
            flow_back = c;
            return 1;

        case SUF_INT_LL:
            type_buffer = INTEGER_CONSTANT;
            flow_back = c;
            return 1;

        case SUF_INT_UNSIGNED:
            if (c == 'l' || c == 'L')
            {
                dfa_status = SUF_INT_L;
                token_buffer += c;
                extended_buffer += "LONG ";
                return 0;
            }
            type_buffer = INTEGER_CONSTANT;
            extended_buffer += "INT";
            flow_back = c;
            return 1;

        case HEX_PRE:
            if ('0' <= c && c <= '9')
            {
                dfa_status = INT_NUMBER;
                token_buffer += c;
                return 0;
            }
            if (c == 'x' || c == 'X')
            {
                dfa_status = HEX;
                token_buffer += c;
                return 0;
            }
            type_buffer = INTEGER_CONSTANT;
            flow_back = c;
            return 1;

        case HEX:
            if ('a' <= c && c <= 'f')
                c = c - 'a' + 'A';
            if ('0' <= c && c <= '9' || 'A' <= c && c <= 'F')
            {
                token_buffer += c;
                return 0;
            }
            if (c == 'l' || c == 'L')
            {
                dfa_status = SUF_INT_L;
                token_buffer += c;
                extended_buffer += "LONG ";
                return 0;
            }
            if (c == 'u' || c == 'U')
            {
                dfa_status = SUF_INT_UNSIGNED;
                token_buffer += c;
                extended_buffer += "UNSIGNED ";
                return 0;
            }
            type_buffer = INTEGER_CONSTANT;
            flow_back = c;
            return 1;

        case FLOAT_EXPONENT_PART:
            if (c == '+')
            {
                dfa_status = POSITIVE_FLOAT_EXPONENT_PART;
                token_buffer += c;
                return 0;
            }
            if (c == '-')
            {
                dfa_status = NEGATIVE_FLOAT_EXPONENT_PART;
                token_buffer += c;
                return 0;
            }
            if (c >= '0' && c <= '9')
            {
                dfa_status = POSITIVE_FLOAT_EXPONENT_PART;
                token_buffer += c;
                return 0;
            }
            dfa_status = DFA_ERROR;
            type_buffer = ERROR;
            extended_buffer += "FLOAT EXPONENT PART ERROR";
            flow_back = c;
            return 1;

        case POSITIVE_FLOAT_EXPONENT_PART:
            if (c == 'f' || c == 'F')
            {
                dfa_status = FLOAT_TYPE;
                token_buffer += c;
                extended_buffer += "FLOAT";
                return 0;
            }
            if (c == 'l' || c == 'L')
            {
                dfa_status = LONG_DOUBLE_TYPE;
                token_buffer += c;
                extended_buffer += "LONG DOUBLE";
                return 0;
            }
            if ('0' <= c && c <= '9')
            {
                token_buffer += c;
                return 0;
            }
            flow_back = c;
            extended_buffer += "DOUBLE";
            type_buffer = FLOAT_CONSTANT;
            return 1;

        case NEGATIVE_FLOAT_EXPONENT_PART:
            if (c == 'f' || c == 'F')
            {
                dfa_status = FLOAT_TYPE;
                token_buffer += c;
                extended_buffer += "FLOAT";
                return 0;
            }
            if (c == 'l' || c == 'L')
            {
                dfa_status = LONG_DOUBLE_TYPE;
                token_buffer += c;
                extended_buffer += "LONG DOUBLE";
                return 0;
            }
            if ('0' <= c && c <= '9')
            {
                token_buffer += c;
                return 0;
            }
            flow_back = c;
            extended_buffer += "DOUBLE";
            type_buffer = FLOAT_CONSTANT;
            return 1;

        case FLOAT_TYPE:
            flow_back = c;
            type_buffer = FLOAT_CONSTANT;
            return 1;
        case LONG_DOUBLE_TYPE:
            flow_back = c;
            type_buffer = FLOAT_CONSTANT;
            return 1;
        case ZERO_POINT_FLOAT:
            if ('0' <= c && c <= '9')
            {
                dfa_status = FLOAT_NUMBER;
                token_buffer += c;
                return 0;
            }
            if (c == '.')
            {
                dfa_status = PUNCTUATOR_DFA;
                token_buffer += c;
                trie_punctuator = punctuator.node[trie_punctuator].point['.'];
                trie_punctuator = punctuator.node[trie_punctuator].point['.'];
                return 0;
            }
            flow_back = c;
            type_buffer = PUNCTUATOR;
            return 1;
        //??????????????????????????????
        case CHARACTER:
            if (c == '\\')
            {
                token_buffer += c;
                dfa_status = SPECIAL_CHARACTER;
                return 0;
            }
            if (c == '\'')
            {
                token_buffer += c;
                dfa_status = CHARACTER_END;
                return 0;
            }
            token_buffer += c;
            return 0;
        case SPECIAL_CHARACTER:
            token_buffer += c;
            dfa_status = CHARACTER;
            return 0;
        case CHARACTER_END:
            type_buffer = CHAR_CONSTANT;
            flow_back = c;
            return 1;
        //??????????????????
        case STRING:
            if (c == '\\')
            {
                token_buffer += c;
                dfa_status = SPECIAL_STRING_CHARACTER;
                return 0;
            }
            if (c == '\"')
            {
                token_buffer += c;
                dfa_status = STRING_END;
                return 0;
            }
            token_buffer += c;
            return 0;
        case SPECIAL_STRING_CHARACTER:
            token_buffer += c;
            dfa_status = STRING;
            return 0;
        case STRING_END:
            type_buffer = STRING_CONSTANT;
            flow_back = c;
            return 1;
        default:
            break;
        }
    }
    int GenerateToken(Token *token)
    {
        token->GenerateToken(type_buffer, token_buffer, extended_buffer);
    }
    int NextToken(Token *token)
    {
        CleanTokenBuffer();
        char c;
        if (flow_back == 0)
            c = getchar();
        else
        {
            c = flow_back;
            if (c == '\n')
                txt_x--;
            flow_back = 0;
        }
        if (c == EOF)
            return 1;
        while (ReceiveNextCharacter(c) == 0)
        {
            //	printf("%d\n",trie_keyword);
            c = getchar();
            if (c == EOF)
            {
                c++;
                c--;
            }
        }
        GenerateToken(token);
        return 0;
    }
}
int main(int argc, char *argv[])
{
    lexer::KeywordTriePrepare();
    lexer::PunctuatorTriePrepare();
    lexer::Token *token = new lexer::Token();
    freopen("test4.in", "r", stdin);
    freopen("1.out", "w", stdout);
    while (lexer::NextToken(token) == 0)
    {
        token->Show();
    }
}
