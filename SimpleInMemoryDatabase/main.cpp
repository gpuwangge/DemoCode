#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
using namespace std;


//https://www.thumbtack.com/challenges/simple-database
//In the Simple Database problem, you'll implement an in-memory database similar to Redis. 
//For simplicity's sake, instead of dealing with multiple clients and communicating over the network, 
//your program will receive commands via standard input(stdin), 
//and should write appropriate responses to standard output(stdout).


/*****Note***********
1. To satisfy performance requirement, use hashmap(unordered_map)
2. To implEment rolling back feature and satisfy space requirement, use a stacked changelist to store modification
3. Based on our hashmap data structure, NUMEQUALTO is searching by value. Since hashmap is not good at this, need another hashmap (and another stacked changelist)
*********************/

/***A class for database***/
class CDatabase{
private:
    /******Data structure******/
    unordered_map<string, int> m_data;
    unordered_map<int, int> m_counter;
    stack<unordered_map<string, int>> changeDataList; //if it is empty, means no transaction
    stack<unordered_map<int, int>> changeCounterList; 
    /*Some functions that can be used to easily change counter*/
    void increaseCounter(unordered_map<int, int> &counter, int value){
        if(counter.find(value) != counter.end()) {
            counter[value]++;
            if(counter[value] == 0) counter.erase(value);
        }else counter[value] = 1;
    }
    void decreaseCounter(unordered_map<int, int> &counter, int value){
        if(counter.find(value) != counter.end()){
            counter[value]--;
            if(counter[value] == 0) counter.erase(value);
        }else counter[value] = -1;
    }
public:
    /******Data commands******/
    void set(string key, int value){
        /*Handle changeList for data*/
        if(!changeDataList.empty())
            if(changeDataList.top().find(key) == changeDataList.top().end())
                if(m_data.find(key) != m_data.end()) changeDataList.top()[key] = m_data[key];
                else changeDataList.top()[key] = NULL;
        /*Handle changeList for counter*/
        if(!changeCounterList.empty()){
            if(m_data.find(key) != m_data.end()) decreaseCounter(changeCounterList.top(), m_data[key]);
            increaseCounter(changeCounterList.top(), value);
        }
        /*Handle counter*/
        if(m_data.find(key) != m_data.end()) decreaseCounter(m_counter, m_data[key]);
        increaseCounter(m_counter, value);
        /*Handle data*/
        m_data[key] = value;
    }
    void unset(string key){
        if(m_data.find(key) != m_data.end()){
            int value = m_data[key];
            /*Hanlde changeList for data*/
            if(!changeDataList.empty())
                if(changeDataList.top().find(key) == changeDataList.top().end())
                   changeDataList.top()[key] = m_data[key];
            /*Handle changeList for counter*/
            if(!changeCounterList.empty())
                decreaseCounter(changeCounterList.top(), value);
            /*Handle data*/
            m_data.erase(key);
            /*Handle counter*/
            decreaseCounter(m_counter, value);
        }
    }
    void get(string key){
        cout<<"> ";
        if(m_data.find(key) != m_data.end()) cout<<m_data[key]<<endl;
        else cout<<"NULL"<<endl;
    }
    void numequalto(int value){
        cout<<"> ";
        if(m_counter.find(value) != m_counter.end()) cout<<m_counter[value]<<endl;
        else cout<<0<<endl;
    }
    /******Transaction commands******/
    void begin(){
        changeDataList.push(unordered_map<string, int>());
        changeCounterList.push(unordered_map<int, int>());
    }
    void rollback(){
        if(changeDataList.empty()) cout<<"> NO TRANSACTION"<<endl;
        else{
            for(auto it = changeDataList.top().begin(); it != changeDataList.top().end(); it++)
                if(it->second == NULL) m_data.erase(it->first);
                else m_data[it->first] = it->second;
            changeDataList.pop();
        }
        if(!changeCounterList.empty()){
            for(auto it = changeCounterList.top().begin(); it != changeCounterList.top().end(); it++)
                if(m_counter.find(it->first) != m_counter.end()) m_counter[it->first] -= it->second;
                else m_counter[it->first] = -it->second;
            changeCounterList.pop();
        }
    }
    void commit(){
        if(changeDataList.empty()) cout<<"> NO TRANSACTION"<<endl;
        else{
            while(!changeDataList.empty()) changeDataList.pop();
            while(!changeCounterList.empty()) changeCounterList.pop();
        }
    }
}db;

/*Segmentation of command*/
void parseLine(string line, string parsedLine[3]){
    string delimiter = " ";
    int i = 0;
    while(line != ""){
        int pos = line.find(delimiter);
        if(pos == string::npos){
            parsedLine[i++] = line;
            line = "";
        }else{
            parsedLine[i++] = line.substr(0, pos);
            line = line.substr(pos+1, line.length() - pos);
        }
    }
}

/*Because can't apply switch on string type, need to convert*/
enum cmd_code{SET, UNSET, GET, NUMEQUALTO, END, BEGIN, ROLLBACK, COMMIT};
int decode(string s){ 
    if(s == "SET") return SET;
    if(s == "UNSET") return UNSET;
    if(s == "GET") return GET;
    if(s == "NUMEQUALTO") return NUMEQUALTO;
    if(s == "END") return END;
    if(s == "BEGIN") return BEGIN;
    if(s == "ROLLBACK") return ROLLBACK;
    if(s == "COMMIT") return COMMIT;
    return END;
}

void ProcessCommand(string line){
	string parsedLine[3];
    parseLine(line, parsedLine);
    switch(decode(parsedLine[0])){
        case SET:
            db.set(parsedLine[1], stoi(parsedLine[2]));
            break;
        case UNSET:
            db.unset(parsedLine[1]);
            break;
        case GET:
            db.get(parsedLine[1]);
            break;
        case NUMEQUALTO:
            db.numequalto(stoi(parsedLine[1]));
            break;
        case END:
            exit(EXIT_SUCCESS);
        case BEGIN:
            db.begin();
            break;
        case ROLLBACK:
            db.rollback();
            break;
        case COMMIT:
            db.commit();
            break;
    }
}

int main(int argc, char* argv[]){//argc: number of arguments, argv[0] is binary name, argv[1] is the first input
	if(argc > 1){
		fstream myfile;
		string filename(argv[1]);
		myfile.open(filename, fstream::in);
	
		string line;
		while(!myfile.eof()){
			getline(myfile, line); 
			ProcessCommand(line);
		}
		myfile.close();
		return 0;
	}

    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    string line;
    while(true){
        getline(cin, line);
        cout<<line<<endl;
        ProcessCommand(line);
	}
    return 0;
}