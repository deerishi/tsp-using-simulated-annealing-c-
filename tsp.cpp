#include "bits/stdc++.h"
int originalGraph[40][40];
using namespace std;

vector<int> cities;
vector<int> finalOrder;
int arr[40];// arr is the array that stores the City order
int numOfCities;

class TSP 
{
    /*This Class defines the Basic functions for the TSP Problem, like taking the Input and calculating the distance matrix for the cities*/
    public:
    
     char node[3];
     int points[40][2],numCities;
     //TSP(int i,int j) : x(i),y(j){}
     
     void inputData()
     {
        
        scanf("%d",&numCities);
        int i=0;
        int x,y;
        getc_unlocked(stdin);
        //double distance;
        while(i<numCities)
        {
            scanf("%s%d%d ",node,&points[i][0],&points[i][1]);
            //cities.push_back()
            i++;
        }
        
     }
     
     int CalculateDistance(int x1,int y1,int x2,int y2)
     {
        return int(sqrt(pow(x1-x2,2)+pow(y1-y2,2)) + 0.5 );
     }
     void createOriginalDistanceGraph()
     {
        int i,j;
        for(i=0;i<numCities;i++)
        {
            originalGraph[i][i]=0;

            for(j=i+1;j<numCities;j++)
            {
                originalGraph[i][j]=CalculateDistance(points[i][0],points[i][1],points[j][0],points[j][1]);
                originalGraph[j][i]=originalGraph[i][j];
                // This is a Symmetric TSP
                
          
            }
        }
     } 
};

int distance(int city1,int city2)
{
    return originalGraph[city1][city2];
}

double getRandomNumber(double i,double j) //This function generates a random number between 
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    uniform_real_distribution<double> distribution (i,j);
    //cout<<"returning "<<(double(distribution(generator)))<<"\n";
    return double(distribution(generator));
    
}


int getTourLength() //This function returns the tour length of the current order of cities
{
    //cout<<"the cuurentcities are \n";
   
    vector<int>:: iterator it=cities.begin();
    /*for(it=cities.begin();it!=cities.end();it++)
    {
        cout<<*it<<" ";
    }
    cout<<"\n";*/
    it=cities.begin();
    int pcity1=*it,ncity;
    //cout<<"\n the pcity is "<<pcity1<<"\n";
    int tourLength=distance(0,pcity1);
    for(it=cities.begin()+1;it!=cities.end();it++)
    {
        ncity=*it;
        tourLength+=distance(pcity1,ncity);
        
        pcity1=ncity;
    }
    
    tourLength+=distance(pcity1,0); //adding the distance back to the source path
    return tourLength;
}

void swap(int i,int j)
{
    int temp=arr[i];
    arr[i]=arr[j];
    arr[j]=temp;
}


int getNearestNeighbourTour() //puts the nearestNeighbourTour in the vector cities
{
    vector<int>::iterator it;
    int numCities=numOfCities,i,j;

    for(i=0;i<numCities;i++)
    {
        arr[i]=i;
        //cout<<"arr is "<<i<<"\n";
    }
    int best,bestIndex;
    for(i=1;i<numCities;i++)
    {
        best=INT_MAX;
        for(j=i;j<numCities;j++)
        {
            if(distance(i-1,j)<best)
            {
                best=distance(i,j);
                bestIndex=j;
            }
        }
        swap(i,bestIndex);
    }
    cities.clear();
    for(i=1;i<numCities;i++)
    {
        cities.push_back(arr[i]);
        finalOrder.push_back(arr[i]);
    }
    
    int nearestNeighbourTourLength=getTourLength();
    return nearestNeighbourTourLength;
}

void swap2(int i,int j)
{
    vector<int>::iterator it=cities.begin();
    int temp=*(it+i);
    *(it+i)=*(it+j);
    *(it+j)=temp;
    
}

double getProbability(int difference,double temperature) //This function finds the probability of how bad the new solution is
{
    return exp(-1*difference/temperature);
}
int mini=INT_MAX;
int main ( int argc, char *argv[] )
{

    int i;
    
    vector<int>::iterator it,it2;
    TSP prob1;
    prob1.inputData();
    prob1.createOriginalDistanceGraph();
    numOfCities=prob1.numCities;

    //Generate the initial city tour and get the  nearestNeighbourTourLength
    int bestTourLength=getNearestNeighbourTour(); //start with a decent point

    if(mini > bestTourLength ) mini=bestTourLength;
    double temperature,coolingRate=0.9,absoluteTemperature=0.00001,probability;
    int position1=0,position2=0;
    int newTourLength,difference;
    std::fstream fs;
    fs.open ("tspResults.txt", std::fstream::in | std::fstream::out );
    for(int rs=0;rs<100;rs++)
    {
        temperature=99999999999999999999999999999999999999999.0; //Initial Temperature
        //temperature=DBL_MAX;
        //cout<<"doing rs "<<rs<<"\n";
        fs<<"[";
        while(temperature > absoluteTemperature) 
        {
            //cout<<"hi";
            
            
            position1=int(getRandomNumber(0,numOfCities-1));
            position2=int(getRandomNumber(0,numOfCities-1));
            while(position1==position2 or( (position1>numOfCities-2) or (position2>numOfCities-2)))
            {
                position1=int(getRandomNumber(0,numOfCities-2));
                position2=int(getRandomNumber(0,numOfCities-2));
                
            }
            //cout<<"position1 is "<<position1<<" position2 is "<<position2<<"\n";
            swap2(position1,position2);
            it2=cities.begin();
            if(position2 > position1)
            random_shuffle(it2+position1,it2+position2);
            newTourLength=getTourLength();
            if(mini > newTourLength ) mini=newTourLength;
            fs<<newTourLength<<",";
            //cout<<"current tour length is "<<newTourLength<<" n bestTourLength is "<<bestTourLength<<"\n\n";
            difference=newTourLength-bestTourLength;
            
            if(difference <0 or (difference >0 and  getProbability(difference,temperature) > getRandomNumber(0,1)))
            {
                finalOrder.clear();
                
                for(it=cities.begin();it!=cities.end();it++)
                {
                    finalOrder.push_back(*it);
                }
                bestTourLength=difference+bestTourLength;
            }
            temperature=temperature*coolingRate;
            
        }
        fs<<"]\n";
        random_shuffle(cities.begin(),cities.end());
        //cout<<"the best solution is "<<bestTourLength<<"\n";
      }  
      fs.close();
    cout<<"the best solution is "<<bestTourLength<<"\n";
    cout<<" the minimum solution found is  "<<mini<<"\n";
    return 0;
}
