#ifndef INC_236LAB3_RELATION_H
#define INC_236LAB3_RELATION_H
#include <string>
#include <set>
#include "Tuple.h"
#include "Header.h"
#include <iterator>

using std::string;
using std::set;

class Relation {


private:

     string name;
    Header* header;
    set<Tuple> rows;

public:

    Relation(string& schemeName, Header* theHeader) {
        header = theHeader;
        name = schemeName;
    }
    Relation() {

    }

    vector<string> GetHeaderNames() {
        return header->GetAttributeNames();
    }

    void insertTuple(Tuple nextTuple) {
     rows.insert(nextTuple);
}
     std::string GetName() {
        return name;
    }

    set<Tuple> GetRows() {
        return rows;
    }

    Relation select(int index, std::string value) {

        Relation temp = Relation(name, header);

        for(set<Tuple>::iterator iter = rows.begin(); iter != rows.end(); ++iter) {

            vector<string> tupleValues = iter->GetValues(); // returning the rows values of each tuple
            // trying to determine which tuple we want to keep from list of tuples rows
            if (tupleValues.at(index) == value) {
                temp.insertTuple(*iter);
            }

        }

        return temp;

    }


    Relation select(int index1, int index2) {

        Relation temp = Relation(name, header);

        for(set<Tuple>::iterator iter = rows.begin(); iter != rows.end(); ++iter) {

            vector<string> tupleValues = iter->GetValues(); // returning the rows values of each tuple
            // trying to determine which tuple we want to keep from list of tuples rows
            if (tupleValues.at(index1) == tupleValues.at(index2)) {
                temp.insertTuple(*iter);
            }

        }


        return temp;
    }

    Relation project(vector<int> indices, vector<string> headerReNames) {

        Header* nextHeader = new Header();

        vector<string> oldHeaderNames = header->GetAttributeNames();

        for(unsigned int i = 0; i < indices.size(); ++i) {

            nextHeader->AddHeader(oldHeaderNames.at(indices.at(i)));
        }

         nextHeader->UpdateNames(headerReNames);

        Relation temp = Relation(name, nextHeader);


        // I only want to keep the the right index of each row
        vector<string> newRow;

        for(set<Tuple>::iterator iter = rows.begin(); iter != rows.end(); ++iter) {

            vector<string> tupleValues = iter->GetValues(); // returning the rows values of each tuple
            Tuple nextTuple = Tuple();
            for(unsigned int i = 0; i < indices.size(); ++i) {

                nextTuple.addToTuple(tupleValues.at(indices.at(i)));

            }
            temp.insertTuple(nextTuple);


        }
        return temp;



    }






};

#endif //INC_236LAB3_RELATION_H
