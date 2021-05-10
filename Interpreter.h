#ifndef INC_236LAB3_INTERPRETER_H
#define INC_236LAB3_INTERPRETER_H

#include "relation.h"
#include "Predicate.h"
#include <vector>
#include <string>
#include "Rules.h"
#include "Header.h"
#include "Tuple.h"
#include <map>

using std::string;
using std:: vector;
using std::map;


class Interpreter {




private:

    vector<Predicate> schemesList;
    vector<Predicate> factsList;
    vector<Predicate> queriesList;
    vector<Rule> rulesList;
    vector<Relation> relations;
    vector<Parameter*> tempParamsList;
    Tuple tuple = Tuple();
    vector<Relation> queryResults;
    vector<int> projectIndices;
    vector<string> reNameNames;

    map<string,Relation> relationMap;



public:

    Interpreter() {}

    void AddLists(vector<Predicate> schemes, vector<Predicate> facts, vector<Rule> rules, vector<Predicate> queries) {
        schemesList = schemes;
        factsList = facts;
        rules = rulesList;
        queriesList = queries;

    };


    void BuildRelations() {

        for (unsigned int i = 0; i < schemesList.size(); ++i) {
            std::string name = schemesList.at(i).GetId();
            tempParamsList = schemesList.at(i).GetHeaderNames();
            Header *attribute = new Header();

            for (unsigned int j = 0; j < tempParamsList.size(); ++j) {
                string headerName = tempParamsList.at(j)->ToString();
                attribute->AddHeader(headerName);
            }
            Relation nextRelation = Relation(name, attribute);
            relations.push_back(nextRelation);

        }

        for (unsigned int i = 0; i < factsList.size(); ++i) {

            tempParamsList = factsList.at(i).GetHeaderNames();
            for (unsigned int j = 0; j < tempParamsList.size(); ++j) {
                tuple.addToTuple(tempParamsList.at(j)->ToString());

            }
            for (unsigned int k = 0; k < relations.size(); ++k) {
                if (factsList.at(i).GetId() == relations.at(k).GetName()) {
                    relations.at(k).insertTuple(tuple);
                }
            }

            tuple = Tuple();


        }
    }

    void BuildRelationMap() {

        for(unsigned int i = 0; i < relations.size(); ++i) {
           relationMap.insert(std::pair<string,Relation>(relations.at(i).GetName(),relations.at(i)));

        }
    }


        void EvaluateQueries() {

            //implementing function with map

            Relation finalRelation;
            bool printRows = false;

            for (unsigned int i = 0; i < queriesList.size(); ++i) {
                 printRows = false;
                if (relationMap.find(queriesList.at(i).GetId()) != relationMap.end()) {
                    finalRelation = relationMap[queriesList.at(i).GetId()];
                    vector<Parameter *> paramNames = queriesList.at(i).GetHeaderNames();
                    map<string, int> nonConstants;
                    for (unsigned int k = 0; k < paramNames.size(); ++k) {
                        if (paramNames.at(k)->isConstant()) {
                            // if theres two selects in a row

                            finalRelation = finalRelation.select(k,paramNames.at(k)->ToString());


                        }

                            // only keep rows that have the same value as index k
                        else {

                            // if param is non constant see if matches any other non constants and pass in the index
                            if (nonConstants.find(paramNames.at(k)->ToString()) == nonConstants.end()) {
                                nonConstants.insert(std::pair<string, int>(paramNames.at(k)->ToString(), k));
                                projectIndices.push_back(k);
                                reNameNames.push_back(paramNames.at(k)->ToString());


                            } else {
                               // queryResults.push_back(relationMap[queriesList.at(i).GetId()].select(k,
                                                                                        //             nonConstants[paramNames.at(
                                                                                             //                k)->ToString()]));
                                         finalRelation = finalRelation.select(k, nonConstants[paramNames.at(
                                               k)->ToString()]);
                            }

                        }

                    }
                    if(projectIndices.size() > 0) {
                     //   queryResults.push_back(relationMap[queriesList.at(i).GetId()].project(projectIndices,reNameNames));
                      finalRelation =  finalRelation.project(projectIndices,reNameNames);
                      projectIndices.clear();
                      reNameNames.clear();
                      printRows = true;

                    }

                }
                queryResults.push_back(finalRelation);
                set<Tuple> rows = finalRelation.GetRows();
                if(rows.empty()) {
                    std::cout << queriesList.at(i).ToString() << "? No" << std::endl;
                }
                else {
                    std::cout << queriesList.at(i).ToString() << "? Yes(" << rows.size() << ")" << std::endl;
                    if(printRows) {
                        for (set<Tuple>::iterator iter = rows.begin(); iter != rows.end(); ++iter) {

                            vector<string> tupleValues = iter->GetValues();
                            vector<string> headerNames = finalRelation.GetHeaderNames();
                            std::cout << "  ";
                            for (unsigned l = 0; l < tupleValues.size(); ++l) {
                                std::cout << headerNames.at(l) << "=" << tupleValues.at(l);
                                if (l != tupleValues.size() - 1) {
                                    std::cout << ", ";
                                }


                            }

                            std::cout << std::endl;
                        }
                    }
                }

            }
            // coult just print out the relation that I want right here
            //queryResults.push_back(finalRelation);
          //  PrintQueryResults();
        }



        void PrintQueryResults() {

         for(unsigned int i = 0; i < queriesList.size(); i++) {


                 if(queriesList.at(i).GetId() == queryResults.at(i).GetName()) {
                     set<Tuple> rows = queryResults.at(i).GetRows();
                     if(rows.empty()) {
                         std::cout << queriesList.at(i).ToString() << "? No" << std::endl;
                     }
                     else {

                         std::cout << queriesList.at(i).ToString() << "? Yes(" << rows.size() << ")" << std::endl;

                         for(set<Tuple>::iterator iter = rows.begin(); iter != rows.end(); ++iter) {

                             vector<string> tupleValues = iter->GetValues();
                             vector<string> headerNames = queryResults.at(i).GetHeaderNames();
                             vector<Parameter*> parameterNames = queriesList.at(i).GetHeaderNames();
                             for(unsigned l = 0; l < tupleValues.size(); ++l) {
                                 if(!parameterNames.at(l)->isConstant()) {
                                     std::cout << headerNames.at(l) << "=" << "'" << tupleValues.at(l) << "'";
                                     if (l != tupleValues.size() - 1) {
                                         std::cout << ",";
                                     }
                                 }

                             }

                            std::cout << std::endl;
                         }
                     }

                 }
         }




    }









};

#endif //INC_236LAB3_INTERPRETER_H
