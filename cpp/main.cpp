#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <fstream>
#include <sstream>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

void createTables(string file, connection *C) {
  string sql, current;
  ifstream s;
  s.open(file.c_str(), ifstream::in);
  while(getline(s, current)) {
    sql += current;
  }
  s.close();
  work W(*C);
  W.exec(sql);
  W.commit();
}

void addColor(string file, connection *C) {
  string color_id, name, current;
  ifstream s;
  s.open(file.c_str(), ifstream::in);
  while(getline(s, current)) {
    stringstream ss;
    ss << current;
    ss >> color_id >> name;
    add_color(C, name);
  }
  s.close();
}

void addState(string file, connection *C) {
  string state_id, name, current;
  ifstream s;
  s.open(file.c_str(), ifstream::in);
  while(getline(s, current)) {
    stringstream ss;
    ss << current;
    ss >> state_id >> name;
    add_state(C, name);
  }
  s.close();
}

void addTeam(string file, connection *C) {
  int team_id, state_id, color_id, wins, losses;
  string name, current;
  ifstream s;
  s.open(file.c_str(), ifstream::in);
  while(getline(s, current)) {
    stringstream ss;
    ss << current;
    ss >> team_id >> name >> state_id >> color_id >> wins >> losses;
    add_team(C, name, state_id, color_id, wins, losses);
  }
  s.close();
}

void addPlayer(string file, connection *C) {
  int player_id, team_id, uniform_num, mpg, ppg, rpg, apg;
  string first_name, last_name, current;
  double spg, bpg;
  ifstream s;
  s.open(file.c_str(), ifstream::in);
  while(getline(s, current)) {
    stringstream ss;
    ss << current;
    ss >> player_id >> team_id >> uniform_num >> first_name >> last_name >> mpg >> ppg >> rpg >> apg >> spg >> bpg;
    add_player(C, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);  
  }
  s.close();
}

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      //cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }

  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  try {
    createTables("create.txt", C);
    addColor("color.txt", C);
    addState("state.txt", C);
    addTeam("team.txt", C);
    addPlayer("player.txt", C);
    exercise(C);
    //Close database connection
    C->disconnect();
  } catch (const std::exception &e) {
    cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
