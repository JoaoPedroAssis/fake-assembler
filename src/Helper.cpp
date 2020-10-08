#include "../include/Helper.h"

const vector<string> split(const string& s, const char& c1, const char& c2) {
	string buff{""};
	vector<string> v;
	
	for(auto n:s) {
		if(n != c1 && n != c2) {
            buff+=n; 
        }
        else if((n == c1 || n == c2) && buff != "") { 
            v.push_back(buff); buff = ""; 
        }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

Line* getLineElements(vector<string> &lineContents) {
    Line *l = new Line();
    int opIdx = 0;
    
    size_t pos = lineContents[0].find(":");
    if (pos != string::npos) {
        l->label = lineContents[0].substr(0,pos);
        try {
            l->operation = lineContents.at(1);
        } catch (exception) {
            // if the index does not exist at this point, do nothing
        };
        opIdx++;
    } else {
        l->operation = lineContents[0];
    }

    for (int i = opIdx+1; i < lineContents.size(); i++) {
        if (lineContents[i].find(";") == string::npos) {
            l->args.push_back(lineContents[i]);
        }
    }

    return l;
}

bool hasOnlyLabel(Line* l) {
    return !l->label.empty() and l->operation.empty() and (l->args.size() == 0);
}

string printArgs(Line *l) {
    string retLine = "";
    for (int i = 0; i < l->args.size(); i++) {
        retLine += l->args[i] + (l->args.size()-1 == i ? "":" ");
    }
    
    return retLine;
}

string toUpper(string str) {
    transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;
}
