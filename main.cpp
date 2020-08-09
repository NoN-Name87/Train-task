#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <ctime>
using namespace std;

struct Trains
{
   vector<double>inf = vector<double>(10);
};

struct Ways
{
   vector<int>opt = vector<int>(5);
   int total_time = 0;
   int total_price = 0;
};

bool Check_station(vector<Trains> spread, vector<Ways> w,int station, int i, int ind_i);
int Convert_time(int hours, int minutes, int end_h, int end_m);
int Location(vector<int> index, vector<Trains> spread, int station);

void Read_file(vector<Trains> &spread)
{
    ifstream file("test_task_data.csv");
    string line;
    double intg;
    int n = 0,j = 0;
    while(getline(file,line))
    {
      stringstream ss(line);
      while(ss >> intg)
      {
          if(ss.peek() == ';' || ss.peek() == ':' || ss.peek() == '.')
            ss.ignore();

            spread[n].inf[j] = intg;
            j++;
      }
      j=0;
      n++;
    }
    file.close();
}

void Sort_Vec(vector<Trains> &spread)
{
  double jump = spread.size();
  bool sw = true;

  while(jump > 1 || sw)
  {
      jump /= 1.25;
      sw = false;

      for(int i=0;i+jump< spread.size();i++)
      {
          if(spread[i].inf[1] > spread[i+jump].inf[1])
          {
              swap(spread[i],spread[i+jump]);
              sw = true;
          }
      }
  }
}

void Count_arr(vector<Trains> &spread,vector<int> &index,int &n)
{
  int st = spread[0].inf[1];
  index[0] = 0;
  for(int i=1;i<spread.size();i++)
  {
    if(st != spread[i].inf[1])
    {
      st = spread[i].inf[1];
      n++;
      index[n] = i;
    }
  }
  index[n+1] = spread.size();
}

void Train_options_time(vector<Trains> &spread,vector<int> &index, int n, vector<Ways> &w)
{
    int time=0,time2=0,t=0;
  while(n)
   {
    t = n;
    w[n-1].total_time=0;
    w[n-1].total_price=0;
  for(int i=0;i<5;i++)
  {
      time += Convert_time(spread[t].inf[4],spread[t].inf[5],spread[t].inf[7],spread[t].inf[8]);
      for(int j=index[t];j!=index[t+1];j++)
      {
        time2 = Convert_time(spread[j].inf[4],spread[j].inf[5],spread[j].inf[7],spread[j].inf[8]);
        if(i==0)
        {
            if(time>time2)
            {
            time = time2;
            w[n-1].opt[i] = j;
            }
        }

        else if(Check_station(spread,w,j,n-1,i))
        {
            time2 += Convert_time(spread[w[n-1].opt[i-1]].inf[7],spread[w[n-1].opt[i-1]].inf[8],spread[j].inf[4],spread[j].inf[5]);
            if(time>time2)
            {
            time = time2;
            w[n-1].opt[i] = j;
            }
        }
      }
      w[n-1].total_time+=time;
      time2 = time = 0;
      t = Location(index,spread,spread[w[n-1].opt[i]].inf[2]);
      time = Convert_time(spread[w[n-1].opt[i]].inf[7],spread[w[n-1].opt[i]].inf[8],spread[t].inf[4],spread[t].inf[5]);
  }
  n--;
  }
}

void Train_best_price(vector<Trains> &spread,vector<int> &index, int n, vector<Ways> &w)
{
 double price1=0,price2=0,t=0;
  while(n)
   {
    t = n;
    w[n-1].total_time=0;
    w[n-1].total_price=0;
  for(int i=0;i<5;i++)
  {
      price1 = spread[index[t]].inf[3];
      for(int j=index[t];j!=index[t+1];j++)
      {
        if(i==0)
        {
            if(price1>spread[j].inf[3])
            {
            price1 = spread[j].inf[3];
            w[n-1].opt[i] = j;
            }
        }

        else if(Check_station(spread,w,j,n-1,i)&&(spread[index[t]].inf[1]==1921||spread[index[t]].inf[1]==1929))
        {
            if(spread[j].inf[2] == 1909)
            {
            price1 = spread[j].inf[3];
            w[n-1].opt[i] = j;
            break;
            }
        }

        else if(Check_station(spread,w,j,n-1,i))
        {
            if(price1>spread[j].inf[3])
            {
            price1 = spread[j].inf[3];
            w[n-1].opt[i] = j;
            }
        }
      }
      w[n-1].total_price+=price1;
      price1 = price2 = 0;
      t = Location(index,spread,spread[w[n-1].opt[i]].inf[2]);
  }
  cout << endl;
  n--;
  }
}

int Convert_time(int hours, int minutes, int end_h, int end_m)
{
  int time=0;
  if(hours>end_h||(hours==end_h&&minutes>end_m))
  {
      minutes+=hours*60;
      end_m+=(end_h+24)*60;
      time = difftime(end_m,minutes);
  }
  else
    {
      minutes+=hours*60;
      end_m+=end_h*60;
      time = difftime(end_m,minutes);
    }
    return time;
}

int Location(vector<int> index, vector<Trains> spread, int station)
{
  for(int i=0;i<index.size();i++)
  {
      if(spread[index[i]].inf[1] == station)
        return i;
  }
}

bool Check_station(vector<Trains> spread, vector<Ways> w,int station, int i, int ind_i)
{
  for(int j=0;j<ind_i;j++)
  {
    if(spread[w[i].opt[j]].inf[1]==spread[station].inf[2])
    return false;
  }
  return true;
}

void Display_Result(vector<Trains> spread, vector<Ways> &w,int n)
{
    while(n)
    {
      for(int i=0;i<5;i++)
      {
          for(int j=0;j<10;j++)
          {
              cout << spread[w[n-1].opt[i]].inf[j] << " ";
          }
          cout << endl;
      }
      cout << "Total time:" << w[n-1].total_time << endl;
      cout << "Total price:" << w[n-1].total_price << endl;
      cout << endl;
      n--;
    }
}

int main()
{
    int num_st = 0;
    vector<Trains>spread(250);
    vector<int>index(7);
    Read_file(spread);
    Sort_Vec(spread);
    Count_arr(spread,index,num_st);
    vector<Ways>w(num_st);

    int value;
    cout << "1-best time,2-best price" << endl;
    cin>>value;

    if(value==1)
    Train_options_time(spread,index,num_st,w);

    else if(value==2)
    Train_best_price(spread,index,num_st,w);

    else
        return main();

    Display_Result(spread,w,num_st);
    return 0;
}
