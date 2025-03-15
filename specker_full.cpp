#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class Move {
public:

    Move(int sh, int sc, int th, int tc) {
        //name = n;
        source_heap = sh;
        source_coins = sc;
        target_heap = th;
        target_coins = tc;
    }

    int getSource() const {
        return source_heap;
    }
    int getSourceCoins() const {
        return source_coins;
    }
    int getTarget() const {
        return target_heap;
    }
    int getTargetCoins() const {
        return target_coins;
    }

    friend ostream & operator << (ostream &out, const Move &move) {
        out << "takes " << move.getSourceCoins() << " coins from heap " << move.getSource() << " and puts ";
        if (move.getTargetCoins() == 0)
            out << "nothing";
        else
            out << move.getTargetCoins() << " coins to heap " << move.getTarget();
        return out;
    }

private:
    int source_heap;
    int source_coins;
    int target_heap;
    int target_coins;
    //string name;

};

class State {
public:
    State(int h, const int c[], int n) {
        max_heaps = h;
        coins = new int[max_heaps];
        for (int i = 0; i < max_heaps; i++) {
            coins[i] = c[i];
        }
        players = n;
        current_player = 0;

    }
    ~State() {
        delete [] coins;
    }

    void next(const Move &move) throw(logic_error) {
        if ((move.getSource() < 0) || (move.getSource() >= max_heaps) || (move.getTarget() < 0) || (move.getTarget() >= max_heaps) || (move.getTargetCoins() >= move.getSourceCoins()) || (move.getSourceCoins() > getCoins(move.getSource()))) {
            throw logic_error("invalid heap");
        }//edw logika oxi giati einai void?
        else {
            //coins[move.getSource()] = move.getSourceCoins();
            coins[move.getSource()] -= move.getSourceCoins();
            coins[move.getTarget()] += move.getTargetCoins();
            if (current_player == players-1) current_player = 0;
            else current_player++;
        }

    }
    bool winning() const {
        bool k = true;
        for(int i = 0; i < max_heaps; i++) {
            if (coins[i] != 0) k = false;
        }
        return k;
    }

    int getHeaps() const {
        return max_heaps;
    }
    int getCoins(int h) const throw(logic_error){
        if ((h < 0) || (h >= max_heaps)) {
            throw logic_error("invalid heap");
        }//mhpws den kanei kati return?
        else {
            return coins[h];
        }
    }

    int getPlayers() const {
        return players;
    }
    int getPlaying() const {
        return current_player;
    }

    friend ostream & operator << (ostream &out, const State &state) {
        for(int i = 0; i < state.getHeaps()-1; i++) {
            out << state.getCoins(i) << ", ";
        }

        out << state.getCoins(state.getHeaps() - 1) << " with " << state.getPlaying() << "/" << state.getPlayers() << " playing next";
        return out;
    }
private:
    int max_heaps;
    int *coins;
    int players;
    int current_player;
};

//#include <iostream>
//#include <string>
//#include <stdexcept>
//using namespace std;

class Player {
public:
    Player(const string &n){
        name = n;
    }

    virtual ~Player(){
        name.clear();
    }

    virtual const string &getType() const = 0;
    virtual Move play(const State &s) = 0;

    friend ostream &operator<<(ostream &out, const Player &player) {
        out << player.getType() << " player " << player.name;
        return out;
    }

protected:
    string name;
};

class GreedyPlayer : public Player {
public:
    GreedyPlayer(const string &n) : Player(n) {
        type = "Greedy";
    }
    virtual const string &getType() const override {
        return type;
    }

    virtual ~GreedyPlayer() override{ //8elei override???
        type.clear();
    }
    virtual Move play(const State &s) override {
        int source_heap = 0;
        int source_coins = 0;
        int target_heap = 0;
        int target_coins = 0;
        for(int i = 0; i < s.getHeaps(); i++) {
            if (source_coins < s.getCoins(i)) {
                source_coins = s.getCoins(i);
                source_heap = i;
            }
        }
        Move greedy(source_heap, source_coins, target_heap, target_coins);
        return greedy;
    }

private:
    string type;
};


class SpartanPlayer : public Player {
public:
    SpartanPlayer(const string &n) : Player(n) {
        type = "Spartan";
    }

    virtual const string &getType() const override {
        return type;
    }

    virtual ~SpartanPlayer() override {
        type.clear();
    }

    virtual Move play(const State &s) override {
        int source_heap = 0;
        int source_coins = 0;
        int target_heap = 0;
        int target_coins = 0;
        for(int i = 0; i < s.getHeaps(); i++) {
            if (source_coins < s.getCoins(i)) {
                source_heap = i;
            }
        }
        Move spartan(source_heap, 1, target_heap, target_coins);
        return spartan;
    }
private:
    string type;
};

class SneakyPlayer : public Player {
public:
    SneakyPlayer(const string &n) : Player(n) {
        type = "Sneaky";
    }

    virtual const string &getType() const override {
        return type;
    }

    virtual ~SneakyPlayer() override {
        type.clear();
    }

    virtual Move play(const State &s) override {
        int source_heap = 0;
        int source_coins = s.getCoins(0);
        int target_heap = 0;
        int target_coins = 0;
        for(int i = 0; i < s.getHeaps(); i++) {
            if (source_coins > s.getCoins(i)) {
                source_heap = i;
                source_coins = s.getCoins(i);
            }
        }
        Move sneaky(source_heap, source_coins, target_heap, target_coins);
        return sneaky;
    }

private:
    string type;
};

class RighteousPlayer : public Player {
public:
    RighteousPlayer(const string &n) : Player(n) {
        type = "Righteous";
    }

    virtual const string &getType() const override {
        return type;
    }

    virtual ~SneakyPlayer() override {
        type.clear();
    }

    virtual Move play(const State &s) override {
        int source_heap = 0;
        int source_coins = 0;
        int target_heap = 0;
        int target_coins = 0;
        int max_coins = 0;
        int min_coins = s.getCoins(0);
        for(int i = 0; i < s.getHeaps(); i++) {
            if (max_coins < s.getCoins(i)) {
                max_coins = s.getCoins(i);
                source_coins = max_coins / 2;
                source_heap = i;
                target_coins = source_coins - 1;
            }
        }
        for(int j = 0; j < s.getHeaps(); j++) {
            if (min_coins > s.getCoins(j)) {
                target_heap = j;
            }
        }
        Move righteous(source_heap, source_coins, target_heap, target_coins);
        return righteous;
    }
private:
    string type;
};

class Game {
public:
    Game(int h, int p) {
        num_of_heaps = h;
        num_of_players = p;
        heaps = new int[num_of_heaps];
        players = new Player *[num_of_players];
        current_heap = 0;
        current_player = 0;
    }
    ~Game() {
        delete [] players;
        delete [] heaps;
    }

    void addHeap(int coins) throw(logic_error) {
        if (coins < 0 || current_heap >= num_of_heaps) throw logic_error("");
        else {
            heaps[current_heap] = coins;
            current_heap += 1;
        }
    }
    void addPlayer(Player *player) throw(logic_error) {
        if (current_player >= num_of_players){
            throw logic_error("");
        }
        else {
            players[current_player] = player; ////////////////////////////////
        }
    }

    void play(ostream &out) throw(logic_error) {
        //State state(num_of_heaps, heaps, num_of_players); // mporei na mpainei katw sto else
        if (current_heap != num_of_heaps || current_player != num_of_players) {
            throw logic_error("");
        }
        else {
            State state(num_of_heaps, heaps, num_of_players);
            current_player = 0;
            while (state.winning() == false) {
                //current_player = 0;
                out << "State: " << state;
                /*for (int i = 0; i < num_of_heaps - 1; i++) {
                    out << heaps[i] << ", ";
                }
                out << heaps[num_of_heaps - 1] << " with " << current_player << "/" << num_of_players << " playing next";*/
                out << " with " << current_player << "/" << num_of_players << " playing next";
                players[current_player]->play(state);
                //out << players[current_player]->getType() << " player " << players[current_player]->name
                //&operator<<(cout, players[current_player]);
                out << *players[current_player] << " " << players[current_player]->play(state);
                current_player++;
            }
            out << "State: " << state << " with " << *players[current_player] << " playing next";
            current_player--;
            out << *players[current_player] << " wins";
        }
    }

    int getPlayers() const {
        return num_of_players;
    }
    const Player *getPlayer(int p) const throw(logic_error) {
        if (p < 0 || p >= num_of_players) {
            throw logic_error("");
        }
        else {
            return players[p];//////////////
        }
    }
private:
    int num_of_heaps;
    int num_of_players;
    int *heaps;
    Player **players;
    int current_heap;
    int current_player;
};

int main() {
    Game specker(3, 4);
    specker.addHeap(10);
    specker.addHeap(20);
    specker.addHeap(17);
    specker.addPlayer(new SneakyPlayer("Tom"));
    specker.addPlayer(new SpartanPlayer("Mary"));
    specker.addPlayer(new GreedyPlayer("Alan"));
    specker.addPlayer(new RighteousPlayer("Robin"));
    specker.play(cout);
}