/*****************************************
** File: Navigator.cpp
** Project: CMSC 202 Project 3, Spring 2024
** Author: John Jo
** Date: 3/26/24
** Section: 34
** E-mail: sunghyj1@umbc.edu
**
** This file contains the Navigator class which manages
** the loading of files, the user input, and the routes.
** It uses a vector (m_routes) to hold all the routes,
** another vector (m_airports) to hold all the airports,
** and a string holding the name of the input file (m_fileName).
**
**
***********************************************/
#include "Navigator.h"

Navigator :: Navigator(string fileName) {
    m_fileName = fileName;
}

Navigator :: ~Navigator() {
    for (uint i = 0; i < m_airports.size(); i++) {
        delete m_airports[i];
    }
    for (uint i = 0; i < m_routes.size(); i++) {
        delete m_routes[i];
    }
}

void Navigator :: Start() {
    ReadFile();
    MainMenu();
}

void Navigator::DisplayAirports() {
    // print the airport number followed by airport information
    for (uint i = 0; i < m_airports.size(); i++) {
        cout << i + 1 << ". " << *m_airports[i] << endl;
    }
}

void Navigator :: ReadFile() {
    ifstream inputFile(m_fileName);
    // check if the file is opened successfully
    if (!inputFile.is_open()) {
        cout << "Failed to open file: " << m_fileName << endl;
        return;
    }
    // variables to store data read from each line
    string code, name, city, country, northStr, westStr;
    double north, west;
    int airportNum = 0;

    while (getline(inputFile, code, ',') &&
           getline(inputFile, name, ',') &&
           getline(inputFile, city, ',') &&
           getline(inputFile, country, ',') &&
           getline(inputFile, northStr, ',') &&
           getline(inputFile, westStr)) {

        // convert strings to doubles
        north = stod(northStr);
        west = stod(westStr);

        // dynamically allocate and insert new airport into m_airports
        Airport* newAirport = new Airport(code, name, city, country, north, west);
        m_airports.push_back(newAirport);
        // to display the number of airports in message
        airportNum++;
    }
    inputFile.close();
    cout << "Opened File" << endl;
    cout << "Airports Loaded: " << airportNum << endl;
}

void Navigator :: InsertNewRoute() {
    // to store airport number from user
    int airportNum;
    // creating a linked list of airports recieved from user
    Route *route = new Route(); 

    cout << "Enter the number of the airport to add to your Route: (-1 to end)" << endl;

    while (true) {
        cin >> airportNum;
        // if user enters -1, check if the route has at least two airports
        if (airportNum == -1) {
            if (route->GetSize() < 2) {
                cout << "A route must have at least two airports" << endl;
                delete route;
                return;
            } else {
                // if the route has at least two airports, add the route to m_routes and display message
                m_routes.push_back(route);
                cout << "Done Building a New Route named " << route->UpdateName() << endl;
                return;
            }
        } 
        // check valid airport number
        if (airportNum >= 1 && airportNum <= 40) {
            // Get the chosen airport pointer from m_airports vector
            Airport* chosen = m_airports[airportNum - 1];
            route->InsertEnd(chosen->GetCode(), chosen->GetName(), 
                             chosen->GetCity(), chosen->GetCountry(), 
                             chosen->GetNorth(), chosen->GetWest());
        } else {
            cout << "Invalid airport number - Try again" << endl;
        }
        cout << "Enter the number of the airport to add to your Route: (-1 to end)" << endl;
    }
}

void Navigator :: MainMenu() {
    int choice = 0;
    do {
      cout << "What would you like to do?:" << endl;
      cout << "1. Create New Route" << endl;
      cout << "2. Display Route" << endl;
      cout << "3. Remove Airport From Route" << endl;
      cout << "4. Reverse Route" << endl;
      cout << "5. Exit" << endl;

      cin >> choice;
      // executes a case given a number 
      switch(choice) {
        case 1:
          DisplayAirports();
          InsertNewRoute();
          break;
        case 2:
          DisplayRoute();
          break;
        case 3:
          RemoveAirportFromRoute();
          break;
        case 4:
          ReverseRoute();
          break;
        case 5:
          cout << "Routes removed from memory" << endl;
          cout << "Deleting Airports" << endl;
          cout << "Deleting Routes" << endl;
          break;
      }
  }
  while(choice != 5);
}

int Navigator :: ChooseRoute() {
    // check if there are any routes available
    if (m_routes.empty()) {
        cout << "No routes to display" << endl;
        return 0;
    }
    int choice;
    do {
        // display available routes with their names
        cout << "Which route would you like to use?" << endl;
        for (size_t i = 0; i < m_routes.size(); i++) {
            cout << i + 1 << ". ";
            cout << m_routes[i]->UpdateName() << endl;
        }
        cin >> choice;
    // continue looping if the choice is out of range
    } while (choice < 1 || choice > static_cast<int>(m_routes.size()));
    // adjusted to index
    return choice - 1;
}

void Navigator :: DisplayRoute() {
    // check if there are any routes available
    if (m_routes.empty()) {
        cout << "No routes to display" << endl;
        return;
    }
    // get the index of the chosen route to get the pointer and display info
    int chosenIndex = ChooseRoute(); 
    Route* chosenRoute = m_routes[chosenIndex];
    cout << chosenRoute->UpdateName() << endl;
    chosenRoute->DisplayRoute(); 

    double totalMiles = RouteDistance(chosenRoute);
    cout << "The total miles of this route is " << totalMiles << " miles" << endl;
}

void Navigator :: RemoveAirportFromRoute() {
    // check if there are any routes available
    if (m_routes.empty()) {
    cout << "No routes to remove airports" << endl;
    return;
    }
    // get the index of the chosen route to get the pointer and display info
    int chosenIndex = ChooseRoute(); 
    Route* chosenRoute = m_routes[chosenIndex];
    if (chosenRoute->GetSize() <= 2) {
        cout << "Route cannot have less than two airports." << endl;
        return;
    }
    cout << chosenRoute->UpdateName() << endl;
    chosenRoute->DisplayRoute();

    int airport;
    bool validInput = false;
    // repeat until the user enters a valid airport number
    while (!validInput) {
        cout << "Which airport would you like to remove?" << endl; 
        cin >> airport;
        // check if the airport number is within the range of airports in the route
        if (airport >= 1 && airport <= chosenRoute->GetSize()) {
            validInput = true;
        }
    }
    chosenRoute->RemoveAirport(airport);
    chosenRoute->DisplayRoute();
    cout << "Route named " << chosenRoute->UpdateName() << " updated" << endl;
}

double Navigator :: RouteDistance(Route* route) {
    // exit if the route has less than 2 airports
    if (route->GetSize() < 2) {
        return 0; 
    }
    double totalMiles = 0.0;
    Airport* current = route->GetData(0);
    // loop through each airport in the route starting from the second airport
    for (int i = 1; i < route->GetSize(); i++) {
    Airport* nextAirport = route->GetData(i);
    double distance = CalcDistance(current->GetNorth(), current->GetWest(),
                                    nextAirport->GetNorth(), nextAirport->GetWest());
    totalMiles += distance;
    // update the current airport pointer to the next airport for the next iteration
    current = nextAirport;
    }
    return totalMiles;   
}

void Navigator::ReverseRoute() {
    // check if there are any routes available
    if (m_routes.empty()) {
    cout << "No routes to reverse" << endl;
    return;
    }
    // ask the user to choose a route to get the pointer and reverse chosen route
    int chosenIndex = ChooseRoute();
    Route* chosenRoute = m_routes[chosenIndex];
    chosenRoute->ReverseRoute();
    cout << "Done reversing Route " << chosenRoute->UpdateName() << endl;
}