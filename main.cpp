#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <string>
#include <cstring>

using namespace std;
struct nod{char val[50] ; nod* urm;};

//GLOBALE
int nr;
char url[50],site[1000000];
string line,readBuffer;
ofstream outfile;
ifstream infile ("auto.in");
bool mode;
nod *pr,*sf;


void add(char str[])
{
    nod *c;
    if(!pr)
    {
        pr = new nod;
        pr->urm = NULL;
        strcpy(pr->val , str);
        sf = pr;
    }
    else
    {

        c = new nod;
        sf->urm = c;
        c->urm = NULL;
        strcpy(c->val , str);
        sf = c;
    }
}
void cleargarbage()
{
    nod*c;
    c = pr;
   nr = 0;
    memset(site,0,sizeof(site));
    readBuffer.clear();
    while(pr)
    {
        pr = pr->urm;
        delete c;
        c = pr;
    }
}
bool unic(char str[])
{
    nod *c;
    c = pr;
    if(!strstr(str,".ro")) return 0;
    while(c)
    {
        if(strcmp(c->val,str) == 0) return 0;
        c = c->urm;
    }
    return 1;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool onchr(char x)
{
    if(x>= '0' && x<='9') return 1; // e cifra
    if(x>= 'a' && x<='z') return 1; // e liter
    if(x=='.' || x=='-') return 1; // altele

    return 0;
}

void crawl(char str[],char url[])
{
    int end,start,k;
    char *p, result[50] = "", last[50] = "",checkstr[50] = "";
    p = strstr(str,"www.");
    cout << "Crawling from " << url <<"...\n";
    while(p)
    {
        start =  p - str ;
        end = start;
        while( onchr(str[end]) )
        {
            end++;
        }
        end--;
        k = 0;
        memset(result,0,sizeof(result));
        memset(checkstr,0,sizeof(checkstr));
        for(int i = start ; i<=end ; i++)
        {
            result[k++] = str[i];
        }
        strcat(checkstr,"http://");
        strcat(checkstr,result);
        if(strcmp(last,result)!= 0 && strcmp(checkstr,url)) // daca link-ul nu este identic cu cel anterior
        {
            if(unic(result))
            {

            add(result);
            cout << result << '\n';
            outfile << result <<'\n';
            nr++;

            }
        }
        strcpy(last,result);


        p = strstr( p+(end-start) , "www." );
    }

    cout << '\n';






}


int main()
{
  system("color 70 ");
  CURL *curl;
  CURLcode res;
  outfile.open("pages.in", std::ios_base::app);
  cout << "CIOACA RADU ULTIMATE .RO WEB CRAWLER" << endl << endl;
  cout<<"0.Automatic(NU MERGE) || 1.Manual\n";
  cout<<"MODE:";
  cin >> mode;
  if(mode == 1)
  {

      cout << "URL:";
      cin >> url;
      cout << "\n\n\n";


      while(strcmp(url,"quit") != 0)
      {
          if(strstr(url,"http://"))
          {


              curl = curl_easy_init();

              if(curl)
              {
                curl_easy_setopt(curl, CURLOPT_URL,url);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                for( unsigned int i = 0 ; i<= readBuffer.length() ; i++) site[i] = readBuffer[i];



              }

              crawl(site,url);

          }
          else cout << "Nu ai introdus un URL valid!\n";
          cout << "\n\nAm extras " << nr << " link-uri!\n\n";

          cleargarbage();
          cout << "URL:";
          cin >> url;
          cout << "\n\n\n";
      }
  }

  /*else // automatic file mode
  {
    if (infile.is_open())
    {
        while ( getline(infile,line) )
        {
          cout << '\n\n' << line << "...CRAWLING"<< '\n';
          cout << "\n\n\n";
          for( int i = 0 ; i<= line.length() ; i++)
          {
              url[i] = line[i];
          }



          if(strstr(url,"http://"))
          {


              curl = curl_easy_init();

              if(curl)
              {
                curl_easy_setopt(curl, CURLOPT_URL,url);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                for( int i = 0 ; i<= readBuffer.length() ; i++)
                {
                    site[i] = readBuffer[i];
                }



              }

              crawl(site,url);

          }
          else cout << "Nu ai introdus un URL valid!";
        }

    }


  }*/

  infile.close();
  outfile.close();
  return 0;
}
