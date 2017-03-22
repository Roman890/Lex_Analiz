
#include <iostream>
#include <fstream>
#include <string>

// начальное значение флага
#define null 0
// флаг число
#define digit 1
// флаг слово
#define word 2
// флаг имя файла
#define filename 3
// флаг строковая константа
#define stringconst 4

using namespace std;


void ravno(char ch, ifstream &f1, ofstream &f2)
{
        char c = f1.peek();
        if (c == ch)
        {
                switch (ch)
                {
                case '=':
                {
                        f1.get();
                    f2 << "EQUAL TO ";
                        break;
                }
                case '+':
                {
                        f1.get();
                        f2 << "INCREMENT ";
                        break;
                }
                case '-':
                {
                        f1.get();
                        f2 << "DECREMENT ";
                        break;
                }
                case '&':
                {
                        f1.get();
                        f2 << "LOGICAL_AND ";
                        break;
                }
                case '|':
                {
                        f1.get();
                        f2 << "LOGICAL_OR ";
                        break;
                }
                case ':':
                {
                        f1.get();
                        f2 << "SCOPE_RESOLUTION ";
                        break;
                }
                }
        }
        else
        {
                switch (ch)
                {
                case '=':
                {
                        f2 << "EQUALS ";
                        break;
                }
                case '+':
                {
                        f2 << "PLUS ";
                        break;
                }
                case '-':
                {
                        f2 << "MINUS ";
                        break;
                }
                case '&':
                {
                        f2 << "BITWISE_AND ";
                        break;
                }
                case '|':
                {
                        f2 << "BITWISE_OR ";
                        break;
                }
                case ':':
                {
                        f2 << "COLON ";
                        break;
                }
                }
        }

        return;
}

// если символ / то мы должны проверить что идет дальше (варианты / и *), чтобы определить какой это тип коментариев
void slash(char &ch, ifstream &f1, ofstream &f2)
{
                char c;
                if (f1.peek() == '/') // функция peek только для просматривания следущего символа, не считывая
                {
                        string temp;
                        ch = f1.get();
                        getline(f1, temp);
                        f2 << "STRING_COMMENT " << endl;
                        return;
                }
                if (f1.peek() == '*')
                {
                        ch = f1.get();
                        f2 << "MULTI-STRING_COMMENT ";
                        while (1)
                        {
                                c = f1.get();
                                if ((c == '*') && (f1.peek() == '/'))
                                {
                                        f1.get();
                                        if (f1.eof())
                                        {
                                                f1.close();
                                                f2.close();
                                                return ;
                                        }
                                        break;
                                }
                        }
                        return;
                }
                else f2 << "DEVIDE ";
                return;
}

void menbol(char ch, ifstream &f1, ofstream &f2)
{
        char c = f1.peek();
        if (c == '=')
        {
                switch (ch)
                {
                case '<':
                {
                        f1.get();
                        f2 << "LESS_THAN_OR_EQUAL_TO ";
                        break;
                }
                case '>':
                {
                        f1.get();
                        f2 << "GREATER_THAN_OR_EQUAL_TO ";
                        break;
                }
                }
        }
        else if(c == ch)
        {
                switch (ch)
                {
                case '<':
                {
                        f1.get();
                        f2 << "OUT_OPERATOR ";
                        break;
                }
                case '>':
                {
                        f1.get();
                        f2 << "IN_OPERATOR ";
                        break;
                }
                }
        }
        else
        {
                switch (ch)
                {
                case '<':
                {
                        f2 << "ANGLE_OPENED_BRACKET ";
                        break;
                }
                case '>':
                {
                        f2 << "ANGLE_CLOSED_BRACKET ";
                        break;
                }
                }
        }

}

void net(char ch, ifstream &f1, ofstream &f2)
{
        char c = f1.peek();
        if (c == '=')
        {
                f1.get();
                f2 << "NOT_EQUAL_TO ";
                return;
        }
        else
        {
                f2 << "LOGICAL_NOT ";
                return;
        }
}


// функция обработки флагов
void flush(int &flag, ofstream &f2)
{
        switch (flag) {
        case null:
                break;
        case digit:
                f2 << "NUMBER ";
                break;
        case word:
                f2 << "WORD ";
                break;
        case filename:
                f2 << "FILENAME ";
                break;
        case stringconst:
                f2 << "STRING_CONST ";
                break;
        }
        flag = null;

}

int main()
{
        bool needFlush = false;// не знаю зачем
        ifstream f1; // на чтение
        ofstream f2; // на запись
        int flag = null; // устанавливаем значение флага в нул

        // проверка на открытие файла для чтения
        f1.open("input.txt");
        if (!f1)
        {
                cout << "Can not open" << endl;
                exit(1);
        }
        // проверка на открытие файла для записи
        f2.open("output.txt");
        if (!f2)
        {
                cout << "Can not open" << endl;
                exit(1);
        }
        char ch; // переменная для посимвольного прочтения файла
        bool quoteReaded = false; // пока не знаю зачем
        while (1)
        {

                ch = f1.get(); // считываем символ
                // проверка на конец файла и соответсвенно выход из цикла
                if (f1.eof())
                {
                        cout << "End of file (main)" << endl;
                        break;
                }



                if (isalpha(ch) || ch == '_')
                {
                        if (quoteReaded)
                        {
                                if (flag == filename)
                                        continue;
                                else
                                {
                                        flag = stringconst;
                                        continue;
                                }
                        }
                        else
                        {
                                flag = word;
                                continue;
                        }
                }
                else if (isdigit(ch))
                {
                        if (quoteReaded)
                        {
                                if (flag == filename)
                                        continue;
                                else
                                {
                                        flag = stringconst;
                                        continue;
                                }
                        }
                        else
                        {
                                if (flag == null)
                                {
                                        flag = digit;
                                        continue;
                                }
                                else
                                        continue;
                        }
                }
                else if (ch == '"')
                {
                        quoteReaded = !quoteReaded;
                        needFlush = true;
                }
                else if ((ch == '-') || (ch == ':') || (ch == '.') || (ch == '/') || (ch == 92))	// 92 - '\'
                {
                        if (quoteReaded)
                        {
                                flag = filename;
                                continue;
                        }
                        else
                        {
                                needFlush = true;
                        }
                }
                else
                {
                        if (quoteReaded)
                        {
                                flag = stringconst;
                                continue;
                        }
                        else
                        {
                                needFlush = true;
                        }
                }

                if (needFlush)
                {
                        flush(flag, f2);
                        needFlush = 0;
                }
//************************************************************************************************************************


                switch (ch)
                {
                case '#': {string temp; f2 << "MACROS " << endl; getline(f1, temp); break; }
                case 0x20: break; //Если пробел пропускаем.
                case '\n': f2 << ch; break; //Если перевод на следующую строку, то записываем в файл
                case ',': f2 << "COMMA "; break; //Если запятая, то пишем в файл COMMA
                case 34: f2 << "QUOTE "; break;//Если " или ' то это будет просто QUOTE (34 = " ; 39 = ')
                case 39: f2 << "QUOTE "; break;//Если " или ' то это будет просто QUOTE (34 = " ; 39 = ')
                case ';': f2 << "SEMICOLON "; break;
                case '=': ravno(ch,f1,f2); break;//Если встретили первое равно, то должны проверить что дальше за ним (варианты = или ничего)
                case '{': f2 << "OBRACE "; break;
                case '}': f2 << "EMBRACE "; break;
                case '+': ravno(ch,f1,f2); break;//Если встретили +, то должны проверить что за ним(варианты + или ничего)
                case '-': ravno(ch, f1, f2); break;//Если встретили -, то должны проверить что за ним(варианты - или ничего)
                case '(': f2 << "OPENED_BRACKET "; break;
                case ')': f2 << "CLOSED_BRACKET "; break;
                case '*': f2 << "MULTIPLICATION "; break;
                case '/': slash(ch,f1,f2); break; // надо исправить
                case '<': menbol(ch, f1, f2); break; //Если встретили < то смотрим что дальше(варианты < или = или ничего)
                case '>': menbol(ch, f1, f2); break; //Если встретили > то смотрим что дальше(варианты > или = или ничего)
                case '[': f2 << "SQUARE_OPENED_BRACKET "; break;
                case ']': f2 << "SQUARE_CLOSED_BRACKET "; break;
                case '&': ravno(ch, f1, f2); break; // если встретили & то смотрим что за ним(варианты & или ничего)
                case '|': ravno(ch, f1, f2); break; // если встретили | то смотрим что за ним(варианты | или ничего)
                case '^': f2 << "BITWISE_XOR "; break;
                case ':': ravno(ch, f1, f2); break; // если встретили : то смотрим что дальше(варианты : или ничего)
                case '.': f2 << "DOT "; break;
                case '!': net(ch,f1,f2); break;
                };
        }
        return 0;
}
