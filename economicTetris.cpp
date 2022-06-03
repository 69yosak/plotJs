#include <iostream>
#include <stdint.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <fstream>
using namespace std;

#define COUNT_PLAYERS 10000
#define MAX_COUNT_COIN 1000000000
#define STEP_BOOST 0.25
#define MAX_SMOOSI 100
#define COST_ONE_TRY 5
#define TRYIES_PER_DAY 10
#define MAX_COUNT_COIN_PER_TRY 500
#define COST_OF_ONE_SMOOSI 10
#define MAX_SMOOSI_CAN_BE_BOUGHT 30
#define COST_BOOST 1.2
#define BASIC_COST_BOOST 10
#define SROK_GODNOSTI 400
#define COUNT_TRYIES 100
#define COUNT_BYE_TRIES 5
#define PRICE_TRYIES 4000
#define STEP_PRICE_TRIES 1000
#define PRICE_CHARACTER 150




double Random()
{
    return 1.*rand()/RAND_MAX;
}
double mainBalance=MAX_COUNT_COIN;
double mainBank=0;
double mainCash=0;
struct player{
    uint16_t id;
    double cash=-PRICE_CHARACTER;
    double boost=1;
    double boostLevel=1;
    int smoosi=MAX_SMOOSI;
    double balance=0;
    int tries=TRYIES_PER_DAY;
    int limit_tries=SROK_GODNOSTI;
    int price_tries=PRICE_TRYIES;
    int count_bye_tryies=COUNT_BYE_TRIES;
    void tryToGame()
    {
        if(tries>0 && smoosi>=COST_ONE_TRY &&limit_tries>0)
        {
            double price=MAX_COUNT_COIN_PER_TRY*Random()*boost;
            balance+=price;
            mainBalance-=price;
            tries--;
            smoosi-=COST_ONE_TRY;
            limit_tries--;
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
        double summ=(Random()/2+0.5)*balance;
        balance-=summ; //доработать
        mainBank+=summ;
        cash+=summ*0.003;
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
        cash-=summ*0.003;
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
    std::ofstream fileBalance("balance.csv");

    fileBalance<<"mainBalance,mainBank,SKOBalance"<<std::endl;
    std::vector<player>players;
    int byesNewPlayers=COUNT_PLAYERS*PRICE_CHARACTER;
    for(int i=0;i<COUNT_PLAYERS;++i)
    {
        player newPlayer;
        newPlayer.id=i;
        players.push_back(newPlayer);
    }

    for(int day=0;day<=3652;++day)
    {
        double SKOBalance=0;
        double AVGcash=0;
        // std::cout<<"day:"<<day<<std::endl;
        for(size_t i=0;i<players.size();++i)
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
                    continue;
                }
                if(Random()<0.5)
                {
                    players[i].byeCoins();
                }
//                if(Random()<0.005)
//                {
//                    double tmp=Random()*100;
//                    mainBalance-=tmp;

//                    players[i].balance+=tmp;
//                    byesNewPlayers+=tmp*0.004;
//                    players[i].cash-=tmp*0.004
//                }
                if(Random()<0.5)
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
                    if(players[i].limit_tries<1 && players[i].count_bye_tryies>0)
                    {
                        if(Random()<0.5 && players[i].balance>=players[i].price_tries)
                        {
                            players[i].balance-=players[i].price_tries;
                            mainBalance+=players[i].price_tries;
                            players[i].price_tries+=STEP_PRICE_TRIES;
                            players[i].count_bye_tryies--;
                        }
                    }
                    if(players[i].limit_tries<1 && players[i].count_bye_tryies<1)
                    {
                        if(Random()<0.5)
                        {

                            player newPlayer;
                            newPlayer.balance=players[i].balance;
                            newPlayer.cash=players[i].cash-PRICE_CHARACTER;
                            players[i]=newPlayer;
                            byesNewPlayers+=PRICE_CHARACTER;
                            mainCash++;

                        }
                    }
                    break;


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

            if(Random()<0.8)
            {
                players[i].sellCoins();
            }
            SKOBalance+=players[i].balance;
            AVGcash+=players[i].cash;

        }
        double check=mainBalance+mainBank;
        for (int i=0;i<players.size();++i)
        {
            check+=players[i].balance;

        }
        std::cout<<"day: "<<day<<" balance: "<<mainBalance<<" bank: "<<mainBank<<" SKOBalance: "<<SKOBalance/players.size()
        <<" check: "<<check<<" mainCash: "<<byesNewPlayers
        <<" avgCash: "<<AVGcash/players.size()//<<"("<<mainCash<<")"
        <<std::endl;
        fileBalance<<to_string(int(mainBalance))<<","<<to_string(int(mainBank))<<","<<to_string(int(SKOBalance/players.size()))<<std::endl;
        float tmp=mainBank*1;
        mainBalance+=tmp;
        mainBank-=tmp;
        byesNewPlayers-=tmp*0.003;

        if(mainBalance<0)break;
        if (day%30==0)byesNewPlayers-=100000;

    }



    return 0;
}
