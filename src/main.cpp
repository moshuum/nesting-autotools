/*
    nesting! with autotools and some threading
    Copyright (C) 2013 Víctor Navarro

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <string>
#include <sstream>
#include <list>
#include <iostream>

#include <pthread.h>
#include <signal.h>
#include <sys/select.h>
extern "C" { 
  #include <sqlite3.h>
}

#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Priority.hh>

using namespace std;

#define DATABASE "database.sqlite"
#define NUM_THREADS 101

// the lock
pthread_mutex_t lock;

// the shared resource
std::list< std::string > queue;

extern "C" {
  void *worker(void *arg);
}

void *worker(void *arg) 
{
  sqlite3 *db;
  char *zErrMsg = 0;

  pthread_mutex_lock( &lock );

  if (queue.size() != 0) {
    // insert a record into the database
    sqlite3_open(DATABASE, &db);
    sqlite3_exec(db, "INSERT INTO records VALUES(NULL)", NULL, 0, &zErrMsg);
    sqlite3_free(zErrMsg);
    sqlite3_close(db);

    queue.pop_front();
  }
  pthread_mutex_unlock( &lock );

  //std::cout << "done.\n";
  pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
  unsigned int i;
  int rc;
  sqlite3 *db;
  sqlite3_stmt *stmt;
  char *zErrMsg = 0;

  std::string sString;

  log4cpp::Appender *appender2 = new log4cpp::FileAppender("default", "nesting.log");
  log4cpp::Category& root = log4cpp::Category::getRoot();

  log4cpp::Category& sub = log4cpp::Category::getInstance(std::string("nesting"));
  sub.addAppender(appender2);

  // setup the database
  if (sqlite3_open_v2(DATABASE, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) == SQLITE_OK) {
    if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS records (ID INTEGER PRIMARY KEY NOT NULL)", NULL, NULL, &zErrMsg) != SQLITE_OK) {
      sString = "Failed to create table\n";
      std::cout << sString;
      sub.error(sString);
      exit(-1);
    }
    if (sqlite3_exec(db, "DELETE FROM records", NULL, NULL, &zErrMsg) != SQLITE_OK) {
        sString = "Failed to delete records\n";
        std::cout << sString;
        sub.error(sString);
        exit(-1);
    }
    sqlite3_close(db);
  }
  else {
    sString = "Failed to open/create database\n";
    std::cout << sString;
    sub.error(sString);
    exit(-1);
  }

  // set-up mutex
  pthread_mutex_init( &lock, NULL );

  std::cout << "Starting " << NUM_THREADS << " threads to store " << NUM_THREADS << " records\n";
  sub.info("Starting %d threads to store %d records\n", NUM_THREADS, NUM_THREADS);

  for (i = 0; i < NUM_THREADS; i++) {
    std::stringstream ss_id;
    ss_id << "id " << i; 
    queue.push_back(ss_id.str());
  }

  // setup a list for thread's id
  std::list< pthread_t* > thread_list;

  // start the threads
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_t *thread_id (new pthread_t);
    thread_list.push_back(thread_id);
    rc = pthread_create( thread_id , NULL, worker, NULL);
    if (rc) {
      sString = "thread_create() error\n";
      std::cout << sString;
      sub.error(sString);
      exit(-1);
    }
  }
  
  // join threads
  std::list< pthread_t* >::iterator it(thread_list.begin());
  while (it != thread_list.end()) {
    if (pthread_join( *(*it), NULL) != 0) {
      sString = "pthread_join() error!\n";
      std::cout << sString;
      sub.error(sString);
      exit(1);
    }
    delete (*it);
    thread_list.erase(it++);
  }
  
  // clean
  pthread_mutex_destroy(&lock);

  // print records
  std::cout << "<xml>\n";
  sqlite3_open(DATABASE, &db);
  sqlite3_prepare_v2(db, "SELECT * FROM records", -1, &stmt, 0);
  do {
    rc = sqlite3_step(stmt);
    switch(rc){
      case SQLITE_DONE:
      break;
      case SQLITE_ROW:
          sString = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
          std::cout << "\t<inicio indice=" << sString << ">HolaMundo</inicio>\n";
        break;
        default:
          std::cout << "sqlite3_step() error.";
          break;
    }
  } while (rc==SQLITE_ROW);
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  std::cout << "</xml>\n";  


  sString = "all done!\n";
  std::cout << sString;
  sub.info(sString);

  return(0);
}
