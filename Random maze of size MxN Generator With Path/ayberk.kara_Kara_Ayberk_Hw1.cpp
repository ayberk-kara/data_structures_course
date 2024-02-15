
using namespace std;
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm> 

class MazeCordinates
{
public:

    MazeCordinates(int _x, int _y) : x(_x), y(_y) {};
    MazeCordinates() {
        x = 0;
        y = 0;

    };

    int x;
    int y;

    bool operator==(const MazeCordinates& other) const {
        return (x == other.x) && (y == other.y);
    }
    friend istream& operator>>(istream& input, MazeCordinates& coord)
    {
        input >> coord.x >> coord.y;
        return input;
    }
private:

};


class MazePoint {
private:


public:
    int x;
    int y;
    int l;
    int r;
    int u;
    int d;
    MazePoint(int _x, int _y) : x(_x), y(_y), l(1), r(1), u(1), d(1) {}
    MazePoint() {
        x = 0;
        y = 0;
        l = 1;
        u = 1;
        d = 1;
        r = 1;
    };

    bool operator==(const MazePoint& other) const {
        return (x == other.x) && (y == other.y);
    }
};


template <class T>
class Node {
public:
    T data;
    Node<T>* next;

    Node(T n) : data(n), next(NULL) {}
};


template <class T>
class Stack {
    Node<T>* top;

public:
    Stack() { top = NULL; }

    bool contains(const T& item) const {
        Node<T>* current = top;

        while (current != nullptr) {
            if (current->data == item) {
                return true;
            }
            current = current->next;
        }
        return false;
    }


    void push(T data)
    {
        Node<T>* temp = new Node<T>(data);


        if (!temp) {
            cout << "\nStack Overflow";
            exit(1);
        }
        temp->data = data;
        temp->next = top;
        top = temp;
    }

    bool isEmpty()
    {
        return top == NULL;
    }


    void pop(T& myData)
    {
        Node<T>* temp;
        if (top == NULL) {
            cout << "\nStack Underflow" << endl;
            exit(1);
        }
        else {
            myData = top->data;
            temp = top->next;
            delete top;
            top = temp;

        }
    }

};






bool compareMazePoints(const MazePoint& point1, const MazePoint& point2) {
    if (point1.y == point2.y) {
        return point1.x < point2.x;
    }
    return point1.y < point2.y;
}

int main()
{
    int NumberOfMazes, M, N;
    cout << "Enter the number of mazes:" << endl;
    cin >> NumberOfMazes;
    cout << endl;
    cout << "Enter the number of rows and columns (M and N):" << endl;
    cin >> N >> M;
    cout << endl;

    srand(static_cast<unsigned>(time(nullptr)));
    Stack<MazePoint> MazeStack;
    Stack<MazeCordinates> CordinatesStack;
    Stack<MazePoint> MazeSearchStack;
    int CurrentPointCount;
    for (int k = 1; k < NumberOfMazes + 1; k++)
    {
        CurrentPointCount = 1;
        MazeCordinates tempCordinate(0, 0);
        MazePoint tempPoint(0, 0);                                  //Pushing start points to cordinate and maze point stacks
        MazePoint CurrentHeadCell(0, 0);                            // Cordinates stack keeps x and y values maze point keeps x,y,u,d,r,l values
        CordinatesStack.push(tempCordinate);                        //Reason for keeping seperate stacks is when we turn back I'm popping the maze values but not popping coordinates
        MazeStack.push(tempPoint);                                      //in order to not go to a already visited point


        for (int i = 0; i < M * N; i++)
        {
            vector<int> directionVec{ 0,1,2,3 };
            MazeStack.pop(CurrentHeadCell);
            tempPoint = CurrentHeadCell;
            CordinatesStack.pop(tempCordinate);
            tempCordinate.x = CurrentHeadCell.x;
            tempCordinate.y = CurrentHeadCell.y;

            tempCordinate.x = tempCordinate.x - 1;
            if (CordinatesStack.contains(tempCordinate) || tempCordinate.x == -1)            //is left side reachable?
            {
                directionVec.erase(remove(directionVec.begin(), directionVec.end(), 0), directionVec.end());

            }
            tempCordinate.x = tempCordinate.x + 1;


            tempCordinate.x = tempCordinate.x + 1;
            if (CordinatesStack.contains(tempCordinate) || tempCordinate.x == N)      //is right side reachable?
            {
                directionVec.erase(remove(directionVec.begin(), directionVec.end(), 1), directionVec.end());


            }
            tempCordinate.x = tempCordinate.x - 1;


            tempCordinate.y = tempCordinate.y + 1;
            if (CordinatesStack.contains(tempCordinate) || tempCordinate.y == M)        //is up side reachable?
            {
                directionVec.erase(remove(directionVec.begin(), directionVec.end(), 2), directionVec.end());

            }
            tempCordinate.y = tempCordinate.y - 1;


            tempCordinate.y = tempCordinate.y - 1;
            if (CordinatesStack.contains(tempCordinate) || tempCordinate.y == -1)            //is down side reachable?
            {
                directionVec.erase(remove(directionVec.begin(), directionVec.end(), 3), directionVec.end());

            }
            tempCordinate.y = tempCordinate.y + 1;

            if (directionVec.size() == 0)
            {
                CordinatesStack.push(tempCordinate);
                do
                {

                    MazeSearchStack.push(CurrentHeadCell);
                    directionVec = { 0,1,2,3 };
                    MazeStack.pop(CurrentHeadCell);

                    tempPoint = CurrentHeadCell;

                    //cout << "Do'ya girildi " << endl;

                    tempCordinate.x = CurrentHeadCell.x;
                    tempCordinate.y = CurrentHeadCell.y;

                    tempCordinate.x = tempCordinate.x - 1;
                    if (CordinatesStack.contains(tempCordinate) || tempCordinate.x == -1)            //is left side reachable?
                    {
                        directionVec.erase(remove(directionVec.begin(), directionVec.end(), 0), directionVec.end());

                    }
                    tempCordinate.x = tempCordinate.x + 1;
                    tempCordinate.x = tempCordinate.x + 1;
                    if (CordinatesStack.contains(tempCordinate) || tempCordinate.x == N)      //is right side reachable?
                    {
                        directionVec.erase(remove(directionVec.begin(), directionVec.end(), 1), directionVec.end());


                    }
                    tempCordinate.x = tempCordinate.x - 1;
                    tempCordinate.y = tempCordinate.y + 1;
                    if (CordinatesStack.contains(tempCordinate) || tempCordinate.y == M)        //is up side reachable?
                    {
                        directionVec.erase(remove(directionVec.begin(), directionVec.end(), 2), directionVec.end());

                    }
                    tempCordinate.y = tempCordinate.y - 1;
                    tempCordinate.y = tempCordinate.y - 1;
                    if (CordinatesStack.contains(tempCordinate) || tempCordinate.y == -1)            //is down side reachable?
                    {
                        directionVec.erase(remove(directionVec.begin(), directionVec.end(), 3), directionVec.end());

                    }
                    tempCordinate.y = tempCordinate.y + 1;

                } while (directionVec.size() == 0 && CurrentPointCount < M * N);
            }




            if (directionVec.size() != 0)
            {

                int random = (rand() % directionVec.size());
                int random_element = directionVec[random];
                if (random_element == 0)
                {
                    CurrentHeadCell.l = 0;
                    tempPoint.x = tempPoint.x - 1;

                    tempPoint.d = 1;
                    tempPoint.r = 1;
                    tempPoint.l = 1;
                    tempPoint.u = 1;

                    tempPoint.r = 0;

                }
                else if (random_element == 1)
                {
                    CurrentHeadCell.r = 0;
                    tempPoint.x = tempPoint.x + 1;

                    tempPoint.d = 1;
                    tempPoint.r = 1;
                    tempPoint.l = 1;
                    tempPoint.u = 1;

                    tempPoint.l = 0;
                }
                else if (random_element == 2)                                                                             //Generating new point that we moved to
                {
                    CurrentHeadCell.u = 0;
                    tempPoint.y = tempPoint.y + 1;

                    tempPoint.d = 1;
                    tempPoint.r = 1;
                    tempPoint.l = 1;
                    tempPoint.u = 1;

                    tempPoint.d = 0;
                }
                else if (random_element == 3)
                {
                    CurrentHeadCell.d = 0;
                    tempPoint.y = tempPoint.y - 1;

                    tempPoint.d = 1;
                    tempPoint.r = 1;
                    tempPoint.l = 1;
                    tempPoint.u = 1;


                    tempPoint.u = 0;
                }
            }
            MazeStack.push(CurrentHeadCell);
            tempCordinate.x = CurrentHeadCell.x;            //Pushing old point and coordinate
            tempCordinate.y = CurrentHeadCell.y;
            CordinatesStack.push(tempCordinate);
            CurrentPointCount++;
            //cout << tempPoint.x << tempPoint.y << endl;
            MazeStack.push(tempPoint);
            tempCordinate.x = tempPoint.x;
            tempCordinate.y = tempPoint.y;                  //Pushing new point and coordinate
            CordinatesStack.push(tempCordinate);




        }













        string textFileName = "";
        textFileName = "maze_" + to_string(k) + ".txt";



        ofstream outputFile(textFileName);

        /*
        if (outputFile.is_open()) {
            outputFile << M << " " << N << endl;
            for (int i = 0; i < M * N; i++)
            {
                MazePoint point;
                MazeStack.pop(point);
                outputFile << "x=" << point.x << " y=" << point.y << " l=" << point.l << " r=" << point.r << " u=" << point.u << " d=" << point.d << "\n";
            }
        }
        outputFile.close();
        */



        if (!MazeSearchStack.isEmpty())
        {
            MazePoint tempPoint1(0, 0);
            while (!MazeSearchStack.isEmpty())
            {
                MazeSearchStack.pop(tempPoint1);
                if (!MazeStack.contains(tempPoint1))
                {
                    MazeStack.push(tempPoint1);
                }
            }
        }


        vector<MazePoint> mazeData;



        while (!MazeStack.isEmpty()) {
            MazePoint currentPoint;
            MazeStack.pop(currentPoint);

            if (find(mazeData.begin(), mazeData.end(), currentPoint) == mazeData.end()) {
                mazeData.push_back(currentPoint);
            }


        }
        while (!CordinatesStack.isEmpty()) {
            CordinatesStack.pop(tempCordinate);
        }
        //sort(mazeData.begin(), mazeData.end(), compareMazePoints);
        //reverse(mazeData.begin(), mazeData.end());

        sort(mazeData.begin(), mazeData.end(), [](const MazePoint& point1, const MazePoint& point2) {
            if (point1.y == point2.y) {
                return point1.x < point2.x;
            }
            return point1.y < point2.y;
            });
        if (outputFile.is_open()) {
            outputFile << M << " " << N << endl;
            for (const MazePoint& point : mazeData) {
                outputFile << "x=" << point.x << " y=" << point.y << " l=" << point.l << " r=" << point.r << " u=" << point.u << " d=" << point.d << "\n";
            }
            outputFile.close();
        }
        else {
            cerr << "Dosya olusmadi." << endl;
        }













        // Maze generation ends here
























    }



    //Path finding starts here




    string textFileName = "";
    MazeCordinates tempCordinate(0, 0);
    MazePoint tempPoint(0, 0);
    MazePoint CurrentHeadCell(0, 0);


    int MazeID;
    MazeCordinates entryCordinates;
    MazeCordinates exitCordinates;
    MazeCordinates currentCordinates;
    MazePoint StartPoint;
    cout << "Enter a maze ID between 1 to 5 inclusive to find a path: ";
    cin >> MazeID;
    cout << endl;
    cout << "Enter x and y coordinates of the entry points(x, y) or (column, row) : ";
    cin >> entryCordinates;
    cout << endl;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> exitCordinates;


    textFileName = textFileName = "maze_" + to_string(MazeID) + ".txt";

    Stack<MazePoint> MoveMazeStack;
    ifstream inputFile(textFileName);



    string line;
    int row = 0;
    MazePoint point;
    int elementcount = 0;
    while (getline(inputFile, line)) {
        if (row > 0) {
            //cout << line << endl;
            int yPos = line.find("y");
            int lPos = line.find("l");
            int xleng = yPos - 3;
            lPos = lPos - 6 - yPos;
            point.x = stoi(line.substr(2, xleng));
            point.y = stoi(line.substr(yPos + 2, lPos));
            point.l = stoi(line.substr(line.length() - 13, 1));
            point.r = stoi(line.substr(line.length() - 9, 1));                                  //Getting maze information from text file
            point.u = stoi(line.substr(line.length() - 5, 1));
            point.d = stoi(line.substr(line.length() - 1, 1));


            MoveMazeStack.push(point);
            elementcount++;
        }
        row++;
    }
    //cout << elementcount << endl;
    inputFile.close();
    tempCordinate = entryCordinates;


    CurrentHeadCell.x = -1;
    while (CurrentHeadCell.x != entryCordinates.x || CurrentHeadCell.y != entryCordinates.y)
    {
        MoveMazeStack.pop(CurrentHeadCell);
        MazeStack.push(CurrentHeadCell);
    }
    StartPoint = CurrentHeadCell;
    while (!MazeStack.isEmpty())
    {
        MazeStack.pop(CurrentHeadCell);
        MoveMazeStack.push(CurrentHeadCell);
    }

    while (!MoveMazeStack.isEmpty())
    {
        MoveMazeStack.pop(point);
        //cout << "x=" << point.x << " y=" << point.y << " l=" << point.l << " r=" << point.r << " u=" << point.u << " d=" << point.d << "\n";
        MazeStack.push(point);
    }
    while (!MazeStack.isEmpty())
    {
        MazeStack.pop(CurrentHeadCell);
        MoveMazeStack.push(CurrentHeadCell);
    }







    CordinatesStack.push(tempCordinate);
    MazeStack.push(StartPoint);                     //Same logic with maze generation applies here
    Stack<MazePoint>MazeSearchStack2;

    while (currentCordinates.x != exitCordinates.x || currentCordinates.y != exitCordinates.y)
    {
        MazePoint searchpoint;
        //cout << "pop1" << endl;
        CordinatesStack.pop(tempCordinate);
        //cout << "pop2" << endl;
        MazeStack.pop(CurrentHeadCell);
        //cout << "normal poplanan head1: " << " CurrentHeadCell x:  " << "   " << CurrentHeadCell.x << " CurrentHeadCell y:  " << "   " << CurrentHeadCell.y << "currenthead u d l r" << CurrentHeadCell.u << CurrentHeadCell.d << CurrentHeadCell.l << CurrentHeadCell.r << endl;
        tempPoint = CurrentHeadCell;

        tempPoint = CurrentHeadCell;

        vector<int> directionVec{ 0,1,2,3 };

        tempCordinate.x = tempCordinate.x - 1;
        if (CordinatesStack.contains(tempCordinate) || tempCordinate.x == -1 || CurrentHeadCell.l == 1)            //is left side reachable?
        {
            directionVec.erase(remove(directionVec.begin(), directionVec.end(), 0), directionVec.end());

        }
        tempCordinate.x = tempCordinate.x + 1;


        tempCordinate.x = tempCordinate.x + 1;
        if (CordinatesStack.contains(tempCordinate) || tempCordinate.x == N || CurrentHeadCell.r == 1)      //is right side reachable?
        {
            directionVec.erase(remove(directionVec.begin(), directionVec.end(), 1), directionVec.end());


        }
        tempCordinate.x = tempCordinate.x - 1;


        tempCordinate.y = tempCordinate.y + 1;
        if (CordinatesStack.contains(tempCordinate) || tempCordinate.y == M || CurrentHeadCell.u == 1)        //is up side reachable?
        {
            directionVec.erase(remove(directionVec.begin(), directionVec.end(), 2), directionVec.end());

        }
        tempCordinate.y = tempCordinate.y - 1;


        tempCordinate.y = tempCordinate.y - 1;
        if (CordinatesStack.contains(tempCordinate) || tempCordinate.y == -1 || CurrentHeadCell.d == 1)            //is down side reachable?
        {
            directionVec.erase(remove(directionVec.begin(), directionVec.end(), 3), directionVec.end());

        }
        tempCordinate.y = tempCordinate.y + 1;




        if (directionVec.size() == 0)
        {
            if (currentCordinates.x != exitCordinates.x || currentCordinates.y != exitCordinates.y)
            {
                CordinatesStack.push(tempCordinate);
                do
                {
                    //cout << "Doddddo'ya girildi " << endl;
                    directionVec = { 0,1,2,3 };


                    //cout << "pop3" << endl;
                    MazeStack.pop(CurrentHeadCell);
                    //cout << "poplanan head1: " << " CurrentHeadCell x:  " << "   " << CurrentHeadCell.x << " CurrentHeadCell y:  " << "   " << CurrentHeadCell.y << endl;

                    tempPoint = CurrentHeadCell;



                    tempCordinate.x = CurrentHeadCell.x;
                    tempCordinate.y = CurrentHeadCell.y;

                    tempCordinate.x = tempCordinate.x - 1;
                    if (CordinatesStack.contains(tempCordinate) || tempCordinate.x == -1 || CurrentHeadCell.l == 1)            //is left side reachable?
                    {
                        directionVec.erase(remove(directionVec.begin(), directionVec.end(), 0), directionVec.end());

                    }
                    tempCordinate.x = tempCordinate.x + 1;
                    tempCordinate.x = tempCordinate.x + 1;
                    if (CordinatesStack.contains(tempCordinate) || tempCordinate.x == N || CurrentHeadCell.r == 1)      //is right side reachable?
                    {
                        directionVec.erase(remove(directionVec.begin(), directionVec.end(), 1), directionVec.end());


                    }
                    tempCordinate.x = tempCordinate.x - 1;
                    tempCordinate.y = tempCordinate.y + 1;
                    if (CordinatesStack.contains(tempCordinate) || tempCordinate.y == M || CurrentHeadCell.u == 1)        //is up side reachable?
                    {
                        directionVec.erase(remove(directionVec.begin(), directionVec.end(), 2), directionVec.end());

                    }
                    tempCordinate.y = tempCordinate.y - 1;
                    tempCordinate.y = tempCordinate.y - 1;
                    if (CordinatesStack.contains(tempCordinate) || tempCordinate.y == -1 || CurrentHeadCell.d == 1)            //is down side reachable?
                    {
                        directionVec.erase(remove(directionVec.begin(), directionVec.end(), 3), directionVec.end());

                    }
                    tempCordinate.y = tempCordinate.y + 1;
                    //cout << "Doddddo'ya cikildi " << endl;

                    if (currentCordinates.x == exitCordinates.x && currentCordinates.y == exitCordinates.y)
                    {
                        break;
                    }
                } while (directionVec.size() == 0);
            }

        }




        if (directionVec.size() != 0)
        {

            int random = (rand() % directionVec.size());
            int random_element = directionVec[random];
            if (random_element == 0)
            {

                tempPoint.x = tempPoint.x - 1;



            }
            else if (random_element == 1)
            {
                ;
                tempPoint.x = tempPoint.x + 1;


            }
            else if (random_element == 2)
            {

                tempPoint.y = tempPoint.y + 1;


            }
            else if (random_element == 3)
            {

                tempPoint.y = tempPoint.y - 1;


            }
        }
        //cout << "tempPoint x:  " << "   " << tempPoint.x << "tempPoint y:  " << "   " << tempPoint.y << endl;

        //cout << "Aranan temp point   X: " << tempPoint.x << " Y: " << tempPoint.y << endl;

        MazeStack.push(CurrentHeadCell);
        tempCordinate.x = CurrentHeadCell.x;
        tempCordinate.y = CurrentHeadCell.y;
        CordinatesStack.push(tempCordinate);
        //cout << "pushlanan head1: " << " CurrentHeadCell x:  " << "   " << CurrentHeadCell.x << " CurrentHeadCell y:  " << "   " << CurrentHeadCell.y << endl;
        //cout << "pushlanan head1: " << " CurrentHeadCell x:  " << "   " << CurrentHeadCell.x << " CurrentHeadCell y:  " << "   " << CurrentHeadCell.y << "currenthead u d l r" << CurrentHeadCell.u << CurrentHeadCell.d << CurrentHeadCell.l << CurrentHeadCell.r << endl;


        searchpoint.x = -1;
        searchpoint.y = -1;
        while (searchpoint.x != tempPoint.x || searchpoint.y != tempPoint.y)
        {
            //cout << "pop4" << endl;
            //MoveMazeStack.pop(searchpoint);
            //MazeSearchStack2.push(searchpoint);
            if (searchpoint.x == tempPoint.x && searchpoint.y == tempPoint.y)
            {
                break;
            }

            MazePoint clonedSearchpoint = searchpoint;

            MoveMazeStack.pop(searchpoint);
            MazeSearchStack2.push(clonedSearchpoint);
        }
        CurrentHeadCell.x = searchpoint.x;
        CurrentHeadCell.y = searchpoint.y;
        CurrentHeadCell.u = searchpoint.u;
        CurrentHeadCell.d = searchpoint.d;                  //Getting next point's (that we moved to) information
        CurrentHeadCell.l = searchpoint.l;
        CurrentHeadCell.r = searchpoint.r;
        while (!MazeSearchStack2.isEmpty())
        {
            //cout << "pop5" << endl;
            //MazeSearchStack2.pop(searchpoint);
            //MoveMazeStack.push(searchpoint);

            MazePoint clonedSearchpoint;
            MazeSearchStack2.pop(clonedSearchpoint);
            MoveMazeStack.push(clonedSearchpoint);
        }





        //cout << "pushlanan head2: " << " CurrentHeadCell x:  " << "   " << CurrentHeadCell.x << " CurrentHeadCell y:  " << "   " << CurrentHeadCell.y << "currenthead u d l r" << CurrentHeadCell.u << CurrentHeadCell.d << CurrentHeadCell.l << CurrentHeadCell.r << endl;
        MazeStack.push(CurrentHeadCell);
        tempCordinate.x = CurrentHeadCell.x;
        tempCordinate.y = CurrentHeadCell.y;
        CordinatesStack.push(tempCordinate);


        currentCordinates = tempCordinate;



    }


    textFileName = "";
    textFileName = "maze_" + to_string(MazeID) + "_path_" + to_string(entryCordinates.x) + "_" + to_string(entryCordinates.y) + "_" + to_string(exitCordinates.x) + "_" + to_string(exitCordinates.y) + ".txt";



    ofstream outputFile(textFileName);



    if (!MazeSearchStack.isEmpty())
    {
        MazePoint tempPoint1(0, 0);
        while (!MazeSearchStack.isEmpty())
        {
            MazeSearchStack.pop(tempPoint1);
            if (!MazeStack.contains(tempPoint1))
            {
                MazeStack.push(tempPoint1);
            }
        }
    }


    vector<MazePoint> mazeData;
    while (!MazeStack.isEmpty()) {
        MazePoint currentPoint;
        MazeStack.pop(currentPoint);
        mazeData.push_back(currentPoint);
    }
    while (!CordinatesStack.isEmpty()) {
        CordinatesStack.pop(tempCordinate);
    }
    //sort(mazeData.begin(), mazeData.end(), compareMazePoints);
    //reverse(mazeData.begin(), mazeData.end());
    /*
    sort(mazeData.begin(), mazeData.end(), [](const MazePoint& point1, const MazePoint& point2) {
        if (point1.y == point2.y) {
            return point1.x < point2.x;
        }
        return point1.y < point2.y;
        });
    */
    reverse(mazeData.begin(), mazeData.end());
    if (outputFile.is_open()) {
        //outputFile << M << " " << N << endl;
        for (const MazePoint& point : mazeData) {
            outputFile << point.x << " " << point.y << "\n";
        }
        outputFile.close();
    }
    else {
        cerr << "Dosya olusmadi." << endl;
    }

    return 0;
}

