#include <iostream>
#include <string>
#include "vector"
#include "stack"

#include "apply.h"

using namespace std;

bool isOperator(char ch) {
    if (ch == '+' || ch == '*')
        return true;
    return false;
}

int getPriority(char ch) {
    int level = 0; // 优先级
    switch (ch) {
        case '(':
            level = 1;
            break;
        case '+':
            level = 2;
            break;
        case '*':
            level = 3;
            break;
        default:
            break;
    }
    return level;
}

vertex *toVertex(const string &s, vector<string> name, int &id, int n) {
    //将每个布尔变量转化成BDD结点
    auto *t = new vertex(nullptr, nullptr, -1, n + 1, True, false);        //终止节点true
    auto *f = new vertex(nullptr, nullptr, -1, n + 1, False, false);        //终止节点false
    for (int i = 0; i < name.size(); i++) {
        if (s == name[i]) {
            auto *v = new vertex(t, f, id++, i + 1, X, false);
            return v;
        }
    }
    return nullptr;
}


vector<string> split(const string &s, const string &seperator) {
    //找出所有的布尔变量
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;

    while (i != s.size()) {
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while (i != s.size() && flag == 0) {
            flag = 1;
            for (string_size x = 0; x < seperator.size(); ++x) {
                if (s[i] == seperator[x]) {
                    ++i;
                    flag = 0;
                    break;
                }
            }
        }

        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while (j != s.size() && flag == 0) {
            for (char x : seperator) {
                if (s[j] == x) {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)
                ++j;
        }
        if (i != j) {
            result.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return result;
}


void exchange(vertex *v) {
    //非运算交换Low与High
    v->setMark(!v->getMark());
    if (v->getValue() != X) {
        if (v->getValue() == True)
            v->setValue(False);
        if (v->getValue() == False)
            v->setValue(True);
    }
    if (v->getMark() != v->getLow()->getMark())
        exchange(v->getLow());
    if (v->getMark() != v->getHigh()->getMark())
        exchange(v->getHigh());
}

void
pushOneArg(string &temp, stack<string> &args, const vector<string> &name, int id, bool &flag1,
           stack<vertex *> &vertices, int n) {
    //参数压入栈，同时对应生成BDD结点入另一个栈
    if (temp.length() != 0) {
        args.push(temp);
        vertex *v = toVertex(temp, name, id, n);
        if (flag1) {        //取反
            exchange(v);
            flag1 = false;
        }
        vertices.push(v);
        temp = "";
    }
}

void popTwoArgs(stack<string> &args, char c, stack<vertex *> &vertices, int n, vector<vector<vertex *>> table) {
    //弹出两个参数，转成BDD结点并进行计算
    string s1 = args.top();
    args.pop();
    string s2 = args.top();
    args.pop();
    s1 = s2.append(string(1, c)).append(s1);
    args.push(s1);

    vertex *v1 = vertices.top();
    vertices.pop();
    vertex *v2 = vertices.top();
    vertices.pop();
    vertex *u = apply(v1, v2, c, n, table);
    vertices.push(u);
}


vertex *toGraph() {
    string s;
    getline(cin, s);
    vector<string> name = split(s, "*+()!");
    int n = name.size();
    //暂存结点数组，重用
    vector<vertex *> t(n, nullptr);
    vector<vector<vertex *>> table(n, t);

    stack<string> args;
    stack<char> op;
    stack<vertex *> vertices;
    int len, i, id = 1;
    char c;
    len = s.length();
    i = 0;
    bool flag1 = false;     //下一个元素是否取反
    bool flag2 = false;     //括号整体取反
    string temp;
    while (i < len) {
        if (s[i] == '(') {
            //之前的参数压入栈，生成对应BDD结点入栈
            pushOneArg(temp, args, name, id, flag1, vertices, n);

            op.push(s[i]);
        } else if (s[i] == '!') {
            if (s[i + 1] == '(') {
                flag2 = true;
            } else {
                flag1 = true;
            }
        } else if (isOperator(s[i])) {   // 操作符
            pushOneArg(temp, args, name, id, flag1, vertices, n);    //之前的参数压入栈
            if (op.empty()) {   // 如果栈空，直接压入栈
                op.push(s[i]);
            } else {
                while (!op.empty()) {
                    c = op.top();
                    if (getPriority(s[i]) <= getPriority(c)) {
                        // 优先级低或等于
                        popTwoArgs(args, c, vertices, n, table);
                        op.pop();
                    } else // ch优先级高于栈中操作符
                        break;
                }
                op.push(s[i]); // 防止不断的推出操作符，最后空栈了;或者ch优先级高了
            }
        } else if (s[i] == ')') {
            pushOneArg(temp, args, name, id, flag1, vertices, n);
            while (op.top() != '(') {
                popTwoArgs(args, op.top(), vertices, n, table);
                if (flag2) {      //整个括号要取反
                    vertex *u = vertices.top();
                    vertices.pop();
                    exchange(u);
                    vertices.push(u);
                    flag2 = false;
                }
                op.pop();
            }
            op.pop(); // 把左括号(推出栈
        } else {
            //布尔变量
            temp.push_back(s[i]);
        }
        i++;
    }
    while (!op.empty()) {
        pushOneArg(temp, args, name, id, flag1, vertices, n);
        popTwoArgs(args, op.top(), vertices, n, table);
        op.pop();
    }
    if(!temp.empty()){
        //无操作符的情况
        pushOneArg(temp, args, name, id, flag1, vertices, n);
    }
    return vertices.top();
}