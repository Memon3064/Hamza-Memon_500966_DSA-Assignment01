#include "uno.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>
using namespace std;

static map<Card*, pair<string,string>> cardInfo;

class UNOGameImpl : public UNOGame {
private:
    int numPlayers;
    vector<vector<Card*>> hands;
    vector<Card*> deck;
    Card* topCard;
    int currentPlayer;
    int direction;
    bool gameOver;
    int winnerIndex;

    Card* makeCard(const string &color, const string &value) {
        Card* c = new Card();
        cardInfo[c] = {color,value};
        return c;
    }

public:
    UNOGameImpl(int n) : UNOGame(n), numPlayers(n), currentPlayer(0), direction(1), gameOver(false), winnerIndex(-1) {}

    void initialize() override {
        hands.assign(numPlayers, {});
        deck.clear();
        currentPlayer = 0;
        direction = 1;
        gameOver = false;
        winnerIndex = -1;

        vector<string> colors = {"Red","Green","Blue","Yellow"};

        for (auto &c : colors) {
            deck.push_back(makeCard(c,"0"));
            for (int v=1; v<=9; v++) {
                deck.push_back(makeCard(c,to_string(v)));
                deck.push_back(makeCard(c,to_string(v)));
            }
            for (int i=0;i<2;i++) {
                deck.push_back(makeCard(c,"Skip"));
                deck.push_back(makeCard(c,"Reverse"));
                deck.push_back(makeCard(c,"Draw Two"));
            }
        }

        mt19937 rng(1234);
        for (size_t i=deck.size()-1; i>0; i--) {
            uniform_int_distribution<size_t> dist(0,i);
            size_t j = dist(rng);
            swap(deck[i], deck[j]);
        }

        for (int i=0;i<7;i++) {
            for (int p=0;p<numPlayers;p++) {
                hands[p].push_back(deck.back());
                deck.pop_back();
            }
        }

        topCard = deck.back();
        deck.pop_back();
    }

    void playTurn() override {
        if (gameOver) return;

        vector<Card*> &hand = hands[currentPlayer];
        int playIndex=-1;

        for (size_t i=0;i<hand.size();i++) {
            auto &info=cardInfo[hand[i]];
            auto &top=cardInfo[topCard];
            if (info.first==top.first || info.second==top.second) { playIndex=i; break; }
        }

        if (playIndex==-1 && !deck.empty()) {
            Card* drawn=deck.back(); deck.pop_back();
            hand.push_back(drawn);
            auto &info=cardInfo[drawn];
            auto &top=cardInfo[topCard];
            if (info.first==top.first || info.second==top.second) playIndex=hand.size()-1;
        }

        bool skipNext=false; int drawCount=0;
        if (playIndex!=-1) {
            Card* played=hand[playIndex];
            topCard=played;
            hand.erase(hand.begin()+playIndex);

            string v=cardInfo[played].second;
            if (v=="Skip") skipNext=true;
            else if (v=="Reverse") direction*=-1;
            else if (v=="Draw Two") { skipNext=true; drawCount=2; }
        }

        if (hand.empty()) { gameOver=true; winnerIndex=currentPlayer; return; }

        int step=direction;
        if (skipNext) {
            currentPlayer=(currentPlayer+step+numPlayers)%numPlayers;
            if (drawCount>0) {
                for (int i=0;i<drawCount && !deck.empty();i++) {
                    hands[currentPlayer].push_back(deck.back()); deck.pop_back();
                }
            }
        }
        currentPlayer=(currentPlayer+step+numPlayers)%numPlayers;
    }

    bool isGameOver() const override { return gameOver; }
    int getWinner() const override { return winnerIndex; }

    string getState() const override {
        string dirStr=(direction==1?"Clockwise":"Counter-clockwise");
        auto &t=cardInfo.at(topCard);
        string state="Player "+to_string(currentPlayer)+"'s turn, Direction: "+dirStr+
                     ", Top: "+t.first+" "+t.second+", Players cards:";
        for (int p=0;p<numPlayers;p++) {
            state+=" P"+to_string(p)+":"+to_string(hands[p].size())+",";
        }
        if (!state.empty() && state.back()==',') state.pop_back();
        return state;
    }
};

UNOGame::UNOGame(int numPlayers) {}
void UNOGame::initialize() {}
void UNOGame::playTurn() {}
bool UNOGame::isGameOver() const { return false; }
int UNOGame::getWinner() const { return -1; }
string UNOGame::getState() const { return ""; }

int main() {
    UNOGameImpl game(2);
    game.initialize();
    cout<<game.getState()<<endl;
    game.playTurn();
    cout<<game.getState()<<endl;
    game.playTurn();
    cout<<game.getState()<<endl;
    game.playTurn();
    cout<<game.getState()<<endl;
}
