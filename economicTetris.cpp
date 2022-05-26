#include <iostream>
#include <stdint.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
using namespace std;

#define COUNT_PLAYERS 10000
#define MAX_COUNT_COIN 1000000000
#define STEP_BOOST 0.05
#define MAX_SMOOSI 100
#define COST_ONE_TRY 5
#define TRYIES_PER_DAY 8
#define MAX_COUNT_COIN_PER_TRY 100
#define COST_OF_ONE_SMOOSI 10
#define MAX_SMOOSI_CAN_BE_BOUGHT 30
#define COST_BOOST 1.005
#define BASIC_COST_BOOST 100



double Random()
{
    return 1.*rand()/RAND_MAX;
}
double mainBalance=MAX_COUNT_COIN;
double mainBank=0;
struct player{
    uint16_t id;
    double boost=1;
    double boostLevel=1;
    int smoosi=MAX_SMOOSI;
    double balance=0;
    int tries=TRYIES_PER_DAY;
    void tryToGame()
    {
        if(tries>0 && smoosi>=COST_ONE_TRY)
        {
            double price=MAX_COUNT_COIN_PER_TRY*Random()*boost;
            balance+=price;
            mainBalance-=price;
            tries--;
            smoosi-=COST_ONE_TRY;
        }
    }
    void buySmoosi()
    {
        if(balance>COST_OF_ONE_SMOOSI*MAX_SMOOSI_CAN_BE_BOUGHT)
        {
            balance-=COST_OF_ONE_SMOOSI*MAX_SMOOSI_CAN_BE_BOUGHT;
            mainBalance+=COST_OF_ONE_SMOOSI*MAX_SMOOSI_CAN_BE_BOUGHT;
            smoosi+=MAX_SMOOSI_CAN_BE_BOUGHT;
        }
        
    }
    void sellCoins()
    {
        double summ=Random()*balance;
        balance-=summ; //доработать
        mainBank+=summ;
    }
    bool byeCoins(double summ)
    {
        if(mainBank<summ)return false;
        balance+=summ; //доработать
        mainBank-=summ;
        return true;
    }
    void byeCoins()
    {
        double summ=Random()*mainBank*0.33;
        balance+=summ; //доработать
        mainBank-=summ;
    }
    void byeBoost()
    {
        double costBoost=(BASIC_COST_BOOST*pow(COST_BOOST,boostLevel));
        if(balance>=costBoost)
        {
            balance-=costBoost;
            boostLevel++;
            boost+=STEP_BOOST;
            mainBalance+=costBoost;
        }
    }
    double getCostBoost()
    {
        return BASIC_COST_BOOST*pow(COST_BOOST,boostLevel);
    }
    void PVP()
    {
        if(smoosi>=COST_ONE_TRY)
        {
            float bet=Random()*balance*0.05;
            balance-=bet;
            mainBalance+=bet;
            smoosi-=COST_ONE_TRY;
        }
        else 
        {
            buySmoosi();
        }
    }
    
};
int main()
{
    std::vector<player>players;
    for(int i=0;i<COUNT_PLAYERS;++i)
    {
        player newPlayer;
        newPlayer.id=i;
        players.push_back(newPlayer);
    }
    
    for(int day=0;day<=3652;++day)
    {
        double SKOBalance=0;
        // std::cout<<"day:"<<day<<std::endl;
        for(int i=0;i<players.size();++i)
        {
            // std::cout<<"player"<<i<<std::endl;
            players[i].tries=10;
            int playersTries=players[i].tries;
            SKOBalance-=players[i].balance;
            while(playersTries>0)
            {
                //std::cout<<"playersTries:"<<playersTries<<std::endl;
                if(Random()<0.2)
                {
                    players[i].sellCoins();
                }
                if(Random()<0.2)
                {
                    players[i].byeCoins();
                }
                if(Random()<0.8)
                {
                    players[i].byeBoost();
                }
                if(Random()<0.5)
                {
                    players[i].buySmoosi();
                }
                players[i].tryToGame();
                if(players[i].tries==playersTries)
                {
                    if(players[i].smoosi<COST_ONE_TRY)
                    {
                        if(players[i].balance>=COST_OF_ONE_SMOOSI*MAX_SMOOSI_CAN_BE_BOUGHT)
                        {
                            players[i].buySmoosi();
                        }
                        else
                        {
                            bool flag=players[i].byeCoins(COST_OF_ONE_SMOOSI*MAX_SMOOSI_CAN_BE_BOUGHT);
                            if(!flag)
                            {
                                --playersTries;
                                continue;
                            }
                            continue;
                        }
                    }
                    
                }
                else
                {
                    --playersTries;
                }
                float pvpMatches=Random()*10;
                for(int i=1;i<pvpMatches;++i)
                {
                    players[i].PVP();
                }
                
                
            }
            SKOBalance+=players[i].balance;
        }
        double check=mainBalance+mainBank;
        for (int i=0;i<players.size();++i)
        {
            check+=players[i].balance;
        
        }
        std::cout<<"day: "<<day<<" balance: "<<mainBalance<<" bank: "<<mainBank<<" SKOBalance: "<<SKOBalance/players.size()<<" check: "<<check<<std::endl;
        if(mainBalance<0)break;
    }
    
    

    return 0;
}
