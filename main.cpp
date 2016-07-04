#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <cstring>

using namespace std;

ifstream f("date.in");
ofstream g("date.out");

char s[1000];
int p;
class Automat
{
    int stare_initiala, n, m;
    vector<int> fin;
    vector<pair< int, char > > *lis;
public:
    Automat();
    void init( char c );
    Automat( const char *s);
    Automat( Automat& A);
    ~Automat();
    void operator =( Automat A );
    Automat& operator +( Automat &A );
    Automat& operator *( Automat &A );
    Automat& operator *();
    Automat& operator ~();
    void afisare();
    Automat& To_Nfa();
    void Hehe( int nod, int nod1, char c, int *&viz, Automat *B, int &nr );
    Automat& To_Dfa();
    void verificare();
    int vf( int stare, int i, char *s, int *viz );
};

int Automat::vf( int stare, int i, char *s, int *viz )
{
    cout << stare << " " << i << endl;
    if( i == strlen(s) )
    {
        bool ok = false;
        for( vector < int > :: iterator it = fin.begin(); it != fin.end(); ++ it )
            if( stare == *it )
                ok = true;
        if( ok == true )
            return 1;
        for( vector < pair < int, char > > :: iterator it = lis[stare].begin(); it != lis[stare].end(); ++it )
            if( it->second == '^' && ( viz[it->first] == -1 || viz[it->first] != i ) )
            {
                int poz = viz[it->first];
                viz[it->first] = i;
                int x = vf( it->first, i, s, viz );
                viz[it->first] = poz;
                if( x == 1 )
                    return 1;
            }
        return 0;
    }
    else
    {
        for( vector < pair < int, char > > :: iterator it = lis[stare].begin(); it != lis[stare].end(); ++ it )
        {
            if( it->second == s[i] )
            {
                int poz = viz[it->first];
                viz[it->first] = i;
                int x = vf( it->first, i+1, s, viz );
                viz[it->first] = poz;
                if( x == 1 )
                    return 1;

            }
            if( it->second == '^' && ( viz[it->first] == -1 || viz[it->first] != i ) )
            {
                int poz = viz[it->first];
                viz[it->first] = i;
                int x = vf( it->first, i, s, viz );
                viz[it->first] = poz;
                if( x == 1 )
                    return 1;
            }
        }
        return 0;
    }
}

void Automat::verificare()
{
    char s[300];
    cin >> s;
    int viz[300];

    for( int i = 0; i < n; ++ i )
        viz[i] = -1;
    viz[stare_initiala] = 0;

    if( vf(stare_initiala,0,s, viz) == 1 )

        cout << "Cuvantul este acceptat";
    else cout << "Cuvantul nu este acceptat";
}

Automat& Automat::To_Dfa()
{
    vector < char > litere;

    for( int i = 0; i < n; ++ i )
    {
        for( vector < pair < int, char > > :: iterator it = lis[i].begin(); it != lis[i].end(); ++ it )
        {
            bool ok = false;
            for( vector < char > :: iterator it2 = litere.begin(); it2 != litere.end(); ++ it2 )
                if( *it2 == it->second )
                    ok = true;
            if( ok == false )
                litere.push_back( it->second );
        }
    }

    Automat *B = new Automat;

    vector < int > vec[200];
    vector < pair < int, char > > liste4[200];
    vector < int > liste5[200];
    int k = 0;
    int contor = 0;
    vec[0].push_back( stare_initiala );

    while( k <= contor )
    {
        for( unsigned i = 0; i < litere.size(); ++ i )
        {
            vector < int > g;

            for( vector < int > :: iterator it = vec[k].begin(); it != vec[k].end(); ++ it )
            {
                int stare = *it;

                for( vector < pair < int, char > > :: iterator itt = lis[stare].begin(); itt != lis[stare].end(); ++ itt )
                {
                    if( itt->second == litere[i] )
                    {
                        bool ok = false;

                        for( vector < int > :: iterator ite = g.begin(); ite != g.end(); ++ ite )
                        {
                            if( *ite == itt->first )
                                ok = true;
                        }

                        if( ok == false )
                            g.push_back( itt->first );
                    }
                }
            }

            sort( g.begin(), g.end() );

            bool ok = false;
            int stare;

            for( int j = 0; j <= contor; ++ j )
            {
                if( g.size() == vec[j].size() )
                {
                    unsigned t;
                    for( t = 0; t < g.size(); ++ t )
                    {
                        if( g[t] != vec[j][t] )
                            break;
                    }
                    if( t == g.size() )
                    {
                        ok = true;
                        stare = j;
                        break;
                    }
                }
            }

            if( ok == false )
            {
                ++ contor;
                for( vector < int > :: iterator it = g.begin(); it != g.end(); ++ it )
                    vec[contor].push_back( *it );
                stare = contor;
            }

            liste4[k].push_back( make_pair( stare, litere[i] ) );
        }

        ++k;
    }


    B->stare_initiala = 0;
    B->n = contor + 1;
    B->m = 15;
    B->lis = new vector< pair < int, char > >[B->n];
    for( int i = 0; i < B->n; ++ i )
    {
        for( vector < pair < int, char > > :: iterator it = liste4[i].begin(); it != liste4[i].end(); ++ it )
        {
            B->lis[i].push_back( make_pair( it->first, it->second ) );
        }
    }

    for( vector < int > :: iterator it = fin.begin(); it != fin.end(); ++ it )
        cout << *it << " ";

    cout << endl;

    for( int i = 0; i < B->n; ++ i )
    {
        for( vector < int > :: iterator it = vec[i].begin(); it != vec[i].end(); ++ it )
        {
            int x = *it;
            bool ok = false;
            for( vector < int > :: iterator itt = fin.begin(); itt != fin.end(); ++ itt )
            {
                if( x == *itt )
                    ok = true;
            }
            if( ok == true )
            {
                bool okk = false;
                for( vector < int > :: iterator itt = B->fin.begin(); itt != B->fin.end(); ++ itt )
                    if( *itt == i )
                        okk = true;
                if( okk == false )
                    B->fin.push_back( i );
            }
        }
    }

    return *B;
}

void Automat::Hehe( int nod, int nod1, char c, int *&viz, Automat *B, int &nr )
{
    for( vector < pair < int, char > > :: iterator it = this->lis[nod].begin(); it != this->lis[nod].end(); ++ it )
    {
        if( c == '^' )
        {
            bool ok = false;

            for( vector < int > :: iterator itt = fin.begin(); itt != fin.end(); ++ itt )
                if( it->first == *itt )
                    ok = true;
            if( ok == true )
            {
                ok = false;
                for( vector < int > :: iterator itt = B->fin.begin(); itt != B->fin.end(); ++ itt )
                    if( nod1 == *itt )
                        ok = true;
                if( ok == false )
                    B->fin.push_back( nod1 );
            }
            if( viz[it->first] == 0 && it->second == '^' )
            {
                viz[it->first] = 1;
                this->Hehe( it->first, nod1, '^', viz, B, nr );
                viz[it->first] = 0;
            }
            else if( (viz[it->first] == 0 || viz[it->first] == 1 ) && it->second != '^' )
            {
                int val = viz[it->first];
                viz[it->first] = 2;
                bool ok = false;

                for( vector < pair < int, char > > :: iterator itt = B->lis[nod1].begin(); itt != B->lis[nod1].end(); ++ itt )
                    if( itt->first == it->first  &&  itt->second == it->second )
                        ok = true;
                if( ok == false )
                {
                    B->lis[nod1].push_back( make_pair( it->first, it->second ) );
                    ++ nr;
                }
                this->Hehe( it->first, nod1, it->second, viz, B, nr );
                viz[it->first] = val;
            }
        }
        else
        {
            if( (viz[it->first] == 0 || viz[it->first] == 1 ) && it->second == '^' )
            {
                int val = viz[it->first];
                viz[it->first] = 2 ;
                bool ok = false;
                for( vector < pair < int, char > > :: iterator itt = B->lis[nod1].begin(); itt != B->lis[nod1].end(); ++ itt )
                    if( itt->first == it->first && itt->second == c )
                        ok = true;
                if( ok == false )
                {
                    B->lis[nod1].push_back( make_pair ( it->first, c ) );
                    ++ nr;
                }
                this->Hehe( it->first, nod1, c, viz, B, nr );
                viz[it->first]  = val;
            }
        }
        if( it->first == nod )
        {
            if( c == '^' && viz[it->first] != 2 )
            {
                int val = viz[it->first];
                viz[it->first]  = 2;
                bool ok = false;
                for( vector < pair < int, char > > :: iterator itt = B->lis[nod1].begin(); itt != B->lis[nod1].end(); ++ itt )
                    if( itt->first == it->first && itt->second == it->second )
                        ok = true;
                if( ok == false )
                {
                    B->lis[nod1].push_back( make_pair( it->first, it->second ));
                }
                this->Hehe( it->first, nod1, it->second, viz, B, nr );
                viz[it->first] = val;
            }
        }
    }
}

Automat& Automat::To_Nfa()
{
    Automat *B;

    B = new Automat;

    B->n = n;

    B->lis = new vector< pair< int, char > >[n];

    for( vector<int>::iterator it=fin.begin(); it!=fin.end(); ++it )
        B->fin.push_back( *it );
    int nr = 0;

    for( int i = 0; i < n; ++ i )
    {

        int *viz;

        viz = new int[n];
        for( int j = 0; j < n; ++ j )
            viz[j] = 0;
        viz[i] = 1;
        this->Hehe( i, i, '^', viz, B, nr );

    }

    B->m = nr;

    return *B;
}

Automat& Automat::operator*()
{
    Automat *B;

    B = new Automat;

    B->stare_initiala = 0;
    B->n = this->n + 1;
    B->m = this->m + this->fin.size();

    B->lis = new vector<pair<int,char> >[B->n];

    B->lis[0].push_back( make_pair( 1, '^' ));

    B->fin.push_back( B->stare_initiala );

    for( vector<int>::iterator it = fin.begin(); it != fin.end(); it ++ )
        B->lis[*it+1].push_back( make_pair( B->stare_initiala, '^' ) );

    for( int i = 0; i < n; i ++ )
    {
        for( vector<pair<int,char> >::iterator it = lis[i].begin(); it != lis[i].end(); it ++ )
            B->lis[i+1].push_back( make_pair( it->first + 1, it->second ) );
    }

    return *B;
}

Automat& Automat::operator*( Automat &A )
{
    Automat *B;

    B = new Automat;

    B->stare_initiala = stare_initiala;
    B->n = this->n + A.n;
    B->m = this->m + A.m + this->fin.size();

    for( vector<int>::iterator it = A.fin.begin(); it != A.fin.end(); it ++ )
        B->fin.push_back( *it + this->n );

    B->lis = new vector<pair<int,char> >[this->n + A.n];

    for( int i = 0; i < this->n; i ++ )
    {
        for( vector<pair<int,char> >::iterator it = lis[i].begin(); it != lis[i].end(); it ++ )
            B->lis[i].push_back( make_pair( it->first, it->second ) );
    }

    for( vector<int>::iterator it = fin.begin(); it != fin.end(); it ++ )
        B->lis[*it].push_back( make_pair( n, '^' ) );

    for( int i = 0; i < A.n; i ++ )
    {
        for( vector<pair<int,char> >::iterator it = A.lis[i].begin(); it != A.lis[i].end(); it ++ )
            B->lis[n+i].push_back( make_pair( it->first + n, it->second ) );
    }

    return *B;
}

Automat& Automat::operator+( Automat &A )
{
    Automat *B;

    B = new Automat;

    B->stare_initiala = 0;
    B->n = A.n + this->n + 1;
    B->m = A.m + this->m + 2;

    for( vector<int>::iterator it = this->fin.begin(); it != this->fin.end(); it ++ )
        B->fin.push_back( *it + 1 );
    for( vector<int>::iterator it = A.fin.begin(); it != A.fin.end(); it ++ )
        B->fin.push_back( *it + 1 + this->n );

    B->lis = new vector<pair<int,char> >[this->n + A.n + 1];
    B->lis[0].push_back( make_pair( 1, '^' ) );
    B->lis[0].push_back( make_pair( n + 1 , '^' ) );

    for( int i = 0; i < this->n; i ++ )
    {
        for( vector<pair<int,char> >::iterator it = this->lis[i].begin(); it != this->lis[i].end(); it ++ )
            B->lis[i+1].push_back( make_pair( it->first + 1, it->second ) );
    }

    for( int i = 0; i < A.n; i ++ )
    {
        for( vector<pair<int,char> >::iterator it = A.lis[i].begin(); it != A.lis[i].end(); it ++ )
            B->lis[n+i+1].push_back( make_pair( it->first + n + 1, it->second ) );
    }

    return *B;
}

Automat& Automat::operator~()
{
    Automat *A;

    A = new Automat;

    A->stare_initiala = stare_initiala;
    A->n = n;
    A->m = m;
    for( vector<int>::iterator it = fin.begin(); it != fin.end(); it ++ )
        A->fin.push_back( *it );

    A->lis = new vector<pair<int, char> >[n];

    for( int i = 0; i < n; i ++ )
    {
        for( vector<pair<int, char> >::iterator it = lis[i].begin(); it != lis[i].end(); it ++ )
        {
            A->lis[ it->first ].push_back( make_pair( i, it->second ) );
        }
    }

    return *A;
}

Automat::Automat()
{
    stare_initiala = 0;
    n = m = 0;
    lis = NULL;
}


void Automat::init( char c )
{
    n = 2;
    m = 1;
    stare_initiala = 0;
    if( lis )
        delete[] lis;
    fin.clear();
    fin.push_back(1);
    lis = new vector<pair<int,char> >[2];
    lis[0].push_back( make_pair( 1, c ) );
}

Automat::Automat( const char *s )
{
    ifstream f( s );

    f >> n >> m >> stare_initiala;

    lis = new vector<pair< int, char > >[n];

    int x, y;
    char c;
    for( int i = 0; i < m; i ++ )
    {
        f >> x >> y >> c;
        lis[x].push_back( make_pair( y, c ) );
    }

    f >> x;

    for( int i = 0; i < x; i ++ )
    {
         f >> y;
         fin.push_back(y);
    }

}

Automat::Automat( Automat &A )
{
    n = A.n;
    m = A.m;
    stare_initiala = A.stare_initiala;

    fin.erase(fin.begin(), fin.end());

    for( vector<int>::iterator it = A.fin.begin(); it != A.fin.end(); it ++ )
        fin.push_back( *it );

    lis = new vector< pair< int, char > >[n];

    for( int i = 0; i < n; i ++ )
    {
        for( vector<pair< int, char > >::iterator it = A.lis[i].begin(); it != A.lis[i].end(); it ++ )
            lis[i].push_back( make_pair( it->first, it->second ) );
    }
}

Automat::~Automat()
{
    delete[] lis;
}

void Automat::afisare()
{
    cout<<"Stare initiala: "<<stare_initiala<< " " <<  n << " " << m << '\n';
    cout<<"Liste:\n";

    for( int i = 0; i < n; i ++ )
    {
        cout << i << ":   ";
        for( vector<pair< int, char > >::iterator it = lis[i].begin(); it != lis[i].end(); it ++ )
            cout << it->first << " " << it->second << ", ";
        cout << '\n';
    }
    cout<<"Stari finale: ";
    for( vector<int>::iterator it = fin.begin(); it != fin.end(); it ++ )
        cout << *it << " ";
    cout << '\n' << '\n';
}

void Automat::operator=( Automat A )
{
    n = A.n;
    m = A.m;
    stare_initiala = A.stare_initiala;

    fin.erase(fin.begin(), fin.end());

    for( vector<int>::iterator it = A.fin.begin(); it != A.fin.end(); it ++ )
        fin.push_back( *it );

    lis = new vector<pair<int,char> >[n];

    for( int i = 0; i < n; i ++ )
        for( vector<pair<int, char> >::iterator it = A.lis[i].begin(); it != A.lis[i].end(); it ++ )
            lis[i].push_back( make_pair( it->first, it->second ) );
}

Automat& functie2();
Automat& functie1()
{
    Automat *A, *B;
    A = new Automat;
    B = new Automat;
    *A = functie2();

    while( s[p] == '|' )
    {
        p++;
        *B = functie2();
        if( s[p] == '*' )
        {
            p++;
            *B = *(*B);
        }
        *A = *A+*B;
    }
    return *A;
}

Automat& functie2()
{
    Automat *A;
    Automat *B;
    B = new Automat;
    A = new Automat;
    while( ( s[p] >= 'a' && s[p] <= 'z' ) || s[p] == '(' )
    {
        if( s[p] == '(' )
        {
            ++ p;
            *B = functie1();
            ++ p;
            if( s[p] == '*' )
            {
                ++ p;
                *B = * ( *B );
            }
        }
        else
        {
            B->init( s[p] );
            ++ p;
            if( s[p] == '*' )
            {
                ++ p;
                *B = * ( *B );
            }
        }
        *A = (*A)*(*B);
    }
    return *A;
}


int main()
{

    f >> s;
    Automat A;

    A = functie1();

    A.afisare();

    A = A.To_Nfa();

    A.afisare();
    A = A.To_Dfa();

    A.afisare();

    //abcbac !!

   // A.verificare();
   /* A.afisare();
    A = A.To_Nfa();
    A.afisare();
    A = A.To_Dfa();
    A.afisare();
    A.verificare();*/


}
/*
5 9 0
0 1 ^
0 2 ^
1 1 a
2 2 b
1 3 b
2 3 a
3 4 a
3 4 b
4 0 ^
1
4
a*((ab|ba)|(cb*|bc*)*)*

*/

