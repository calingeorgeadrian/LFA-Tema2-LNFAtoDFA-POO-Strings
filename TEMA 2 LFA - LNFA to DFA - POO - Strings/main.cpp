#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

ifstream fin("date2.in");
ofstream g("dfa.out");

struct Nod{
    int state,new_state;
    char letter[101];
    Nod *next;
};

struct DFA
{
   vector<vector<int> > states;
   vector<vector<int> > dfa1;
   vector<vector<int> > dfa2;
   vector<vector<int> > final_states;
   vector<char*> alphabet;
};

class LNFA{
    int states_nr;
    int letters_nr;
    int trans_nr;
    int final_states_nr;
    int init_state;
    int states[101];
    int final_states[101];
    char alphabet[101][101];
    char ch;
    Nod *v[101];
public:
///setter
    void set_states_nr(int x){states_nr=x;}
    void set_letters_nr(int x){letters_nr=x;}
    void set_init_state(int x){init_state=x;}
    void set_final_states_nr(int x){final_states_nr=x;}
    void set_trans_nr(int x){trans_nr=x;}
    void set_transitions(){for(int i=0;i<50;i++) v[i]=NULL;}
///getter
    int get_states_nr(){return states_nr;}
    int get_letter_nr(){return letters_nr;}
    int get_final_states_nr(){return final_states_nr;}
    int get_trans_nr(){return trans_nr;}
    int get_init_state(){return init_state;}
    int get_states(int i){return states[i];}
    char* get_alphabet(int i){return alphabet[i];}
    int get_final_states(int i){return final_states[i];}
///metode
    int convertToInt(char a){
        if(a>='a' && a<='z')
            return a-'a';
        else if(a>='A' && a<='Z')
            return a-'A';
        else return a-'0';
    }

    void readStates(){
        char a[100];
        for(int i=0;i<states_nr;i++)
        {
            fin>>a;
            states[i]=atoi(a);
        }
    }
    void readAlphabet(){ ///citim alfabetul
        char a;
        for(int i=0;i<letters_nr;i++)
        {
            fin>>alphabet[i];
        }
    }

    void readFinalStates(){ ///citim statesle finale si le convertim la int
        char a[100];
        for(int i=0;i<final_states_nr;i++)
        {
            fin>>a;
            final_states[i]=atoi(a);
        }
    }

    void readTransitions(){ ///citim tranzitiile si le salvam intr--un vector de liste pentru fiecare state
        char a[101],b[101],c[101];
        int x,y;
        for(int i=0;i<trans_nr;i++)
        {
            fin>>a>>b>>c; ///citim elementele tranzitiei (state1, state2, letter)
            x=atoi(a);
            y=atoi(b);
            Nod *p; ///initializam un nod p in care salvam tranzitia curenta
            p=new Nod;
            p->state=x;
            p->new_state=y;
            strcpy(p->letter,c);
            p->next=NULL;
            if(v[x]==NULL) ///daca nu exista tranzitii pentru statea x, initializam prima tranzitie
                v[x]=p;
            else ///altfel o adaugam in lista de tranzitii pentru statea x
            {
                Nod *q=v[x];
                while(q->next!=NULL)
                    q=q->next;
                q->next=p;
            }
        }
    }

    bool checkFinalState(int state)
    {
        //cout<<"FINAL "<<state<<'\n';
        for(int i=0;i<final_states_nr;i++)
            if(state==final_states[i]) return 1;
        //cout<<"NU"<<'\n';
        return 0;
    }

    void lambda(vector <int> &p, int k, Nod *v[101])
    {
        Nod *q=v[k];
        while(q!=NULL && strcmp(q->letter,"~")==0)
        {
            //cout<<q->new_state<<" ";
            p.push_back(q->new_state);
            lambda(p,q->new_state,v);
            q=q->next;
        }
    }

    bool checkQueue( vector<vector<int> > dfa, vector<int> p)
    {
       // cout<<"COADA: "<<dfa.size()<<'\n';
        for(int i=0;i<dfa.size();i++)
            {
                vector<int> vect = dfa[i];
                int ok=0;
    /*
                for(int j=0;j<vect.size();j++)
                       cout<<(char)(vect[j]+'a');
                cout<<" ";
    */
                if(vect.size()==p.size())
                    {
                        for(int j=0;j<vect.size();j++)
                            if(vect[j]!=p[j]) ok=1;
                        if(ok==0) return 1;
                    }
            }
       // cout<<'\n';
        return 0;
    }

    void printDFADigits(DFA x)
    {
        int i,j;

        /// afis states
        g<<x.states.size()<<" ";
        for(i=0;i<x.states.size();i++)
        {
            for(j=0;j<x.states[i].size();j++)
                g<<(x.states[i][j]);
            g<<" ";
        }
        g<<'\n';

        ///afis alphabet
        g<<letters_nr<<" ";
        for(i=0;i<letters_nr;i++) g<<alphabet[i]<<" ";
        g<<'\n';

        ///st initiala
        g<<(x.dfa1[0][0])<<'\n';

        ///st finale
        g<<x.final_states.size()<<" ";
        for(i=0;i<x.final_states.size();i++)
        {
            for(j=0;j<x.final_states[i].size();j++)
                g<<(x.final_states[i][j]);
            g<<" ";
        }
        g<<'\n';

        /// afis tranzitii
        g<<x.dfa1.size()-1<<'\n';
        for(i=1;i<x.dfa1.size();i++)
        {
           vector<int> vect = x.dfa1[i];

            for(j=0;j<vect.size();j++)
                    g<<(vect[j]);

            g<<" ";

            vect = x.dfa2[i];

            for(j=0;j<vect.size();j++)
                    g<<(vect[j]);

            g<<" ";
            g<<x.alphabet[i];

            g<<'\n';
        }
    }

    void printDFALetters(DFA x)
    {
        int i,j;

        /// afis states
        g<<x.states.size()<<" ";
        for(i=0;i<x.states.size();i++)
        {
            for(j=0;j<x.states[i].size();j++)
                g<<char(x.states[i][j]+'a');
            g<<" ";
        }
        g<<'\n';

        ///afis alphabet
        g<<letters_nr<<" ";
        for(i=0;i<letters_nr;i++) g<<alphabet[i]<<" ";
        g<<'\n';

        ///st initiala
        g<<char(x.dfa1[0][0]+'a')<<'\n';

        ///st finale
        g<<x.final_states.size()<<" ";
        for(i=0;i<x.final_states.size();i++)
        {
            for(j=0;j<x.final_states[i].size();j++)
                g<<char(x.final_states[i][j]+'a');
            g<<" ";
        }
        g<<'\n';

        /// afis tranzitii
        g<<x.dfa1.size()-1<<'\n';
        for(i=1;i<x.dfa1.size();i++)
        {
           vector<int> vect = x.dfa1[i];

            for(j=0;j<vect.size();j++)
                    g<<char(vect[j]+'a');

            g<<" ";

            vect = x.dfa2[i];

            for(j=0;j<vect.size();j++)
                    g<<char(vect[j]+'a');

            g<<" ";
            g<<x.alphabet[i];

            g<<'\n';

        }
    }

    void LNFAtoDFA(char ch)
    {
        queue<vector<int> > coada;
        DFA x;
        vector<int> p,state,p2;
        queue <int> c;

        ///Lambda[y]
        int y=init_state;
        while(strcmp(v[y]->letter,"~")!=0)
        {
            y++;
            //cout<<y;
        }
        p.clear();
        p.push_back(y);
        c.push(y);
        while(!c.empty())
        {
            int k = c.front();
            //cout<<k<<'\n';
            Nod *q = v[k];
            if(q!=NULL && strcmp(q->letter,"~")==0)
            {
                while(q!=NULL &&  strcmp(q->letter,"~")==0)
                {
                    c.push(q->new_state);
                    p.push_back(q->new_state);
                    q=q->next;
                }
            }
            c.pop();
        }

        p2.clear();
        sort(p.begin(),p.end());
        copy(p.begin(),unique(p.begin(),p.end()),back_inserter(p2));

        /*for(int i =0;i<p.size();i++)
        cout<<p2[i];
        cout<<'\n';*/

        bool stateF = 0;
        for(int i=0;i<p2.size();i++)
            if(checkFinalState(p2[i]))
            stateF=1;

        if(stateF)
            x.final_states.push_back(p2);

        coada.push(p2);
        x.dfa2.push_back(p2);
        x.dfa1.push_back(p2);
        x.alphabet.push_back("0");
        x.states.push_back(p2);

        p.clear();
        p2.clear();
        /*
        if(checkFinalState(final_states_nr,final_states,init_state))
            x.final_states.push_back(p2);
       */

        while(!coada.empty())
        {
            state = coada.front();
            /*cout<<"Stare: ";
            for(int k=0;k<state.size();k++)
                cout<<(char)(state[k]+'a');
            cout<<'\n';*/


            for(int j=0;j<letters_nr;j++)
               {
                   //cout<<"Litera: "<<alphabet[j]<<" "<<state.size()<<'\n';
                    bool stateF = 0;
                    for(int i=0;i<state.size();i++)
                    {
                        int currentState = state[i];
                        //cout<<"StareActuala : "<<(currentState)<<'\n';

                        Nod *q=v[currentState];
                        if(q!=NULL)
                        {
                            while(q!=NULL && (strcmp(q->letter,alphabet[j])<0 || strcmp(q->letter,"~")==0))
                                q=q->next;

                            // if(q) cout<<"A: "<<q->state<<" "<<q->new_state<<" "<<q->letter<<'\n';;
                            if(q!=NULL && strcmp(q->letter,alphabet[j])==0)
                            {
                                //cout<<q->letter;
                                while(q!=NULL && strcmp(q->letter,alphabet[j])==0)
                                {
                                    //cout<<"laMBDA: ";
                                    if(checkFinalState(q->new_state))
                                        stateF=1;
                                    p.push_back(q->new_state);
                                    lambda(p,q->new_state,v);
                                    //cout<<'\n';
                                    q=q->next;
                                }
                            }
                        }
                    }

                    p2.clear();
                    sort(p.begin(),p.end());
                    copy(p.begin(),unique(p.begin(),p.end()),back_inserter(p2));

                    /*cout<<"VECTTOR : ";
                    for(int k=0;k<p2.size();k++)
                        cout<<(p2[k]);
                    cout<<'\n';*/

                    if(!p2.empty())
                    {
                        if(!checkQueue(x.states,p2))
                        {
                            //cout<<"DA";
                            coada.push(p2);
                            x.states.push_back(p2);
                            if(stateF==1) x.final_states.push_back(p2);
                        }
                        x.dfa2.push_back(p2);
                        x.dfa1.push_back(state);
                        x.alphabet.push_back(alphabet[j]);
                    }
                    p.clear();
                    p2.clear();
               }
            coada.pop();
        }

        if(ch>='0' && ch<='9')
            printDFADigits(x);
        else
            printDFALetters(x);
    }
};

int main()
{
    LNFA automat;
    int x;
    char a;
    fin>>x;
    automat.set_states_nr(x);
    automat.readStates(); ///citire states

    /*cout<<automat.get_states_nr();
    for(int i=0;i<automat.get_states_nr();i++)
        cout<<" "<<automat.get_states(i);*/

    fin>>x;
    automat.set_letters_nr(x);
    automat.readAlphabet(); ///citire alfabet

    /*cout<<endl<<automat.get_letter_nr();
    for(int i=0;i<automat.get_letter_nr();i++)
        cout<<" "<<automat.get_alphabet(i);*/

    fin>>a;
    x=automat.convertToInt(a);
    automat.set_init_state(x); ///citire state initiala

    //cout<<endl<<automat.get_init_state();

    fin>>x;
    automat.set_final_states_nr(x);
    automat.readFinalStates(); ///citire states finale

    /*cout<<endl<<automat.get_final_states_nr();
    for(int i=0;i<automat.get_final_states_nr();i++)
    cout<<" "<<automat.get_final_states(i);*/

    fin>>x;
    automat.set_trans_nr(x);
    automat.set_transitions();
    automat.readTransitions(); ///citire tranzitii

    automat.LNFAtoDFA(a);

    return 0;
}
