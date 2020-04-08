#include "query_funcs.h"
#include <iostream>
#include <sstream>
#include <iomanip>

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg) {
    work W(*C);
    stringstream ss;
    ss << "INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG)"
        << " VALUES (" << team_id << ", " << jersey_num << ", " << W.quote(first_name) << ", " 
        << W.quote(last_name) << ", " << mpg << ", "<< ppg << ", " << rpg << ", " << apg << ", "
        << spg << ", " << bpg << ");";
    W.exec(ss.str());
    W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses) {
    work W(*C);
    stringstream ss;
    ss << "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES (" << W.quote(name) 
        << ", " << state_id << ", " << color_id << ", " << wins << ", " << losses << ");";
    W.exec(ss.str());
    W.commit();
}


void add_state(connection *C, string name) {
    work W(*C);
    stringstream ss;
    ss << "INSERT INTO STATE (NAME) VALUES (" << W.quote(name) << ");";
    W.exec(ss.str());
    W.commit();
}


void add_color(connection *C, string name) {
    work W(*C);
    stringstream ss;
    ss << "INSERT INTO COLOR (NAME) VALUES (" << W.quote(name) << ");";
    W.exec(ss.str());
    W.commit();
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            ) {
    stringstream sql;
    sql << "SELECT * FROM PLAYER";
    nontransaction N(*C);
    if (use_mpg || use_ppg || use_rpg || use_apg || use_spg || use_bpg) {
        sql << " WHERE ";
        if (use_mpg) {
            sql << " MPG >= " << min_mpg << " AND " << "MPG <= " << max_mpg << " AND ";
        }
        if (use_ppg) {
            sql << " PPG >= " << min_ppg << " AND " << "PPG <= " << max_ppg << " AND ";
        }
        if (use_rpg) {
            sql << " RPG >= " << min_rpg << " AND " << "RPG <= " << max_rpg << " AND ";
        }
        if (use_apg) {
            sql << " APG >= " << min_apg << " AND " << "APG <= " << max_apg << " AND ";
        }
        if (use_spg) {
            sql << " SPG >= " << min_spg << " AND " << "SPG <= " << max_spg << " AND ";
        }
        if (use_bpg) {
            sql << " BPG >= " << min_bpg << " AND " << "BPG <= " << max_bpg << " AND ";
        }
        sql << "TRUE;";
    }
    result R(N.exec(sql));
    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<int>() << " " << c[1].as<int>() << " " 
             << c[2].as<int>() << " " << c[3].as<string>() << " "
             << c[4].as<string>() << " " << c[5].as<int>() << " "
             << c[6].as<int>() << " " << c[7].as<int>() << " "
             << c[8].as<int>() << " " << fixed << setprecision(1) 
             << c[9].as<double>() << " " << c[10].as<double>() << endl;
    }

}


void query2(connection *C, string team_color) {
    nontransaction N(*C);
    string sql = "SELECT TEAM.NAME "
                 "FROM TEAM, COLOR "
                 "WHERE TEAM.COLOR_ID = COLOR.COLOR_ID AND COLOR.NAME = " + N.quote(team_color) + ";";
    result R(N.exec(sql));
    cout << "NAME" << endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<string>() << endl;
    }
}


void query3(connection *C, string team_name) {
    nontransaction N(*C);
    string sql = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME "
                 "FROM PLAYER, TEAM "
                 "WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.NAME = " + N.quote(team_name) + 
                 " ORDER BY PPG DESC;";
    result R(N.exec(sql));
    cout << "FIRST_NAME LAST_NAME" << endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<string>() << " " << c[1].as<string>() << endl;
    }
}


void query4(connection *C, string team_state, string team_color) {
    nontransaction N(*C);
    string sql = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, PLAYER.UNIFORM_NUM "
                 "FROM PLAYER, TEAM, STATE, COLOR "
                 "WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.STATE_ID = STATE.STATE_ID "
                 "AND TEAM.COLOR_ID = COLOR.COLOR_ID AND STATE.NAME = " + N.quote(team_state) + 
                 " AND COLOR.NAME = " + N.quote(team_color) + ";";
    result R(N.exec(sql));
    cout << "FIRST_NAME LAST_NAME UNIFORM_NUM" << endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<string>() << " " << c[1].as<string>() << " " << c[2].as<int>() << endl;
    }
}


void query5(connection *C, int num_wins) {
    nontransaction N(*C);
    string sql = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS "
                 "FROM PLAYER, TEAM "
                 "WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.WINS > " + to_string(num_wins) + ";";
    result R(N.exec(sql));
    cout << "FIRST_NAME LAST_NAME NAME WINS" << endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<string>() << " " << c[1].as<string>() << " " << c[2].as<string>() 
             << " " << c[3].as<int>() << endl;
    }
}
