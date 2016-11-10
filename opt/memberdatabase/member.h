/**************************************
 * Member.h
 *
 * interface for member class and a few 
 * helper functions
 *
 * part of RFIDacs
 *
 * Nile Mittow
 * ************************************/


#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <sstream>
#include <json/json.h>
#include <iostream>
#include <iomanip>

#include <string>
#include <sstream>
#include <ctime>
#include <sqlite3.h>
#include <json/json_tokener.h>
#include "/opt/libs/doorappMacros.h"

using namespace std;

class member {

  private:
 
  string   id;
  int      intId;
  string   fullName;
  string   salutation;
  string   accessCardId;
  int      intAccessCardId;
  bool     active;
  string   updatedOn;
  string   type;
  long     lastChecked;

  public:

  member();
  member(string _id, 
         string _fn, 
         string _sal, 
         string _acid, 
         string _act, 
         string _upd, 
         string _type, 
         long _last);

  ~member();

  bool operator==(member &other);
  bool operator!=(member &other);
 
  friend ostream &operator<<( ostream &os , const member &mem);
 
  void setId(string val);
  void setId(int val);
  string getId();
  int getIntId();

  void setFullName(string val);
  string getFullName();

  void setSalutation(string val);
  string getSalutation();
  
  void setAccessCardId(string val);
  void setAccessCardId(int val);
  string getAccessCardId();
  int getIntAccessCardId();

  void setActive(bool b);
  void setActive(string val);
  bool getActive();
  string getStringActive();

  void setUpdatedOn(string val);
  string getUpdatedOn();

  void setType(string val);
  string getType();

  long getLastChecked();
  void setLastChecked(string var);
  void setLastChecked(long var);
  void setLastChecked();

  bool initializeFromDB(string _dbname, string _id);
  bool initializeFromDBByAccid(string _dbname, string _accid);
  bool writeBackToDB(string _dbname);

  bool initializeFromJSON(json_object * data);
  bool initializeFromJSONString(string * data);
  bool initializeFromJSONString(string data);
  string getInsertString();
  string getUpdateString();
  void updateTag();

  bool isAllowed();

};/* END MEMBER */
#endif
