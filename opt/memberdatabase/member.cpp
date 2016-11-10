/***********************************
 * member.cpp
 *
 * implementation for member class
 * part of RFIDacs
 *
 * Nile Mittow
 * ********************************/

#include "member.h"

using namespace std;

/*static int update_callback(void* none, int argc, char **argv, char **azColName){
  return 0;
}*/

static int initialize_callback(void* memb, int argc, char **argv, char **azColName){
  int rv = 0;
  member * mem = (member*)memb;
  if (argc > 0){
    
    if(argv[0] == NULL) mem->setId("null");
    else mem->setId(string(argv[0]));
    
    if(argv[1] == NULL) mem->setFullName("null");
    mem->setFullName(string(argv[1]));
    
    if(argv[2] == NULL) mem->setSalutation("null");
    else mem->setSalutation(string(argv[2]));
    
    if(argv[3] == NULL) mem->setAccessCardId("null");
    else mem->setAccessCardId(string(argv[3]));
    
    if(argv[4] == NULL) mem->setActive("false");
    else mem->setActive(string(argv[4]));
    
    if(argv[5] == NULL) mem->setUpdatedOn("null");
    else mem->setUpdatedOn(string(argv[5])); 
    
    if(argv[6] == NULL) mem->setType("null");
    else mem->setType(string(argv[6]));
    
    if(argv[7] == NULL) mem->setLastChecked("null");
    else mem->setLastChecked(string(argv[7]));
  }
  else {
    cerr << "Error during initialize member db request" << endl;
    rv = 1;
  }
  return rv;
}

string intToString(int val) {
  stringstream ss;
  ss << val;
  return ss.str();
}

member::member() {
  id              = "\0";
  intId           =   0;
  fullName        = "\0";
  salutation      = "\0";
  accessCardId    = "\0";
  intAccessCardId =   0;
  active          = false;
  updatedOn       = "\0";
  type            = "\0";
  lastChecked     =   0;
}

member::member(string _id,
               string _fn,
               string _sal,
               string _acid,
               string _act,
               string _upd,
               string _type,
               long _last) {
  this->setId(_id);
  fullName = _fn;
  salutation = _sal;
  this->setAccessCardId(_acid);
  this->setActive(_act);
  updatedOn = _upd;
  type = _type;
  lastChecked = _last;
}

member::~member(){}

bool member::operator==(member &other) {
  return ((this->id         == other.getId())         &&
          (this->intId      == other.getIntId())      &&
          (this->fullName   == other.getFullName())   &&
          (this->salutation == other.getSalutation()) &&
          (this->active     == other.getActive())     &&
          (this->updatedOn  == other.getUpdatedOn())  &&
          (this->type       == other.getType())); 
}
  
bool member::operator!=(member &other) { return !(*this == other); }

ostream& operator<< ( ostream& os, const member& mem)
{
    os << setw(9)  << "_____ID_|"
       << setw(20) << "_________FULLNAME__|"
       << setw(13) << "_SALUTATION_|"
       << setw(6)  << "_ACT_|"
       << setw(13) << "__ACCESS_ID_|"
       << setw(21) << "_________UPDATED_ON_|"
       << setw(30) << "________________________TYPE_|"
       << setw(11) << "_LAST_CHK_|\n"
       << setw(8)  << mem.id << ' '
       << setw(19) << mem.fullName << ' '
       << setw(12) << mem.salutation << ' '
       << setw(5)  << mem.active << ' '
       << setw(12) << mem.accessCardId << ' '
       << setw(20) << mem.updatedOn << ' '
       << setw(29) << mem.type << ' '
       << setw(10) << mem.lastChecked << ' ';
    return os;
}

void member::setId(string val) {
  id = val;
  istringstream(val) >> intId;
  return;
}

void member::setId(int val) {
  intId = val;
  stringstream iid;
  iid << val;
  id = iid.str();
  return;
}

string member::getId() {return id;}
int member::getIntId() {return intId;}

void member::setFullName(string val) {fullName = val; return;}
string member::getFullName() {return fullName;}

void member::setSalutation(string val) {salutation = val; return;}
string member::getSalutation() {return salutation;}
  
void member::setAccessCardId(string val) {
  accessCardId = val;
  if (val == "null") intAccessCardId = 0;
  else istringstream(val) >> intAccessCardId;
  return;
}

void member::setAccessCardId(int val) {
  intAccessCardId = val;
  if(val == 0) accessCardId = "null";
  else {
    stringstream iid;
    iid << val;
    accessCardId = iid.str();
  }
  return;
}
string member::getAccessCardId() {return accessCardId;}
int member::getIntAccessCardId() {return intAccessCardId;}

void member::setActive(bool b) {active = b;return;}
void member::setActive(string val) {
  if ((val == "true")||(val == "1")) active = true;
  else active = false;
  return;
}
bool member::getActive() {return active;}
string member::getStringActive() {
  if (active) return "1";
  else return "0";
}

void member::setUpdatedOn(string val) {updatedOn = val; return;}
string member::getUpdatedOn() {return updatedOn;}

void member::setType(string val) {
  type = val;
  return;
}
string member::getType() {
  return type;
}

long member::getLastChecked() {
  return lastChecked;
}
void member::setLastChecked(string var) {
  istringstream(var) >> lastChecked;
  return;
}
void member::setLastChecked(long var) {
  lastChecked = var;
  return;
}

void member::setLastChecked() {
  lastChecked = time(0);
  return;
}

bool member::initializeFromDB(string _dbname, string _id) {
  int ret;
  bool rv = false;
  char * zErrMsg = 0;
  sqlite3 *db;
  sqlite3_open(_dbname.c_str(), &db);
  if (SQLITE_OK != (ret = sqlite3_initialize())) {
    cerr << "Failed to initialize library" << endl;
  }
  else {
    string query = "select id, fname, salutation, accid, active, lastmod, type, lastchecked from member where id = " + _id + ";";
    sqlite3_exec(db, query.c_str(), initialize_callback, this, &zErrMsg);
    rv = true; /// NEED TO ADD MORE SQL ERROR CHECKING
  }
  sqlite3_close(db);
  return rv;
}

bool member::initializeFromDBByAccid(string _dbname, string _accid) {
  int ret;
  bool rv = false;
  char * zErrMsg = 0;
  sqlite3 *db;
  sqlite3_open(_dbname.c_str(), &db);
  if (SQLITE_OK != (ret = sqlite3_initialize())) {
    cerr << "Failed to initialize library" << endl;
  }
  else {
    string query = "select id, fname, salutation, accid, active, lastmod, type, lastchecked from member where accid = " + _accid + ";";
    sqlite3_exec(db, query.c_str(), initialize_callback, this, &zErrMsg);
    rv = true; /// NEED TO ADD MORE SQL ERROR CHECKING
  }
  sqlite3_close(db);
  return rv;
}

bool member::writeBackToDB(string _dbname) {
  int ret;
  bool rv = true;
  char * zErrMsg = 0;
  sqlite3 *db;
  sqlite3_open(_dbname.c_str(), &db);
  if (SQLITE_OK != (ret = sqlite3_initialize())) {
    cerr << "Failed to initialize library" << endl;
  }
  else {
    string stract = "0";
    if (active) stract = "1";
    
    stringstream numss;       //converting long to string
    numss << lastChecked;


/*
td::string createIfNot = "create table if not exists MEMBER(" \
                            "ID INT PRIMARY KEY NOT NULL," \
                            "FNAME TEXT NOT NULL," \
                            "SALUTATION TEXT NOT NULL," \
                            "ACCID INT UNIQUE," \
                            "ACTIVE BOOLEAN NOT NULL CHECK (ACTIVE IN (0,1))," \
                            "LASTMOD TEXT," \
                            "TYPE TEXT, "\
                            "LASTCHECKED INT);"; */
    string acid;
    if (accessCardId.empty()) acid = "null";
    else acid = accessCardId;

    string insertquery = "insert into member (ID,FNAME,SALUTATION,ACCID,ACTIVE,LASTMOD,TYPE,LASTCHECKED) values (" +
                                             id + ","+
                                             "\"" + fullName + "\"," +
                                             "\"" + salutation + "\"," +
                                             acid + "," +
                                             stract + "," +
                                             "\"" + updatedOn + "\"," +
                                             "\"" + type + "\"," +
                                             numss.str() + ");";

    string updatequery = "update member set fname   = \"" + fullName     + "\" , " +
                                   "salutation   = \"" + salutation   + "\" , " +
                                   "accid        = "   + acid + ","     +
                                   "active       = "   + stract       + ","     +
                                   "lastmod      = \"" + updatedOn    + "\" , " +
                                   "type         = \"" + type         + "\" , " +
                                   "lastchecked  =   " + numss.str()  +
                   " where id = " + id + ";";

   //cout << insertquery << endl << updatequery << endl;
   ret = sqlite3_exec(db, insertquery.c_str(), 0 , 0, &zErrMsg);
   if (ret != SQLITE_OK) {

     string target = "UNIQUE constraint failed: MEMBER.ID";
     string arrow  = string(zErrMsg);
     if (target != arrow) {
       cout << time(0) << "insert " << zErrMsg << endl;
       cout << insertquery << endl;
     }
     sqlite3_free(zErrMsg);
   }

   ret = sqlite3_exec(db, updatequery.c_str(), 0 , 0, &zErrMsg);
   if (ret != SQLITE_OK) {
      cout << time(0) << "update " << zErrMsg << endl;
      sqlite3_free(zErrMsg);
   }

   //cout << updatequery << endl;
   /** TODO: error check for DB write success **/
  }
  sqlite3_close(db);
  return rv;

}

bool member::initializeFromJSON(json_object * data) {
  bool rv = true;
  json_object * currField;

  json_object_object_get_ex(data,"Id",&currField);
  if (json_object_is_type(currField, json_type_null)) {
     this->setId("null");
  }
  else {
    this->setId(json_object_get_string(currField));
  }

  json_object_object_get_ex(data,"FullName",&currField);
  if (json_object_is_type(currField, json_type_null)) {
    this->setFullName("null");
  }
  else {
    this->setFullName(json_object_get_string(currField));
  }

  json_object_object_get_ex(data,"Salutation",&currField);
  if (json_object_is_type(currField, json_type_null)) {
    this->setSalutation("null");
  }
  else {
    this->setSalutation(json_object_get_string(currField));
  }

  json_object_object_get_ex(data,"AccessCardId",&currField);
  if (json_object_is_type(currField, json_type_null)) {
    this->setAccessCardId("null");
  }
  else {
    this->setAccessCardId(json_object_get_string(currField));
  }

  json_object_object_get_ex(data,"Active",&currField);
  string act = json_object_get_string(currField);
  this->setActive(act);

  json_object_object_get_ex(data,"UpdatedOn",&currField);
  if (json_object_is_type(currField, json_type_null)) {
    this->setUpdatedOn("null");
  }
  else {
    this->setUpdatedOn(json_object_get_string(currField));
  }

  json_object_object_get_ex(data,"TariffName",&currField);
  if (json_object_is_type(currField, json_type_null)) {
    this->setType("null");
  }
  else {
    this->setType(json_object_get_string(currField));
  }

  return rv;
}

bool member::initializeFromJSONString(string * data) {
  json_object * input = json_tokener_parse(data->c_str());
  return this->initializeFromJSON(input);
}

bool member::initializeFromJSONString(string data) {
  return this->initializeFromJSONString(&data);
}

string member::getInsertString() {
  
  return "rstr";
}

string member::getUpdateString() {
  return "rstr";
}

void member::updateTag() {
  return;
}

bool member::isAllowed() {
  return this->getActive();
}

