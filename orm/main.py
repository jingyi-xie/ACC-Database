import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "orm.settings")
import django
django.setup()

from acc.models import *

def createTable():
    #create color table
    f = open('data/color.txt')
    for cur in f:
       color_id, name= cur.split(' ')
       add_color(name.rstrip())
    f.close()
    
    #create state table
    f = open('data/state.txt')
    for cur in f:
       state_id, name= cur.split(' ')
       add_state(name.rstrip())
    f.close()

    #create team table
    f = open('data/team.txt')
    for cur in f:
       team_id, name, state_id, color_id, wins, losses = cur.split(' ')
       add_team(name, state_id, color_id, wins, losses)
    f.close()

    #create player table
    f = open('data/player.txt')
    for cur in f:
       player_id, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg = cur.split(' ')
       add_player(team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg)
    f.close()
    return
    
def add_player(team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg):
    Player.objects.get_or_create(team = Team.objects.get(team_id = team_id), uniform_num = uniform_num, first_name = first_name, last_name = last_name, mpg = mpg, ppg = ppg, rpg = rpg, apg = apg, spg = spg, bpg = bpg)
    return

def add_team(name, state_id, color_id, wins, losses):
    Team.objects.get_or_create(name = name, state = State.objects.get(state_id = state_id), color = Color.objects.get(color_id = color_id), wins = wins, losses = losses)
    return

def add_state(name):
    State.objects.get_or_create(name = name)
    return

def add_color(name):
    Color.objects.get_or_create(name = name)
    return

def query1(use_mpg, min_mpg, max_mpg,
	    use_ppg, min_ppg, max_ppg,
	    use_rpg, min_rpg, max_rpg,
	    use_apg, min_apg, max_apg,
	    use_spg, min_spg, max_spg,
	    use_bpg, min_bpg, max_bpg):
    results = Player.objects.all()
    if use_mpg:
        results = results.filter(mpg__gte = min_mpg, mpg__lte = max_mpg)
    if use_ppg:
        results = results.filter(ppg__gte = min_ppg, ppg__lte = max_ppg)
    if use_rpg:
        results = results.filter(rpg__gte = min_rpg, rpg__lte = max_rpg)
    if use_apg:
        results = results.filter(apg__gte = min_apg, apg__lte = max_apg)
    if use_spg:
        results = results.filter(spg__gte = min_spg, spg__lte = max_spg)
    if use_bpg:
        results = results.filter(bpg__gte = min_bpg, bpg__lte = max_bpg)
    # print("==============query1=============")
    print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
    for player in results:
        print(player.player_id, player.team.team_id, player.uniform_num, player.first_name, 
            player.last_name, player.mpg, player.ppg, player.rpg, player.apg, player.spg, player.bpg)
    return

def query2(team_color):
    results = Team.objects.filter(color__name = team_color)
    # print("==============query2=============")
    print("NAME")
    for team in results:
        print(team.name)
    return

def query3(team_name):
    results = Player.objects.filter(team__name = team_name).order_by('-ppg')
    # print("==============query3=============")
    print("FIRST_NAME LAST_NAME")
    for player in results:
        print(player.first_name, player.last_name)
    return

def query4(team_state, team_color):
    results = Player.objects.filter(team__state__name = team_state, team__color__name = team_color)
    # print("==============query4=============")
    print("FIRST_NAME LAST_NAME UNIFORM_NUM")
    for player in results:
        print(player.first_name, player.last_name, player.uniform_num)
    return

def query5(num_wins):
    results = Player.objects.filter(team__wins__gt = num_wins)
    # print("==============quer5=============")
    print("FIRST_NAME LAST_NAME NAME WINS")
    for player in results:
        print(player.first_name, player.last_name, player.team.name, player.team.wins)
    return


if __name__ == "__main__":
    createTable()
    query1(1, 10, 30, 0, 0, 0, 1, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    query1(0, 0, 0, 1, 10, 20, 0, 0, 0, 1, 30, 35, 0, 0, 0, 0, 0, 0)
    query2("Gold")
    query2("Black")
    query3("Pittsburgh")
    query3("WakeForest")
    query4("NC", "LightBlue")
    query4("NC", "DarkBlue")
    query5(5)
    query5(15)

