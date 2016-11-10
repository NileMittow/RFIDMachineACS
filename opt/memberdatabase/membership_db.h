/*****************************
 * membership_db.h 
 *
 * wrapper for db calls to
 * sqlite3 member db
 * **************************/

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <unistd.h>

#include <json/json.h>
#include <json/json_tokener.h>
#include <sqlite3.h>
#include <curl/curl.h>
#include <sqlite3.h>

#include "member.h"
#include "/opt/libs/doorappMacros.h"

using namespace std;

static int null_callback(void *data, int argc, char **argv, char **azColName){ return 0;}


int createDBIfNotExists(string db_location) {
  char * zErrMsg = 0;
  int ret = 0;
  sqlite3 *db;
  sqlite3_open(db_location.c_str(), &db);
  if (SQLITE_OK != (ret = sqlite3_initialize())) {
    std::cerr << "Failed to initialize library" << std::endl;
  }
  std::string createIfNot = "create table if not exists MEMBER(" \
                            "ID INT PRIMARY KEY NOT NULL," \
                            "FNAME TEXT NOT NULL," \
                            "SALUTATION TEXT NOT NULL," \
                            "ACCID INT UNIQUE," \
                            "ACTIVE BOOLEAN NOT NULL CHECK (ACTIVE IN (0,1))," \
                            "LASTMOD TEXT," \
                            "TYPE TEXT, "\
                            "LASTCHECKED INT);";

  sqlite3_exec(db, createIfNot.c_str(), null_callback, 0, &zErrMsg);
  sqlite3_close(db);
  /** TODO: ADD check for error **/
  return ret;
}

void deleteMissingMembers(string db_name, long checkpoint) {

  int ret;
  //bool rv = false;
  char * zErrMsg = 0;
  sqlite3 *db;
  sqlite3_open(db_name.c_str(), &db);
  if (SQLITE_OK != (ret = sqlite3_initialize())) {
    cerr << "Failed to initialize library" << endl;
  }
  else {
    stringstream sscheck;
    sscheck << checkpoint;
    string query = "delete from member where LASTCHECKED < " + sscheck.str() + ";";
    sqlite3_exec(db, query.c_str(), null_callback, 0, &zErrMsg);
    //rv = true; /// TODO: ADD MORE SQL ERROR CHECKING
  }
  sqlite3_close(db);
  return;
}

size_t write_data_callback(void *ptr, size_t size, size_t nmemb, string *data_out) {
  data_out->append((char*)ptr,0,size*nmemb);
  return size * nmemb;
}

int get_curl_result(string * listptr, string url) {
  //cout << "\n curl request started" << endl;
  CURL *curl_handle;
  CURLcode res;
  int code = 0;

  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_handle, CURLOPT_USERPWD, USER_PASS);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data_callback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, listptr);
  curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 120);
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  res = curl_easy_perform(curl_handle);

  /** Check for curl running fine & host resolution **/
  if(res != CURLE_OK) {
    cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
    code = 1;
  }

  /** Check for requested data being found by Nexudus **/
  else if (*listptr == "\"Not found\"") {
    cout << "Nexudus: requested data not found" << endl;
    code = 2;
  }

  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();

  //cout << "Curl result: " << *listptr << endl;

  return code;
}

int get_curl_member_list(string * listptr) {return get_curl_result(listptr,CURL_LIST_STRING);}

int get_curl_member(string *listptr, string member_id) {
  string call_str = CURL_USER_STRING + member_id;
  return get_curl_result(listptr, call_str);
}

int get_curl_member(string *listptr, int member_id) {
  string call_str;
  ostringstream convert;
  convert << member_id;
  call_str = CURL_USER_STRING + convert.str();
  return get_curl_result(listptr,call_str);
}

string get_curl_member_string(string member_id) {
  string json = "";
  get_curl_member(&json, member_id);
  return json;
}


int update(string db_location, int * trycount) {

/** MASSIVE TODO:  There is a select statement that needs to go 
 ** here somewhere.                                            **/
 
  createDBIfNotExists(DB_LOCATION); 
 
  int ret = 0;
  long begin;
 
           
 /** Get List From Nexudus, if success, continue **/
  string * pulled_list = new string;
  if(get_curl_member_list(pulled_list) == 0) {

    //cout << "From update: " << *pulled_list << endl;

    /** Set beginning time **/
    begin = time(0);
    json_object * memberArray;

    /** Check that records are present **/


    if (json_object_object_get_ex(json_tokener_parse(pulled_list->c_str()),"Records",&memberArray)) {      

      int i = 0;
      int numRecords = json_object_array_length(memberArray);

      //cout << "\n Number of records pulled: " << numRecords << endl;

      /** Check each new record against db **/
      for (i=0;i<numRecords;i++){
        member newmem, oldmem;
        newmem.initializeFromJSON(json_object_array_get_idx(memberArray,i));
        oldmem.initializeFromDB(DB_LOCATION, newmem.getId());



        if(oldmem != newmem) {
          sleep(1);
          int t = 0;
          string curlmem;
          while ((!newmem.initializeFromJSONString(curlmem = get_curl_member_string(newmem.getId())))&&(t<4)){
            t++;
            sleep(10);
            newmem.initializeFromJSONString(get_curl_member_string(newmem.getId()));
          }
          if (t==3) {
            cerr << "Error: cannot find member that should exist" << endl;
            /**TODO: add robust connection error handling **/
          }
          //cout << curlmem << endl;
          cout << "\nUpdating:\n"<< oldmem << "\nTo:\n" << newmem << endl;
	  newmem.setLastChecked();
	  newmem.writeBackToDB(db_location); 
        }
        //newmem.setLastChecked();
        //newmem.writeBackToDB(db_location);
      }
      /*** Delete anyone who did not appear in the nexudus pull **/
      //deleteMissingMembers(db_location, begin);
    }
    else {
      ret = 2;
    }
  }
  else{
    ret = 1;
  }
  delete pulled_list;
  return ret;
}


