/*
 * node2.cc
 *
 *  Created on: Apr 26, 2021
 *      Author: saidtig
 */


    #include <string.h>
    #include <omnetpp.h>
    #include <stdio.h>
    #include "myID_m.h"
    #include "propose_m.h"
    #include "accept_m.h"
    #include "refuse_m.h"
    #include "noColor_m.h"
    #include <vector>
    #include <algorithm>
    #include <map>


    using namespace omnetpp;



    class Node : public cSimpleModule
    {
    private:
        std::vector<int> neighbors;
        std::vector<int> list_color;
        std::vector<int> to_erase;

        bool start;
        int count;
        int col;
        int colAcc;
        int colRef;
        int s;
        int index;
        int master;
        int higher;
        int nextId;
        std::string connexion_color;
        int countPropose;
        int countAccept;
        int k;
        int t;
        int conflit;

        std::map <int, int> a;
        std::map <int, int> b;
        std::map <int, int> refused;
        std::map <int, int> rej;
     protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual MyID *generateMessage();
        virtual Propose *generateMessage1();
        virtual Accept *generateMessage2();
        virtual Refuse *generateMessage3();
        virtual NoColor *generateMessage4();


    };


    Define_Module(Node);



    void Node::initialize()
    {

        for (int i; i<par("nbr_colors").intValue(); i++)
            list_color.push_back(i+1);

        start = true;
        count = 0;
        s=0;
        nextId = -1;
        higher = 0;
        countPropose = 0;
        countAccept = 0;
        k = 0;
        t = 0;
        conflit = 0;

        MyID *msg = generateMessage();
            for (int i = 0; i < gateSize("gate$o"); i++)
            {
                MyID *copy = msg->dup();
                send(copy, "gate$o", i);

            }
            delete msg;

        }


    void Node::handleMessage(cMessage *msg)
    {
        if (strcmp(msg->getName(),"my id")==0){
                    MyID *id_msg = check_and_cast<MyID *>(msg);
                    count+=1;

                    neighbors.push_back(int(id_msg->getSource()));
                    a[int(id_msg->getSource())] = (id_msg->getArrivalGate())->getIndex();
                    b[(id_msg->getArrivalGate())->getIndex()] = int(id_msg->getSource());
                    rej[id_msg->getSource()] = 0;
                    //EV<< " ajout de "<< int(id_msg->getSource())<<"\n";
                    if(count == gateSize("gate$o")){
                        for(auto& n:neighbors){
                            if(getIndex() < n) {start =false; higher++;}
                        }
                    }

         if(start == true && count == gateSize("gate$o")){
             bubble("START");
             std::sort(list_color.begin(),list_color.end());
             std::sort(neighbors.begin(),neighbors.end());
             k = neighbors.size();
             to_erase = neighbors;
             for(int i=0;i<neighbors.size();i++){
                 if (list_color.size()>i) {
                     EV<<"Le noeud "<<getIndex()<<" propose C"<< list_color[i] <<" au noeud " <<neighbors[i]<<"\n";
                     col = list_color[i];
                     Propose *msg1 = generateMessage1();
                     send(msg1,"gate$o",a[neighbors[i]]);
                 }
                 else{
                     EV<<"Le nombre de couleurs est insuffisants pour colorier le lien ("<<getIndex()<<","<<neighbors[i]<<") \n";
                     NoColor *msg10 = generateMessage4();
                     send(msg10,"gate$o",a[neighbors[i]]);
                     to_erase.erase(std::remove(to_erase.begin(),to_erase.end(),neighbors[i]),to_erase.end());
                     countAccept++;
                 }

             }
             neighbors = to_erase;
    }
        }
        else if(strcmp(msg->getName(),"proposition")==0){
            Propose *propose_msg = check_and_cast<Propose *>(msg);
            if(std::find(list_color.begin(), list_color.end(), propose_msg->getColor()) != list_color.end()) {
                countPropose++;
                EV<<"Le noeud " << getIndex() << " accepte C" << propose_msg->getColor()<<" avec le noeud " << propose_msg->getSource()<<"\n";
                master = propose_msg->getSource();
                //if (propose_msg->getColor()>t) t = propose_msg->getColor();
                //WATCH(t);

                if (propose_msg->getColor() == 1) connexion_color = "ls=blue,3";
                else if (propose_msg->getColor() == 2) connexion_color = "ls=cyan,3";
                else if (propose_msg->getColor() == 3)  connexion_color = "ls=green,3";
                else if (propose_msg->getColor() == 4)  connexion_color = "ls=yellow,3";
                else if (propose_msg->getColor() == 5)  connexion_color = "ls=red,3";
                else if (propose_msg->getColor() == 6) connexion_color = "ls=grey,3";
                else if (propose_msg->getColor() == 7) connexion_color = "ls=gold,3";
                else if (propose_msg->getColor() == 8) connexion_color = "ls=orange,3";
                else if (propose_msg->getColor() == 9) connexion_color = "ls=pink,3";
                else if (propose_msg->getColor() == 10) connexion_color = "ls=brown,3";
                else if (propose_msg->getColor() == 11) connexion_color = "ls=violet,3";
                else if (propose_msg->getColor() == 12) connexion_color = "ls=black,3";
                else  connexion_color = "ls=black,3";

                char p[connexion_color.length()+1];
                for (int i = 0; i < sizeof(p); i++) p[i] = connexion_color[i];
                cDisplayString& connDispStr = gate("gate$o",propose_msg->getArrivalGate()->getIndex())->getDisplayString();
                connDispStr.parse(p);
                colAcc = propose_msg->getColor();
                list_color.erase(std::remove(list_color.begin(),list_color.end(),propose_msg->getColor()),list_color.end());
                neighbors.erase(std::remove(neighbors.begin(),neighbors.end(),propose_msg->getSource()),neighbors.end());
                Accept *msg2 = generateMessage2();
                send(msg2,"gate$o",propose_msg->getArrivalGate()->getIndex());

                if(countPropose == higher){
                    EV<< "neighbors: ";
                    for(auto& n:neighbors){
                        EV<<n<<", ";
                        if(getIndex()> n) s++;
                    }
                    EV<<"\n";
                    if(s== neighbors.size()){

                        std::sort(list_color.begin(),list_color.end());
                        std::sort(neighbors.begin(),neighbors.end());
                        k = neighbors.size();
                        to_erase = neighbors;
                        for(int i=0;i<neighbors.size();i++){
                            if (list_color.size()>i){
                            EV<<"Le noeud "<<getIndex()<<" propose C"<< list_color[i] <<" au noeud " <<neighbors[i]<<"\n";
                            col = list_color[i];
                            Propose *msg1 = generateMessage1();
                            send(msg1,"gate$o",a[neighbors[i]]);
                            }
                            else{
                                EV<<"Le nombre de couleurs est insuffisants pour colorier le lien ("<<getIndex()<<","<<neighbors[i]<<") \n";
                                NoColor *msg10 = generateMessage4();
                                send(msg10,"gate$o",a[neighbors[i]]);
                                to_erase.erase(std::remove(to_erase.begin(),to_erase.end(),neighbors[i]),to_erase.end());
                                countAccept++;
                            }
                            s = 0;
                        }
                        neighbors = to_erase;
                    }
                }

            }
            else {
                EV<<"Le noeud " << getIndex() << " refuse C" << propose_msg->getColor()<<" avec le noeud " << propose_msg->getSource()<<"\n";
                    //rej[propose_msg->getSource()]++;
                    colRef = propose_msg->getColor();
                    //if (rej[propose_msg->getSource()]<=par("nbr_colors").intValue()){
                    Refuse *msg3 = generateMessage3();
                    send(msg3,"gate$o",propose_msg->getArrivalGate()->getIndex());
                    //}
                    //else EV<<"Le nombre de couleurs est insuffisant pour colorier le lien \n";
            }
        }
        else if(strcmp(msg->getName(),"accept")==0){
            countAccept++;
            Accept *accept_msg = check_and_cast<Accept *>(msg);
            list_color.erase(std::remove(list_color.begin(),list_color.end(),accept_msg->getColor()),list_color.end());
            neighbors.erase(std::remove(neighbors.begin(),neighbors.end(),accept_msg->getSource()),neighbors.end());
            EV<<"-----Le lien ("<<getIndex()<<","<<accept_msg->getSource()<<") prend la couleur C"<<accept_msg->getColor()<<"-----\n";

            if (accept_msg->getColor() == 1) connexion_color = "ls=blue,3";
            else if (accept_msg->getColor() == 2) connexion_color = "ls=cyan,3";
            else if (accept_msg->getColor() == 3) connexion_color = "ls=green,3";
            else if (accept_msg->getColor() == 4) connexion_color = "ls=yellow,3";
            else if (accept_msg->getColor() == 5) connexion_color = "ls=red,3";
            else if (accept_msg->getColor() == 6) connexion_color = "ls=grey,3";
            else if (accept_msg->getColor() == 7) connexion_color = "ls=gold,3";
            else if (accept_msg->getColor() == 8) connexion_color = "ls=orange,3";
            else if (accept_msg->getColor() == 9) connexion_color = "ls=pink,3";
            else if (accept_msg->getColor() == 10) connexion_color = "ls=brown,3";
            else if (accept_msg->getColor() == 11) connexion_color = "ls=violet,3";
            else if (accept_msg->getColor() == 12) connexion_color = "ls=black,3";
            else connexion_color = "ls=black,3";
            char p[connexion_color.length()+1];
            for (int i = 0; i < sizeof(p); i++) p[i] = connexion_color[i];

            cDisplayString& connDispStr = gate("gate$o",accept_msg->getArrivalGate()->getIndex())->getDisplayString();
            connDispStr.parse(p);
            if (countAccept == k){
                for(auto& m:refused){
                    if(rej[b[m.first]]<list_color.size()){
                        col = list_color[(m.second+1) % (list_color.size())];
                        //col = int(refuse_msg->getColor()+1)%int(list_color.size());
                        EV<<"Le noeud "<<getIndex()<<" RE-propose C"<< col <<" au noeud " <<b[m.first]<<"\n";
                        Propose *msg1 = generateMessage1();
                        send(msg1,"gate$o",m.first);
                    }
                    else{
                        EV<<"Le nombre de couleurs est insuffisants pour colorier le lien ("<<getIndex()<<","<<b[m.first]<<") \n";
                        NoColor *msg10 = generateMessage4();
                        send(msg10,"gate$o",b[m.first]);
                        neighbors.erase(std::remove(neighbors.begin(),neighbors.end(),b[m.first]),neighbors.end());
                        countAccept++;
                    }

                }
                countAccept = 0;
                k = neighbors.size();
                refused.clear();
            }
        }
        else if(strcmp(msg->getName(),"refuse") == 0){
            countAccept++;
            Refuse *refuse_msg = check_and_cast<Refuse *>(msg);
            rej[refuse_msg->getSource()]++;
            for (int i=0 ; i < int(list_color.size()) ; i++)
                if (int(refuse_msg->getColor()) == list_color[i]) index = i;
            refused[(refuse_msg->getArrivalGate())->getIndex()] = index;
            if (countAccept == k){
                for(auto& m:refused){
                    if(rej[b[m.first]]<list_color.size()){
                        col = list_color[(m.second+1) % (list_color.size())];
                        //col = int(refuse_msg->getColor()+1)%int(list_color.size());
                        EV<<"Le noeud "<<getIndex()<<" RE-propose C"<< col <<" au noeud " <<b[m.first]<<"\n";
                        Propose *msg1 = generateMessage1();
                        send(msg1,"gate$o",m.first);
                }
                    else{
                        EV<<"Le nombre de couleurs est insuffisants pour colorier le lien ("<<getIndex()<<","<<b[m.first]<<") \n";
                        NoColor *msg10 = generateMessage4();
                        send(msg10,"gate$o",m.first);
                        neighbors.erase(std::remove(neighbors.begin(),neighbors.end(),b[m.first]),neighbors.end());
                        countAccept++;
                    }
                }
            countAccept = 0;
            k = neighbors.size();
            refused.clear();
            }
        }


        else if(strcmp(msg->getName(),"no color") == 0){
            NoColor *nocolor_msg = check_and_cast<NoColor *>(msg);
            countPropose++;
            neighbors.erase(std::remove(neighbors.begin(),neighbors.end(),nocolor_msg->getSource()),neighbors.end());
            if(countPropose == higher){

                EV<< "neighbors: ";
                for(auto& n:neighbors){
                    EV<<n<<", ";
                    if(getIndex()> n) s++;
                }
                EV<<"\n";
                if(s== neighbors.size()){
                    std::sort(list_color.begin(),list_color.end());
                    std::sort(neighbors.begin(),neighbors.end());
                    k = neighbors.size();
                    to_erase = neighbors;
                    for(int i=0;i<neighbors.size();i++){
                        if (list_color.size()>i){
                        EV<<"Le noeud "<<getIndex()<<" propose C"<< list_color[i] <<" au noeud " <<neighbors[i]<<"\n";
                        col = list_color[i];
                        Propose *msg1 = generateMessage1();
                        send(msg1,"gate$o",a[neighbors[i]]);
                        }
                        else {
                            EV<<"Le nombre de couleurs est insuffisants pour colorier le lien ("<<getIndex()<<","<<neighbors[i]<<") \n";
                            NoColor *msg10 = generateMessage4();
                            send(msg10,"gate$o",a[neighbors[i]]);
                            to_erase.erase(std::remove(to_erase.begin(),to_erase.end(),neighbors[i]),to_erase.end());
                            countAccept++;
                        }

                        s = 0;
                    }
                    neighbors = to_erase;
                }
            }
        }

    }
    MyID *Node::generateMessage()
    {
            int source = getIndex();
            MyID *msg = new MyID("my id");
            msg->setDegre(gateSize("gate$o"));
            msg->setSource(source);
            return msg;
    }

    Propose *Node::generateMessage1()
        {
                int source = getIndex();
                Propose *msg = new Propose("proposition");
                msg->setSource(source);
                msg->setColor(col);
                return msg;
        }
    Accept *Node::generateMessage2()
        {
                int source = getIndex();
                Accept *msg = new Accept("accept");
                msg->setSource(source);
                msg->setColor(colAcc);
                return msg;
        }
    Refuse *Node::generateMessage3()
        {
                int source = getIndex();
                Refuse *msg = new Refuse("refuse");
                msg->setSource(source);
                msg->setColor(colRef);
                return msg;
        }
    NoColor *Node::generateMessage4()
        {
                int source = getIndex();
                NoColor *msg = new NoColor("no color");
                msg->setSource(source);
                return msg;
        }
